#include "stdafx.h"
#include "GDIndicator.h"

#include "GDIndicatorDoc.h"
#include "GDIndicatorView.h"
#include "..\Common\Process.h"
#include "..\Common\GdiResources.h"
#include "..\GDInject\_GDInject.h"
#include "..\common\Helpers.h"




#ifdef _DEBUG
   #define new DEBUG_NEW
   #undef THIS_FILE
   static char THIS_FILE[] = __FILE__;
#endif


// each line in CGDIndicatorView has a IndicatorViewElement as item data
//
struct IndicatorViewElement
{
   CProcess*      pProcess;
   CGdiResources  resGDI;
};


// content of each column
const int COLUMN_NAME         =  0;
const int COLUMN_PID          =  1;
const int COLUMN_GDICOUNT     =  2;
const int COLUMN_BITMAP       =  3;
const int COLUMN_BRUSH        =  4;
const int COLUMN_DC           =  5;
const int COLUMN_ENHMETAFILE  =  6;
const int COLUMN_FONT         =  7;
const int COLUMN_PALETTE      =  8;
const int COLUMN_PEN          =  9;
const int COLUMN_EXTPEN       = 10;
const int COLUMN_REGION       = 11;

const int COLUMN_OTHER        = 12;

const int COLUMN_FIRST        = 0;
const int COLUMN_LAST         = COLUMN_OTHER;


IMPLEMENT_DYNCREATE(CGDIndicatorView, CSortListView)

BEGIN_MESSAGE_MAP(CGDIndicatorView, CSortListView)
//{{AFX_MSG_MAP(CGDIndicatorView)
   ON_WM_CREATE()
   ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
   ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
   ON_WM_LBUTTONDBLCLK()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()



//--------------------------------------------------------------------
// construction/destruction
// 

CGDIndicatorView::CGDIndicatorView()
{
}

CGDIndicatorView::~CGDIndicatorView()
{
}

BOOL CGDIndicatorView::PreCreateWindow(CREATESTRUCT& cs)
{
// default processing
   BOOL bReturn = CSortListView::PreCreateWindow(cs);

// the list is on REPORT mode by default
   cs.style |= LVS_REPORT;

// the list is single selection only
   cs.style |= LVS_SINGLESEL;

// how the selected item in a different background color even 
// if the listview has not the focus
   cs.style |= LVS_SHOWSELALWAYS;

   return(bReturn);
}


int CGDIndicatorView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
// default MFC processing
   if (CSortListView::OnCreate(lpCreateStruct) == -1)
      return(-1);

// get a reference to the mapped CListCtrl   
   CListCtrl& Itself = GetListCtrl();

// set the colums for the header
   LV_COLUMN Column;
   Column.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

// Add the columns.
   Column.cx = 40;
   Column.iSubItem = COLUMN_PID;
   Column.pszText = _T("Id");
   Column.fmt = LVCFMT_RIGHT;
   Itself.InsertColumn(Column.iSubItem, &Column);

   Column.cx = 100;
   Column.iSubItem = COLUMN_NAME;
   Column.pszText = _T("Name");
   Column.fmt = LVCFMT_LEFT;
   Itself.InsertColumn(Column.iSubItem, &Column);

   Column.cx = 60;
   Column.iSubItem = COLUMN_GDICOUNT;
   Column.pszText = _T("Total");
   Column.fmt = LVCFMT_RIGHT;
   Itself.InsertColumn(Column.iSubItem, &Column);

   Column.cx = 65;
   Column.iSubItem = COLUMN_BITMAP;
   Column.pszText = _T("Bitmap");
   Column.fmt = LVCFMT_RIGHT;
   Itself.InsertColumn(Column.iSubItem, &Column);

   Column.cx = 60;
   Column.iSubItem = COLUMN_BRUSH;
   Column.pszText = _T("Brush");
   Column.fmt = LVCFMT_RIGHT;
   Itself.InsertColumn(Column.iSubItem, &Column);

   Column.cx = 50;
   Column.iSubItem = COLUMN_DC;
   Column.pszText = _T("DC");
   Column.fmt = LVCFMT_RIGHT;
   Itself.InsertColumn(Column.iSubItem, &Column);

   Column.cx = 75;
   Column.iSubItem = COLUMN_ENHMETAFILE;
   Column.pszText = _T("EnhMeta");
   Column.fmt = LVCFMT_RIGHT;
   Itself.InsertColumn(Column.iSubItem, &Column);

   Column.cx = 55;
   Column.iSubItem = COLUMN_FONT;
   Column.pszText = _T("Font");
   Column.fmt = LVCFMT_RIGHT;
   Itself.InsertColumn(Column.iSubItem, &Column);

   Column.cx = 65;
   Column.iSubItem = COLUMN_PALETTE;
   Column.pszText = _T("Palette");
   Column.fmt = LVCFMT_RIGHT;
   Itself.InsertColumn(Column.iSubItem, &Column);

   Column.cx = 50;
   Column.iSubItem = COLUMN_PEN;
   Column.pszText = _T("Pen");
   Column.fmt = LVCFMT_RIGHT;
   Itself.InsertColumn(Column.iSubItem, &Column);

   Column.cx = 65;
   Column.iSubItem = COLUMN_EXTPEN;
   Column.pszText = _T("ExtPen");
   Column.fmt = LVCFMT_RIGHT;
   Itself.InsertColumn(Column.iSubItem, &Column);

   Column.cx = 65;
   Column.iSubItem = COLUMN_REGION;
   Column.pszText = _T("Region");
   Column.fmt = LVCFMT_RIGHT;
   Itself.InsertColumn(Column.iSubItem, &Column);

   Column.cx = 60;
   Column.iSubItem = COLUMN_OTHER;
   Column.pszText = _T("Other");
   Column.fmt = LVCFMT_RIGHT;
   Itself.InsertColumn(Column.iSubItem, &Column);


