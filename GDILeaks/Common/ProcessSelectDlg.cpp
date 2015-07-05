#include "stdafx.h"
#include "ProcessSelectDlg.h"


#ifdef _DEBUG
   #define new DEBUG_NEW
   #undef THIS_FILE
   static char THIS_FILE[] = __FILE__;
#endif



// column description
// 
const DWORD COLUMN_NAME = 0;
const DWORD COLUMN_PID  = 1;




CProcessListCtrl::CProcessListCtrl()
{

}

CProcessListCtrl::~CProcessListCtrl()
{

}


//------------------------------------------------------------------
// C++ public interface
// 
BOOL CProcessListCtrl::InitColumns()
{
// subclass the corresponding control
   if (!BindToControl(m_hWnd))
      return(FALSE);

// if there is any column, it fails
   LV_COLUMN Column;
   Column.mask = LVCF_WIDTH;
   if (GetColumn(0, &Column))
      return(FALSE);

// set the columns
// set the colums for the header
   Column.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

// Add the columns.
   Column.cx = 330;
   Column.iSubItem = COLUMN_NAME;
   Column.pszText = _T("Name");
   Column.fmt = LVCFMT_LEFT;
   InsertColumn(Column.iSubItem, &Column);

   Column.cx = 50;
   Column.iSubItem = COLUMN_PID;
   Column.pszText = _T("ID");
   Column.fmt = LVCFMT_RIGHT;
   InsertColumn(Column.iSubItem, &Column);

// no need to delete the ItemData
   m_bAutoDelete = FALSE;

// sort on the ID column by default
   SortColumn(COLUMN_NAME, TRUE);

   return(TRUE);
}


int CProcessListCtrl::FillList()
{
   CWaitCursor WaitUntilTheEnd;

   int iCount = 0;

// don't forget to delete the actual content of the listview
   DeleteAllItems();

// get one process after the other
   m_ProcessList.Refresh();

   POSITION    Pos = 0;
   CProcess*   pProcess = NULL;
   for (
         pProcess = m_ProcessList.GetFirst(Pos); 
         (pProcess != NULL); 
         pProcess = m_ProcessList.GetNext(Pos)
       )
   {
   // add each process in the list
      if (pProcess != NULL)
      {
      // insert the process into the listview
         InsertItem(pProcess, FALSE, iCount);

      // next...
         iCount++;
      }
   }

// sort the list 
   SortColumn(m_SortedColumn, m_bAscending);

// set the selection on the first process
   if (iCount > 0)
   {
      EnsureVisible(0, FALSE);
      SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
   }

   return(m_ProcessList.GetCount());
}

DWORD CProcessListCtrl::GetSelectedPID()
{
   DWORD CurrentPID = 0;
   int iSelection = GetNextItem(-1, LVNI_SELECTED);
   if (iSelection != -1)
   {
   // get the process ID
      CProcess* pInfo = (CProcess*)GetItemData(iSelection);
      if (pInfo != NULL)
      {
         CurrentPID = pInfo->GetPID();
      }
   }

   return(CurrentPID);
}
//
//------------------------------------------------------------------



//------------------------------------------------------------------
// overridables
// 

// ask for the text of each cell corresponding to pItem ("lParam" of each line)
//
void CProcessListCtrl::OnNeedText(LPVOID pItem, int nColumn, CString& szText)
{
   szText = _T("");

   CProcess* pObject = (CProcess*)pItem;

   if (pObject != NULL)
      switch (nColumn)
      {
         case COLUMN_NAME:
            szText = pObject->GetName() + _T(" ") + pObject->GetCmdLine();
         break;

         case COLUMN_PID:
         {
            DWORD dwPID = pObject->GetPID();
            if (dwPID != (DWORD)-1)
               szText.Format(_T("%u"), dwPID);
            else
               szText = _T("?");
         }
         break;


         default:
         // not supposed to happen...
            ASSERT(FALSE);
         break;
      }
   else
   // not supposed to happen...
      ASSERT(FALSE);
}


int CProcessListCtrl::OnCompareItems(void* pItem1, void* pItem2, int iColumn)
{
   int nResult = 0;

   CProcess* p1 = (CProcess*)pItem1;
   CProcess* p2 = (CProcess*)pItem2;

   if ((p1 != NULL) && (p2 != NULL))
   {
      switch (iColumn) 
      {
         case COLUMN_NAME:
            nResult = p1->GetName().CompareNoCase(p2->GetName());
         break;

         case COLUMN_PID:
            nResult = p1->GetPID() - p2->GetPID();
         break;


         default:
         // not supposed to happen...
            ASSERT(FALSE);
         break;
      }   
   }
   else
   // not supposed to happen...
      ASSERT(FALSE);

   return(nResult);
}
//
//------------------------------------------------------------------








BEGIN_MESSAGE_MAP(CProcessSelectDlg, CDialog)
//{{AFX_MSG_MAP(CProcessSelectDlg)
   ON_NOTIFY(NM_DBLCLK, IDC_LIST_PROCESS, OnDblclkListProcess)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()



CProcessSelectDlg::CProcessSelectDlg(CWnd* pParent /*=NULL*/)
   : CDialog(CProcessSelectDlg::IDD, pParent)
{
//{{AFX_DATA_INIT(CProcessSelectDlg)
   // NOTE: the ClassWizard will add member initialization here
//}}AFX_DATA_INIT

   m_PID = 0;
}


void CProcessSelectDlg::DoDataExchange(CDataExchange* pDX)
{
   CDialog::DoDataExchange(pDX);

//{{AFX_DATA_MAP(CProcessSelectDlg)
   DDX_Control(pDX, IDC_LIST_PROCESS, m_ProcessListCtrl);
//}}AFX_DATA_MAP
}


BOOL CProcessSelectDlg::OnInitDialog() 
{
// default processing
   CDialog::OnInitDialog();
   
// set the list columns
   if (!m_ProcessListCtrl.InitColumns())
   {
      ASSERT(FALSE);
      EndDialog(IDCANCEL);
   }
   
   if (m_ProcessListCtrl.FillList() == 0)
   {
      ASSERT(FALSE);
      EndDialog(IDCANCEL);
   }

   return TRUE;  // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}







void CProcessSelectDlg::OnOK() 
{
   m_PID = m_ProcessListCtrl.GetSelectedPID();
   if (m_PID == 0)
   {
      ::MessageBox(m_hWnd, _T("Please select a process"), _T("Process selection"), MB_OK | MB_ICONINFORMATION);
   }
   else
   {
   // default processing
      CDialog::OnOK();
   }
}


void CProcessSelectDlg::OnDblclkListProcess(NMHDR* pNMHDR, LRESULT* pResult) 
{
   OnOK();

   *pResult = 0;
}
