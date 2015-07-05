// SortListCtrl.h : header file
//

#ifndef _SORTLISTCTRL_H
   #define _SORTLISTCTRL_H


#include "SortHeader.h"


class CSortListCtrl : public CListCtrl
{
   DECLARE_DYNCREATE(CSortListCtrl)

// construction/destruction
//
public:
   CSortListCtrl();
   virtual ~CSortListCtrl();


// C++ interface
//
public:
   BOOL BindToControl(HWND hListCtrl);
   int SortColumn(int iColumn, BOOL bAscending);
   void SortList();


// Attributes
//
public:
   BOOL m_bAutoDelete;


// Overrides
//
public:
   virtual int InsertItem (void*, BOOL bImageCallback = FALSE, int = -1);


// Operations
//
public:
   int GetCurSel();
   int SetCurSel(int iSelection);


// internal members
//
protected:
   BOOL  m_SortedColumn;
   BOOL  m_bAscending;
   int   m_SelectedItem;
   CSortHeader m_HeaderCtrl;


// overridable methods
//
protected:
// Must be overriden
   virtual int OnCompareItems(void*, void*, int);
   virtual void OnNeedText(LPVOID pItem, int nColumn, CString& szText);
   virtual int OnNeedImage(void*, int);
   virtual void OnDeleteItem(void*);


// implementation details
// 
private:
   static int CALLBACK CompareFunc (LPARAM, LPARAM, LPARAM);


// MFC stuff
//
public:
// Overrides
// ClassWizard generated virtual function overrides
//{{AFX_VIRTUAL(CSortListCtrl)
protected:
   virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
//}}AFX_VIRTUAL


// Generated message map functions
protected:
//{{AFX_MSG(CSortListCtrl)
   afx_msg void OnDestroy();
   afx_msg LONG OnDeleteItemMsg(UINT, LONG);
   afx_msg LONG OnDeleteAllItemsMsg(UINT, LONG);
   afx_msg void OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult);
   afx_msg void OnGetdispinfo(NMHDR* pNMHDR, LRESULT* pResult);
   afx_msg void OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult);
//}}AFX_MSG


   DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////


// structure used for the sorting process
   typedef struct tagSORTINFO 
   {
      CSortListCtrl* pThis;
      int nColumn;
   } SORTINFO, *PSORTINFO;



#endif
