#if !defined(AFX_GDINDICATORVIEW_H__770B867C_CFA3_4C50_A475_F19E16B5B51B__INCLUDED_)
#define AFX_GDINDICATORVIEW_H__770B867C_CFA3_4C50_A475_F19E16B5B51B__INCLUDED_

#if _MSC_VER > 1000
   #pragma once
#endif // _MSC_VER > 1000


#include "..\Common\Process.h"
#include "..\Common\SortListView.h"



class CGDIndicatorView : public CSortListView
{
// Construction/Destruction
// 
protected: // create from serialization only
   CGDIndicatorView();
   DECLARE_DYNCREATE(CGDIndicatorView)
   virtual ~CGDIndicatorView();


// overridables
// 
protected:
   virtual void OnNeedText(LPVOID pItem, int nColumn, CString& szContent);
   virtual int OnCompareItems(void*, void*, int);
   virtual void OnDeleteItem(void* pItem);


// Internal helpers
//
private:
   int FillList();
   LPSTR GetClipboardString();


// MFC stuff
// 
// Attributes
public:
   CGDIndicatorDoc* GetDocument();

// Overrides
// ClassWizard generated virtual function overrides
//{{AFX_VIRTUAL(CGDIndicatorView)
   public:
   virtual void OnDraw(CDC* pDC);  // overridden to draw this view
   virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
   virtual void OnInitialUpdate(); // called first time after construct
   virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
   //}}AFX_VIRTUAL

// Implementation
public:
#ifdef _DEBUG
   virtual void AssertValid() const;
   virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
//{{AFX_MSG(CGDIndicatorView)
   afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
   afx_msg void OnEditCopy();
   afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
   afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
//}}AFX_MSG
   
   DECLARE_MESSAGE_MAP()
};


#ifndef _DEBUG  // debug version in GDIndicatorView.cpp
inline CGDIndicatorDoc* CGDIndicatorView::GetDocument()
   { return (CGDIndicatorDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GDINDICATORVIEW_H__770B867C_CFA3_4C50_A475_F19E16B5B51B__INCLUDED_)
