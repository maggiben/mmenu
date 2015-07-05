#include "stdafx.h"
#include "GDIDebugger.h"
#include "LibraryInfo.h"

#include "Resource.h"
#include "..\GDITrace\_GDITrace.h"


#ifdef _DEBUG
   #undef THIS_FILE
   static char THIS_FILE[]=__FILE__;
   #define new DEBUG_NEW
#endif


CGDIDebugger::CGDIDebugger(LPCTSTR szCmdLine, HWND hDialog, HWND hStringListBox)
   : CApplicationDebugger(szCmdLine)
{
   ASSERT(::IsWindow(hDialog));
   m_hDialog         = hDialog;
   m_hStringListBox  = hStringListBox;
}


CGDIDebugger::~CGDIDebugger()
{
   DefaultReset();
}


void CGDIDebugger::AddDebugString(LPCTSTR format, ...)
{
   if (::IsWindow(m_hStringListBox))
   {
      TCHAR szBuffer[4096];
      va_list argptr;

      va_start(argptr, format);
      wvsprintf(szBuffer, format, argptr);
      va_end(argptr);


      int iPos = ::SendMessage(m_hStringListBox, LB_ADDSTRING, 0, LPARAM(szBuffer));
      ::SendMessage(m_hStringListBox, LB_SETCARETINDEX, iPos, FALSE);
   }
}


void CGDIDebugger::GetGDIList(DWORD Notification)
{
// ask the debuggee for its current GDI objects list
   ::RequestList(Notification);
}


void CGDIDebugger::DefaultReset()
{
// do nothing if the map is empty
   if (m_Libraries.IsEmpty())
      return;

// get each element of the map and delete it
   POSITION pos;
   DWORD    key;

// delete one element after the other
   CLibraryInfo* pInfo;

   for (pos = m_Libraries.GetStartPosition(); pos != NULL; )
   {
      m_Libraries.GetNextAssoc(pos, key, pInfo);
      delete pInfo;
   }

// delete the map itself
   m_Libraries.RemoveAll();
}


void CGDIDebugger::OnThreadMessage(LPMSG pMsg)
{
   if (pMsg->message == TM_GET_LIST)
   {
   // notify the parent dialog that the GDI object list is ready
      ::SendMessage(m_hDialog, pMsg->wParam, 0, (LPARAM)GetDataPointer()); 
   }
   else
   {
      TRACE("Unknown Thread message %u (0x%08X)\n", pMsg->message, pMsg->message);
   }
}



DWORD CGDIDebugger::PreLoadingProcess()
{
   DefaultReset();

   return(DP_OK);
}


DWORD CGDIDebugger::OnCreateProcessDebugEvent(CREATE_PROCESS_DEBUG_INFO* pInfo)
{
   AddDebugString(
      _T("Process %u (starting at %08X) has been loaded at: %08X with thread %u"), 
      GetCurrentProcessId(),
      pInfo->lpStartAddress, 
      pInfo->lpBaseOfImage,
      m_ThreadID
      );

// Initialize the GDI objects spy engine
   StartTraceGDI(m_ProcessID, ::GetCurrentThreadId());

   return(DBG_CONTINUE);
}


DWORD CGDIDebugger::OnExitProcessDebugEvent(DWORD dwExitCode)
{
   AddDebugString(_T("Process returns %u (0x%x)"), dwExitCode, dwExitCode);

// Stop the GDI objects spy engine
   StopTraceGDI();

   return(DBG_CONTINUE);
}



DWORD CGDIDebugger::OnOutputDebugStringDebugEvent(OUTPUT_DEBUG_STRING_INFO* pInfo)
{
   TCHAR    szODS[4096];
   unsigned readLen;
   BOOL     fOK;

   if (pInfo->fUnicode)
   {
      AddDebugString(_T("OutputDebugString not unicodified"));
   }
   else
   {
   // We have to read the string from the address space of the debuggee
      readLen = min(sizeof(szODS)/sizeof(TCHAR)-1, pInfo->nDebugStringLength);
      fOK = 
         ReadProcessMemoryEx(
            pInfo->lpDebugStringData, 
            szODS, 
            readLen 
            );

      if (fOK)
      {
         static BOOL bRelocation = FALSE;
         LPTSTR pszTok;

         try
         {
            szODS[(readLen < 4096) ? readLen : (4096-1)] = _T('\0');
            if (
                  (readLen < 2) || 
                  (_tcscmp(szODS, _T("\r\n")) == 0)
               )
            {
            // send info to standard interface
               AddDebugString(_T("> %s"), szODS);
            }
            else
            {
               pszTok = _tcstok(szODS, _T("\r\n"));   // use strtok to split apart
               do                                     // strings separated by \r\n
               {
               // first, check for Windows NT internal messages such as DLL relocation
                  if (_tcsstr(pszTok, _T("LDR: Automatic DLL Relocation")) != NULL)
                     bRelocation = TRUE;
                  else
                  {
                     if (bRelocation)
                     {
                        LPCTSTR pInfo = _tcsstr(pszTok, _T("LDR: "));
                        m_szReason = pInfo + 5; // skip "LDR: "
                     }

                     bRelocation = FALSE;
                  }

               // send info to standard interface
                  AddDebugString(_T("> %s"), pszTok);
               } while (pszTok = strtok(0, "\r\n"));
            }
         }
         catch(...)
         {
         // send info to standard interface
            AddDebugString(_T("> %s"), szODS);
         }         
      }

   }

   return(DBG_CONTINUE);
}