// sort on the Name column by default
   SortColumn(COLUMN_NAME, TRUE);

// the ItemData should be deleted automatically
   m_bAutoDelete = TRUE;

// creation is successfull
   return(0);
}
//
//--------------------------------------------------------------------


//--------------------------------------------------------------------
// Initialization methods
// 

// fill the list en return the number of inserted processes
int CGDIndicatorView::FillList()
{
// don't forget to notify the user that he has to wait
   CWaitCursor CursorIsChangedToWaitUntilTheEndOfBlock;
   CFrameWnd* pMainWnd = (CFrameWnd*)AfxGetMainWnd();
   if (pMainWnd != NULL)
      pMainWnd->SetMessageText(_T("Sorting..."));


// don't forget to delete the actual content of the listview
   GetListCtrl().DeleteAllItems();

// count the number of process to be returned
   int iCount = 0;

// get the process list from the document
   CGDIndicatorDoc* pDoc = GetDocument();
   ASSERT(pDoc != NULL);

   if (pDoc != NULL)
   {
   // get process list stored into the document
      CProcessList&  ProcessList = pDoc->GetProcessList();

   // get one process after the other
      CProcess* pProcess = NULL;
      POSITION  Pos = 0;
      for (
            pProcess = ProcessList.GetFirst(Pos); 
            (pProcess != NULL); 
            pProcess = ProcessList.GetNext(Pos)
          )
      {
      // add each process in the list
         if (pProcess != NULL)
         {
            IndicatorViewElement* pInfo = new IndicatorViewElement;
            if (pInfo == NULL)
               break;

            pInfo->pProcess = pProcess;
            pInfo->resGDI.GetGDIList(pProcess->GetPID());

         // insert the process into the listview
            InsertItem(pInfo);

         // next...
            iCount++;
         }
      }

   // sort the list 
      SortColumn(m_SortedColumn, m_bAscending);

   // set the selection on the first process
      if (iCount > 0)
      {
         GetListCtrl().EnsureVisible(0, FALSE);
         GetListCtrl().SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
      }
   }

// reset status bar info
   if (pMainWnd != NULL)
      pMainWnd->SetMessageText(AFX_IDS_IDLEMESSAGE);

   return(iCount);
}


void CGDIndicatorView::OnInitialUpdate()
{
// fill the listview with the processes
   FillList();
}
//
//--------------------------------------------------------------------


//--------------------------------------------------------------------
// drawing & diagnostics
//

void CGDIndicatorView::OnDraw(CDC* pDC)
{
   CGDIndicatorDoc* pDoc = GetDocument();
   ASSERT_VALID(pDoc);
   // TODO: add draw code for native data here
}


#ifdef _DEBUG
void CGDIndicatorView::AssertValid() const
{
   CSortListView::AssertValid();
}

void CGDIndicatorView::Dump(CDumpContext& dc) const
{
   CSortListView::Dump(dc);
}

