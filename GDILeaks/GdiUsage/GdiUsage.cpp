#include "stdafx.h"
#include "GdiUsage.h"
#include "GdiUsageDlg.h"

#ifdef _DEBUG
   #define new DEBUG_NEW
   #undef THIS_FILE
   static char THIS_FILE[] = __FILE__;
#endif


BEGIN_MESSAGE_MAP(CGdiUsageApp, CWinApp)
//{{AFX_MSG_MAP(CGdiUsageApp)
   // NOTE - the ClassWizard will add and remove mapping macros here.
   //    DO NOT EDIT what you see in these blocks of generated code!
//}}AFX_MSG

   ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// The one and only application object
//
CGdiUsageApp theApp;


// CGdiUsageApp construction
//
CGdiUsageApp::CGdiUsageApp() : CWinApp("GDI Usage")
{
}


// CGdiUsageApp initialization
//
BOOL CGdiUsageApp::InitInstance()
{
/*
// check if we are running under Windows 2000
   OSVERSIONINFO VersionInfo;
   ::ZeroMemory(&VersionInfo, sizeof(VersionInfo));
   VersionInfo.dwOSVersionInfoSize = sizeof(VersionInfo);
   if (
         (!::GetVersionEx(&VersionInfo)) ||
         (VersionInfo.dwPlatformId != VER_PLATFORM_WIN32_NT) ||
         (VersionInfo.dwMajorVersion != 5)
      )
   {
      AfxMessageBox(IDS_WIN2000_ONLY, MB_OK | MB_ICONINFORMATION);
      return(FALSE);
   }
*/


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
//
// Note:
// It might not be a good idea to use a .INI file stored in the 
// same directory as the application. In fact, Program Files folder
// is forbidden in write access for a standard user. But since we
// need to be an administrator to debug an application, this should
// not be an issue in our case :^)


// Standard initialization
   AfxEnableControlContainer();

// display dialog as the main window
   CGdiUsageDlg dlg;
   m_pMainWnd = &dlg;
   dlg.DoModal();

// Since the dialog has been closed, return FALSE so that we exit the
//  application, rather than start the application's message pump.
   return FALSE;
}
