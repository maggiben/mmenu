#include "stdafx.h"
#include "resource.h"
#include "GdiUsageDlg.h"
#include "GDIDebugger.h"
#include "FindDialog.h"
#include "..\Common\ApplicationDebugger.h"
#include "..\GDITrace\_GDITrace.h"


#ifdef _DEBUG
   #define new DEBUG_NEW
   #undef THIS_FILE
   static char THIS_FILE[] = __FILE__;
#endif



//------------------------------------------------------------------
// CAboutDlg dialog used for App About
//
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
//
//------------------------------------------------------------------



//------------------------------------------------------------------
// CGdiUsageDlg dialog
//

// define static members
// 
CGDIDebugger* CGdiUsageDlg::m_pGDIDebugger = NULL;


BEGIN_MESSAGE_MAP(CGdiUsageDlg, CDialog)
//{{AFX_MSG_MAP(CGdiUsageDlg)
   ON_WM_PAINT()
   ON_BN_CLICKED(IDC_BTN_START, OnBtnStart)
   ON_COMMAND(ID_DELETEALL, OnDeleteAll)
   ON_COMMAND(ID_COPY_TO_CLIPBOARD, OnCopyToClipboard)
   ON_COMMAND(ID_FIND, OnFind)
   ON_BN_CLICKED(ID_BTN_TAKE_SNAPSHOT, OnBtnTakeSnapshot)
   ON_BN_CLICKED(ID_BTN_COMPARE, OnBtnCompare)
   ON_BN_CLICKED(ID_BTN_DETAILS_SNAPSHOT, OnBtnDetailsSnapshot)
   ON_BN_CLICKED(ID_BTN_DETAILS_COMPARE, OnBtnDetailsCompare)
   ON_WM_QUERYDRAGICON()
   ON_WM_SYSCOMMAND()
   ON_WM_DESTROY()
//}}AFX_MSG_MAP

   ON_MESSAGE(UM_SNAPSHOT_READY, OnGetSnapshotList)
   ON_MESSAGE(UM_COMPARE_READY, OnGetCompareList)
END_MESSAGE_MAP()


// .INI sections and entries
// 
LPCTSTR SECTION_INFO    = _T("Info");
   LPCTSTR ENTRY_APPLICATION  = _T("Application");
   LPCTSTR ENTRY_ALWAYSONTOP  = _T("AlwaysOnTop");


CGdiUsageDlg::CGdiUsageDlg(CWnd* pParent /*=NULL*/)
   : CDialog(CGdiUsageDlg::IDD, pParent)
{
//{{AFX_DATA_INIT(CGdiUsageDlg)
   // NOTE: the ClassWizard will add member initialization here
//}}AFX_DATA_INIT

// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
   m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

// load the small icon which must be destroyed
   m_hSmallIcon = (HICON)::LoadImage(AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME), IMAGE_ICON, 16, 16, 0); 

// set default values for data members
   m_hSmallFont       = NULL;
   m_pDebuggerThread  = NULL;

// load the menu for the Output listbox
// --> keep track of the root menu and destroy it, not the submenu (leak...)
   m_hMenu = ::LoadMenu(AfxGetResourceHandle(), MAKEINTRESOURCE(IDM_OUTPUT));
   if (m_hMenu != NULL)
      m_hOutputPopupMenu = ::GetSubMenu(m_hMenu, 0);
   ASSERT(m_hOutputPopupMenu != NULL);

   m_pPositionHelper = NULL;
}


CGdiUsageDlg::~CGdiUsageDlg()
{
// only destroy the root menu, not the submenu
   if (m_hMenu != NULL)
      ::DestroyMenu(m_hMenu);

   delete m_pPositionHelper;
}


void CGdiUsageDlg::DoDataExchange(CDataExchange* pDX)
{
   CDialog::DoDataExchange(pDX);

//{{AFX_DATA_MAP(CGdiUsageDlg)
   DDX_Control(pDX, IDC_EDIT_APPLICATION, m_EditApplication);
//}}AFX_DATA_MAP
}