CGDIndicatorDoc* CGDIndicatorView::GetDocument() // non-debug version is inline
{
   ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGDIndicatorDoc)));
   return (CGDIndicatorDoc*)m_pDocument;
}
#endif //_DEBUG
//
//--------------------------------------------------------------------


//--------------------------------------------------------------------
// Overridables
//
void CGDIndicatorView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
   CSortListView::OnUpdate(pSender, lHint, pHint);
}


void CGDIndicatorView::OnNeedText(LPVOID pItem, int nColumn, CString& szContent)
{
// set default value
   szContent = _T("");

// retrieve the IndicatorViewElement* inserted with the item as lParam
   IndicatorViewElement* pItemData = (IndicatorViewElement*)pItem;
   CProcess* pObject = pItemData->pProcess;

   if (pObject != NULL)
      switch (nColumn)
      {
         case COLUMN_PID:
         {
            DWORD dwPID = pObject->GetPID();
            if (dwPID != (DWORD)-1)
               szContent.Format(_T("%u"), dwPID);
            else
               szContent = _T("?");
         }
         break;

         case COLUMN_NAME:
            szContent = pObject->GetName() + _T(" ") + pObject->GetCmdLine();
         break;

         case COLUMN_GDICOUNT:
         {
            DWORD Value = pObject->GetGDIHandleCount();
            DWORD Sum = pItemData->resGDI.GetTotalCount();
            if (Value != (DWORD)-1)
               szContent.Format(_T("%u [%u]"), Value, Sum);
            else
               szContent = _T("?");
         }
         break;

         case COLUMN_BITMAP:
         {
            DWORD Value = pItemData->resGDI.NbBitmap();
            if (Value != (DWORD)-1)
               szContent.Format(_T("%u"), Value);
            else
               szContent = _T("?");
         }
         break;

         case COLUMN_BRUSH:
         {
            DWORD Value = pItemData->resGDI.NbBrush();
            if (Value != (DWORD)-1)
               szContent.Format(_T("%u"), Value);
            else
               szContent = _T("?");
         }
         break;

         case COLUMN_DC:
         {
            DWORD Value = pItemData->resGDI.NbDC();
            if (Value != (DWORD)-1)
               szContent.Format(_T("%u"), Value);
            else
               szContent = _T("?");
         }
         break;

         case COLUMN_ENHMETAFILE:
         {
            DWORD Value = pItemData->resGDI.NbEnhMetafile();
            if (Value != (DWORD)-1)
               szContent.Format(_T("%u"), Value);
            else
               szContent = _T("?");
         }
         break;

         case COLUMN_FONT:
         {
            DWORD Value = pItemData->resGDI.NbFont();
            if (Value != (DWORD)-1)
               szContent.Format(_T("%u"), Value);
            else
               szContent = _T("?");
         }
         break;

         case COLUMN_PALETTE:
         {
            DWORD Value = pItemData->resGDI.NbPalette();
            if (Value != (DWORD)-1)
               szContent.Format(_T("%u"), Value);
            else
               szContent = _T("?");
         }
         break;

         case COLUMN_PEN:
         {
            DWORD Value = pItemData->resGDI.NbPen();
            if (Value != (DWORD)-1)
               szContent.Format(_T("%u"), Value);
            else
               szContent = _T("?");
         }
         break;

         case COLUMN_EXTPEN:
         {
            DWORD Value = pItemData->resGDI.NbExtPen();
            if (Value != (DWORD)-1)
               szContent.Format(_T("%u"), Value);
            else
               szContent = _T("?");
         }
         break;

         case COLUMN_REGION:
         {
            DWORD Value = pItemData->resGDI.NbRegion();
            if (Value != (DWORD)-1)
               szContent.Format(_T("%u"), Value);
            else
               szContent = _T("?");
         }
         break;

         case COLUMN_OTHER:
         {
            DWORD Value = pItemData->resGDI.NbOther();
            if (Value != (DWORD)-1)
               szContent.Format(_T("%u"), Value);
            else
               szContent = _T("?");
         }
         break;
      }
}


