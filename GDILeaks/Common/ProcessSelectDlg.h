#if !defined(AFX_PROCESSSELECTDLG_H__A87838A5_8055_4C01_A5B9_8B44D53C6ED4__INCLUDED_)
#define AFX_PROCESSSELECTDLG_H__A87838A5_8055_4C01_A5B9_8B44D53C6ED4__INCLUDED_

#if _MSC_VER > 1000
   #pragma once
#endif // _MSC_VER > 1000



#include "..\Common\SortListCtrl.h"
#include "..\Common\Process.h"



// check that the needed declarations and resources are includes into your project
// 
#ifndef IDC_LIST_PROCESS
   #pragma message("You have to add IDD_SELECT_PROCESS and IDC_LIST_PROCESS into your project resource.h")
//
// definition of needed resource identifiers to add in the resource.h
// 
   #define IDD_SELECT_PROCESS    130
   #define IDC_LIST_PROCESS      1000
#endif


#ifndef IDD_GRAPHIC_DISPLAY
   #pragma message("You have to add the IDD_SELECT_PROCESS dialog description into your resources")
//
// you should add the following dialog box description into your .RC file
//
/*
IDD_SELECT_PROCESS DIALOGEX 0, 0, 276, 135
STYLE DS_MODALFRAME | WS_POPUP | WS_CAPTION | WS_SYSMENU
CAPTION "Select a process..."
FONT 8, "MS Sans Serif"
BEGIN
    CONTROL         "List1",IDC_LIST_PROCESS,"SysListView32",LVS_REPORT | 
                    LVS_SINGLESEL | LVS_SHOWSELALWAYS | WS_BORDER | 
                    WS_TABSTOP,2,3,271,107,WS_EX_CLIENTEDGE
    DEFPUSHBUTTON   "&OK",IDOK,163,115,50,14
    PUSHBUTTON      "&Cancel",IDCANCEL,222,115,50,14
END
*/
#endif


class CProcessListCtrl : public CSortListCtrl  
{
// contruction/destruction
//
public:
   CProcessListCtrl();
   virtual ~CProcessListCtrl();


// C++ public Interface
// 
public:
   BOOL InitColumns();
   int FillList();
   DWORD GetSelectedPID();


// Overridable methods
// 
protected:
   virtual void OnNeedText(LPVOID pItem, int nColumn, CString& szText);
   virtual int OnCompareItems(void*, void*, int);


// Implementation details
// 
private:
   CProcessList m_ProcessList;
};





class CProcessSelectDlg : public CDialog
{
// Construction
public:
   CProcessSelectDlg(CWnd* pParent = NULL);   // standard constructor


// C++ interface
// 
public:
   DWORD GetSelectedProcess()    { return(m_PID);  }


// implementation details
// 
private:
   DWORD m_PID;


// MFC stuff
// 
public:
// Dialog Data
//{{AFX_DATA(CProcessSelectDlg)
   enum { IDD = IDD_SELECT_PROCESS };
   CProcessListCtrl m_ProcessListCtrl;
// NOTE: the ClassWizard will add data members here
//}}AFX_DATA


// Overrides
// ClassWizard generated virtual function overrides
//{{AFX_VIRTUAL(CProcessSelectDlg)
protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
//}}AFX_VIRTUAL

// Implementation
protected:

// Generated message map functions
//{{AFX_MSG(CProcessSelectDlg)
   virtual BOOL OnInitDialog();
   virtual void OnOK();
   afx_msg void OnDblclkListProcess(NMHDR* pNMHDR, LRESULT* pResult);
//}}AFX_MSG

   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROCESSSELECTDLG_H__A87838A5_8055_4C01_A5B9_8B44D53C6ED4__INCLUDED_)
