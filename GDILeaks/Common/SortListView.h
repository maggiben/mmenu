#ifndef _SORTLISTVIEW_H
#define _SORTLISTVIEW_H

#include "SortHeader.h"
#include <afxcview.h>


class CSortListView : public CListView
{
   DECLARE_DYNCREATE(CSortListView)

// Construction/Destruction
//
public:
   CSortListView();
   virtual ~CSortListView();


// C++ interface
//
public:
   int SortColumn(int iColumn, BOOL bAscending);
   virtual int InsertItem(void*, BOOL bImageCallback = FALSE, int = -1);


// Attributes
//
protected:
   BOOL        m_bAutoDelete;
   BOOL        m_SortedColumn;
   BOOL        m_bAscending;
   CSortHeader m_HeaderCtrl;


// Internal helpers
//
protected:
// Must be overriden!!!
   virtual int OnCompareItems(void*, void*, int);
   virtual void OnNeedText(LPVOID pItem, int nColumn, CString& szText);
   virtual int OnNeedImage(void*, int);
   virtual void OnDeleteItem(void*);


// Internal helpers
//
private:
   static int CALLBACK CompareFunc (LPARAM, LPARAM, LPARAM);


// MFC stuff
//
public:
// Overrides
// ClassWizard generated virtual function overrides
//{{AFX_VIRTUAL(CSortListView)
protected:
   virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
   afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
//}}AFX_VIRTUAL


// Generated message map functions
protected:
//{{AFX_MSG(CSortListView)
   afx_msg void OnDestroy();
   afx_msg LONG OnDeleteItemMsg(UINT, LONG);
   afx_msg LONG OnDeleteAllItemsMsg(UINT, LONG);
   afx_msg void OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult);
   afx_msg void OnGetdispinfo(NMHDR* pNMHDR, LRESULT* pResult);
//}}AFX_MSG

   DECLARE_MESSAGE_MAP()
};



#endif