int CGDIndicatorView::OnCompareItems(void* pItem1, void* pItem2, int iColumn)
{
   int nResult = 0;

// retrieve the IndicatorViewElement* inserted with the item as lParam
   IndicatorViewElement* pItemData1 = (IndicatorViewElement*)pItem1;
   IndicatorViewElement* pItemData2 = (IndicatorViewElement*)pItem2;

   CProcess* p1 = pItemData1->pProcess;
   CProcess* p2 = pItemData2->pProcess;

   if ((p1 != NULL) && (p2 != NULL))
   {
      switch (iColumn) 
      {
         case COLUMN_PID:
            nResult = p1->GetPID() - p2->GetPID();
         break;

         case COLUMN_NAME:
            nResult = p1->GetName().CompareNoCase(p2->GetName());
         break;

         case COLUMN_GDICOUNT:
            nResult = p1->GetGDIHandleCount() - p2->GetGDIHandleCount();
         break;

         case COLUMN_BITMAP:
            nResult = pItemData1->resGDI.NbBitmap() - pItemData2->resGDI.NbBitmap();
         break;

         case COLUMN_BRUSH:
            nResult = pItemData1->resGDI.NbBrush() - pItemData2->resGDI.NbBrush();
         break;

         case COLUMN_DC:
            nResult = pItemData1->resGDI.NbDC() - pItemData2->resGDI.NbDC();
         break;

         case COLUMN_ENHMETAFILE:
            nResult = pItemData1->resGDI.NbEnhMetafile() - pItemData2->resGDI.NbEnhMetafile();
         break;

         case COLUMN_FONT:
            nResult = pItemData1->resGDI.NbFont() - pItemData2->resGDI.NbFont();
         break;

         case COLUMN_PALETTE:
            nResult = pItemData1->resGDI.NbPalette() - pItemData2->resGDI.NbPalette();
         break;

         case COLUMN_PEN:
            nResult = pItemData1->resGDI.NbPen() - pItemData2->resGDI.NbPen();
         break;

         case COLUMN_EXTPEN:
            nResult = pItemData1->resGDI.NbExtPen() - pItemData2->resGDI.NbExtPen();
         break;

         case COLUMN_REGION:
            nResult = pItemData1->resGDI.NbRegion() - pItemData2->resGDI.NbRegion();
         break;

         case COLUMN_OTHER:
            nResult = pItemData1->resGDI.NbOther() - pItemData2->resGDI.NbOther();
         break;
      }
   }
   else
      ASSERT(FALSE);

   return(nResult);
}


void CGDIndicatorView::OnDeleteItem(void* pItem)
{
   IndicatorViewElement* pInfo = (IndicatorViewElement*)pItem;
   delete pInfo;
}
//
//--------------------------------------------------------------------


//--------------------------------------------------------------------
// clipboard helpers
//

static LPCTSTR pszClipboardHeader = _T("ID\tName\t#Handles\tBitmap\tBrush\tDC\tEnhMetafile\tFont\tPalette\tPen\tExtPen\tRegion\tOther\r\n");

LPSTR CGDIndicatorView::GetClipboardString()
{
   TCHAR* pText = NULL;

   CGDIndicatorDoc* pDoc = GetDocument();
   ASSERT(pDoc != NULL);
   if (pDoc != NULL)
   {
   // get the process list from document
      CProcessList&  ProcessList = pDoc->GetProcessList();

   // allocate the returned buffer
      pText = new TCHAR[_tcslen(pszClipboardHeader) + ProcessList.GetCount() * (4*12 + 32)];
      if (pText != NULL)
      {
         TCHAR* pNext = pText;
         IndicatorViewElement* pItemData = NULL;

         pText[0] = _T('\0');

      // title first
         _tcscpy(pNext, pszClipboardHeader);
         pNext += _tcslen(pszClipboardHeader);

      // concat each process description
         CString szDescription;
         const TCHAR* pTAB =_T("\t"); 

         int Count = GetListCtrl().GetItemCount();
         for (int Current = 0; Current < Count; Current++)
         {
         // add each process into the buffer
            pItemData = (IndicatorViewElement*)GetListCtrl().GetItemData(Current);
            if (pItemData != NULL)
            {
               for (
                     DWORD dwColumn = COLUMN_FIRST; 
                     dwColumn <= COLUMN_LAST; 
                     dwColumn++
                   )
               {
                  OnNeedText(pItemData, dwColumn, szDescription);
                  _tcscpy(pNext, szDescription);
                  pNext += szDescription.GetLength();
                  if (dwColumn != COLUMN_LAST)
                  {
                     _tcscpy(pNext, pTAB);
                     pNext += _tcslen(pTAB);
                  }
               }

            // end of line
               _tcscpy(pNext, _T("\r\n"));
               pNext += _tcslen(_T("\r\n"));
            }
         }
      }
   }

   return(pText);
}


