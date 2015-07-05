//*************************************************************
// DragDropDemo
//
// Drop files from the desktop/explorer will show path/file
// name in the listbox.
//
// Drag the icons from the listview onto/into desktop/explorer.
//
// The files will be created with a small text message as their
// contents.
//
// John
//
//*************************************************************

// Link with urlmon.lib ole2.lib uuid.lib shell32.lib

#include <windows.h>
#include <stdio.h>
#include <ole2.h>

#include "IDragDrop.h"

#include "dragdropdemores.h"

void HandleDragDrop(HWND hwndList); // proto from OleDragDrop.c

HINSTANCE hInst;
HWND hwndMain;

static BOOL CALLBACK DialogFunc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);

int APIENTRY WinMain(HINSTANCE hinst, HINSTANCE hinstPrev, LPSTR lpCmdLine, int nCmdShow)
{
	if(OleInitialize(NULL) != S_OK) // important
		return FALSE;

	hInst = hinst;

	WNDCLASS wc;
	memset(&wc,0,sizeof(wc));
	wc.lpfnWndProc 	 = DefDlgProc;
	wc.style		 = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS ;
	wc.cbWndExtra 	 = DLGWINDOWEXTRA;
	wc.hInstance 	 = hinst;
	wc.hCursor 		 = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon 		 = LoadIcon(hinst, MAKEINTRESOURCE(1));
	wc.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
	wc.lpszMenuName  = MAKEINTRESOURCE(ID_MENU);
	wc.lpszClassName = "DragDropDemo";
	RegisterClass(&wc);

	// Ensure that the common control DLL is loaded.
	INITCOMMONCONTROLSEX icex;
	icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icex.dwICC  = ICC_LISTVIEW_CLASSES;
	InitCommonControlsEx(&icex);

	DialogBox(hinst, MAKEINTRESOURCE(IDD_MAINDIALOG), NULL, (DLGPROC) DialogFunc);
	OleUninitialize();
	return 0;
}

#define FILE_COUNT   10

BOOL InsertListViewItems(HWND hwndListView)
{
	//empty the list
	ListView_DeleteAllItems(hwndListView);

	UINT     i;
	LVITEM   lvItem;
	char szName[MAX_PATH];

	for(i = 0; i < FILE_COUNT; i++){

		sprintf(szName, "File%02d.txt", i);

		memset(&lvItem, 0, sizeof(LVITEM));
		lvItem.iItem 	= i;
		lvItem.mask 	= LVIF_TEXT | LVIF_IMAGE;
		lvItem.pszText  = szName;
		lvItem.iImage 	= 0;
		ListView_InsertItem(hwndListView, &lvItem);
	}

	return TRUE;
}

BOOL InitListView(HWND hwndListView)
{
	LV_COLUMN   lvColumn;
	HIMAGELIST  himlSmall;
	HIMAGELIST  himlLarge;

	//empty the list
	ListView_DeleteAllItems(hwndListView);

	//initialize the columns
	lvColumn.mask 	= LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvColumn.fmt 	= LVCFMT_LEFT;
	lvColumn.cx 	= 150;
	lvColumn.pszText = TEXT("Source File Name");
	ListView_InsertColumn(hwndListView, 0, &lvColumn);

	//set the image lists
	himlSmall = ImageList_Create(16, 16, ILC_COLORDDB | ILC_MASK, 1, 0);

	if(himlSmall){
		HICON hIcon;

		//set up the small image list
		hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(1));
		ImageList_AddIcon(himlSmall, hIcon);

		ListView_SetImageList(hwndListView, himlSmall, LVSIL_SMALL);
	}

	InsertListViewItems(hwndListView);

	return TRUE;
}

static BOOL CALLBACK DialogFunc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	RECT rc;
	LPNM_LISTVIEW lpnmlv;
	int idx;
	long long llsize;

	switch (msg) {
	case WM_INITDIALOG:
		hwndMain = hDlg;

		IDropTarget * pDropTarget = DropTarget_CDropTarget();
		DropTargetEx * pDTEx = (DropTargetEx *)pDropTarget;
		pDTEx->m_hListBox = GetDlgItem(hDlg, ID_LISTBOX);

		if(pDropTarget)
			RegisterDragDrop(hDlg, pDropTarget);

		InitListView(GetDlgItem(hDlg, ID_LISTVIEW));
		return TRUE;

	case WM_NOTIFY:{
		lpnmlv = (LPNM_LISTVIEW) lParam;
		switch(lpnmlv->hdr.code){
		case LVN_BEGINDRAG:
		case LVN_BEGINRDRAG:
			HandleDragDrop(GetDlgItem(hDlg, ID_LISTVIEW));
			return 0;
		}
	}
	break;

	case WM_COMMAND:
		switch (LOWORD(wParam)){
		case IDM_EXIT:
			PostMessage(hDlg, WM_CLOSE, 0, 0);
			return 0;
		}
		break;

	case WM_CLOSE:
		EndDialog(hDlg,0);
		break;

	case WM_DESTROY:
		DragAcceptFiles(hDlg, FALSE);
		return FALSE;

	}
	return FALSE;

}

