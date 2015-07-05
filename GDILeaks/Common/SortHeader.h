#if !defined(AFX_SORTHEADER_H__B6A4EC33_7EE7_11D2_83A8_00A0C9A528B7__INCLUDED_)
#define AFX_SORTHEADER_H__B6A4EC33_7EE7_11D2_83A8_00A0C9A528B7__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class CSortHeader : public CHeaderCtrl
{
// Construction/Destruction
//
public:
   CSortHeader();
   virtual ~CSortHeader();


// C++ Interface
//
public:
   int SetSortImage(int nCol, BOOL bAsc);


// Implementation details
// 
private:
   int   m_nSortCol;
   BOOL  m_bSortAsc;


// MFC stuff
//
public:
// Overrides
// ClassWizard generated virtual function overrides
//{{AFX_VIRTUAL(CSortHeader)
   void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
//}}AFX_VIRTUAL

// Generated message map functions
protected:
//{{AFX_MSG(CSortHeader)
//}}AFX_MSG


   DECLARE_MESSAGE_MAP()
};



//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SORTHEADER_H__B6A4EC33_7EE7_11D2_83A8_00A0C9A528B7__INCLUDED_)
