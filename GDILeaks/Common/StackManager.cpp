// 
// Most of the stack walking/symbol finding code is "stolen" from John Robbins 
// source code from its "Debugging Applications" book
// 


#include "stdafx.h"
#include "StackManager.h"
#include "ImageHlp.h"
#include "SymbolEngine.h"
#include "Helpers.h"



// STL will not compile at /W4 /WX.  Not good.
#ifndef _DEBUG
// function '' not inlined
   #pragma warning (disable : 4710)
#endif
#pragma warning(push, 3)
#include <vector>
using namespace std;
#pragma warning(pop)


#ifdef _DEBUG
   #undef THIS_FILE
   static char THIS_FILE[]=__FILE__;
   #define new DEBUG_NEW
#endif



//--------------------------------------------------------------------------
// Needed types and definitions
// 
typedef vector<ULONG> ADDRVECTOR;

// The symbol engine.
//
static CSymbolEngine g_cSym;

// If TRUE, the symbol engine has been initialized.
//
static BOOL g_bSymIsInit = FALSE;
//


//--------------------------------------------------------------------------
// Helpers
//
static DWORD ConvertAddress(DWORD dwAddr, LPTSTR szOutBuff)
{
   char              szTemp[MAX_PATH + sizeof (IMAGEHLP_SYMBOL)];
   PIMAGEHLP_SYMBOL  pIHS = (PIMAGEHLP_SYMBOL)&szTemp;
   IMAGEHLP_MODULE   stIHM;
   LPTSTR            pCurrPos = szOutBuff;

   ZeroMemory(pIHS, MAX_PATH + sizeof(IMAGEHLP_SYMBOL));
   ZeroMemory(&stIHM, sizeof(IMAGEHLP_MODULE));

   pIHS->SizeOfStruct = sizeof(IMAGEHLP_SYMBOL);
   pIHS->Address = dwAddr;
   pIHS->MaxNameLength = MAX_PATH;

   stIHM.SizeOfStruct = sizeof(IMAGEHLP_MODULE);

// Always stick the address in first.
   pCurrPos += wsprintf(pCurrPos, _T("0x%08X "), dwAddr);

// Get the module name.
   if (0 != g_cSym.SymGetModuleInfo(dwAddr , &stIHM))
   {
   // Strip off the path.
      LPTSTR szName = _tcsrchr(stIHM.ImageName, _T('\\'));
      if ( NULL != szName )
      {
         szName++;
      }
      else
      {
         szName = stIHM.ImageName;
      }
      pCurrPos += wsprintf (pCurrPos, _T("%s: "), szName);
   }
   else
   {
      pCurrPos += wsprintf(pCurrPos, _T("<unknown module>: "));
   }

// Get the function.
   DWORD dwDisp;
   if (0 != g_cSym.SymGetSymFromAddr(dwAddr, &dwDisp, pIHS))
   {
      if (0 == dwDisp)
      {
         pCurrPos += wsprintf(pCurrPos, _T("%s"), pIHS->Name);
      }
      else
      {
         pCurrPos += wsprintf(pCurrPos, _T("%s + %d bytes"), pIHS->Name, dwDisp);
      }

   // If I got a symbol, give the source and line a whirl.
      IMAGEHLP_LINE stIHL;
      ZeroMemory(&stIHL, sizeof(IMAGEHLP_LINE));

      stIHL.SizeOfStruct = sizeof(IMAGEHLP_LINE);

      if (0 != g_cSym.SymGetLineFromAddr(dwAddr, &dwDisp, &stIHL))
      {
      // Put this on the next line and indented a bit.
         pCurrPos += wsprintf(pCurrPos, _T("\r\n      %s, Line %d"), stIHL.FileName, stIHL.LineNumber);
         if (0 != dwDisp)
         {
            pCurrPos += wsprintf(pCurrPos, _T(" + %d bytes"), dwDisp);
         }
      }
   }
   else
   {
      pCurrPos += wsprintf(pCurrPos, _T("<unknown symbol>"));
   }

   return(pCurrPos - szOutBuff);
}


