#include "stdafx.h"
#include "SortListView.h"


#ifdef _DEBUG
   #define new DEBUG_NEW
   #undef THIS_FILE
   static char THIS_FILE[] = __FILE__;
#endif



// structure used for the sorting process
//
typedef struct tagSORTINFO 
{
   CSortListView* pThis;
   int            nColumn;
} SORTINFO, *PSORTINFO;



IMPLEMENT_DYNCREATE(CSortListView, CListView)

BEGIN_MESSAGE_MAP(CSortListView, CListView)
//{{AFX_MSG_MAP(CSortListView)
   ON_WM_CREATE()
   ON_WM_DESTROY()
   ON_MESSAGE(LVM_DELETEITEM, OnDeleteItemMsg)
   ON_MESSAGE(LVM_DELETEALLITEMS, OnDeleteAllItemsMsg)
   ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColumnclick)
   ON_NOTIFY_REFLECT(LVN_GETDISPINFO, OnGetdispinfo)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()



//--------------------------------------------------------------------
// Construction/Destruction
//
CSortListView::CSortListView()
{
   m_bAutoDelete = TRUE;

// the sort is ascending by default
   m_bAscending = TRUE;

// no sorted column by default
   m_SortedColumn = -1;
}


int CSortListView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
// default processing
   if (CListView::OnCreate(lpCreateStruct) == -1)
      return(-1);

// subclass the header control
   m_HeaderCtrl.SubclassWindow(::GetDlgItem(m_hWnd, 0)); 
   
// change the extended style in order to get full row selection
   ListView_SetExtendedListViewStyle(m_hWnd, LVS_EX_FULLROWSELECT);

   return(0);
}


BOOL CSortListView::PreCreateWindow(CREATESTRUCT& cs) 
{
   if (!CListView::PreCreateWindow(cs))
      return(FALSE);
//
// Make sure the control has headers with clickable buttons. Also make
// sure that neither the LVS_SORTASCENDING nor the LVS_SORTDESCENDING
// flag is set.
//
   cs.style &= ~(LVS_NOCOLUMNHEADER | LVS_NOSORTHEADER | LVS_SORTASCENDING | LVS_SORTDESCENDING);
   return(TRUE);
}


CSortListView::~CSortListView()
{
}


void CSortListView::OnDestroy() 
{
//
// If m_bAutoDelete is nonzero, delete the objects that the items'
// lParams point to.
//
   int nCount;
   if (m_bAutoDelete && (nCount = GetListCtrl().GetItemCount())) 
   {
      for (int i = 0; i < nCount; i++) 
      {
         DWORD dwItem = GetListCtrl().GetItemData(i);
         OnDeleteItem((void*) dwItem);
      }
   }

// default processing
   CListView::OnDestroy ();
}
//
//--------------------------------------------------------------------



//--------------------------------------------------------------------
// C++ Interface
//
int CSortListView::SortColumn(int iColumn, BOOL bAscending)
{
   int iOldColumn = m_SortedColumn;

   m_SortedColumn = iColumn;
   m_bAscending = bAscending;

   SORTINFO si;
   si.pThis = this;
   si.nColumn = iColumn;

// update header
   m_HeaderCtrl.SetSortImage(m_SortedColumn, m_bAscending);

// sort items
   GetListCtrl().SortItems(CompareFunc, (DWORD) &si);

   return(iOldColumn);
}


int CSortListView::InsertItem(void* pItem, BOOL bImageCallback, int nItem)
{
// Insert an item into the control. The key here is that the item's
// lParam is actually a pointer to the object that holds the item's
// data. pszText is set to LPSTR_TEXTCALLBACK so our OnGetDispInfo
// function will be called to get the text of an item or subitem.
//
   LV_ITEM item;
   item.mask = LVIF_TEXT | LVIF_PARAM;
   item.pszText = LPSTR_TEXTCALLBACK;
   item.iItem = (nItem == -1) ? GetListCtrl().GetItemCount() : nItem;
   item.iSubItem = 0;
   item.lParam = (LPARAM) pItem;

// check for image callback if needed
   if (bImageCallback)
   {
      item.mask |= LVIF_IMAGE;
      item.iImage = I_IMAGECALLBACK;
   }

   return(GetListCtrl().InsertItem(&item));
}
//
//--------------------------------------------------------------------


