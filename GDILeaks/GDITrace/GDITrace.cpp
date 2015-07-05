//
// Note!
//
//    If this DLL is dynamically linked against the MFC
//    DLLs, any functions exported from this DLL which
//    call into MFC must have the AFX_MANAGE_STATE macro
//    added at the very beginning of the function.
//
//    For example:
//
//    extern "C" BOOL PASCAL EXPORT ExportedFunction()
//    {
//       AFX_MANAGE_STATE(AfxGetStaticModuleState());
//       // normal function body here
//    }
//
//    It is very important that this macro appear in each
//    function, prior to any calls into MFC.  This means that
//    it must appear as the first statement within the 
//    function, even before any object variable declarations
//    as their constructors may generate calls into the MFC
//    DLL.
//
//    Please see MFC Technical Notes 33 and 58 for additional
//    details.
//


#include "stdafx.h"
#include <process.h>

#include "GDITrace.h"
#include "..\Common\Wrappers.h"
#include "..\Common\Helpers.h"


// force functions export
//
#define _GDITRACE_EXPORT
#include "_GDITrace.h"



#ifdef _DEBUG
   #define new DEBUG_NEW
   #undef THIS_FILE
   static char THIS_FILE[] = __FILE__;
#endif



// The one and only CGDITraceApp object
//
CGDITraceApp theApp;



//------------------------------------------------------------------
// shared variables
//
#pragma data_seg(".shared")
// debugger
   DWORD s_dwCallingThreadID  = 0;

// debuggee 
   DWORD s_dwInfiltratedThreadID = 0;
   DWORD s_dwProcessID           = 0;

// API hooking initialization flag
// --> set to TRUE once the injected hook function has been called
   BOOL  s_bDebuggeeIsStarted = FALSE; 
#pragma data_seg()

#pragma comment(linker, "-section:.shared,rws")
//
//------------------------------------------------------------------



//------------------------------------------------------------------
// global variables
//
const DWORD MAX_MEMSIZE = MAX_GDI_ITEMS * sizeof(GDI_ITEM) + sizeof(DWORD);
LPCTSTR     g_szEventName = _T("EVENT_GDITRACE_INFILTRATED");
//
//------------------------------------------------------------------


//------------------------------------------------------------------
// DLL Interface functions: called by the debugger
//
void WINAPI StartTraceGDI(DWORD ProcessID, DWORD NotifThreadID)
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());

   s_bDebuggeeIsStarted = FALSE;

// default init
   theApp.SetSharedVariables(ProcessID, NotifThreadID);
}


void WINAPI StopTraceGDI()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());

   theApp.ResetSharedVariables();
   s_bDebuggeeIsStarted = FALSE;
}


BOOL WINAPI PatchModule(HMODULE hModule)
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());

// we only accept when spy engine is started
   if (!s_bDebuggeeIsStarted)
      return(FALSE);

   return(::PostThreadMessage(s_dwInfiltratedThreadID, TM_NEW_DLL, 0, (LPARAM)hModule));
}


BOOL WINAPI UnpatchModule(HMODULE hModule)
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());

   return(::PostThreadMessage(s_dwInfiltratedThreadID, TM_FREE_DLL, 0, (LPARAM)hModule));
}


BOOL WINAPI RequestList(DWORD Notification)
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());

   return(::PostThreadMessage(s_dwInfiltratedThreadID, TM_GET_LIST, Notification, 0));
}


BOOL WINAPI ShowRemoteList(DWORD Count, HWND hWnd)
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());

   return(::PostThreadMessage(s_dwInfiltratedThreadID, TM_SHOW_LIST, Count, (LPARAM)(hWnd)));
}


BOOL WINAPI GetDataFromRequest(LPVOID pData, DWORD cbSize)
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());

   return(theApp.GetDataFromRequest(pData, cbSize));
}


BOOL WINAPI SetDataForRequest(LPVOID pData, DWORD cbSize)
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());

   return(theApp.SetDataForRequest(pData, cbSize));
}


LPVOID WINAPI GetDataPointer()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());

   return(theApp.GetDataPointer());
}
//
//------------------------------------------------------------------



