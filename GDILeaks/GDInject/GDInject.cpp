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
#include "GDInject.h"
#include "..\Common\Inject.h"
#include "..\Common\Helpers.h"
#include "..\Common\GdiResources.h"
#include "..\Common\GdiResourcesDlg.h"



// force functions export
//
#define _GDINJECT_EXPORT
#include "_GDInject.h"



#ifdef _DEBUG
   #define new DEBUG_NEW
   #undef THIS_FILE
   static char THIS_FILE[] = __FILE__;
#endif


// shared variables
//
#pragma data_seg(".shared")
// debuggee 
   DWORD s_dwProcessID  = 0;

// communication details
   DWORD s_Action       = 0;
#pragma data_seg()

#pragma comment(linker, "-section:.shared,rws")


// global variables
//
const DWORD MAX_MEMSIZE = MAX_GDI_ITEMS * sizeof(GDI_ITEM) + sizeof(DWORD);


// The one and only CGDInjectApp object
//
CGDInjectApp theApp;



// Exported API
// 
BOOL WINAPI ExecuteRemoteAction(DWORD ProcessID, DWORD Action)
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());

   BOOL bReturn = FALSE;

// inject ourself into the process
   TCHAR szFilename[_MAX_PATH];
   if (::GetModuleFileName(AfxGetInstanceHandle(), szFilename, _MAX_PATH) != 0)
   {
   // I want to be sure the injection will succeed: 
   // --> enable the DEBUG privilege for the process
      EnablePrivilege(SE_DEBUG_NAME);

   // here is the action to execute
      s_Action      = Action;
      s_dwProcessID = ProcessID;

   // let's go: the action is done in InitInstance()
      bReturn = InjectLib(ProcessID, szFilename);

   // remove ourself from the process once the action executed
      if (bReturn)
         EjectLib(ProcessID, szFilename);
   }

   return(bReturn);
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




BEGIN_MESSAGE_MAP(CGDInjectApp, CWinApp)
//{{AFX_MSG_MAP(CGDInjectApp)
   // NOTE - the ClassWizard will add and remove mapping macros here.
   //    DO NOT EDIT what you see in these blocks of generated code!
//}}AFX_MSG_MAP
END_MESSAGE_MAP()



// Construction/Destruction
//
CGDInjectApp::CGDInjectApp()
{
// handle to file mapping
   m_hMapObject   = NULL;
   m_lpvMem       = NULL;
}


BOOL CGDInjectApp::InitInstance() 
{
// create the shared memory if we are 
// either the caller or the callee
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
            _T("GDInject_SharedBuffer")   // name of map object
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

   // create the responding thread if in the callee
      if (s_dwProcessID == ::GetCurrentProcessId())
      {
      // start the spy engine for the right process only
         TRACE(
            "Mapped into process %u address space with %u command...\n", 
            s_dwProcessID, s_Action
            );

      // execute the action
         switch(s_Action)
         {
            case TM_SHOW_LIST:
               theApp.DoShowList();
            break;

            default:
            break;
         }
      
      // reset current action to execute
         s_Action = 0;
      }
      else
      {
      // The caller initialize memory
         ::ZeroMemory(m_lpvMem, MAX_MEMSIZE);  
      }
   }


// default MFC processing
   return(CWinApp::InitInstance());
}


int CGDInjectApp::ExitInstance() 
{
   if (s_dwProcessID != 0)
   {
   // reset the shared variables
      s_dwProcessID = 0;
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


LPVOID CGDInjectApp::GetDataPointer()
{
   return(m_lpvMem);
}



void CGDInjectApp::DoGetTypes()
{
// iterate each supposed GDI object and retrieve its type
   GDI_LIST* pList = (GDI_LIST*)m_lpvMem;
   DWORD     Count = pList->dwCount;
   for (DWORD Current = 0; Current < pList->dwCount; Current++)
   {
      pList->Items[Current].dwType = ::GetObjectType(pList->Items[Current].hObject);
   }
}


void CGDInjectApp::DoShowList()
{
static bListAlreadyThere = FALSE;

// avoid reentrancy
   if (!bListAlreadyThere)
   {
      bListAlreadyThere = TRUE;

      GDI_LIST* pList = (GDI_LIST*)m_lpvMem;
      try
      {
         TRACE("DoShowList(%u)\n", pList->dwCount);

      // create a CGdiResources from the GDITRACELIST received through the shared buffer 
         CGdiResources SnapshotReceived;
      
      // translate the list into something the dialog will understand
         DWORD ObjectCount = SnapshotReceived.CreateFromList(pList);

      // sanity checks
         if (ObjectCount == pList->dwCount)
         {
            CGdiResourcesDlg Dlg(NULL, &SnapshotReceived);
            int iReturn = Dlg.DoModal();
         }
         else
         {
            TRACE("OnShowList(): impossible to build the list...\n");
         }
      }
      catch(...)
      {
      }

      bListAlreadyThere = FALSE;
   }
   else
   {
      TRACE("OnShowList(): reentrancy is forbidden here...\n");
   }
}


BOOL CGDInjectApp::GetDataFromRequest(LPVOID pData, DWORD cbSize)
{
// sanity check
   if (m_lpvMem == NULL)
      return(FALSE);

   memcpy(pData, m_lpvMem, min(cbSize, MAX_MEMSIZE));
   return(cbSize <= MAX_MEMSIZE);   
}


BOOL CGDInjectApp::SetDataForRequest(LPVOID pData, DWORD cbSize)
{
// sanity check
   if (m_lpvMem == NULL)
      return(FALSE);

   BOOL bReturn = TRUE;

   if (cbSize <= MAX_MEMSIZE)
      memcpy(m_lpvMem, pData, cbSize);
   else
      bReturn = FALSE;

   return(bReturn);
}


