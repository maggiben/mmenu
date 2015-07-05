// SortListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include <afxwin.h>
#include <afxcmn.h>

#include "SortListCtrl.h"


#ifdef _DEBUG
   #define new DEBUG_NEW
   #undef THIS_FILE
   static char THIS_FILE[] = __FILE__;
#endif


// CSortListCtrl message map
//
IMPLEMENT_DYNCREATE(CSortListCtrl, CListCtrl)

BEGIN_MESSAGE_MAP(CSortListCtrl, CListCtrl)
//{{AFX_MSG_MAP(CSortListCtrl)
   ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColumnclick)
   ON_NOTIFY_REFLECT(LVN_GETDISPINFO, OnGetdispinfo)
   ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, OnItemchanged)

   ON_MESSAGE(LVM_DELETEITEM, OnDeleteItemMsg)
   ON_MESSAGE(LVM_DELETEALLITEMS, OnDeleteAllItemsMsg)

   ON_WM_DESTROY()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()



// CSortListCtrl constructors/destructors
//
CSortListCtrl::CSortListCtrl()
{
   m_bAutoDelete = TRUE;

// the sort is ascending by default
   m_bAscending = TRUE;

// no sorted column by default
   m_SortedColumn = -1;

// no selected item by default
   m_SelectedItem = -1;
}


BOOL CSortListCtrl::PreCreateWindow(CREATESTRUCT& cs) 
{
   if (!CListCtrl::PreCreateWindow(cs))
      return(FALSE);
//
// Make sure the control has headers with clickable buttons. Also make
// sure that neither the LVS_SORTASCENDING nor the LVS_SORTDESCENDING
// flag is set.
//
   cs.style &= ~(LVS_NOCOLUMNHEADER | LVS_NOSORTHEADER | LVS_SORTASCENDING | LVS_SORTDESCENDING);
   return(TRUE);
}


CSortListCtrl::~CSortListCtrl()
{
}



BOOL CSortListCtrl::BindToControl(HWND hListCtrl)
{
// sanity checks
   ASSERT(hListCtrl != NULL);
   ASSERT(hListCtrl == m_hWnd);
   if ((hListCtrl == NULL) || (hListCtrl != m_hWnd))
      return(FALSE);


// subclass the header control
	m_HeaderCtrl.SubclassWindow(::GetDlgItem(m_hWnd, 0));	
	
// change the extended style in order to get full row selection
   ListView_SetExtendedListViewStyle(m_hWnd, LVS_EX_FULLROWSELECT);

// change the default styles
   DWORD dwStyle;
   dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);
   dwStyle &= ~(LVS_NOCOLUMNHEADER | LVS_NOSORTHEADER | LVS_SORTASCENDING | LVS_SORTDESCENDING);
   ::SetWindowLong(m_hWnd, GWL_STYLE, dwStyle);

   return(TRUE);
}



// CSortListCtrl message handlers
//

void CSortListCtrl::OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
// When a header button is clicked, call the control's SortItems
// function to sort the data in the column. The column number in the
// second parameter is the lParamSort parameter that's passed to the
// comparison function.
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

   SortItems(CompareFunc, (DWORD) &si);
}


void CSortListCtrl::OnGetdispinfo(NMHDR* pNMHDR, LRESULT* pResult) 
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


void CSortListCtrl::OnDestroy() 
{
// If m_bAutoDelete is nonzero, delete the objects that the items'
// lParams point to.
   int nCount;
   if (m_bAutoDelete && (nCount = GetItemCount())) 
   {
      for (int i = 0; i < nCount; i++) 
      {
         DWORD dwItem = GetItemData(i);
         OnDeleteItem((void*) dwItem);
      }
   }

// default processing
   CListCtrl::OnDestroy ();
}


LONG CSortListCtrl::OnDeleteItemMsg(UINT wParam, LONG lParam)
{
// If m_bAutoDelete is nonzero, delete the object that the item's
// lParam points to. Allow the default window procedure to delete the
// item itself.
   if (m_bAutoDelete) 
   {
      DWORD dwItem = GetItemData((int) wParam);
   
   // Must come BEFORE OnDeleteItem!
      LRESULT lResult = Default(); 
      OnDeleteItem ((void*) dwItem);
      return(lResult);
   }

// default processing
   return(Default());
}