//------------------------------------------------------------------
// Application class methods: called by the debugger
//
BOOL CGDITraceApp::InstallHook()
{
   HINSTANCE hInstance = AfxGetInstanceHandle();
   s_hHook = ::SetWindowsHookEx(WH_GETMESSAGE, GetMessageHookProc, hInstance, 0);

// trace if failure
   if (s_hHook == NULL)
   {
      LPVOID lpMsgBuf;
      ::FormatMessage(
         FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
         NULL,
         ::GetLastError(),
         MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
         (LPTSTR) &lpMsgBuf,
         0,
         NULL
         );

      TRACE("CGDITraceApp::InstallHook() failed: %s\n", lpMsgBuf);

   // Free the buffer.
      LocalFree(lpMsgBuf);
   }      
   else
   {
      TRACE("CGDITraceApp::InstallHook is successful\n");
   }

   return(s_hHook != NULL);
}


BOOL CGDITraceApp::UninstallHook()
{
   TRACE("CGDITraceApp::UninstallHook()\n");

   if (s_hHook == NULL)
      return(TRUE);

   BOOL bReturn = ::UnhookWindowsHookEx(s_hHook);
   s_hHook = NULL;
   return(bReturn);
}


void CGDITraceApp::SetSharedVariables(DWORD ProcessID, DWORD NotifThreadID)
{
// set the shared global variables
   s_dwCallingThreadID  = NotifThreadID;
   s_dwProcessID        = ProcessID;

   TRACE(
      "CGDITraceApp::SetSharedVariables(%u --> %u)\n", 
      s_dwCallingThreadID, s_dwProcessID
      );

// install the system hook
   InstallHook();
}


// Note: 
//    this methods is supposed to be used before the debuggee
//    dies to ask the infiltrated thread to end its life using
//    an event.
//    Unfortunately, in the current implementation, this method
//    gets called when the debugger receives the notification
//    that the debuggee is dead. Too late...
//    That's why the event could not be opened and you get the 
//    following trace message:
//
// CGDITraceApp::StopInfiltratedThread()
// StopInfiltratedThread() failed: impossible to open the event...
//    The system cannot find the file specified.
//   
// 
BOOL CGDITraceApp::StopInfiltratedThread()
{
   TRACE("CGDITraceApp::StopInfiltratedThread()\n");

   HANDLE hEvent = ::OpenEvent(EVENT_MODIFY_STATE, FALSE, g_szEventName);
   if (hEvent == NULL)
   {
      LPVOID lpMsgBuf;
      ::FormatMessage(
         FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
         NULL,
         ::GetLastError(),
         MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
         (LPTSTR) &lpMsgBuf,
         0,
         NULL
         );

      TRACE("StopInfiltratedThread() failed: impossible to open the event...\n   %s\n", lpMsgBuf);

   // Free the buffer.
      LocalFree(lpMsgBuf);

      return(FALSE);
   }
   
// signal the event to wake the infiltrated thread up
   ::SetEvent(hEvent);
   ::Sleep(0);
   
// don't forget to free resource
   ::CloseHandle(hEvent);

   return(TRUE);
}


void CGDITraceApp::ResetSharedVariables()
{
// remove the system hook
   UninstallHook();

// stop the infiltrated thread
//
// in fact, this call is useless since the debuggee is already dead
// at the time this method is called
   StopInfiltratedThread();
   
// reset shared variables
   s_dwCallingThreadID     = 0;
   s_dwInfiltratedThreadID = 0;
   s_dwProcessID           = 0;
}
//
//------------------------------------------------------------------



//------------------------------------------------------------------
// static members used in the debuggee only
//
HHOOK CGDITraceApp::s_hHook   = NULL;
HANDLE CGDITraceApp::s_hEvent = NULL;
//
//------------------------------------------------------------------



// Even a DLL needs a message map...
//
BEGIN_MESSAGE_MAP(CGDITraceApp, CWinApp)
//{{AFX_MSG_MAP(CGDITraceApp)
   // NOTE - the ClassWizard will add and remove mapping macros here.
   //    DO NOT EDIT what you see in these blocks of generated code!
//}}AFX_MSG_MAP
END_MESSAGE_MAP()


