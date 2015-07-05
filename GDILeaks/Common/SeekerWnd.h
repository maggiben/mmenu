#if !defined(AFX_SEEKERWND_H__5550B5C1_73E8_11D2_83A1_00A0C9A528B7__INCLUDED_)
#define AFX_SEEKERWND_H__5550B5C1_73E8_11D2_83A1_00A0C9A528B7__INCLUDED_

#if _MSC_VER >= 1000
   #pragma once
#endif // _MSC_VER >= 1000


class CSeekerEdit : public CEdit
{
// Construction/Destruction
//
public:
   CSeekerEdit();
   virtual ~CSeekerEdit();


// C++ interface
// 
public:
   void SetCursorID(DWORD CursorID);


// Implementation details
//
private:
   HWND     m_hSeekedWindow;
   BOOL     m_bSeeking;
   HCURSOR  m_hSeekingCursor;


// MFC stuff
//
public:
// Overrides
// ClassWizard generated virtual function overrides
//{{AFX_VIRTUAL(CSeekerEdit)
//}}AFX_VIRTUAL

// Generated message map functions
protected:
//{{AFX_MSG(CSeekerEdit)
   afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
   afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
   afx_msg void OnMouseMove(UINT nFlags, CPoint point);
//}}AFX_MSG

   DECLARE_MESSAGE_MAP()
};




class CSeekerStatic : public CStatic
{
// Construction
public:
   CSeekerStatic();
   virtual ~CSeekerStatic();


// C++ interface
// 
public:
   void SetCursorIDs(DWORD CursorID, DWORD IconUp, DWORD IconDown);
   void SetTwinWnd(HWND hWnd);


// Implementation details
//
private:
   HWND     m_hSeekedWindow;
   BOOL     m_bSeeking;

   HCURSOR  m_hSeekingCursor;
   HICON    m_hIconUp;
   HICON    m_hIconDown;

   HWND     m_hTwinWnd;


// MFC stuff
// 
public:
// Overrides
// ClassWizard generated virtual function overrides
//{{AFX_VIRTUAL(CSeekerStatic)
//}}AFX_VIRTUAL

// Generated message map functions
protected:
//{{AFX_MSG(CSeekerStatic)
   afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
   afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
   afx_msg void OnMouseMove(UINT nFlags, CPoint point);
//}}AFX_MSG

   DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEEKERWND_H__5550B5C1_73E8_11D2_83A1_00A0C9A528B7__INCLUDED_)