void EmptyStringMap(CMapPtrToPtr& PtrMap)
{
// do nothing if the map is empty
   if (PtrMap.IsEmpty())
      return;

// get each element of the map and delete it
   POSITION pos;
   UINT     key;

// delete one string after the other
   void*    pObject;
   LPTSTR   pInfo;

   for (pos = PtrMap.GetStartPosition(); pos != NULL; )
   {
      PtrMap.GetNextAssoc(pos, (void*&)key, pObject);
      pInfo = (LPTSTR)pObject;
      delete pInfo;
   }

// empty the map itself
   PtrMap.RemoveAll();
}
//
//--------------------------------------------------------------------------



//--------------------------------------------------------------------------
// CStackManager
//
int CStackManager::GetMapCount()
{
   return(m_AddressToName.GetCount());
}


// construction/destruction
// 
CStackManager::CStackManager()
{
   m_AddressToName.InitHashTable(2039);
}

CStackManager::~CStackManager()
{
// cleanup the adress map
   EmptyStringMap(m_AddressToName);

   Cleanup();
}


// C++ interface
// 
BOOL CStackManager::Init()
{
   if (g_bSymIsInit)
      return(TRUE);

// If the symbol engine is not initialized, do it now.
   HANDLE hProcess = GetCurrentProcess();

   DWORD dwOpts = SymGetOptions();

// Turn on load lines.
   SymSetOptions(dwOpts | SYMOPT_DEFERRED_LOADS | SYMOPT_LOAD_LINES);

// build the search path
   TCHAR szPath[_MAX_PATH];
   ::ZeroMemory(szPath, sizeof(szPath));
   TCHAR szSearchPath[0x4000];
   ::ZeroMemory(szSearchPath, sizeof(szSearchPath));

// 1. debugged application folder
   ::GetModuleFileName(NULL, szSearchPath, _MAX_PATH);
   LPTSTR pEnd = _tcsrchr(szSearchPath, _T('\\'));
   if (pEnd != NULL)
      *pEnd = _T('\0');

// 2. Windows symbols if any
   if (::GetEnvironmentVariable(_T("NT_SYMBOL_PATH"), szPath, sizeof(szPath)) > 0)
   {
      _tcscat(szSearchPath, _T(";"));
      _tcscat(szSearchPath, szPath);
   }
   if (::GetEnvironmentVariable(_T("_NT_ALTERNATE_SYMBOL_PATH"), szPath, sizeof(szPath)) > 0)
   {
      _tcscat(szSearchPath, _T(";"));
      _tcscat(szSearchPath, szPath);
   }
   
// 3. windows folder if no symbol
   GetWindowsDirectory(szPath, sizeof(szPath));
   _tcscat(szSearchPath, _T(";"));
   _tcscat(szSearchPath, szPath);

// 4. current directory
   GetCurrentDirectory(sizeof(szPath), szPath);
   _tcscat(szSearchPath, _T(";"));
   _tcscat(szSearchPath, szPath);
   _tcscat(szSearchPath, _T(";."));


   if (!g_cSym.SymInitialize(hProcess, szSearchPath, TRUE))
   {
      TRACE("CStackManager::Init() : Unable to initialize the symbol engine!!!\n");
      return(FALSE);
   }
   else
   {
      g_bSymIsInit = TRUE;
   }

   if (SymGetSearchPath(hProcess, szSearchPath, sizeof(szSearchPath)))
   {
      TRACE("CStackManager::Init(\n");
      TRACE("     hProcess = 0x%x\n", hProcess);
      TRACE("  Search Path = %s\n", szSearchPath);
      TRACE("                   )\n");
   }


// don't forget to load the debugging information for the application itself
   ::GetModuleFileName(NULL, szPath, MAX_PATH);
   HINSTANCE hModule = GetModuleHandle(NULL);
   DWORD dwAddress = g_cSym.SymLoadModule(NULL, szPath, NULL, (DWORD)hModule, 0);
   TRACE("g_cSym.SymLoadModule(0x%x==0x%x - %s)\n", hModule, dwAddress, szPath);


// and for the spying dll
   hModule = AfxGetInstanceHandle();
   ::GetModuleFileName(hModule, szPath, MAX_PATH);
   dwAddress = g_cSym.SymLoadModule(NULL, szPath, NULL, (DWORD)hModule, 0);
   TRACE("g_cSym.SymLoadModule(0x%x==0x%x - %s)\n", hModule, dwAddress, szPath);

   return(TRUE);
}


