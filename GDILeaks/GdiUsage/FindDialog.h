#if !defined(AFX_FINDDIALOG_H__DE5F0163_A74E_4393_8BF6_5298790D130F__INCLUDED_)
#define AFX_FINDDIALOG_H__DE5F0163_A74E_4393_8BF6_5298790D130F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FindDialog.h : header file
//


#include "resource.h"


// this message is sent to the parent window:
//    wParam: 0 (not used)
//    lParam: string to find
//    return value is 0 if the string was found and -1 otherwise
const DWORD UM_FIND = WM_APP+100;



class CFindDialog : public CDialog
{
// Construction/Destrction
//
public:
   CFindDialog(LPCTSTR szLastFind, CWnd* pParent = NULL);   // standard constructor

// C++ interface
// 
public:
   void GetFindString(CString& szFind);

private:
   CString m_szLastFind;

// MFC stuff
// 
public:
// Dialog Data
//{{AFX_DATA(CFindDialog)
   enum { IDD = IDD_FIND };
   // NOTE: the ClassWizard will add data members here
//}}AFX_DATA


// Overrides
// ClassWizard generated virtual function overrides
//{{AFX_VIRTUAL(CFindDialog)
protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
//}}AFX_VIRTUAL

// Implementation
protected:
// Generated message map functions
//{{AFX_MSG(CFindDialog)
   afx_msg void OnFind();
   virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	//}}AFX_MSG

   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FINDDIALOG_H__DE5F0163_A74E_4393_8BF6_5298790D130F__INCLUDED_)
