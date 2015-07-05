#include "stdafx.h"
#include "FindDialog.h"

#ifdef _DEBUG
   #define new DEBUG_NEW
   #undef THIS_FILE
   static char THIS_FILE[] = __FILE__;
#endif


BEGIN_MESSAGE_MAP(CFindDialog, CDialog)
//{{AFX_MSG_MAP(CFindDialog)
   ON_BN_CLICKED(ID_FIND, OnFind)
   ON_WM_DESTROY()
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()



CFindDialog::CFindDialog(LPCTSTR szLastFind, CWnd* pParent /*=NULL*/)
   : CDialog(CFindDialog::IDD, pParent)
{
//{{AFX_DATA_INIT(CFindDialog)
   // NOTE: the ClassWizard will add member initialization here
//}}AFX_DATA_INIT

   if (szLastFind != NULL)
      m_szLastFind = szLastFind;
}


void CFindDialog::DoDataExchange(CDataExchange* pDX)
{
   CDialog::DoDataExchange(pDX);

//{{AFX_DATA_MAP(CFindDialog)
   // NOTE: the ClassWizard will add DDX and DDV calls here
//}}AFX_DATA_MAP
}



void CFindDialog::GetFindString(CString& szFind)
{
   szFind = m_szLastFind;
}


void CFindDialog::OnFind() 
{
   CString szFind;
   if (GetDlgItemText(IDC_EDIT_FIND, szFind) > 0)
   {
      HWND hParent = ::GetParent(m_hWnd);
      LRESULT lResult = ::SendMessage(::GetParent(m_hWnd), UM_FIND, 0, (LPARAM)(LPCTSTR)szFind);
      if (lResult == -1)
      {
         AfxMessageBox(IDS_FIND_FAILED, MB_OK|MB_ICONINFORMATION);
      }
   }
}


BOOL CFindDialog::OnInitDialog() 
{
// default processing
   CDialog::OnInitDialog();
   
// set the last Find string
   SetDlgItemText(IDC_EDIT_FIND, m_szLastFind);
   
   return TRUE;  // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}


void CFindDialog::OnDestroy() 
{
// store the last find string
   GetDlgItemText(IDC_EDIT_FIND, m_szLastFind);

// default processing
   CDialog::OnDestroy();
}
