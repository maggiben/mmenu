#if !defined(AFX_GDINDICATOR_H__C0763817_8D0C_4401_91BD_DDB658653C00__INCLUDED_)
#define AFX_GDINDICATOR_H__C0763817_8D0C_4401_91BD_DDB658653C00__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
   #error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "..\Common\BaseClass.h"



class CGDIndicatorApp : public CBaseApp
{
public:
   CGDIndicatorApp();


// Overrides
// ClassWizard generated virtual function overrides
//{{AFX_VIRTUAL(CGDIndicatorApp)
public:
   virtual BOOL InitInstance();
//}}AFX_VIRTUAL

// Implementation
//{{AFX_MSG(CGDIndicatorApp)
   afx_msg void OnAppAbout();
   // NOTE - the ClassWizard will add and remove member functions here.
   //    DO NOT EDIT what you see in these blocks of generated code !
//}}AFX_MSG

   DECLARE_MESSAGE_MAP()
};



//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GDINDICATOR_H__C0763817_8D0C_4401_91BD_DDB658653C00__INCLUDED_)