BOOL CGdiUsageDlg::OnInitDialog()
{
// default MFC processing
   CDialog::OnInitDialog();

// create the save/load position helper
   m_pPositionHelper = new CWndSaveRestoreHelper(m_hWnd, _T("Settings"), _T("DialogPosition"));
   if (m_pPositionHelper != NULL)
      m_pPositionHelper->Load();

// update system menu with "About..." and "Always on Top" commands
   CMenu* pSysMenu = GetSystemMenu(FALSE);
   if (pSysMenu != NULL)
   {
      CString strMenu;

   // Add "Always on Top" menu item to system menu.
      strMenu.LoadString(IDS_ALWAYSONTOP);
      if (!strMenu.IsEmpty())
      {
         pSysMenu->AppendMenu(MF_SEPARATOR);
         pSysMenu->AppendMenu(MF_STRING, IDM_ALWAYSONTOP, strMenu);
      }

   // Add "About..." menu item to system menu.
      strMenu.LoadString(IDS_ABOUTBOX);
      if (!strMenu.IsEmpty())
      {
         pSysMenu->AppendMenu(MF_SEPARATOR);
         pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strMenu);
      }
   }


// Set the icon for this dialog.  The framework does this automatically
//  when the application's main window is not a dialog
   SetIcon(m_hIcon, TRUE);       // Set big icon
   SetIcon(m_hSmallIcon, FALSE); // Set small icon
   
   
// get from INI if it was "always on top" or not
   m_bAlwaysOnTop = (AfxGetApp()->GetProfileString(SECTION_INFO, ENTRY_ALWAYSONTOP, _T("0")) == _T("1"));
   if (m_bAlwaysOnTop)
   {
      SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
      CMenu* pSysMenu = GetSystemMenu(FALSE);
      if (pSysMenu != NULL)
         pSysMenu->CheckMenuItem(IDM_ALWAYSONTOP, MF_BYCOMMAND | MF_CHECKED);
   }
   else
   {
      SetWindowPos(&wndNoTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
      CMenu* pSysMenu = GetSystemMenu(FALSE);
      if (pSysMenu != NULL)
         pSysMenu->CheckMenuItem(IDM_ALWAYSONTOP, MF_BYCOMMAND | MF_UNCHECKED);
   }

// set the File Browse edit filter
   m_EditApplication.SetFilter(_T("*.EXE"), _T("Applications (*.exe)"));
   CString szApplication = AfxGetApp()->GetProfileString(SECTION_INFO, ENTRY_APPLICATION, _T(""));
   m_EditApplication.SetWindowText(szApplication);


// disable snapshot controls
   UpdateUI(FALSE);


// create the small font used by the Output listbox
   CDC* pDC = GetDC();
   LOGFONT logFont;
   memset(&logFont, 0, sizeof(LOGFONT));
   _tcscpy(logFont.lfFaceName, _T("Verdana"));
   logFont.lfHeight = 
      MulDiv(
         8, 
         -::GetDeviceCaps(pDC->m_hDC, LOGPIXELSY), 
         72
         );
   m_hSmallFont = ::CreateFontIndirect(&logFont);
   

   return(TRUE);  // return TRUE  unless you set the focus to a control
}


void CGdiUsageDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
   if ((nID & 0xFFF0) == IDM_ABOUTBOX)
   {
      CAboutDlg dlgAbout;
      dlgAbout.DoModal();
   }
   else
   if ((nID & 0xFFF0) == IDM_ALWAYSONTOP)
   {
   // swap the "Always on top" status
      m_bAlwaysOnTop = !m_bAlwaysOnTop;

   // update system menu
      if (m_bAlwaysOnTop)
      {
         SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
         CMenu* pSysMenu = GetSystemMenu(FALSE);
         if (pSysMenu != NULL)
            pSysMenu->CheckMenuItem(IDM_ALWAYSONTOP, MF_BYCOMMAND | MF_CHECKED);
      }
      else
      {
         SetWindowPos(&wndNoTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
         CMenu* pSysMenu = GetSystemMenu(FALSE);
         if (pSysMenu != NULL)
            pSysMenu->CheckMenuItem(IDM_ALWAYSONTOP, MF_BYCOMMAND | MF_UNCHECKED);
      }
   }
   else
      CDialog::OnSysCommand(nID, lParam);
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CGdiUsageDlg::OnPaint() 
{
   if (IsIconic())
   {
      CPaintDC dc(this); // device context for painting

      SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

      // Center icon in client rectangle
      int cxIcon = GetSystemMetrics(SM_CXICON);
      int cyIcon = GetSystemMetrics(SM_CYICON);
      CRect rect;
      GetClientRect(&rect);
      int x = (rect.Width() - cxIcon + 1) / 2;
      int y = (rect.Height() - cyIcon + 1) / 2;

      // Draw the icon
      dc.DrawIcon(x, y, m_hIcon);
   }
   else
   {
      CDialog::OnPaint();
   }
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CGdiUsageDlg::OnQueryDragIcon()
{
   return (HCURSOR) m_hIcon;
}

void CGdiUsageDlg::OnDestroy() 
{
// save to .INI if the frame was "always on top" or not
   if (m_bAlwaysOnTop)
      AfxGetApp()->WriteProfileString(SECTION_INFO, ENTRY_ALWAYSONTOP, _T("1")); 
   else
      AfxGetApp()->WriteProfileString(SECTION_INFO, ENTRY_ALWAYSONTOP, _T("0"));

// and the last launched application
   CString szApplication;
   m_EditApplication.GetWindowText(szApplication);
   AfxGetApp()->WriteProfileString(SECTION_INFO, ENTRY_APPLICATION, szApplication);

// and the window position
   if (m_pPositionHelper != NULL)
      m_pPositionHelper->Save();

// don't forget to destroy the small icon
   if (m_hSmallIcon != NULL)
      ::DestroyIcon(m_hSmallIcon);

// ... and the small font
   if (m_hSmallFont != NULL)
      DeleteObject(m_hSmallFont);


// ask the debugging thread to end
   CApplicationDebugger::AskToQuit();
   ::Sleep(100);

// we have to notify the debugger thread to stop the execution if it's still running
   if (m_pDebuggerThread != NULL)
   {
   // ask it to stop the debugging session if it's still running
      if (m_pDebuggerThread->m_hThread != NULL)
      {
      // and wait for him to finish
         WaitForSingleObject(m_pDebuggerThread->m_hThread, INFINITE);
      }

   // don't forget to free resources
      delete m_pDebuggerThread;
      m_pDebuggerThread = NULL;
   }


// default processing 
   CDialog::OnDestroy();
}


void CGdiUsageDlg::ResetCounter(int Counter)
{
   SetDlgItemText(Counter, _T("-"));
}

// update enable/disable controls state 
//
void CGdiUsageDlg::UpdateUI(BOOL bIsRunning)
{
// if bIsRunning is TRUE, the Start button and its application Edit are disabled
// but the other buttons are enabled. Do the opposite if bIsRunning is FALSE
//
   ::EnableWindow(::GetDlgItem(m_hWnd, IDC_BTN_START),            !bIsRunning);
   ::EnableWindow(::GetDlgItem(m_hWnd, IDC_EDIT_APPLICATION),     !bIsRunning);

   ::EnableWindow(::GetDlgItem(m_hWnd, ID_BTN_TAKE_SNAPSHOT),     bIsRunning);
   ::EnableWindow(::GetDlgItem(m_hWnd, ID_BTN_COMPARE),           bIsRunning);
   ::EnableWindow(::GetDlgItem(m_hWnd, ID_BTN_DETAILS_SNAPSHOT),  bIsRunning);
   ::EnableWindow(::GetDlgItem(m_hWnd, ID_BTN_DETAILS_COMPARE),   bIsRunning);

// reset the counters
   if (bIsRunning)
   {
   // left snapshot column
      ResetCounter(IDC_BITMAP_SET);
      ResetCounter(IDC_BRUSH_SET);
      ResetCounter(IDC_PEN_SET);
      ResetCounter(IDC_DC_SET);
      ResetCounter(IDC_MEMDC_SET);
      ResetCounter(IDC_FONT_SET);
      ResetCounter(IDC_REGION_SET);
      ResetCounter(IDC_PALETTE_SET);
      ResetCounter(IDC_METAFILE_SET);
      ResetCounter(IDC_ENHMETAFILE_SET);
      ResetCounter(IDC_METAFILEDC_SET);
      ResetCounter(IDC_ENHMETAFILEDC_SET);

   // right compare columns
      ResetCounter(IDC_BITMAP_COMPARE_NEW);
      ResetCounter(IDC_BITMAP_COMPARE_SAME);
      ResetCounter(IDC_BITMAP_COMPARE_FREE);

      ResetCounter(IDC_BRUSH_COMPARE_NEW);
      ResetCounter(IDC_BRUSH_COMPARE_SAME);
      ResetCounter(IDC_BRUSH_COMPARE_FREE);

      ResetCounter(IDC_PEN_COMPARE_NEW);
      ResetCounter(IDC_PEN_COMPARE_SAME);
      ResetCounter(IDC_PEN_COMPARE_FREE);

      ResetCounter(IDC_DC_COMPARE_NEW);
      ResetCounter(IDC_DC_COMPARE_SAME);
      ResetCounter(IDC_DC_COMPARE_FREE);

      ResetCounter(IDC_MEMDC_COMPARE_NEW);
      ResetCounter(IDC_MEMDC_COMPARE_SAME);
      ResetCounter(IDC_MEMDC_COMPARE_FREE);

      ResetCounter(IDC_FONT_COMPARE_NEW);
      ResetCounter(IDC_FONT_COMPARE_SAME);
      ResetCounter(IDC_FONT_COMPARE_FREE);

      ResetCounter(IDC_REGION_COMPARE_NEW);
      ResetCounter(IDC_REGION_COMPARE_SAME);
      ResetCounter(IDC_REGION_COMPARE_FREE);

      ResetCounter(IDC_PALETTE_COMPARE_NEW);
      ResetCounter(IDC_PALETTE_COMPARE_SAME);
      ResetCounter(IDC_PALETTE_COMPARE_FREE);

      ResetCounter(IDC_METAFILE_COMPARE_NEW);
      ResetCounter(IDC_METAFILE_COMPARE_SAME);
      ResetCounter(IDC_METAFILE_COMPARE_FREE);

      ResetCounter(IDC_ENHMETAFILE_COMPARE_NEW);
      ResetCounter(IDC_ENHMETAFILE_COMPARE_SAME);
      ResetCounter(IDC_ENHMETAFILE_COMPARE_FREE);

      ResetCounter(IDC_METAFILEDC_COMPARE_NEW);
      ResetCounter(IDC_METAFILEDC_COMPARE_SAME);
      ResetCounter(IDC_METAFILEDC_COMPARE_FREE);

      ResetCounter(IDC_ENHMETAFILEDC_COMPARE_NEW);
      ResetCounter(IDC_ENHMETAFILEDC_COMPARE_SAME);
      ResetCounter(IDC_ENHMETAFILEDC_COMPARE_FREE);
   }
}


// parameters given to the thread responsible to drive the debuggee application
// 
class THREADINFO 
{
public:
   THREADINFO(CGdiUsageDlg* pThis, HWND hOutputListBox) 
   {
      ASSERT(pThis != NULL);
      m_pThis = pThis;
      m_hOutputListBox = hOutputListBox;
   };

public:
   CGdiUsageDlg*  m_pThis;
   HWND           m_hOutputListBox;
};



UINT CGdiUsageDlg::DebugThreadProc(LPARAM lParam)
{
   UINT uResult = 0;
   THREADINFO* pInfo = reinterpret_cast<THREADINFO*>(lParam);


// sanity checks
   if (pInfo == NULL)
   {
   // bizarre...
      ASSERT(FALSE);
      return(-1);
   }

// store the parameters
   CGdiUsageDlg*  pDialog  = pInfo->m_pThis;
   HWND           hListBox = pInfo->m_hOutputListBox;

// don't forget to delete the received parameter
   delete pInfo;

// sanity checks
   if (pDialog == NULL)
   {
      ASSERT(FALSE);
      return(-1);
   }
   else
   {
   // start de debugging session
      CString szCmdLine;
      pDialog->GetDlgItemText(IDC_EDIT_APPLICATION, szCmdLine);

      CGDIDebugger
         Debugger(
            szCmdLine, 
            pDialog->m_hWnd, 
            hListBox
            );
      try
      {
      // store the debugger session object
         m_pGDIDebugger = &Debugger;

      // start the debugging session
         DWORD dwReturn = Debugger.DebugProcess();
         switch(dwReturn)
         {
            case DP_OK: 
            break;

            case DP_CREATEPROCESS_FAILED: 
            {
               CString szTitle;
               szTitle.LoadString(IDS_SESSION_FAILED);
               CString szText;
               szText.LoadString(IDS_NO_LAUNCH);
               ::MessageBox(
                  pDialog->m_hWnd, 
                  szText, 
                  szTitle, 
                  MB_OK | MB_ICONHAND
                  );
            }
            break;

         }
      }
      catch(...)
      {
      }

   // current debugger session is no more valid
      m_pGDIDebugger = NULL;

   // restore UI only if the calling thread is not waiting for us to end
   // --> read the comment related to deadlock in the article
      if (!Debugger.NeedToQuit())
         pDialog->UpdateUI(FALSE);
   }


   if (pDialog->m_pDebuggerThread != NULL)
   // don't auto delete outself, otherwise there is no way 
   // for the dialog to detect that the thread has ended
      pDialog->m_pDebuggerThread->m_bAutoDelete = FALSE;

   return(uResult);
}


void CGdiUsageDlg::OnBtnStart() 
{
   CString szApplicationName;
   m_EditApplication.GetWindowText(szApplicationName);

// automatically ask for the application name
   if (szApplicationName.IsEmpty())
   {
      AfxMessageBox(IDS_CHOOSE_APPLICATION_NAME, MB_OK|MB_ICONHAND);
      if (m_EditApplication.Browse())
         m_EditApplication.GetWindowText(szApplicationName);
   }
   
   if (szApplicationName.IsEmpty())
   {
   // sorry...
   }
   else
   {
   // update controls state
      UpdateUI(TRUE);

   // create the listbox used to display the OutputDebugString()
   // called by the debuggee application if it is not already created
      if (!IsWindow(m_OutputListBox.m_hWnd))
      {
         DWORD dwStyle =
            WS_POPUP | 
            WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | WS_THICKFRAME | 
            WS_VSCROLL | LBS_NOTIFY | 
            LBS_MULTIPLESEL | LBS_EXTENDEDSEL | LBS_NOINTEGRALHEIGHT;

      // center it on the desktop
         RECT rcScreen;
         DWORD Width    = 600;
         DWORD Height   = 300;
         rcScreen.left     = (GetSystemMetrics(SM_CXFULLSCREEN) - Width) / 2;
         rcScreen.right    = rcScreen.left + Width; 
         rcScreen.top      = (GetSystemMetrics(SM_CYFULLSCREEN) - Height) / 2;
         rcScreen.bottom   = rcScreen.top + Height;

         m_OutputListBox.SubclassWindow( 
            ::CreateWindow(
               _T("LISTBOX"),
               _T("Debuggee Output"),
               dwStyle,
               rcScreen.left, rcScreen.top, Width, Height,
               m_hWnd,
               (HMENU)0,
               (HINSTANCE)::GetWindowLong(m_hWnd, GWL_HINSTANCE),
               NULL
               )
            );

      // move it to its last position if any
         m_OutputListBox.SetPlacement();
         if (!m_OutputListBox.IsWindowVisible())
            m_OutputListBox.ShowWindow(SW_SHOW);

      // set the small font 
         if (m_hSmallFont != NULL)
            m_OutputListBox.SendMessage(WM_SETFONT, (WPARAM)m_hSmallFont, (LPARAM)FALSE); 

      // set the popup menu
         m_OutputListBox.SetPopupMenu(m_hOutputPopupMenu);
      }
      ASSERT(::IsWindow(m_OutputListBox.m_hWnd));


   // start the debugging session
      THREADINFO* pInfo = new THREADINFO(this, m_OutputListBox.m_hWnd);
      if (pInfo == NULL)
      {
         AfxMessageBox(IDS_NOT_ENOUGH_MEMORY, MB_OK|MB_ICONHAND);
         EndDialog(IDCANCEL);
      }
      else
      {
      // don't forget to free the previous thread if any
         if (m_pDebuggerThread != NULL)
            delete m_pDebuggerThread;

      // Note: pInfo will be deleted by the called thread
         m_pDebuggerThread = AfxBeginThread((AFX_THREADPROC)DebugThreadProc, (LPVOID)pInfo);
      }
   }
}


void CGdiUsageDlg::OnDeleteAll()
{
   m_OutputListBox.ResetContent();
}



void CGdiUsageDlg::OnFind()
{
// set the last text to find
   CFindDialog FindDlg(m_szLastFind, &m_OutputListBox);

   FindDlg.DoModal();

// store the last find string
   FindDlg.GetFindString(m_szLastFind);
}


DWORD GetSelectedLen(HWND hListBox)
{
// sanity checks
   ASSERT(::IsWindow(hListBox));
   if (!::IsWindow(hListBox))
      return(0);

   DWORD dwLen = 0;
   LRESULT lResult;

// check the selection
   lResult = ::SendMessage(hListBox, LB_GETSELCOUNT, 0, 0);
   if ((lResult == 0) || (lResult == LB_ERR))
      return(0);

   for (DWORD Current = 0; ; ++Current)
   {
      lResult = ::SendMessage(hListBox, LB_GETSEL, Current, 0);
      if (lResult == LB_ERR)
         break;
      
   // is the item selected?
      if (lResult > 0)
      {
         lResult = ::SendMessage(hListBox, LB_GETTEXTLEN, Current, 0);
         if (lResult == LB_ERR)
            break;

         dwLen += lResult + 2;   // add 2 for '\r\n'
      }
   } 

   return(dwLen);
}


BOOL GetSelectedText(HWND hListBox, LPTSTR pszText)
{
// sanity checks
   ASSERT(::IsWindow(hListBox));
   if (!::IsWindow(hListBox))
      return(0);

   DWORD    dwLen = 0;
   LRESULT  lResult;
   BOOL     bReturn = TRUE;

// check the selection
   lResult = ::SendMessage(hListBox, LB_GETSELCOUNT, 0, 0);
   if ((lResult == 0) || (lResult == LB_ERR))
      return(0);

   try
   {
      LPTSTR   pPos = pszText;
      TCHAR    szLine[1024];
      DWORD    dwLen;
      for (DWORD Current = 0; ; ++Current)
      {
         lResult = ::SendMessage(hListBox, LB_GETSEL, Current, 0);
         if (lResult == LB_ERR)
            break;
      
      // is the item selected?
         if (lResult > 0)
         {
            lResult = ::SendMessage(hListBox, LB_GETTEXTLEN, Current, 0);
            if (lResult == LB_ERR)
               break;
            dwLen = lResult;

            lResult = ::SendMessage(hListBox, LB_GETTEXT, (WPARAM)Current, (LPARAM)szLine);
            if (lResult == LB_ERR)
               break;

            _tcscpy(pPos, szLine);
            pPos += dwLen;
            _tcscat(pPos, _T("\r\n"));
            pPos += 2;
         }
      } 
   }
   catch (...)
   {
      bReturn = FALSE;
   }
   
   return(bReturn);
}


void CGdiUsageDlg::OnCopyToClipboard()
{
// copy the current listbox selection to the clipboard

// open the clipboard for the application main window
// --> impossible to work if there is no such a window...
   ASSERT(::IsWindow(m_hWnd));
   if (!::OpenClipboard(m_hWnd))
   {
      AfxMessageBox(IDS_OPEN_CLIPBOARD, MB_OK|MB_ICONHAND);
      return;
   }

// empty the clipboard 
   ::EmptyClipboard();

// create a memory block to be sent to the clipboard
   DWORD dwTextLen = GetSelectedLen(m_OutputListBox.m_hWnd);
   if (dwTextLen != 0)
   {
      ++dwTextLen;   // count the '\0'
      HGLOBAL  hText = ::GlobalAlloc(GHND, dwTextLen * sizeof(TCHAR));
      if (hText != NULL)
      {
         LPSTR pszText = (LPSTR)::GlobalLock(hText);
         if (pszText != NULL)
         {
         // get selected text from the listbox
            VERIFY(GetSelectedText(m_OutputListBox.m_hWnd, pszText));

         // don't forget to unlock the memory block
            ::GlobalUnlock(hText);

         // sent it the clipboard
            ::SetClipboardData(CF_TEXT, hText);
         }
      }
      else
      {
         AfxMessageBox(IDS_NOT_ENOUGH_MEMORY, MB_OK|MB_ICONHAND);
      }

   // don't forget to close the clipboard
      ::CloseClipboard();
   }
   else
   {
      AfxMessageBox(IDS_NO_SELECTION, MB_OK|MB_ICONINFORMATION);
   }
}


void CGdiUsageDlg::OnBtnTakeSnapshot() 
{
// sanity checks
   ASSERT(m_pGDIDebugger != NULL);
   if (m_pGDIDebugger == NULL)
      return;

// ask the debugging session to get the list from the debuggee
// and notify us with a UM_SNAPSHOT_READY message
   m_pGDIDebugger->GetGDIList(UM_SNAPSHOT_READY);
}


// this method is called on response to a GetList() action 
// -->   the debuggee fill a shared buffer, the debugger receives
//       a notification when the list is ready and then send to
//       this dialog the list content in lParam under a GDI_LIST* 
//       format and the notification message in wParam
LRESULT CGdiUsageDlg::OnGetSnapshotList(WPARAM wParam, LPARAM lParam)
{
// sanity checks
   GDI_LIST* pData = (GDI_LIST*)lParam;
   ASSERT(pData != NULL);
   if (pData == NULL)
      return(-1);

// fill the reference
   m_Reference.CreateFromList(pData);

// update UI
   SetDlgItemInt(IDC_BITMAP_SET,          m_Reference.NbBitmap(),       FALSE);
   SetDlgItemInt(IDC_BRUSH_SET,           m_Reference.NbBrush(),        FALSE);
   SetDlgItemInt(IDC_PEN_SET,             m_Reference.NbPen(),          FALSE);
   SetDlgItemInt(IDC_DC_SET,              m_Reference.NbDC(),           FALSE);
   SetDlgItemInt(IDC_MEMDC_SET,           m_Reference.NbMemDC(),        FALSE);
   SetDlgItemInt(IDC_FONT_SET,            m_Reference.NbFont(),         FALSE);
   SetDlgItemInt(IDC_REGION_SET,          m_Reference.NbRegion(),       FALSE);
   SetDlgItemInt(IDC_PALETTE_SET,         m_Reference.NbPalette(),      FALSE);
   SetDlgItemInt(IDC_METAFILE_SET,        m_Reference.NbMetaFile(),     FALSE);
   SetDlgItemInt(IDC_ENHMETAFILE_SET,     m_Reference.NbEnhMetafile(),  FALSE);
   SetDlgItemInt(IDC_METAFILEDC_SET,      m_Reference.NbMetaDC(),       FALSE);
   SetDlgItemInt(IDC_ENHMETAFILEDC_SET,   m_Reference.NbEnhMetaDC(),    FALSE);

   return(0);
}


void CGdiUsageDlg::OnBtnCompare() 
{
// sanity checks
   ASSERT(m_pGDIDebugger != NULL);
   if (m_pGDIDebugger == NULL)
      return;

// ask the debugging session to get the list from the debuggee
// and notify us with a UM_COMPARE_READY message
   m_pGDIDebugger->GetGDIList(UM_COMPARE_READY);
}


LRESULT CGdiUsageDlg::OnGetCompareList(WPARAM wParam, LPARAM lParam)
{
// sanity checks
   GDI_LIST* pData = (GDI_LIST*)lParam;
   ASSERT(pData != NULL);
   if (pData == NULL)
      return(-1);

// fill the current state
   m_Current.CreateFromList(pData);

// compare with reference
   m_Compare.ComputeCompare(m_Current, m_Reference);

// update UI
   SetDlgItemInt(IDC_BITMAP_COMPARE_NEW, m_Compare.GetNewFromType(OBJ_BITMAP));
   SetDlgItemInt(IDC_BITMAP_COMPARE_SAME, m_Compare.GetSameFromType(OBJ_BITMAP));
   SetDlgItemInt(IDC_BITMAP_COMPARE_FREE, m_Compare.GetOutFromType(OBJ_BITMAP));

   SetDlgItemInt(IDC_BRUSH_COMPARE_NEW, m_Compare.GetNewFromType(OBJ_BRUSH));
   SetDlgItemInt(IDC_BRUSH_COMPARE_SAME, m_Compare.GetSameFromType(OBJ_BRUSH));
   SetDlgItemInt(IDC_BRUSH_COMPARE_FREE, m_Compare.GetOutFromType(OBJ_BRUSH));

   SetDlgItemInt(IDC_PEN_COMPARE_NEW, m_Compare.GetNewFromType(OBJ_PEN));
   SetDlgItemInt(IDC_PEN_COMPARE_SAME, m_Compare.GetSameFromType(OBJ_PEN));
   SetDlgItemInt(IDC_PEN_COMPARE_FREE, m_Compare.GetOutFromType(OBJ_PEN));

   SetDlgItemInt(IDC_DC_COMPARE_NEW, m_Compare.GetNewFromType(OBJ_DC));
   SetDlgItemInt(IDC_DC_COMPARE_SAME, m_Compare.GetSameFromType(OBJ_DC));
   SetDlgItemInt(IDC_DC_COMPARE_FREE, m_Compare.GetOutFromType(OBJ_DC));

   SetDlgItemInt(IDC_MEMDC_COMPARE_NEW, m_Compare.GetNewFromType(OBJ_MEMDC));
   SetDlgItemInt(IDC_MEMDC_COMPARE_SAME, m_Compare.GetSameFromType(OBJ_MEMDC));
   SetDlgItemInt(IDC_MEMDC_COMPARE_FREE, m_Compare.GetOutFromType(OBJ_MEMDC));

   SetDlgItemInt(IDC_FONT_COMPARE_NEW, m_Compare.GetNewFromType(OBJ_FONT));
   SetDlgItemInt(IDC_FONT_COMPARE_SAME, m_Compare.GetSameFromType(OBJ_FONT));
   SetDlgItemInt(IDC_FONT_COMPARE_FREE, m_Compare.GetOutFromType(OBJ_FONT));

   SetDlgItemInt(IDC_REGION_COMPARE_NEW, m_Compare.GetNewFromType(OBJ_REGION));
   SetDlgItemInt(IDC_REGION_COMPARE_SAME, m_Compare.GetSameFromType(OBJ_REGION));
   SetDlgItemInt(IDC_REGION_COMPARE_FREE, m_Compare.GetOutFromType(OBJ_REGION));

   SetDlgItemInt(IDC_PALETTE_COMPARE_NEW, m_Compare.GetNewFromType(OBJ_PAL));
   SetDlgItemInt(IDC_PALETTE_COMPARE_SAME, m_Compare.GetSameFromType(OBJ_PAL));
   SetDlgItemInt(IDC_PALETTE_COMPARE_FREE, m_Compare.GetOutFromType(OBJ_PAL));

   SetDlgItemInt(IDC_METAFILE_COMPARE_NEW, m_Compare.GetNewFromType(OBJ_METAFILE));
   SetDlgItemInt(IDC_METAFILE_COMPARE_SAME, m_Compare.GetSameFromType(OBJ_METAFILE));
   SetDlgItemInt(IDC_METAFILE_COMPARE_FREE, m_Compare.GetOutFromType(OBJ_METAFILE));

   SetDlgItemInt(IDC_ENHMETAFILE_COMPARE_NEW, m_Compare.GetNewFromType(OBJ_ENHMETAFILE));
   SetDlgItemInt(IDC_ENHMETAFILE_COMPARE_SAME, m_Compare.GetSameFromType(OBJ_ENHMETAFILE));
   SetDlgItemInt(IDC_ENHMETAFILE_COMPARE_FREE, m_Compare.GetOutFromType(OBJ_ENHMETAFILE));

   SetDlgItemInt(IDC_METAFILEDC_COMPARE_NEW, m_Compare.GetNewFromType(OBJ_METADC));
   SetDlgItemInt(IDC_METAFILEDC_COMPARE_SAME, m_Compare.GetSameFromType(OBJ_METADC));
   SetDlgItemInt(IDC_METAFILEDC_COMPARE_FREE, m_Compare.GetOutFromType(OBJ_METADC));

   SetDlgItemInt(IDC_ENHMETAFILEDC_COMPARE_NEW, m_Compare.GetNewFromType(OBJ_ENHMETADC));
   SetDlgItemInt(IDC_ENHMETAFILEDC_COMPARE_SAME, m_Compare.GetSameFromType(OBJ_ENHMETADC));
   SetDlgItemInt(IDC_ENHMETAFILEDC_COMPARE_FREE, m_Compare.GetOutFromType(OBJ_ENHMETADC));

   return(0);
}



void CGdiUsageDlg::OnBtnDetailsSnapshot() 
{
// sanity checks
   ASSERT(m_pGDIDebugger != NULL);
   if (m_pGDIDebugger == NULL)
      return;

// ask the debugging session let the debuggee display the list stored m_Current
   m_pGDIDebugger->ShowList(&m_Reference, m_hWnd);
}

void CGdiUsageDlg::OnBtnDetailsCompare() 
{
// sanity checks
   ASSERT(m_pGDIDebugger != NULL);
   if (m_pGDIDebugger == NULL)
      return;

// ask the debugging session let the debuggee display the list stored m_Current
   m_pGDIDebugger->ShowList(&m_Compare, m_hWnd);
}
//
//------------------------------------------------------------------