LONG CSortListCtrl::OnDeleteAllItemsMsg(UINT wParam, LONG lParam)
{
// If m_bAutoDelete is nonzero, delete the objects that the items'
// lParams point to. Allow the default window procedure to delete the
// items themselves.
   int nCount;
   if (m_bAutoDelete && (nCount = GetItemCount())) 
   {
      for (int i = 0; i < nCount; i++) 
      {
         DWORD dwItem = GetItemData(i);
         OnDeleteItem((void*) dwItem);
      }
   }

// default processing
   return(Default());
}



// Overridables
//
void CSortListCtrl::OnDeleteItem(void* pItem)
{
// If this function gets called, m_bAutoDelete better be FALSE.
// Otherwise, somebody forgot to override it in the derived class.
   ASSERT(!m_bAutoDelete);
}


int CSortListCtrl::OnCompareItems(void*, void*, int)
{
   TRACE("OnCompareItems() must be overriden !\n");
   ASSERT(FALSE);

   return(0);
}


void CSortListCtrl::OnNeedText(void* pItem, int nColumn, CString& szText)
{
   TRACE("OnNeedText() must be overriden !\n");
   ASSERT(FALSE);

   szText = _T("");
}


int CSortListCtrl::OnNeedImage(void* pItem, int nColumn)
{
   TRACE("OnNeedImage() must be overriden !\n");
   ASSERT(FALSE);

   return(0);
}


// Public interface functions
//
int CSortListCtrl::InsertItem(void* pItem, BOOL bImageCallback, int nItem)
{
// Insert an item into the control. The key here is that the item's
// lParam is actually a pointer to the object that holds the item's
// data. pszText is set to LPSTR_TEXTCALLBACK so our OnGetDispInfo
// function will be called to get the text of an item or subitem.
//
   LV_ITEM item;
   item.mask = LVIF_TEXT | LVIF_PARAM;
   item.pszText = LPSTR_TEXTCALLBACK;
   item.iItem = (nItem == -1) ? GetItemCount() : nItem;
   item.iSubItem = 0;
   item.lParam = (LPARAM) pItem;

// check for image callback if needed
   if (bImageCallback)
   {
      item.mask |= LVIF_IMAGE;
      item.iImage = I_IMAGECALLBACK;
   }

   return(CListCtrl::InsertItem (&item));
}


// Comparison function
//
int CALLBACK CSortListCtrl::CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
// Call the derived class's virtual OnCompareItems function to
// compare the items or subitems referenced by lParam1 and lParam2.
   PSORTINFO psi = (PSORTINFO) lParamSort;
   CSortListCtrl* pThis = psi->pThis;
   int nColumn = psi->nColumn;

// check for sorting direction
   if (psi->pThis->m_bAscending)
      return(pThis->OnCompareItems((void*) lParam1, (void*) lParam2, nColumn));
   else
      return(-pThis->OnCompareItems((void*) lParam1, (void*) lParam2, nColumn));
}


void CSortListCtrl::OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
   NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

// remember the selected item
   if (pNMListView->uNewState == 3) 
                              // ^-- LVIS_FOCUSED | LVIS_SELECTED
      m_SelectedItem = pNMListView->iItem;

   *pResult = 0;
}


int CSortListCtrl::GetCurSel()
{
   return(m_SelectedItem);
}


int CSortListCtrl::SetCurSel(int iSelection)
{
// change the state for the new selection
   SetItemState(iSelection, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);

// force a refresh
   RedrawItems(iSelection, iSelection);
   UpdateWindow();

   return(0);
}



int CSortListCtrl::SortColumn(int iColumn, BOOL bAscending)
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
   SortItems(CompareFunc, (DWORD) &si);

   return(iOldColumn);
}


void CSortListCtrl::SortList()
{
   SORTINFO si;
   si.pThis = this;
   si.nColumn = m_SortedColumn;

// sort items
   SortItems(CompareFunc, (DWORD) &si);
}