//--------------------------------------------------------------------
// Overrdables
//
void CSortListView::OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
//
// When a header button is clicked, call the control's SortItems
// function to sort the data in the column. The column number in the
// second parameter is the lParamSort parameter that's passed to the
// comparison function.
//
   NM_LISTVIEW* pnmlv = (NM_LISTVIEW*) pNMHDR;

   SORTINFO si;
   si.pThis = this;
   si.nColumn = pnmlv->iSubItem;

// check if it's the same column to change the sorting direction
   if (m_SortedColumn == pnmlv->iSubItem)
      m_bAscending = !m_bAscending;

   m_SortedColumn = pnmlv->iSubItem;

// update header
   m_HeaderCtrl.SetSortImage(m_SortedColumn, m_bAscending);

// sort items
   GetListCtrl().SortItems(CompareFunc, (DWORD) &si);
}



void CSortListView::OnGetdispinfo(NMHDR* pNMHDR, LRESULT* pResult) 
{
// Call the derived class's virtual OnNeedText function to get the
// display text for the requested item/subitem.
   LV_DISPINFO* plvdi = (LV_DISPINFO*) pNMHDR;

// call virtual method to get the text to display in each column
   if (plvdi->item.mask & LVIF_TEXT) 
   {
      void* pItem = (void*) plvdi->item.lParam;
      CString szInfo;
      OnNeedText(pItem, plvdi->item.iSubItem, szInfo);
      ::lstrcpy(plvdi->item.pszText, szInfo);
   }

// call virtual method to get the bitmap to display in each column
// --> only for the first column ?
   if (plvdi->item.mask & LVIF_IMAGE) 
   {
      void* pItem = (void*) plvdi->item.lParam;
      plvdi->item.iImage = OnNeedImage(pItem, plvdi->item.iSubItem);
   }
}


LONG CSortListView::OnDeleteItemMsg(UINT wParam, LONG lParam)
{
//
// If m_bAutoDelete is nonzero, delete the object that the item's
// lParam points to. Allow the default window procedure to delete the
// item itself.
//
   if (m_bAutoDelete) 
   {
      DWORD dwItem = GetListCtrl().GetItemData((int) wParam);
   
   // Must come BEFORE OnDeleteItem!
      LRESULT lResult = Default(); 
      OnDeleteItem ((void*) dwItem);
      return(lResult);
   }

// default processing
   return(Default());
}


LONG CSortListView::OnDeleteAllItemsMsg(UINT wParam, LONG lParam)
{
//
// If m_bAutoDelete is nonzero, delete the objects that the items'
// lParams point to. Allow the default window procedure to delete the
// items themselves.
//
   int nCount;
   if (m_bAutoDelete && (nCount = GetListCtrl().GetItemCount())) 
   {
      for (int i = 0; i < nCount; i++) 
      {
         DWORD dwItem = GetListCtrl().GetItemData(i);
         OnDeleteItem((void*) dwItem);
      }
   }

// default processing
   return(Default());
}


void CSortListView::OnDeleteItem(void* pItem)
{
//
// If this function gets called, m_bAutoDelete better be FALSE.
// Otherwise, somebody forgot to override it in the derived class.
//
   ASSERT(!m_bAutoDelete);
}


int CSortListView::OnCompareItems(void*, void*, int)
{
   TRACE("OnCompareItems() must be overriden !\n");
   ASSERT(FALSE);

   return(0);
}


void CSortListView::OnNeedText(LPVOID pItem, int nColumn, CString& szText)
{
   TRACE("OnNeedText() must be overriden !\n");
   ASSERT(FALSE);

   szText = _T("");
}

int CSortListView::OnNeedImage(void* pItem, int nColumn)
{
   TRACE("OnNeedImage() must be overriden !\n");
   ASSERT(FALSE);

   return(0);
}
//
//--------------------------------------------------------------------




//--------------------------------------------------------------------
// Comparison function
//
int CALLBACK CSortListView::CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
//
// Call the derived class's virtual OnCompareItems function to
// compare the items or subitems referenced by lParam1 and lParam2.
//
   PSORTINFO psi = (PSORTINFO) lParamSort;
   CSortListView* pThis = psi->pThis;
   int nColumn = psi->nColumn;

// check for sorting direction
   if (psi->pThis->m_bAscending)
      return(pThis->OnCompareItems((void*) lParam1, (void*) lParam2, nColumn));
   else
      return(-pThis->OnCompareItems((void*) lParam1, (void*) lParam2, nColumn));
}
//
//--------------------------------------------------------------------

