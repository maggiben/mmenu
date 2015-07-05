#include "stdafx.h"
#include "GDIndicator.h"

#include "GDIndicatorDoc.h"

#ifdef _DEBUG
   #define new DEBUG_NEW
   #undef THIS_FILE
   static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CGDIndicatorDoc, CDocument)

BEGIN_MESSAGE_MAP(CGDIndicatorDoc, CDocument)
//{{AFX_MSG_MAP(CGDIndicatorDoc)
   // NOTE - the ClassWizard will add and remove mapping macros here.
   //    DO NOT EDIT what you see in these blocks of generated code!
//}}AFX_MSG_MAP
END_MESSAGE_MAP()



//--------------------------------------------------------------------
// Construction/Destruction
//

CGDIndicatorDoc::CGDIndicatorDoc()
{
}

CGDIndicatorDoc::~CGDIndicatorDoc()
{
}


BOOL CGDIndicatorDoc::OnNewDocument()
{
// default MFC processing
   if (!CDocument::OnNewDocument())
      return FALSE;

// don't forget to notify the user that he has to wait
   CWaitCursor CursorIsChangedToWaitUntilTheEndOfBlock;
   CFrameWnd* pMainWnd = (CFrameWnd*)AfxGetMainWnd();
   if (pMainWnd != NULL)
      pMainWnd->SetMessageText(_T("Searching processes..."));

// fill the process list
   m_ProcessList.Refresh();

// reset status bar info
   if (pMainWnd != NULL)
      pMainWnd->SetMessageText(AFX_IDS_IDLEMESSAGE);


// initialization is successful
   return(TRUE);
}
//
//--------------------------------------------------------------------


//--------------------------------------------------------------------
// Serialization & Diagnostics
//

void CGDIndicatorDoc::Serialize(CArchive& ar)
{
   if (ar.IsStoring())
   {
      // TODO: add storing code here
   }
   else
   {
      // TODO: add loading code here
   }
}


#ifdef _DEBUG
void CGDIndicatorDoc::AssertValid() const
{
   CDocument::AssertValid();
}

void CGDIndicatorDoc::Dump(CDumpContext& dc) const
{
   CDocument::Dump(dc);
}
#endif //_DEBUG
//
//--------------------------------------------------------------------




