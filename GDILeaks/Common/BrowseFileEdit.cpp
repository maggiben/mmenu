// BrowseFileEdit.cpp : implementation file
//

#include "stdafx.h"
#include "BrowseFileEdit.h"

#ifdef _DEBUG
   #define new DEBUG_NEW
   #undef THIS_FILE
   static char THIS_FILE[] = __FILE__;
#endif


BEGIN_MESSAGE_MAP(CBrowseFileEdit, CEdit)
//{{AFX_MSG_MAP(CBrowseFileEdit)
   ON_WM_LBUTTONDBLCLK()
   ON_MESSAGE(WM_DROPFILES, OnDropFiles)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CBrowseFileEdit::CBrowseFileEdit()
{
   m_szFilter        = _T("*.*");
   m_szDescription   = _T("All files");

   MakeFullFilter();
}


CBrowseFileEdit::~CBrowseFileEdit()
{
}


void CBrowseFileEdit::SetFilter(LPCTSTR szFilter, LPCTSTR szDescription)
{
   ASSERT((szFilter != NULL) && (_tcslen(szFilter) > 0));
   ASSERT((szDescription != NULL) && (_tcslen(szDescription) > 0));

   m_szFilter        = szFilter;
   m_szDescription   = szDescription;
   MakeFullFilter();
}


BOOL CBrowseFileEdit::Browse()
{
   OPENFILENAME ofn;
   TCHAR szFilename[512] = _T("");

   memset(&ofn, 0, sizeof(OPENFILENAME));

   ofn.lStructSize  = sizeof(OPENFILENAME);
   ofn.hwndOwner    = m_hWnd;
   ofn.lpstrFilter  = m_szFullFilter;
   ofn.nFilterIndex = 1;
   ofn.lpstrFile    = szFilename;
   ofn.nMaxFile     = sizeof(szFilename) / sizeof(TCHAR);
   ofn.Flags        = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

   BOOL bOk = ::GetOpenFileName(&ofn);
   if (bOk)
      SetWindowText(szFilename);
   
   return(bOk);
}


void CBrowseFileEdit::MakeFullFilter()
{
   LPTSTR pInfo = m_szFullFilter;
   _tcscpy(m_szFullFilter, m_szDescription);

   pInfo += m_szDescription.GetLength();
   pInfo[0] = _T('\0');
   pInfo++;

   _tcscpy(pInfo, m_szFilter);

   pInfo += m_szFilter.GetLength();
   pInfo++;
   pInfo[0] = _T('\0');
}



void CBrowseFileEdit::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
   Browse();

// no default processing
//   CEdit::OnLButtonDblClk(nFlags, point);
}


LRESULT CBrowseFileEdit::OnDropFiles(WPARAM wParam, LPARAM lParam)
{
   HDROP hDrop = reinterpret_cast<HDROP>(wParam);

   int iNumOfFiles = ::DragQueryFile(hDrop, -1, NULL, 0);
   if (iNumOfFiles == 1)
   {
      char szFileName[_MAX_PATH] = {0};
      ::DragQueryFile(hDrop, 0, szFileName, MAX_PATH);
      SetWindowText(szFileName);
   }
   else
   {
/*
   // this is what to do if we need to get each dropped file 
   // one after the other 
      TCHAR szFileName[MAX_PATH] = {0};
      for(int i = 0 ; i < iNumOfFiles; i++)
      {
         DragQueryFile(hDrop, i, szFileName, MAX_PATH);
      }
*/
   }

   ::DragFinish(hDrop);

   return(0);
}
