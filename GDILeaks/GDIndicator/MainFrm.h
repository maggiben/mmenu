#if !defined(AFX_MAINFRM_H__765F0426_5B5E_407A_9841_1DB4AFEF3F2A__INCLUDED_)
#define AFX_MAINFRM_H__765F0426_5B5E_407A_9841_1DB4AFEF3F2A__INCLUDED_

#if _MSC_VER > 1000
   #pragma once
#endif // _MSC_VER > 1000


#include "..\Common\BaseClass.h"


class CMainFrame : public CBaseFrame
{
// Construction/Destruction
// 
protected: // create from serialization only
   CMainFrame();
   DECLARE_DYNCREATE(CMainFrame)
public:
   virtual ~CMainFrame();


// MFC stuff
// 
public:
// Overrides
// ClassWizard generated virtual function overrides
//{{AFX_VIRTUAL(CMainFrame)
public:
   virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
//}}AFX_VIRTUAL

// Implementation
public:
#ifdef _DEBUG
   virtual void AssertValid() const;
   virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
   CStatusBar  m_wndStatusBar;
   CToolBar    m_wndToolBar;

// Generated message map functions
protected:
//{{AFX_MSG(CMainFrame)
   afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
//}}AFX_MSG

   DECLARE_MESSAGE_MAP()
};



//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__765F0426_5B5E_407A_9841_1DB4AFEF3F2A__INCLUDED_)
