#if !defined(AFX_DISPLAYDLG_H__D2FEC1E5_367E_11D2_837A_00A0C9A528B7__INCLUDED_)
#define AFX_DISPLAYDLG_H__D2FEC1E5_367E_11D2_837A_00A0C9A528B7__INCLUDED_

#if _MSC_VER >= 1000
   #pragma once
#endif // _MSC_VER >= 1000


#include "GdiResources.h"


// check that the needed declarations and resources are included into your project
// 
#ifndef IDC_ITEM_LIST
   #pragma message("You have to add IDD_GRAPHIC_DISPLAY and IDC_ITEM_LIST into your project resource.h")
//
// definition of needed resource identifiers to add in the resource.h
// 
   #define IDD_GRAPHIC_DISPLAY   131
   #define IDC_ITEM_LIST         1005
#endif


#ifndef IDD_GRAPHIC_DISPLAY
   #pragma message("You have to add the IDD_GRAPHIC_DISPLAY dialog description into your resources")
//
// you should add the following dialog box description into your .RC file
//
/*
IDD_GRAPHIC_DISPLAY DIALOGEX 0, 0, 286, 173
STYLE DS_CENTER | WS_POPUP | WS_VISIBLE | WS_CLIPCHILDREN | WS_CAPTION | 
    WS_SYSMENU | WS_THICKFRAME
EXSTYLE WS_EX_TOOLWINDOW | WS_EX_CLIENTEDGE
CAPTION "Resource display"
FONT 8, "MS Sans Serif", 0, 0, 0x1
BEGIN
    LISTBOX         IDC_ITEM_LIST,0,0,91,172,LBS_SORT | LBS_USETABSTOPS | 
                    LBS_NOINTEGRALHEIGHT | WS_VSCROLL | WS_TABSTOP
END
*/
#endif



// Interface called when events occur
//  
class INotificationCallBack
{
public:
   virtual void OnDoubleClick(HWND hWnd, DWORD hObject) = 0;
   virtual void OnSelect(HWND hWnd, DWORD hObject) = 0;
};


class CGdiResourcesDlg : public CDialog
{
// Construction/destruction
//
public:
   CGdiResourcesDlg(CWnd* pParent, CGdiResources* pResources);
private:
   CGdiResourcesDlg(CWnd* pParent = NULL);


// C++ Interface
// 
public:
   void SetNotificationCallBack(INotificationCallBack* pNotifications);


// implementation helpers
// 
private:
   void DrawResource(CDC* pDC, CGdiObj* pObject);


// implementation details
// 
private:
   CRect                   m_DlgRect;
   CGdiResources*          m_pResources;
   INotificationCallBack*  m_pNotifications;

// MFC stuff
//
public:
// Dialog Data
//{{AFX_DATA(CGdiResourcesDlg)
   enum { IDD = IDD_GRAPHIC_DISPLAY };
   // NOTE: the ClassWizard will add data members here
//}}AFX_DATA

// Overrides
// ClassWizard generated virtual function overrides
//{{AFX_VIRTUAL(CGdiResourcesDlg)
protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
//}}AFX_VIRTUAL


// Generated message map functions
//{{AFX_MSG(CGdiResourcesDlg)
   afx_msg void OnSelChangeItemList();
   afx_msg void OnDblclkItemList();
   afx_msg void OnSize(UINT nType, int cx, int cy);
   afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
   afx_msg void OnPaint();
   virtual BOOL OnInitDialog();
//}}AFX_MSG

   DECLARE_MESSAGE_MAP()
};



//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DISPLAYDLG_H__D2FEC1E5_367E_11D2_837A_00A0C9A528B7__INCLUDED_)