//------------------------------------------------------------------
// DLL initialization and termination
// 
CGDITraceApp::CGDITraceApp()
{
// handle to file mapping
   m_hMapObject   = NULL;
   m_lpvMem       = NULL;
}


BOOL CGDITraceApp::InitInstance() 
{
// create the shared memory if we are 
// either the debugger or the debuggee
   if (
         (s_dwProcessID == 0) ||
         (s_dwProcessID == ::GetCurrentProcessId())
      )
   {
   // create the shared memory mapped file
      m_hMapObject = 
         ::CreateFileMapping( 
            (HANDLE) 0xFFFFFFFF,          // use paging file
            NULL,                         // no security attributes
            PAGE_READWRITE,               // read/write access
            0,                            // size: high 32-bits
            MAX_MEMSIZE,                  // size: low 32-bits
            _T("GDITrace_SharedBuffer")   // name of map object
         );

      if (m_hMapObject == NULL)
      {
         TRACE("Impossible to create file mapping...\n");
         return(FALSE);
      }

   // Get a pointer to the file-mapped shared memory. 
      m_lpvMem = 
         MapViewOfFile( 
            m_hMapObject,     // object to map view of
            FILE_MAP_WRITE,   // read/write access
            0,                // high offset:  map from
            0,                // low offset:   beginning
            0                 // default: map entire file
            );

      if (m_lpvMem == NULL)
      {
         TRACE("Impossible to map view...\n");
         return(FALSE);
      }

   // init the spy engine for the debuggee 
      if (s_dwProcessID == ::GetCurrentProcessId())
      {
      // start the spy engine for the right process only
         TRACE("Mapped into process %u address space...\n", s_dwProcessID);

         if (!m_GDISpy.Init())
            TRACE("m_GDISpy.Init() failed for process %u...\n", s_dwProcessID);

      // create the event used to synchronize the caller and the callee
      //    auto-reset and non signaled by default
         s_hEvent = ::CreateEvent(NULL, FALSE, FALSE, g_szEventName);
         if (s_hEvent == NULL)
         {
            LPVOID lpMsgBuf;
            ::FormatMessage(
               FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
               NULL,
               ::GetLastError(),
               MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
               (LPTSTR) &lpMsgBuf,
               0,
               NULL
               );

            TRACE("Impossible to create the communication event...\n   %s\n", lpMsgBuf);

         // Free the buffer.
            LocalFree(lpMsgBuf);
         }

         TRACE("Loaded into the debuggee at 0x%x\n", m_hInstance);
      }
      else
      {
      // The debugger initialize memory
         ::ZeroMemory(m_lpvMem, MAX_MEMSIZE);  
      }
   }

// default MFC processing
   return(CWinApp::InitInstance());
}


int CGDITraceApp::ExitInstance() 
{
   if (s_dwProcessID == ::GetCurrentProcessId())
   {
   // dump leaks if we are in the spyed process 
   // --> close to the MFC allocator/desallocator behaviour
      m_GDISpy.DumpLeaks();

   // don't forget to free system resource
      ::CloseHandle(s_hEvent);

   // reset the shared variables
      s_dwInfiltratedThreadID = 0;
      s_dwCallingThreadID     = 0;
      s_dwProcessID           = 0;
      s_bDebuggeeIsStarted    = FALSE;
   }


// Unmap shared memory from the process's address space. 
   if (m_lpvMem != NULL)
      ::UnmapViewOfFile(m_lpvMem);  

// Close the process's handle to the file-mapping object. 
   if (m_hMapObject != NULL)
      ::CloseHandle(m_hMapObject); 


// default MFC processing
   return(CWinApp::ExitInstance());
}
//
//------------------------------------------------------------------


