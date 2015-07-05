#if !defined(AFX_GDIUSAGEDLG_H__1E1946A6_AFC5_4661_8A59_598129771427__INCLUDED_)
#define AFX_GDIUSAGEDLG_H__1E1946A6_AFC5_4661_8A59_598129771427__INCLUDED_

#if _MSC_VER > 1000
   #pragma once
#endif // _MSC_VER > 1000


#include "resource.h"
#include "..\Common\BrowseFileEdit.h"
#include "OutputListBox.h"
#include "..\Common\GdiResources.h"
#include "GDIDebugger.h"
#include "..\Common\WndSaveRestoreHelper.h"



// internal messages definition
// 
// these messages are used notify the dialog when a GDI objects list has been filled up
// by the debuggee application. 
// 
// ID                   wParam      lParam         Description
// -------------------------------------------------------------------------------------
// UM_SNAPSHOT_READY       0        GDITRACELIST*  lParam points to the list content
// UM_COMPARE_READY        0        GDITRACELIST*  lParam points to the list content
//
const DWORD UM_SNAPSHOT_READY = WM_APP+200;
const DWORD UM_COMPARE_READY  = WM_APP+201;


class CGdiUsageDlg : public CDialog
{
// Construction/destruction
public:
   CGdiUsageDlg(CWnd* pParent = NULL); // standard constructor
   ~CGdiUsageDlg();

// implementation details
// 
private:
   // remember if the dialog must be "always on top"
   BOOL  m_bAlwaysOnTop;

   // display the OutputDebugString() and other output from
   // the debuggee application into a dedicated popup listbox 
   COutputListBox m_OutputListBox;
   CString        m_szLastFind;
   HFONT          m_hSmallFont;
   HMENU          m_hOutputPopupMenu;
   HMENU          m_hMenu;

   // thread dedicated to manage the debuggee application
   CWinThread*          m_pDebuggerThread;
   static CGDIDebugger* m_pGDIDebugger;

   // GDI object snapshots
   CGdiResources     m_Current;
   CGdiResources     m_Reference;
   CResourcesCompare m_Compare;

   // placement recovery
   CWndSaveRestoreHelper* m_pPositionHelper;



// internal helpers
// 
private:
   void UpdateUI(BOOL bIsRunning);
   void ResetCounter(int Counter);

private:
static UINT DebugThreadProc(LPARAM lParam);



// MFC stuff
// 
public:
// Dialog Data
//{{AFX_DATA(CGdiUsageDlg)
   enum { IDD = IDD_GDIUSAGE_DIALOG };
   CBrowseFileEdit m_EditApplication;
   // NOTE: the ClassWizard will add data members here
//}}AFX_DATA

// ClassWizard generated virtual function overrides
//{{AFX_VIRTUAL(CGdiUsageDlg)
protected:
   virtual void DoDataExchange(CDataExchange* pDX);   // DDX/DDV support
//}}AFX_VIRTUAL

// Implementation
protected:
   HICON m_hIcon;
   HICON m_hSmallIcon;

// Generated message map functions
//{{AFX_MSG(CGdiUsageDlg)
   virtual BOOL OnInitDialog();
   afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
   afx_msg void OnPaint();
   afx_msg HCURSOR OnQueryDragIcon();
   afx_msg void OnDestroy();
   afx_msg void OnBtnStart();
   afx_msg void OnDeleteAll();
   afx_msg void OnCopyToClipboard();
   afx_msg void OnFind();
   afx_msg void OnBtnTakeSnapshot();
   afx_msg void OnBtnCompare();
   afx_msg void OnBtnDetailsSnapshot();
   afx_msg void OnBtnDetailsCompare();
   afx_msg LRESULT OnGetSnapshotList(WPARAM wParam, LPARAM lParam);
   afx_msg LRESULT OnGetCompareList(WPARAM wParam, LPARAM lParam);
//}}AFX_MSG

   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GDIUSAGEDLG_H__1E1946A6_AFC5_4661_8A59_598129771427__INCLUDED_)
