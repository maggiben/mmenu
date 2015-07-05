#if !defined(AFX_GDINJECT_H__9E371EA2_0471_4BFC_A0CE_A59FD1F9B939__INCLUDED_)
#define AFX_GDINJECT_H__9E371EA2_0471_4BFC_A0CE_A59FD1F9B939__INCLUDED_

#if _MSC_VER > 1000
   #pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
   #error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"      // main symbols


class CGDInjectApp : public CWinApp
{
// Construction/Destruction
// 
public:
   CGDInjectApp();
 

// C++ interface
// 
public:
   BOOL GetDataFromRequest(LPVOID pData, DWORD cbSize);
   BOOL SetDataForRequest(LPVOID pData, DWORD cbSize);
   LPVOID GetDataPointer();


// Implementation details
// 
private:
   // shared memory mapped file
   LPVOID   m_lpvMem;
   HANDLE   m_hMapObject;


// Internal helpers
// 
private:
   void DoGetTypes();
   void DoShowList();


// MFC stuff
// 
public:
// Overrides
// ClassWizard generated virtual function overrides
//{{AFX_VIRTUAL(CGDInjectApp)
public:
   virtual BOOL InitInstance();
   virtual int ExitInstance();
//}}AFX_VIRTUAL

//{{AFX_MSG(CGDInjectApp)
   // NOTE - the ClassWizard will add and remove member functions here.
   //    DO NOT EDIT what you see in these blocks of generated code !
//}}AFX_MSG

   DECLARE_MESSAGE_MAP()
};




//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GDINJECT_H__9E371EA2_0471_4BFC_A0CE_A59FD1F9B939__INCLUDED_)