BOOL CStackManager::Cleanup()
{
   g_cSym.SymCleanup();
   g_bSymIsInit = FALSE;
   return(TRUE);
}


LPCTSTR CStackManager::ConvertStackAddressIntoFunctionName(LPVOID pAddress)
{
   LPTSTR pName = NULL;

   if (!m_AddressToName.Lookup(pAddress, (void*&)pName))
   {
      TRACE("Unknown stack address...\n");
   }

   return(pName);
}


void CStackManager::DumpStackAllocation(DWORD Depth, DWORD* pStack, LPCTSTR szLineSeparator, CString& szInfo)
{
   LPCTSTR pStackFunction = NULL;
   CString szStep;
   for (DWORD Step = 0; Step < Depth; Step++)
   {
      pStackFunction = ConvertStackAddressIntoFunctionName((LPVOID)pStack[Step]);
      if (pStackFunction == NULL)
      {
         szStep.Format("   %u -  0x%x%s", Depth - Step, pStack[Step], szLineSeparator);
      }
      else
      {
         szStep.Format("   %u -  %s%s", Depth - Step, pStackFunction, szLineSeparator);
      }
      szInfo += szStep;
   }
}


DWORD CALLBACK _SymGetModuleBase(HANDLE hProcess, DWORD dwAddress)
{
   return(g_cSym.SymGetModuleBase(dwAddress));
}


LPVOID CALLBACK _SymFunctionTableAccess(HANDLE hProcess, DWORD AddrBase)
{
   return(g_cSym.SymFunctionTableAccess(AddrBase));
}