//------------------------------------------------------------------
// this code runs inside the context of the debuggee
// 
void CGDITraceApp::OnNewDLL(HMODULE hModule)
{
// we only accept when spy engine is started
   if (!s_bDebuggeeIsStarted)
   {
      TRACE("CGDITraceApp::OnNewDLL() called when debuggee is not started...\n");
      return;
   }
   
   if (hModule == 0)
   {
      NEWMODULELIST* pList = (NEWMODULELIST*)m_lpvMem;
  
      TRACE("CGDITraceApp::OnNewDLL(0) --> spy the %u following DLLs\n", pList->dwCount);
      TCHAR szModuleName[_MAX_PATH+1];
      
      HMODULE ModuleAlreadyInTheMap;

   // attach to every already loaded module
      for (DWORD dwCount = 0; dwCount < pList->dwCount; dwCount++)
      {
         if (m_LoadedModules.Lookup(pList->hModules[dwCount], ModuleAlreadyInTheMap))
         {
         // skip the already patched DLLs
         }
         else
         {
            if (::GetModuleFileName(pList->hModules[dwCount], szModuleName, _MAX_PATH))
            {
               TRACE(" %02u\n", dwCount+1);
               m_LoadedModules[pList->hModules[dwCount]] = pList->hModules[dwCount];
               m_GDISpy.DoReflect(pList->hModules[dwCount]);
            }
            else
               TRACE(" %02u   ???   no spy\n", dwCount+1);
         }
      }
      TRACE("-----------------------------------------------------\n");
   }
   else
   {
      HMODULE ModuleAlreadyInTheMap;
      if (m_LoadedModules.Lookup(hModule, ModuleAlreadyInTheMap))
      {
      // skip the already patched DLLs
      }
      else
      {
         TCHAR szModuleName[_MAX_PATH+1];
         if (::GetModuleFileName(hModule, szModuleName, _MAX_PATH))
         {
            m_LoadedModules[hModule] = hModule;
            m_GDISpy.DoReflect(hModule);
            TRACE("   CGDITraceApp::OnNewDLL(0x%x -- %s) --> spy it\n", hModule, szModuleName);
         }
         else
            TRACE("   CGDITraceApp::OnNewDLL(0x%x -- ???) --> no spy\n", hModule);
      }
   }
}


void CGDITraceApp::OnFreeDLL(HMODULE hModule)
{
   HMODULE ModuleAlreadyInTheMap;
   if (m_LoadedModules.Lookup(hModule, ModuleAlreadyInTheMap))
   {
   // remove it from the map
      m_LoadedModules.RemoveKey(hModule);
   }
   else
   {
   // this should never occur
   }
}


void CGDITraceApp::OnGetList(DWORD Notification)
{
// fill the shared memory with the list
   GDI_LIST*      pList = (GDI_LIST*)m_lpvMem;
   DWORD          Count = 0;
   HANDLE         hObject;
   CHandleInfo*   pInfo;
   for (POSITION pos = m_GDISpy.GetFirstItem(hObject, pInfo); ; pos = m_GDISpy.GetNextItem(pos, hObject, pInfo))
   {
      if (hObject != NULL)
      {
         pList->Items[Count].dwType   = pInfo->m_Type;
         pList->Items[Count].hObject  = (HGDIOBJ)hObject;
         Count++;
      }

      if (pos == NULL)
         break;
   }

   pList->dwCount = Count;

// notify the calling thread that data is ready to use
   ::PostThreadMessage(s_dwCallingThreadID, TM_GET_LIST, Notification, 0);
}


void CGDITraceApp::OnDoubleClick(HWND hWnd, DWORD hObject)
{
// create the C++ dialog
   CWnd* pWndParent = CWnd::GetActiveWindow();
   CCallStackDlg Dlg(pWndParent);

// get the call stack manager
   CStackManager* pStackManager = m_GDISpy.GetStackManager();
   Dlg.SetStackManager(pStackManager);

// get the call stack for the GDI object
   CHandleInfo* pInfo = m_GDISpy.GetHandleInfo((HANDLE)hObject);
   if (pInfo != NULL)
   {
      Dlg.SetCallStack(pInfo->m_Depth, pInfo->m_pStack);

   // display the dialog
      Dlg.DoModal();
   }
   else
   {
      ::MessageBox(
         ::GetActiveWindow(), 
         _T("This object has been deleted"), 
         _T("No call stack..."), 
         MB_ICONINFORMATION|MB_OK
         );
   }
}


void CGDITraceApp::OnSelect(HWND hWnd, DWORD hObject)
{
// nothing special to do here...
//
// if the stack dialog would have been modeless, 
// its content could have been updated here
}


