#include "stdafx.h"
#include "BaseClass.h"

#ifdef _DEBUG
   #define new DEBUG_NEW
   #undef THIS_FILE
   static char THIS_FILE[] = __FILE__;
#endif



BEGIN_MESSAGE_MAP(CBaseApp, CWinApp)
//{{AFX_MSG_MAP(CBaseApp)
   // NOTE - the ClassWizard will add and remove mapping macros here.
//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CBaseApp::CBaseApp()
{
}

CBaseApp::~CBaseApp()
{
}


BOOL CBaseApp::InitInstance()
{
// change the name of the .INI file
//
// First free the string allocated by MFC at CWinApp startup.
// The string is allocated before InitInstance is called and released 
// in CWinApp destructor
   free((void*)m_pszProfileName);

// build the filename according to the application filename
   TCHAR szINIFile[_MAX_PATH];
   ::GetModuleFileName(NULL, szINIFile, _MAX_PATH);
   _tcsrchr(szINIFile, _T('.'))[0] = _T('\0');
   _tcscat(szINIFile, _T(".ini"));
   m_pszProfileName = _tcsdup(szINIFile);


   return(TRUE);
}


BOOL CBaseApp::ProcessCommandLine()
{
// Parse command line for standard shell commands, DDE, file open
   CCommandLineInfo cmdInfo;
   ParseCommandLine(cmdInfo);

//--DOC
// get the main frame display mode for later
// if you don't do this, ProcessShellCommand() will begin to mess around
// with the frame 
// --> see SUPERPAD sample code for more
   AfxGetApp()->m_nCmdShow = SW_HIDE;

// Dispatch commands specified on the command line
   if (!ProcessShellCommand(cmdInfo))
      return(FALSE);

// if you want to take care of minimized/maximized states, you should
// do some work 
   ((CBaseFrame*)m_pMainWnd)->RestoreFramePosition();

   return(TRUE);
}


BOOL CBaseApp::CopyTextToClipboard(LPCTSTR szText)
{
// defensive programming
   ASSERT(szText != NULL);

// open the clipboard for the application main window
// --> impossible to work if there is no such a window...
   ASSERT(::IsWindow(m_pMainWnd->m_hWnd));
   if (!::OpenClipboard(m_pMainWnd->m_hWnd))
      return(FALSE);

// return value
   BOOL bReturn = FALSE;

// empty the clipboard 
   ::EmptyClipboard();

// create a memory block to be sent to the clipboard
   DWORD    dwTextSize = _tcslen(szText) + 1;
   HGLOBAL  hText = ::GlobalAlloc(GHND, dwTextSize * sizeof(TCHAR));
   if (hText != NULL)
   {
      LPSTR lpText = (LPSTR)::GlobalLock(hText);
      if (lpText != NULL)
      {
      // copy the given string into the memory block
         _tcscpy(lpText, szText);

      // don't forget to unlock the memory block
         ::GlobalUnlock(hText);

      // sent it the clipboard
         ::SetClipboardData(CF_TEXT, hText);
   
      // everything is under control
         bReturn = TRUE;
      }
   }

// don't forget to close the clipboard
   ::CloseClipboard();

   return(bReturn);
}





IMPLEMENT_DYNCREATE(CBaseFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CBaseFrame, CFrameWnd)
//{{AFX_MSG_MAP(CBaseFrame)
   ON_WM_CREATE()
   ON_WM_DESTROY()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()


//--------------------------------------------------------------------
// Construction/Destruction
//
CBaseFrame::CBaseFrame()
{
   m_pPositionHelper = NULL;
}

CBaseFrame::~CBaseFrame()
{
   delete m_pPositionHelper;
}
//
//--------------------------------------------------------------------



//--------------------------------------------------------------------
// Initialization methods
// 
int CBaseFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
   if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
      return(-1);
   
// create the save/load position helper
   m_pPositionHelper = new CWndSaveRestoreHelper(m_hWnd, _T("Settings"), _T("MainWindowPosition"));
   
   if (m_pPositionHelper == NULL)
      return(-1);

   return(0);
}
//
//--------------------------------------------------------------------


//--------------------------------------------------------------------
// Message handlers
//

// don't forget to save the position and size of the frame
// before it is destroyed
void CBaseFrame::OnDestroy() 
{
// save the frame position
   if (m_pPositionHelper != NULL)
      m_pPositionHelper->Save();

// default processing
   CFrameWnd::OnDestroy();
}
//
//--------------------------------------------------------------------


//--------------------------------------------------------------------
// public helpers
//
void CBaseFrame::RestoreFramePosition()
{
   if (m_pPositionHelper != NULL)
   {
      if (!m_pPositionHelper->Load())
         CenterWindow();
      
      if (!IsWindowVisible())
         ShowWindow(SW_SHOW);
   }
}
//--------------------------------------------------------------------



