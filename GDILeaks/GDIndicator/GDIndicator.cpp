#include "stdafx.h"
#include "GDIndicator.h"

#include "MainFrm.h"
#include "GDIndicatorDoc.h"
#include "GDIndicatorView.h"

#ifdef _DEBUG
   #define new DEBUG_NEW
   #undef THIS_FILE
   static char THIS_FILE[] = __FILE__;
#endif



class CAboutDlg : public CDialog
{
public:
   CAboutDlg();

// Dialog Data
//{{AFX_DATA(CAboutDlg)
   enum { IDD = IDD_ABOUTBOX };
//}}AFX_DATA

// ClassWizard generated virtual function overrides
//{{AFX_VIRTUAL(CAboutDlg)
protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
//}}AFX_VIRTUAL

// Implementation
protected:
//{{AFX_MSG(CAboutDlg)
   // No message handlers
//}}AFX_MSG

   DECLARE_MESSAGE_MAP()
};


BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
//{{AFX_MSG_MAP(CAboutDlg)
   // No message handlers
//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
//{{AFX_DATA_INIT(CAboutDlg)
//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
   CDialog::DoDataExchange(pDX);
//{{AFX_DATA_MAP(CAboutDlg)
//}}AFX_DATA_MAP
}

// App command to run the dialog
void CGDIndicatorApp::OnAppAbout()
{
   CAboutDlg aboutDlg;
   aboutDlg.DoModal();
}



// The one and only CGDIndicatorApp object
//
CGDIndicatorApp theApp;


BEGIN_MESSAGE_MAP(CGDIndicatorApp, CWinApp)
//{{AFX_MSG_MAP(CGDIndicatorApp)
   ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
   // NOTE - the ClassWizard will add and remove mapping macros here.
   //    DO NOT EDIT what you see in these blocks of generated code!
//}}AFX_MSG_MAP

// Standard file based document commands
   ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
END_MESSAGE_MAP()



CGDIndicatorApp::CGDIndicatorApp()
{
// TODO: add construction code here,
// Place all significant initialization in InitInstance
}


BOOL CGDIndicatorApp::InitInstance()
{
// check if we are running under Windows 2000
   OSVERSIONINFO VersionInfo;
   ::ZeroMemory(&VersionInfo, sizeof(VersionInfo));
   VersionInfo.dwOSVersionInfoSize = sizeof(VersionInfo);
   if (
         (!::GetVersionEx(&VersionInfo)) ||
         (VersionInfo.dwPlatformId != VER_PLATFORM_WIN32_NT)
      )
   {
      AfxMessageBox(_T("Sorry but we are only running under Windows NT/2000/XP"), MB_OK | MB_ICONINFORMATION);
      return(FALSE);
   }

// default processing
   if (!CBaseApp::InitInstance())
      return(FALSE);

   LoadStdProfileSettings(0);  // Load standard INI file options


// Register the application's document templates.  Document templates
//  serve as the connection between documents, frame windows and views.
   CSingleDocTemplate* pDocTemplate;
   pDocTemplate = new CSingleDocTemplate(
      IDR_MAINFRAME,
      RUNTIME_CLASS(CGDIndicatorDoc),
      RUNTIME_CLASS(CMainFrame),       // main SDI frame window
      RUNTIME_CLASS(CGDIndicatorView));
   AddDocTemplate(pDocTemplate);

// let's do it!
   CBaseApp::ProcessCommandLine();

// it is a good idea to enhance the level of traces in DEBUG build
#ifdef _DEBUG
   afxDump.SetDepth(1);
#endif

   return(TRUE);
}