void CGDIDebugger::BuildLibraryName(CLibraryInfo* pInfo, CString& szName)
{
   if (pInfo != NULL)
   {
      CString szInfo;

      szName.Format(
         _T("%s\t%s\t(%u/%u)\t0x%08X\t%s\t0x%08X\t%s"), 
         pInfo->m_szDllName,
         (pInfo->m_bLoaded) ? _T("L") : _T("*"),
         pInfo->m_iRemoved,
         pInfo->m_iLoaded,
         pInfo->m_dwLoadAddress,
         (pInfo->m_dwLoadAddress == pInfo->m_dwPreferedLoadAddress) ? _T("--") : _T("=>"),
         pInfo->m_dwPreferedLoadAddress,
         (pInfo->m_szReason == _T("")) ? _T("ok" : (LPCTSTR)pInfo->m_szReason)
         );
   }
   else
   {
      szName = _T("Unknown library");
   }
}


void CGDIDebugger::AddLibrary(CLibraryInfo* pLibrary)
{
   if (pLibrary != NULL)
   {
      pLibrary->m_bLoaded = TRUE;
      pLibrary->m_iLoaded++;

   // spy for GDI objects
      PatchModule((HMODULE)pLibrary->m_dwLoadAddress);
   }
   else
   {
   // bizarre...
   }
}


DWORD CGDIDebugger::OnLoadDLLDebugEvent(LOAD_DLL_DEBUG_INFO* pInfo)
{
   TCHAR    szDllName[64];
   LPVOID   preferredLoadAddr;

   GetModuleNameInProcess(
      (HMODULE)pInfo->lpBaseOfDll,
      szDllName, 
      sizeof(szDllName)/sizeof(TCHAR)
      );

   AddDebugString(_T("DLL Load: %s at address %08X"), szDllName, pInfo->lpBaseOfDll);

// Check if the actual load address matched the preferred load address
   preferredLoadAddr = GetPreferredLoadAddress(pInfo->lpBaseOfDll);
   if (preferredLoadAddr != pInfo->lpBaseOfDll)
      AddDebugString(_T("  *** %s would prefer to be loaded at %08X ***"), szDllName, preferredLoadAddr);

// store it into the map
   CLibraryInfo* pLibrary;
   if (!m_Libraries.Lookup((LPVOID)(pInfo->lpBaseOfDll), pLibrary))
   {
   // add it into the map
      pLibrary = new CLibraryInfo(szDllName, (DWORD)(pInfo->lpBaseOfDll), (DWORD)preferredLoadAddr, m_szReason);

      if (pLibrary != NULL)
      // store into the map
         m_Libraries[(LPVOID)(pInfo->lpBaseOfDll)] = pLibrary;

   // reset current relocation status
      m_szReason = _T("");
   }
   else
   {
   // DLL is already in the map
   }

// notify the debuggee
   AddLibrary(pLibrary);

   return(DBG_CONTINUE);
}


DWORD CGDIDebugger::OnUnloadDLLDebugEvent(LPVOID lpBaseOfDll)
{
   TCHAR szDllName[64];
   GetModuleNameInProcess(
      (HMODULE)lpBaseOfDll,
      szDllName, 
      sizeof(szDllName)/sizeof(TCHAR)
      );

   CLibraryInfo* pLibrary = NULL;
   if (m_Libraries.Lookup((LPVOID)lpBaseOfDll, pLibrary))
   {
      AddDebugString(_T("DLL Unload: %s"), pLibrary->m_szDllName);
      pLibrary->m_bLoaded = FALSE;
      pLibrary->m_iRemoved++;
   }
   else
      AddDebugString(_T("Unknown (%08X) DLL Unloaded"), lpBaseOfDll);
   
   UnpatchModule((HMODULE)lpBaseOfDll);

   return(DBG_CONTINUE);
}



void CGDIDebugger::ShowList(CGdiResources* pList, HWND hWnd)
{
// sanity checks
   ASSERT(pList != NULL);
   if (pList == NULL)
      return;

   if (pList->GetTotalCount() == 0)
   {
      AfxMessageBox(IDS_NOTHING_TO_DISPLAY, MB_OK | MB_ICONINFORMATION);
      return;
   }

// serialize the snapshot into the shared memory
   GDI_LIST* pData = (GDI_LIST*)GetDataPointer();
   if (pData == NULL)
      return;

   DWORD    CurrentObject = 0;
   POSITION Pos;
   CGdiObj* pGdiObject = NULL;
   LPCTSTR  pDesc = NULL;
   CString  szInfo;
   for (
         pGdiObject = pList->GetFirst(Pos); 
         (pGdiObject != NULL); 
         pGdiObject = pList->GetNext(Pos)
       )
   {
      pData->Items[CurrentObject].dwType   = pGdiObject->GetType();
      pData->Items[CurrentObject].hObject  = (HGDIOBJ)pGdiObject->GetHandle();

      CurrentObject++;
   }
   pData->dwCount = CurrentObject;


// ask the debuggee to display the snapshot
   ShowRemoteList(CurrentObject, hWnd);
}


