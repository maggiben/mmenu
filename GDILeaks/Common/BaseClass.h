#if !defined(AFX_BASECLASS_H__F2C61701_7AA7_4A68_ADAD_53D208860E33__INCLUDED_)
#define AFX_BASECLASS_H__F2C61701_7AA7_4A68_ADAD_53D208860E33__INCLUDED_

#if _MSC_VER > 1000
   #pragma once
#endif // _MSC_VER > 1000



#include "..\Common\WndSaveRestoreHelper.h"



class CBaseApp : public CWinApp
{
public:
   CBaseApp();
   virtual ~CBaseApp();


// C++ interface
//
public:
   BOOL CopyTextToClipboard(LPCTSTR szText);
   BOOL ProcessCommandLine();


// MFC stuff
// 
public:
// Overrides
// ClassWizard generated virtual function overrides
//{{AFX_VIRTUAL(CBaseApp)
public:
   virtual BOOL InitInstance();
//}}AFX_VIRTUAL

// Implementation
protected:

// Generated message map functions
//{{AFX_MSG(CBaseApp)
   // NOTE - the ClassWizard will add and remove member functions here.
//}}AFX_MSG

   DECLARE_MESSAGE_MAP()
};




class CBaseFrame : public CFrameWnd
{
// Construction/Destruction
// 
   DECLARE_DYNCREATE(CBaseFrame)
public:
   CBaseFrame();
   virtual ~CBaseFrame();


// C++ interface
// 
public:
   void RestoreFramePosition();


// Implementation details
// 
private:
   CWndSaveRestoreHelper* m_pPositionHelper;


// MFC stuff
// 
public:
// Overrides
// ClassWizard generated virtual function overrides
//{{AFX_VIRTUAL(CBaseFrame)
//}}AFX_VIRTUAL

// Generated message map functions
protected:
//{{AFX_MSG(CBaseFrame)
   afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
   afx_msg void OnDestroy();
//}}AFX_MSG

   DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BASECLASS_H__F2C61701_7AA7_4A68_ADAD_53D208860E33__INCLUDED_)
