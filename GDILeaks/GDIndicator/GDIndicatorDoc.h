#if !defined(AFX_GDINDICATORDOC_H__E59895B4_003E_4296_AC36_2512D9BA039A__INCLUDED_)
#define AFX_GDINDICATORDOC_H__E59895B4_003E_4296_AC36_2512D9BA039A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\Common\Process.h"


class CGDIndicatorDoc : public CDocument
{
   DECLARE_DYNCREATE(CGDIndicatorDoc)

// Construction/Destruction
// 
protected: // create from serialization only
   CGDIndicatorDoc();
public:
   virtual ~CGDIndicatorDoc();


// C++ interface
//
public:
   inline CProcessList& GetProcessList()  
      { return(m_ProcessList); }


// Implementation details
//
private:
   CProcessList m_ProcessList;


// MFC stuff
//
public:
// Overrides
// ClassWizard generated virtual function overrides
//{{AFX_VIRTUAL(CGDIndicatorDoc)
public:
   virtual BOOL OnNewDocument();
   virtual void Serialize(CArchive& ar);
//}}AFX_VIRTUAL

// Implementation
public:
#ifdef _DEBUG
   virtual void AssertValid() const;
   virtual void Dump(CDumpContext& dc) const;
#endif


// Generated message map functions
protected:
//{{AFX_MSG(CGDIndicatorDoc)
   // NOTE - the ClassWizard will add and remove member functions here.
   //    DO NOT EDIT what you see in these blocks of generated code !
//}}AFX_MSG

   DECLARE_MESSAGE_MAP()
};



//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GDINDICATORDOC_H__E59895B4_003E_4296_AC36_2512D9BA039A__INCLUDED_)