DWORD* CStackManager::DoStackAddressDump(DWORD& rDepth, DWORD StartFrom)
{
   DWORD* pAddressArray = NULL;

// The symbol engine is initialized so do the stack walk.
   if (!g_bSymIsInit)
      return(NULL);

   HANDLE hProcess = GetCurrentProcess();

// The array of addresses.
   ADDRVECTOR vAddrs;

// The thread information.
   CONTEXT    stCtx;

   stCtx.ContextFlags = CONTEXT_FULL;

// Get the thread context.  Since I am doing this on the CURRENT
// executing thread, the context will be from down in the bowls of
// KERNEL32.DLL.  Probably GetThreadContext itself.
   if (GetThreadContext(GetCurrentThread(), &stCtx))
   {
      STACKFRAME  stFrame;
      DWORD       dwMachine;

      ZeroMemory(&stFrame, sizeof(STACKFRAME));
      stFrame.AddrPC.Mode = AddrModeFlat;

#if defined (_M_IX86)
      dwMachine                  = IMAGE_FILE_MACHINE_I386;
      stFrame.AddrPC.Offset      = stCtx.Eip;
      stFrame.AddrStack.Offset   = stCtx.Esp;
      stFrame.AddrFrame.Offset   = stCtx.Ebp;
      stFrame.AddrStack.Mode     = AddrModeFlat;
      stFrame.AddrFrame.Mode     = AddrModeFlat;

#elif defined (_M_ALPHA)
      dwMachine               = IMAGE_FILE_MACHINE_ALPHA;
      stFrame.AddrPC.Offset   = (unsigned long)stCtx.Fir;
#else
   #error ("Unknown machine!")
#endif

   // Loop for the first 512 stack elements.
      for (DWORD i = 0; i < 512; i++ )
      {
         if (
               !StackWalk(
                  dwMachine,
                  hProcess,
                  hProcess,
                  &stFrame,
                  &stCtx,
                  NULL,
                  (PFUNCTION_TABLE_ACCESS_ROUTINE)_SymFunctionTableAccess,
                  (PGET_MODULE_BASE_ROUTINE)_SymGetModuleBase,
                  NULL
                  )
            )
         {
//            TRACE("stack depth = %u\n", i);
            break;
         }

      // Also check that the address is not zero.  Sometimes
      // StackWalk returns TRUE with a frame of zero.
         if (0 != stFrame.AddrPC.Offset)
         {
            vAddrs.push_back(stFrame.AddrPC.Offset);
//            TRACE("stack[%u] = 0x%x\n", i, stFrame.AddrPC.Offset);
         }
      }

   // Now start converting the addresses.
      DWORD dwSymSize;
      TCHAR szSym[MAX_PATH * 2];
      pAddressArray = new DWORD[vAddrs.size()];
      if (pAddressArray != NULL)
      {
         ADDRVECTOR::iterator loop;

//         TRACE("Stack Address Dump:\n");

         rDepth = 0;
         for (loop =  vAddrs.begin(); loop != vAddrs.end(); loop++)
         {
         // skip the first stack entries
            if (StartFrom > 0)
            {
               StartFrom--;
               continue;
            }

            pAddressArray[rDepth] = *loop;
            
            LPTSTR pName = NULL;
         // check if not in the cache first
            if (m_AddressToName.Lookup((LPVOID)pAddressArray[rDepth], (void*&)pName))
            {
            // nothing special to do
//                  TRACE("%u -  %s\n", rDepth, pName);
            }
            else
            {
            // else, get function name from symbol engine
               dwSymSize = ConvertAddress(*loop, szSym);
//               TRACE("%u -  %s\n", rDepth, szSym);

               if (dwSymSize != 0)
               {
                  pName = new TCHAR[dwSymSize+1];
                  if (pName != NULL)
                  {
                     _tcscpy(pName, szSym);
                     m_AddressToName[(LPVOID)(pAddressArray[rDepth])] = (LPVOID)pName;
                  }
               }
            }

         // next stack step
            rDepth++;
         }
//         TRACE("______________________________________\n");
      }
   }

   return(pAddressArray);
}
//
//--------------------------------------------------------------------------




//--------------------------------------------------------------------------
// CCallStackDlg dialog
//
BEGIN_MESSAGE_MAP(CCallStackDlg, CDialog)
//{{AFX_MSG_MAP(CCallStackDlg)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()


CCallStackDlg::CCallStackDlg(CWnd* pParent /*=NULL*/)
   : CDialog(CCallStackDlg::IDD, pParent)
{
//{{AFX_DATA_INIT(CCallStackDlg)
   // NOTE: the ClassWizard will add member initialization here
//}}AFX_DATA_INIT

// set default values
   m_pStackManager = NULL;
}


void CCallStackDlg::DoDataExchange(CDataExchange* pDX)
{
   CDialog::DoDataExchange(pDX);

//{{AFX_DATA_MAP(CCallStackDlg)
   // NOTE: the ClassWizard will add DDX and DDV calls here
//}}AFX_DATA_MAP
}

void CCallStackDlg::SetStackManager(CStackManager* pStackManager)
{
   m_pStackManager = pStackManager;
}


void CCallStackDlg::SetCallStack(DWORD Depth, DWORD* pStack)
{
   ASSERT(m_pStackManager != NULL);
   if (m_pStackManager != NULL)
   {
      m_pStackManager->DumpStackAllocation(Depth, pStack, _T("\r\n"), m_szCallStack);
      if (::IsWindow(::GetDlgItem(m_hWnd, IDC_EDIT_CALL_STACK)))
      {
         SetDlgItemText(IDC_EDIT_CALL_STACK, m_szCallStack);
         SendDlgItemMessage(IDC_EDIT_CALL_STACK, EM_SETSEL, 0, 0);
      }
   }
}


BOOL CCallStackDlg::OnInitDialog() 
{
// default processing
   CDialog::OnInitDialog();
   
   SetDlgItemText(IDC_EDIT_CALL_STACK, m_szCallStack);
   
   return TRUE;  // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}
//
//--------------------------------------------------------------------------

