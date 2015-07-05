// DisplayDlg.h : header file
//
#if !defined(AFX_DISPLAYDLG_H__D2FEC1E5_367E_11D2_837A_00A0C9A528B7__INCLUDED_)
#define AFX_DISPLAYDLG_H__D2FEC1E5_367E_11D2_837A_00A0C9A528B7__INCLUDED_

#if _MSC_VER >= 1000
   #pragma once
#endif // _MSC_VER >= 1000

#include "resource.h"
#include "GDIReflect.h"

/////////////////////////////////////////////////////////////////////////////
// CDisplayDlg dialog

class CDisplayDlg : public CDialog
{
// Construction
public:
   CDisplayDlg(CWnd* pParent, CGDIReflect* pGDIReflect);

protected:
   void DrawResource(CDC* pDC, HGDIOBJ hObject);

private:
   CRect          m_DlgRect;
   CGDIReflect*   m_pGDIReflect;



// MFC stuff
//
public:
// Dialog Data
//{{AFX_DATA(CDisplayDlg)
   enum { IDD = IDD_GRAPHIC_DISPLAY };
   // NOTE: the ClassWizard will add data members here
//}}AFX_DATA

// Overrides
// ClassWizard generated virtual function overrides
//{{AFX_VIRTUAL(CDisplayDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
//}}AFX_VIRTUAL


// Implementation
protected:

// Generated message map functions
//{{AFX_MSG(CDisplayDlg)
   afx_msg void OnSelChangeItemList();
   virtual BOOL OnInitDialog();
   afx_msg void OnSize(UINT nType, int cx, int cy);
   afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
   afx_msg void OnPaint();
//}}AFX_MSG

   DECLARE_MESSAGE_MAP()
};



//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DISPLAYDLG_H__D2FEC1E5_367E_11D2_837A_00A0C9A528B7__INCLUDED_)