void CGDIndicatorView::OnEditCopy() 
{
// get the string to be sent to the clipboard
   LPSTR pText = GetClipboardString();
   if (pText != NULL)
   {
      CGDIndicatorApp* pApp = (CGDIndicatorApp*)AfxGetApp();
      if (!pApp->CopyTextToClipboard(pText))
         ::MessageBox(
            ::GetActiveWindow(),
            _T("Impossible to copy the process list to clipboard !"),
            _T("Process Tree"),
            MB_OK | MB_ICONHAND
            );
   // don't forget to delete the string
      delete [] pText;
   }
   else
      ::MessageBox(
         ::GetActiveWindow(),
         _T("Nothing to copy to clipboard..."),
         _T("Process Tree"),
         MB_OK | MB_ICONINFORMATION
         );
}


void CGDIndicatorView::OnUpdateEditCopy(CCmdUI* pCmdUI) 
{
   pCmdUI->Enable(GetListCtrl().GetItemCount() > 0);
}
//
//--------------------------------------------------------------------


// wrap AllowSetForegroundWindow() under Windows 2000/XP
// 
typedef BOOL (CALLBACK * ALLOWSETFOREGROUNDWINDOW_PROC)(DWORD);
BOOL _AllowSetForegroundWindow(DWORD PID)
{
// get the function pointer from USER.DLL 
   ALLOWSETFOREGROUNDWINDOW_PROC pfnAllowSetForegroundWindow = (ALLOWSETFOREGROUNDWINDOW_PROC)
      ::GetProcAddress(::GetModuleHandle(_T("user32.dll")), _T("AllowSetForegroundWindow"));
   if (pfnAllowSetForegroundWindow == NULL)
      return(FALSE);

   BOOL bReturn = pfnAllowSetForegroundWindow(PID);

   return(bReturn);
}



// double-click handler
// 
void CGDIndicatorView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
// default processing
   CSortListView::OnLButtonDblClk(nFlags, point);


// get current process
   int iSelection = GetListCtrl().GetNextItem(-1, LVNI_SELECTED);
   if (iSelection == -1)
      return;
   IndicatorViewElement* pItemData = 
      (IndicatorViewElement*)GetListCtrl().GetItemData(iSelection);
   if (pItemData == NULL)
      return;

// check if it has already allocated GDI objects
   if (pItemData->pProcess->GetGDIHandleCount() == 0)
      return;

// refuse for "csrss" process --> crash the machine...
   CString szName = pItemData->pProcess->GetName();
   if (szName == _T("csrss"))
      return;

// serialize the snapshot into the shared memory
   DWORD Size = pItemData->resGDI.GetTotalCount()*sizeof(GDI_ITEM) + sizeof(DWORD);
   GDI_LIST* pData = (GDI_LIST*)new BYTE [Size];
   if (pData != NULL)
   {
      DWORD    CurrentObject = 0;
      POSITION Pos;
      CGdiObj* pGdiObject = NULL;
      LPCTSTR  pDesc = NULL;
      CString  szInfo;
      for (
            pGdiObject = pItemData->resGDI.GetFirst(Pos); 
            (pGdiObject != NULL); 
            pGdiObject = pItemData->resGDI.GetNext(Pos)
          )
      {
         pData->Items[CurrentObject].dwType   = pGdiObject->GetType();
         pData->Items[CurrentObject].hObject  = (HGDIOBJ)pGdiObject->GetHandle();
         CurrentObject++;
      }
      pData->dwCount = CurrentObject;

   // copy it into the shared buffer
      SetDataForRequest(pData, CurrentObject*sizeof(GDI_ITEM) + sizeof(DWORD));

   // don't forget to delete the buffer
      delete [] pData;


   // hide the main window since the following call will lock it
      AfxGetMainWnd()->ShowWindow(SW_HIDE);

   // allow the other process to call SetForegroundWindow()
      _AllowSetForegroundWindow(pItemData->pProcess->GetPID());

   // let's see what's going on in the other process
      ExecuteRemoteAction(pItemData->pProcess->GetPID(), TM_SHOW_LIST);
   
   // restore the main window
      AfxGetMainWnd()->ShowWindow(SW_SHOW);
   }
}