void CGDITraceApp::OnShowList(DWORD Count, HWND hWnd)
{
static bListAlreadyThere = FALSE;

// avoid reentrancy
   if (!bListAlreadyThere)
   {
      bListAlreadyThere = TRUE;

   // get a MFC object linked to the given window handle
      CWnd ParentWnd;
      ParentWnd.Attach(hWnd);   

      GDI_LIST* pList = (GDI_LIST*)m_lpvMem;
      try
      {
      // create a CGdiResources from the GDITRACELIST received through the shared buffer 
         CGdiResources SnapshotReceived;
      
      // sanity checks
         if (pList->dwCount == Count)
         {
         // translate the list into something the dialog will understand
            DWORD ObjectCount = SnapshotReceived.CreateFromList(pList);

         // sanity checks
            if (ObjectCount == pList->dwCount)
            {
               CGdiResourcesDlg Dlg(&ParentWnd, &SnapshotReceived);
               Dlg.SetNotificationCallBack(this);
               Dlg.DoModal();
            }
            else
            {
               TRACE("OnShowList(): impossible to build the list...\n");
            }
         }
         else
         {
            TRACE("%u != %u\n", Count, pList->dwCount);
            LPDWORD pDWORD = (LPDWORD)pList;
            TRACE("OnShowList(): invalid TM_SHOW_LIST received...\n");
            TRACE(
               "%x %x %x %x %x %x %x %x\n", 
               pDWORD[0], 
               pDWORD[1], 
               pDWORD[2], 
               pDWORD[3], 
               pDWORD[4], 
               pDWORD[5], 
               pDWORD[6], 
               pDWORD[7]
               );
            TRACE(
               "%u %u %u %u %u %u %u %u\n", 
               pDWORD[0], 
               pDWORD[1], 
               pDWORD[2], 
               pDWORD[3], 
               pDWORD[4], 
               pDWORD[5], 
               pDWORD[6], 
               pDWORD[7]
               );
         }
      }
      catch(...)
      {
      }
      ParentWnd.Detach();

      bListAlreadyThere = FALSE;
   }
   else
   {
      TRACE("OnShowList(): reentrancy is forbidden here...\n");
   }
}


unsigned __stdcall CGDITraceApp::InfiltratedThreadProc(LPVOID pData)
{
   do
   {
   // wake up when a either a 
      DWORD dwCause = 
         ::MsgWaitForMultipleObjectsEx(
            1, 
            &s_hEvent,
            INFINITE, 
            QS_ALLEVENTS, 
            MWMO_INPUTAVAILABLE
            );
   
      if (dwCause == WAIT_OBJECT_0)
      {
      // the synchronization event has been signaled
      // --> the thread must end now
      //
         TRACE("InfiltratedThreadProc(): stop the thread right now\n");
         return(0); 
      }
      else
      if (dwCause == WAIT_OBJECT_0 + 1)
      {
      // one or more messages have been posted to our thread
         MSG msg;
         while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
         {
            if (msg.hwnd == NULL)
            {
            // patch GDI calls for this new dll
            //--> lParam contains the loading address of this DLL
               if (msg.message == TM_NEW_DLL)
               {
                  TRACE(".TM_NEW_DLL (0x%x) \n", msg.lParam);
                  theApp.OnNewDLL((HMODULE)msg.lParam);
               }
               else
            // this DLL is unloaded
            //--> lParam contains the loading address of this DLL
               if (msg.message == TM_FREE_DLL)
               {
                  TRACE(".TM_FREE_DLL (0x%x) \n", msg.lParam);
                  theApp.OnFreeDLL((HMODULE)msg.lParam);
               }
               else
            // send the list to the calling thread
               if (msg.message == TM_GET_LIST)
               {
                  theApp.OnGetList(msg.wParam);
               }
               else
            // show the list
               if (msg.message == TM_SHOW_LIST)
               {
                  theApp.OnShowList(msg.wParam, (HWND)msg.lParam);
               }
               else
               {
                  TRACE("   unknown %u received\n", msg.message);
               }
            }
            else
            {
            // this should never happen...
            }
         }
      }
      else
      {
      // should never happen...
         return(-1);
      }
   }
   while (TRUE);

   return(0);
}


