#if !defined(AFX_BROWSEFILEEDIT_H__6A805B0F_823B_11D2_B10A_000000000000__INCLUDED_)
#define AFX_BROWSEFILEEDIT_H__6A805B0F_823B_11D2_B10A_000000000000__INCLUDED_

#if _MSC_VER > 1000
   #pragma once
#endif // _MSC_VER > 1000
// BrowseFileEdit.h : header file
//



class CBrowseFileEdit : public CEdit
{
// Construction/destruction
//
public:
   CBrowseFileEdit();
   virtual ~CBrowseFileEdit();


// C++ public interface
//
public:
   void SetFilter(LPCTSTR szFilter, LPCTSTR szDescription);
   BOOL Browse();


// Attributes
public:
   CString  m_szFilter;
   CString  m_szDescription;


// Implementation helpers
// 
private:
   void MakeFullFilter();


// Implementation details
// 
private:
   TCHAR m_szFullFilter[_MAX_PATH+1];


//
// MFC stuff
//
// Overrides
// ClassWizard generated virtual function overrides
//{{AFX_VIRTUAL(CBrowseFileEdit)
//}}AFX_VIRTUAL

// Generated message map functions
protected:
//{{AFX_MSG(CBrowseFileEdit)
   afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
   afx_msg LRESULT OnDropFiles(WPARAM wParam, LPARAM lParam);
//}}AFX_MSG

   DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BROWSEFILEEDIT_H__6A805B0F_823B_11D2_B10A_000000000000__INCLUDED_)
