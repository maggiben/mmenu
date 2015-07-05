// OutputListBox.cpp : implementation file
//

#include "stdafx.h"
#include "OutputListBox.h"
#include "resource.h"
#include "FindDialog.h"

#ifdef _DEBUG
   #define new DEBUG_NEW
   #undef THIS_FILE
   static char THIS_FILE[] = __FILE__;
#endif


BEGIN_MESSAGE_MAP(COutputListBox, CListBox)
//{{AFX_MSG_MAP(COutputListBox)
   ON_WM_RBUTTONUP()
   ON_MESSAGE(UM_FIND, OnStringToFind)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



COutputListBox::COutputListBox()
{
// no popup menu to display on mouse right click
   m_hPopupMenu = NULL;

   m_pPositionHelper = NULL;
 }


COutputListBox::~COutputListBox()
{
// don't manage the menu lifetime

   delete m_pPositionHelper;
}


void COutputListBox::SetPlacement()
{
// create the save/load position helper
   m_pPositionHelper = new CWndSaveRestoreHelper(m_hWnd, _T("Settings"), _T("OutputListPosition"));
   if (m_pPositionHelper != NULL)
      m_pPositionHelper->Load();
}


void COutputListBox::OnDestroy() 
{
// save placement
   if (m_pPositionHelper != NULL)
      m_pPositionHelper->Save();

// default processing
	CListBox::OnDestroy();
}


void COutputListBox::OnClose() 
{
// minimize it instead of closing it
   ShowWindow(SW_MINIMIZE);
}


HMENU COutputListBox::SetPopupMenu(HMENU hMenu)
{
   HMENU hPrevMenu = m_hPopupMenu;
   m_hPopupMenu = hMenu;
   return(hPrevMenu);
}


void COutputListBox::OnRButtonUp(UINT nFlags, CPoint point) 
{
// default processing
   CListBox::OnRButtonUp(nFlags, point);

// convert point into window relative coordinates
   ClientToScreen(&point);

// display the popup menu
   LRESULT lResult;
   lResult = 
      ::TrackPopupMenu(
         m_hPopupMenu, 
         TPM_RETURNCMD | TPM_NONOTIFY | TPM_RIGHTBUTTON | TPM_LEFTALIGN | TPM_TOPALIGN | TPM_VERTICAL,
         point.x, point.y, 
         0, 
         m_hWnd, 
         NULL
         );

   if ((lResult == 0) && (::GetLastError() != 0))
   {
      AfxMessageBox(IDS_ERROR_MENU_DISPLAY, MB_OK | MB_ICONINFORMATION);
   }
   else
   {
   // notify its parent from the user choice
      ::SendMessage(::GetParent(m_hWnd), WM_COMMAND, MAKELONG(LOWORD(lResult), 0), NULL);
   }
}


int COutputListBox::FindSubString(int StartPos, LPCTSTR szSubStringToFind)
{
   LRESULT  lResult = LB_ERR;
   int      CurrentPos = StartPos;
   CString  szLine;
   BOOL     bAgain = FALSE;  // is TRUE if the search has restarted from the beginning
   int      LineCount = GetCount();
   do 
   {
   // first, check if this is a valid line, otherwise GetText() will fail
      if (LineCount < CurrentPos)
         lResult = LB_ERR;
      else
         lResult = GetTextLen(CurrentPos);

   // get the line content
      if (lResult != LB_ERR)
      {
         GetText(CurrentPos, szLine);
         if (szLine.IsEmpty())
            lResult = LB_ERR;
      }

   // end is reached?
      if (lResult == LB_ERR)
      {
      // don't repeat a second time
         if (bAgain)
         {
            break;
         }
         else 
         {
         // try from the beginning
            bAgain = TRUE;
            CurrentPos = 0;
         }
      }
      else
      {
      // there is a line to parse
         if (szLine.Find(szSubStringToFind) != -1)
         {
         // this is it!
            lResult = 0;
            break;
         }
         else
         {
         // next line please
            ++CurrentPos;

         // check for an infinite loop
            if (bAgain && (CurrentPos == StartPos))
            {
               CurrentPos = LB_ERR;
               break;
            }
         }
      }
   }
   while (TRUE);


   return(CurrentPos);
}


LRESULT COutputListBox::OnStringToFind(WPARAM wParam, LPARAM lParam)
{
// the lParam points to the string to find
   LPCTSTR pszStringToFind = (LPCTSTR)lParam;

// start the search where the caret is or at the beginning of the listbox
   int CurrentFindPos = 0;
   int iPos = GetCaretIndex();
   if (iPos != LB_ERR)
      CurrentFindPos = iPos+1;

   iPos = FindSubString(CurrentFindPos, pszStringToFind);
   if (iPos != LB_ERR)
   {
   //                      v-- make this item visible   
      SetCaretIndex(iPos, TRUE);

   // reset the selection before seting the new one
      SendMessage(LB_SETSEL, FALSE, -1);
      SendMessage(LB_SETSEL, TRUE, iPos);

      return(0);
   }
   else
   {
   // display a message
      CString szText;
      szText.LoadString(IDS_FIND_FAILED);
      MessageBox(szText);

   // indicate a search failure
      return(-1);
   }
}