BOOL CGDITraceApp::StartInfiltratedThread()
{
   TRACE("CGDITraceApp::StartInfiltratedThread()\n");

// create the infiltrated thread which waits for remote commands
   HANDLE hThread = (HANDLE)
      _beginthreadex(
         NULL, 
         0, 
         InfiltratedThreadProc, 
         NULL, 
         0, 
         (unsigned int*)&s_dwInfiltratedThreadID
         );
   if (hThread != NULL)
   {
      ::SetThreadPriority(hThread, THREAD_PRIORITY_HIGHEST);
      ::CloseHandle(hThread);

      TRACE("CGDITraceApp::StartInfiltratedThread --> %u as thread ID\n", s_dwInfiltratedThreadID);

      return(TRUE);
   }
   
   TRACE("CGDITraceApp::StartInfiltratedThread(): impossible to create the thread...\n");
   return(FALSE);
}


void CGDITraceApp::HookAlreadyLoadedDLLs()
{
   TRACE("CGDITraceApp::HookAlreadyLoadedDLLs()\n");

// TO DO: replace this code using ..\Common\CModuleList
//

// check each memory block from our process address space
// to find the loaded DLLs
//    from Jeffrey Richter  
//    "Programming Applications For Windows Fourth Edition"
   PBYTE pBlock = NULL;
   MEMORY_BASIC_INFORMATION mbi;
   DWORD dWLen = 0;
   DWORD dwCount = 0;
   TCHAR szPathName[MAX_PATH];
   while (::VirtualQuery(pBlock, &mbi, sizeof(mbi)) == sizeof(mbi)) 
   {
      if (mbi.State == MEM_FREE)
         mbi.AllocationBase = mbi.BaseAddress;

   // Don't care about the following cases:
   //    1. If this region contains this DLL
   //    2. If this block is NOT the beginning of a region
   //    3. If the address is NULL
      if (
            (mbi.AllocationBase != AfxGetInstanceHandle()) &&
            (mbi.AllocationBase == mbi.BaseAddress) &&
            (mbi.AllocationBase != NULL)
         ) 
      {
         if (GetModuleFileName((HMODULE)mbi.AllocationBase, szPathName, _MAX_PATH) != NULL)
         {
         // hook this DLL
            OnNewDLL((HMODULE)mbi.AllocationBase);
            ++dwCount;
         }
      }

   // next memory block please
      pBlock += mbi.RegionSize;
   }

   TRACE("--%u DLLs----------------------------------------------\n", dwCount);
}



LRESULT CGDITraceApp::GetMessageHookProc(int iCode, WPARAM wParam, LPARAM lParam)
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());

   MSG*  pMessage = (MSG*)lParam;   

   if (::GetCurrentProcessId() == s_dwProcessID)
   {
   // try to hook already loaded DLLs when the first message is intercepted
      if (!s_bDebuggeeIsStarted) 
      {
      // now, we are initialized
         s_bDebuggeeIsStarted = TRUE;
         theApp.HookAlreadyLoadedDLLs();
         theApp.StartInfiltratedThread();
      }
   }

// chain to the next hook
   return(::CallNextHookEx(s_hHook, iCode, wParam, lParam));
}
//
//------------------------------------------------------------------



//------------------------------------------------------------------
// general helpers
// 
BOOL CGDITraceApp::GetDataFromRequest(LPVOID pData, DWORD cbSize)
{
   BOOL bReturn = TRUE;

   memcpy(pData, m_lpvMem, min(cbSize, MAX_MEMSIZE));

   return(cbSize <= MAX_MEMSIZE);   
}


BOOL CGDITraceApp::SetDataForRequest(LPVOID pData, DWORD cbSize)
{
   BOOL bReturn = TRUE;

   if (cbSize <= MAX_MEMSIZE)
      memcpy(m_lpvMem, pData, cbSize);
   else
      bReturn = FALSE;

   return(bReturn);
}


LPVOID CGDITraceApp::GetDataPointer()
{
   return(m_lpvMem);
}
//
//------------------------------------------------------------------


