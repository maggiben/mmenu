#if !defined(AFX_OUTPUTLISTBOX_H__F18A325D_DCAD_430B_994A_5DF4138FDA37__INCLUDED_)
#define AFX_OUTPUTLISTBOX_H__F18A325D_DCAD_430B_994A_5DF4138FDA37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "..\Common\WndSaveRestoreHelper.h"


// this listbox display a popup menu on a right mouse event
// Note: the menu lifetime is not managed by the listbox
// 
class COutputListBox : public CListBox
{
// construction/destruction
//
public:
   COutputListBox();
   virtual ~COutputListBox();


// C++ interface
// 
public:
   HMENU SetPopupMenu(HMENU hMenu);
   void SetPlacement();

// implementation details
// 
private:
   HMENU                   m_hPopupMenu;
   CWndSaveRestoreHelper*  m_pPositionHelper;


// implementation helpers
// 
private:
   int FindSubString(int StartPos, LPCTSTR szSubStringToFind);


// MFC stuff
// 
public:
// ClassWizard generated virtual function overrides
//{{AFX_VIRTUAL(COutputListBox)
//}}AFX_VIRTUAL

   // Generated message map functions
protected:
//{{AFX_MSG(COutputListBox)
   afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
   afx_msg LRESULT OnStringToFind(WPARAM wParam, LPARAM lParam);
   afx_msg void OnClose();
   afx_msg void OnDestroy();
//}}AFX_MSG

   DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OUTPUTLISTBOX_H__F18A325D_DCAD_430B_994A_5DF4138FDA37__INCLUDED_)
