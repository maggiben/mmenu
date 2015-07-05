///////////////////////////////////////////////////////////////////////////////////
// Título:      		manage_menu.c                                             //
// Author:		        Benjamin Maggi                                           //
// Descripcion: 		XML Handling functions                                   //
// Org. Date:           13/03/2008                                               //
// Last Modification:   30/03/2008                                               //
// Ver:                 0.0.1                                                    //
// compiler:            uses ansi-C / C99 tested with LCC & Pellesc              //
// Author mail:         benjaminmaggi@gmail.com                                  //
// License:             GNU                                                      //
//                                                                               //
// License:                                                                      //
// This program is free software; you can redistribute it                        //
// and/or modify it under the terms of the GNU General Public                    //
// License as published by the Free Software Foundation;                         //
// either version 2 of the License, or (at your option) any          	         //
// later version.                                                    	         //
//                                                                    	         //
// This program is distributed in the hope that it will be useful,   	         //
// but WITHOUT ANY WARRANTY; without even the implied warranty of    	         //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the     	         //
// GNU General Public License for more details.                      	         //
//                                                                               //
///////////////////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include "manage_menu.h"
#include "xmleng.h"

//======================Handles================================================//
HINSTANCE hInst; // main function handler
#define WIN32_LEAN_AND_MEAN // this will assume smaller exe
TV_ITEM tvi;
HTREEITEM Selected;
TV_INSERTSTRUCT tvinsert;   // struct to config out tree control
HTREEITEM Parent;           // Tree item handle
HTREEITEM Before;           // .......
HTREEITEM Root;             // .......
HIMAGELIST hImageList;      // Image list array hadle
HBITMAP	hBitMap;            // bitmap handler
BOOL	flagSelected = FALSE;

// for drag and drop
HWND hTree;
TVHITTESTINFO tvht; 
HTREEITEM hitTarget;
POINTS Pos;
BOOL Dragging;

// for lable editing
HWND hEdit;

BUTTONDETAILS gBttnDet[TOTAL_BUTTONS] =
							{NULL, 8003, 1330, 0, 0, 0, 0,
						     NULL, 8003, 1331, 0, 0, 0, 0,
						     NULL, 8003, 1332, 0, 0, 0, 0,};

///////////////////////////////////////////////////////////////////////////////////
//	Registro de la clase para la ventana                                         //
///////////////////////////////////////////////////////////////////////////////////
BOOL manage_menu(HINSTANCE hInstance,HWND parent)
{
	WNDCLASSEX 	wc;
	HWND		hwnd = NULL;
	MSG			msg;
	HANDLE		hAccelTable = NULL;
	memset(&wc,0,sizeof(WNDCLASS));

	wc.style			= CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS|CS_DROPSHADOW;
	wc.cbClsExtra		= 0;
    wc.cbWndExtra		= 0;
	wc.lpfnWndProc		= (WNDPROC)main_menu_wndProc;
	wc.hInstance		= hInstance;
	wc.hbrBackground	= (HBRUSH)(CreateSolidBrush(RGB(195,195,195)));
	wc.lpszClassName	= "manager_menu_windowClass";
	wc.lpszMenuName		= NULL; //MAKEINTRESOURCE (IDMAINMENU);
	wc.hCursor			= LoadCursor(NULL,IDC_ARROW);
    wc.hIcon			= LoadImage(hInstance, MAKEINTRESOURCE(8001), IMAGE_ICON, 32, 32, LR_CREATEDIBSECTION);
	wc.hIconSm			= LoadImage(hInstance, MAKEINTRESOURCE(8001), IMAGE_ICON, 16, 16, LR_CREATEDIBSECTION);
	wc.cbSize			= sizeof(WNDCLASSEX);

	if(!RegisterClassEx(&wc))
	{
		MessageBox(NULL, "Window Registration Failed!", "Error!",MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	hwnd = CreateWindowEx(
			WS_EX_LEFT,
			"manager_menu_windowClass",
			"The title of my window",
			WS_POPUP,
			200,
			200,
			650,
			450,
			NULL,
			NULL,
			hInstance,
			NULL);

	if(hwnd == NULL)
	{
		MessageBox(NULL, "Window Creation Failed!", "Error!",MB_ICONEXCLAMATION | MB_OK);
		return 0;
    }

	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	while (GetMessage(&msg,NULL,0,0))
	{
		if (!TranslateAccelerator(msg.hwnd,hAccelTable,&msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	RegisterClassEx(&wc);
	return msg.wParam;
}

LRESULT CALLBACK treeWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	TV_ITEM tvi;
	HTREEITEM Selected;
	TV_INSERTSTRUCT tvinsert;   // struct to config out tree control
	HTREEITEM Parent;           // Tree item handle
	HTREEITEM Before;           // .......
	HTREEITEM Root;             // .......
	HIMAGELIST hImageList;      // Image list array hadle
	HBITMAP hBitMap;            // bitmap handler
	//BOOL flagSelected = FALSE;

	// for drag and drop
	HWND hTree;
	TVHITTESTINFO tvht; 
	HTREEITEM hitTarget;
	POINTS Pos;
	BOOL Dragging;

	// for lable editing
	HWND hEdit;


	return 0;
}
LRESULT CALLBACK main_menu_wndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	HINSTANCE hInstance = GetModuleHandle(NULL);
	static HBITMAP g_hbmBall = NULL;

	switch(msg)
	{
		case WM_CREATE:
		{	
			LPCREATESTRUCT lpCreate = (LPCREATESTRUCT)lParam;
			initBtnClass(lpCreate->hInstance);
			
			/*
			g_hbmBall = LoadBitmap(GetModuleHandle(NULL),MAKEINTRESOURCE(8003));
			if(g_hbmBall == NULL)
			{
				MessageBox(hwnd, "Could not load IDB_BALL!", "Error", MB_OK | MB_ICONEXCLAMATION);
			}
			*/
			createControls(hInstance,hwnd);


			TV_INSERTSTRUCT tvinsert;   // struct to config out tree control
			tvinsert.hParent				= NULL;			// top most level no need handle
			tvinsert.hInsertAfter			= TVI_ROOT; // work as root level
            tvinsert.item.mask				= TVIF_TEXT;
	        tvinsert.item.pszText			= "Parent";
			tvinsert.item.iImage			= 0;
			tvinsert.item.iSelectedImage	= 1;

			HTREEITEM Parent = (HTREEITEM)SendDlgItemMessage(hwnd,IDC_TREE1,TVM_INSERTITEM,0,(LPARAM)&tvinsert);
			IXMLDOMNodeList *node;
			IXMLDOMElement*	element;
			HRESULT hr;	
			IXMLDOMDocument* document = loadXML("menu2.xml");
			if(document)
			{
				hr = document->lpVtbl->get_documentElement(document,&element);
				if (FAILED(hr) || element == NULL)
				{
					MessageBox(NULL,"Empty document!", "Error Loading XML", MB_ICONWARNING);
				}
				IXMLDOMNodeList* childList;
				hr = element->lpVtbl->get_childNodes(element,&childList);
				buildTree(childList,Parent,hwnd);
				childList->lpVtbl->Release(childList);
				element->lpVtbl->Release(element);
				document->lpVtbl->Release(document);

			}
			break;
		}
		/*
		case WM_LBUTTONDOWN:
		{
			char szFileName[MAX_PATH];
			HINSTANCE hInstance = GetModuleHandle(NULL);

			GetModuleFileName(hInstance, szFileName, MAX_PATH);
			MessageBox(hwnd, szFileName, "This program is:", MB_OK | MB_ICONINFORMATION);
			break;
		}
		*/
		case WM_LBUTTONDOWN:
		{
			///////////////////////////////////////////////////////////////////////////
			//	Move the window around                                               //
			///////////////////////////////////////////////////////////////////////////
			ReleaseCapture();
			SendMessage(hwnd, WM_NCLBUTTONDOWN, HTCAPTION, 0);
			return 0;
		}
		case WM_PAINT:
		{

			///////////////////////////////////////////////////////////////////////////
			// Paint the backgound of our window                                     //
			///////////////////////////////////////////////////////////////////////////
			PAINTSTRUCT	pPaint;
   			COLORREF	crColor;
			HBRUSH		hBrush;
			RECT		rct;
			RECT		rcOrg;

			// Work out where to draw
    		GetClientRect(hwnd, &rct);
			GetClientRect(hwnd, &rcOrg);
			crColor	= RGB(195,195,195);		//window background
			hBrush  = CreateSolidBrush(crColor);
   			HDC hDC = BeginPaint(hwnd, &pPaint);

			wallpaper(hDC,&rct,8004);
			//rcPaint

			//FillRect(hDC, &rcOrg, hBrush);
			DeleteObject(hBrush);
			hBrush  = CreateSolidBrush(RGB(0,0,0));
			FrameRect(hDC, &rcOrg, hBrush);	// it will draw a 1px border only
			DeleteObject(hBrush);

			///////////////////////////////////////////////////////////////////////////
			// Titlebar                                                              //
			///////////////////////////////////////////////////////////////////////////
			hBrush  = CreateSolidBrush(RGB(0,0,0));
			rct.top		= 0;
			rct.bottom	= 30;
			rct.left	= 0;
			rct.right	= rcOrg.right - 12;

			FrameRect(hDC, &rct, hBrush);				// it will draw a 1px border only
			DeleteObject(hBrush);
			hBrush  = CreateSolidBrush(RGB(220,220,220));
			InflateRect(&rct,-1,-1);
			FillRect(hDC, &rct, hBrush);
			DeleteObject(hBrush);

			///////////////////////////////////////////////////////////////////////////
			// Sidebar                                                               //
			///////////////////////////////////////////////////////////////////////////
			hBrush  = CreateSolidBrush(RGB(0,0,0));
			rct.top		= 0;
			rct.bottom	= rcOrg.bottom;
			rct.left	= rcOrg.right - 13;
			rct.right	= rcOrg.right;
			
			FillRect(hDC, &rct, hBrush);
			DeleteObject(hBrush);

			hBrush  = CreateSolidBrush(RGB(150,150,150));
			rct.top		= 1;
			rct.bottom	= rcOrg.bottom - 1;
			rct.left	= rcOrg.right - 12;
			rct.right	= rcOrg.right - 1;
			
			FillRect(hDC, &rct, hBrush);
			DeleteObject(hBrush);

			///////////////////////////////////////////////////////////////////////////
			//	be shure to clean this up                                            //
			///////////////////////////////////////////////////////////////////////////
			ReleaseDC(hwnd, hDC);
    		EndPaint(hwnd, &pPaint);
			break;
		}
		case WM_DRAWITEM:
		{
			///////////////////////////////////////////////////////////////////////////
			// Each ownerdraw control has it's own draw function                     //
			///////////////////////////////////////////////////////////////////////////
			LPDRAWITEMSTRUCT lpDs = (LPDRAWITEMSTRUCT)lParam;
			switch(lpDs->CtlType)
			{
				case ODT_BUTTON:
				{
					DrawButtons(lParam);
					break;
				}
			break; //end switch
			}
		break;
		}
		break;
		case WM_COMMAND:
		{
			switch (LOWORD(wParam))
            {
			
				case 1330:
				//DestroyWindow(hwnd);
				break;
			}
		break;
		}
		case WM_CLOSE:
			DestroyWindow(hwnd);
		break;
		case WM_DESTROY:
			PostQuitMessage(0);
		break;
		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
		break;
	}
	return 0;
}
BOOL createControls(HINSTANCE hInstance, HWND hwndParent)
{
	RECT rcOrg;
	GetClientRect(hwndParent, &rcOrg);
	TCHAR PROP_LPWNDPROC[]	= "btnOldProc";
	HWND closeBtn = CreateWindowEx(WS_EX_WINDOWEDGE|WS_EX_NOPARENTNOTIFY,"ButtonCtrl32", NULL, WS_CHILD | WS_VISIBLE | BS_PUSHLIKE |BS_OWNERDRAW,  //BS_OWNERDRAW
					rcOrg.right - 60,	// x
					rcOrg.top + 1,		// y
				    29,					// width
				    11,					// height
					hwndParent, (HMENU)(1330), hInstance, NULL);
	HWND tree = CreateWindowEx(WS_EX_WINDOWEDGE, "SysTreeView32", NULL, WS_VISIBLE|WS_CHILD|WS_CLIPSIBLINGS|TVS_HASBUTTONS|TVS_HASLINES|TVS_LINESATROOT|TVS_EDITLABELS|TVS_SINGLEEXPAND|TVS_INFOTIP|TVS_FULLROWSELECT|WS_BORDER|WS_TABSTOP, rcOrg.left + 10, rcOrg.top + 40, rcOrg.left + 200, rcOrg.bottom - 50, hwndParent, (HMENU)(IDC_TREE1) ,hInstance, NULL);

	return TRUE;
}
BOOL buildTree(IXMLDOMNodeList *node, HTREEITEM rootMenu, HWND parentHwnd)
{
	HRESULT 			hr 			= S_OK;
	long				listLength	= 0;
	long				depth		= 0;	
	BSTR				nodeName	= NULL;
	IXMLDOMNode*		nodeList	= NULL;
	IXMLDOMNode*		resultNode	= NULL;
	VARIANT_BOOL		hasChild	= VARIANT_FALSE;
	TV_ITEM				tvi;		// Tree Item UNICODE
	TV_INSERTSTRUCT		tvinsert;   // struct to config out tree control UNICODE
	IXMLDOMNodeList*	childList;
	VARIANT				varNodeValue;
	VARTYPE				returnType;

	ZeroMemory(&tvi, sizeof(TV_ITEM));
	ZeroMemory(&tvinsert, sizeof(TV_INSERTSTRUCT));

	hr = node->lpVtbl->get_length(node,&listLength);
	if (FAILED(hr))
	{
		goto cleanup;
	}
	for(long i = 0; i < listLength; i++)
	{
		hr = node->lpVtbl->get_item(node,i, &nodeList);

		hr = nodeList->lpVtbl->selectSingleNode(nodeList,L"CAPTION",&resultNode);
		BSTR caption = NULL;
		resultNode->lpVtbl->get_text(resultNode,&caption);
		resultNode->lpVtbl->Release(resultNode);

		///////////////////////////////////////////////////////////////////////////////////
		// search for nested objects                                                     //
		///////////////////////////////////////////////////////////////////////////////////
		IXMLDOMNodeList*	treeChild;
		hr = nodeList->lpVtbl->selectNodes(nodeList,L"MENUITEM",&treeChild);
		hr = treeChild->lpVtbl->get_length(treeChild,&depth);
		
		if (FAILED(hr))
		{
			MessageBoxW(NULL,L"selectSingleNode()",L"Error",MB_ICONINFORMATION);
			return hr;
		}
		/////////////////////////////////////////////////////////////////////////////
		// For Items with nested nodes build a popup menu                          //
		/////////////////////////////////////////////////////////////////////////////
		if(depth > 0)
		{
			tvinsert.hParent		= rootMenu;		// handle of the above data
			tvinsert.hInsertAfter	= TVI_LAST;	// below parent
			tvinsert.item.mask		= TVIF_TEXT;
			tvinsert.item.pszText	= "caca";//(LPWSTR)malloc(sizeof(WCHAR) * wcslen(caption));
			//CopyMemory(tvinsert.item.pszText,caption,sizeof(WCHAR) * wcslen(caption));
			//wsprintf(tvinsert.item.pszText,"caca");

			HTREEITEM parentTree = (HTREEITEM)SendDlgItemMessage(parentHwnd,IDC_TREE1,TVM_INSERTITEM,0,(LPARAM)&tvinsert);
			if (caption) SysFreeString(caption);
			buildTree(treeChild,parentTree,parentHwnd);
		}
		else
		{
			tvinsert.hParent		= rootMenu;		// handle of the above data
			tvinsert.hInsertAfter	= TVI_LAST;	// below parent
			tvinsert.item.mask		= TVIF_TEXT;
			LPSTR *chr;
			ConvertBStrToAnsiStrx(caption,chr);
			tvinsert.item.pszText	= *chr;//(LPSTR)malloc(sizeof(char) * 1024);
			//wsprintf(tvinsert.item.pszText,"caca");
			SendDlgItemMessage(parentHwnd,IDC_TREE1,TVM_INSERTITEM,0,(LPARAM)&tvinsert);
			if (caption) SysFreeString(caption);
		}

	}
noerror:
	if (nodeList)	nodeList->lpVtbl->Release(nodeList);
	
	return TRUE;
cleanup:
	if (nodeList)	nodeList->lpVtbl->Release(nodeList);
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////////////////
//	Purpose:	This paints our owner draw buttons based on their current state (ie if //
//				the button has the focus, selected etc).							   //
//																					   //
//	Parameter:  lParam - The LPARAM from our WinProc which contains a pointer to a     //
//              (LPDRAWITEMSTRUCT) item.											   //
/////////////////////////////////////////////////////////////////////////////////////////

int DrawButtons(LPARAM lParam)
{
    LPDRAWITEMSTRUCT lpDs = (LPDRAWITEMSTRUCT)lParam;
	static HBITMAP g_hbmBall = NULL;
    HDC 	hDC;
	HBITMAP hBmp;
	HBITMAP temp_bmp[3];
	int 	iBttn, iState;

 	hDC = CreateCompatibleDC(lpDs->hDC);				// Create a memory DC
	if(!hDC)											// If we can't create a suitable
		return(FALSE);									//    DC then we return here.

	///////////////////////////////////////////////////////////////////////////
	// Each ownerdraw control has it's own draw function                     //
	///////////////////////////////////////////////////////////////////////////
	switch(lpDs->CtlType)
	{
		case ODT_BUTTON:
		{
			LPDRAWITEMSTRUCT lpDs = (LPDRAWITEMSTRUCT)lParam;
			HBITMAP temp_bmp[3];
			BITMAP	bmp;

			switch(lpDs->CtlID)
			{
				case 1330: //main_menu
				temp_bmp[0] = g_hbmBall;
				temp_bmp[1] = g_hbmBall;
				temp_bmp[2] = g_hbmBall;
				GetObject(g_hbmBall, sizeof(bmp), &bmp);
				break;
			}
			if(hDC)
			{
				if(lpDs->itemState & ODS_SELECTED) //  if button is down
				{
					SelectObject(hDC, temp_bmp[1]);
				}
				else
				{
					SelectObject(hDC, temp_bmp[0]);
				}
				BitBlt(lpDs->hDC, 0, 0, bmp.bmWidth, bmp.bmHeight, hDC, 0, 0, SRCCOPY);
				DeleteDC(hDC);
			}
			break;
		}
		break;
	}

    DeleteDC(hDC);									// We're finished with the DC
    return(TRUE);                         			// If we were successful
}
///////////////////////////////////////////////////////////////////////////////////////////
//  Main SUPERCLASS Procedure                                                                //
///////////////////////////////////////////////////////////////////////////////////////////
LRESULT WINAPI btnProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HBITMAP bntUp = NULL;	// Normal
	static HBITMAP bntDn = NULL;	// Down
	static HBITMAP bntHt = NULL;	// Hot

	switch(msg)
	{
		case WM_CREATE:
		{
			LPCREATESTRUCT lpCreate = (LPCREATESTRUCT)lParam;
			bntUp = LoadBitmap(lpCreate->hInstance,MAKEINTRESOURCE(8003));
			if(bntUp == NULL)
			{
				MessageBox(hWnd, "Could not load IDB_BALL!", "Error", MB_OK | MB_ICONEXCLAMATION);
			}
			break;
		}
		case WM_DRAWITEM:
		{
			MessageBox(NULL,"WM_DRAWITEM","errrrrr",MB_OK);
			return 0;
		}
		case WM_MEASUREITEM:
		{
			MessageBox(NULL,"WM_MEASUREITEM","errrrrr",MB_OK);
			return 0;
		}
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
	  		COLORREF	crColor;
			HBRUSH		hBrush;
			RECT		rct;
			BeginPaint(hWnd, &ps);

			crColor	= RGB(0,0,0);
			hBrush  = CreateSolidBrush (crColor);
			
			GetWindowRect(hWnd,&rct);
			FillRect(ps.hdc, &ps.rcPaint, hBrush);					// it will draw a 1px border only
			
			EndPaint(hWnd, &ps);
			break;
		}
		case WM_MOUSEMOVE:
		{
			int xPos = GET_X_LPARAM(lParam);
			int yPos = GET_Y_LPARAM(lParam);
			///////////////////////////////////////////////////////////////////////////
			//	Enable Hot Traking for this button                                   //
			///////////////////////////////////////////////////////////////////////////
			TRACKMOUSEEVENT     stTRACKMOUSEEVENT;

			typedef BOOL (CALLBACK * TRACKMOUSE) (LPTRACKMOUSEEVENT );

			stTRACKMOUSEEVENT.cbSize 		= sizeof(stTRACKMOUSEEVENT);
			stTRACKMOUSEEVENT.hwndTrack		= hWnd;
			stTRACKMOUSEEVENT.dwFlags		= TME_LEAVE;
			stTRACKMOUSEEVENT.dwHoverTime 	= HOVER_DEFAULT;



			TRACKMOUSE	TrackMouseEvent;
			HMODULE		LibUser32;
			LibUser32 = LoadLibrary("user32.dll");

			if (LibUser32) {
	    		TrackMouseEvent = (TRACKMOUSE)GetProcAddress(LibUser32,"TrackMouseEvent");
			}
			else
			{
				MessageBox(hWnd,"Error loading user32.dll","Oops Error !", IDOK);
			}

	    	if (TrackMouseEvent == NULL) 
			{
	    		FreeLibrary(LibUser32);
				MessageBox(hWnd,"Error TrackMouseEvent() in user32.dll, wrong windows version?","Oops Error !", IDOK);
				break;
			}
			TrackMouseEvent(&stTRACKMOUSEEVENT);
			break;
		}
		case WM_MOUSELEAVE:
		{
			//MessageBox(hWnd,"WM_MOUSELEAVE","Oops Error !", IDOK);
		}
		default:
			return CallWindowProc(btnOldProc, hWnd, msg, wParam, lParam);
	}
	return CallWindowProc(btnOldProc, hWnd, msg, wParam, lParam);
}
BOOL initBtnClass(HINSTANCE hInst)
{
	WNDCLASSEX wc;
	OSVERSIONINFO verInfo = { 0 };
	wc.cbSize = sizeof(WNDCLASSEX);
	LPCSTR className    = "button";

	if(!GetClassInfoEx(hInst,className,&wc))
		return FALSE;
	
	//If we get NULL as hInst set it to the current thread PID
	if(hInst == NULL){hInst = GetModuleHandle(NULL);}
	
	btnOldProc			= wc.lpfnWndProc;			// Save Old proc
	wc.lpfnWndProc		= (WNDPROC)btnProc;			// Put new Proc
	wc.hInstance		= hInst;					// Instance handler
	wc.lpszClassName	= BUTTON_CLASS;				// Class Name

	if(!RegisterClassEx(&wc))
		return FALSE;
	
	// Determine what system we're on 
	verInfo.dwOSVersionInfoSize = sizeof (verInfo);
	GetVersionEx(&verInfo);
	
	return TRUE;
}
void wallpaper(HDC hdc,RECT *lprc,int idbitmap){
	HDC shdc = NULL;
	HINSTANCE hinst = NULL;
	BITMAP bmp;
	HBITMAP hbmp = NULL;

	long bmx, bmy;

	

	if (idbitmap >0)
	{
		hinst	= GetModuleHandle(NULL);
		hbmp	= LoadBitmap(hinst, MAKEINTRESOURCE(idbitmap));
		if(hbmp == NULL)
		{
			MessageBoxW(NULL,L"LoadBitmap()",L"Error",MB_ICONINFORMATION);
		}
		shdc	= CreateCompatibleDC(hdc);
		SelectObject(shdc, hbmp);
		GetObject(hbmp, sizeof(BITMAP), &bmp);
		bmx = bmp.bmWidth;
		bmy = bmp.bmHeight;

		long r = lprc->right;
		long l = lprc->left;
		long b = lprc->bottom;
		long t = lprc->top;

		long x,y;
		for(x=l; x<r; x += bmx)
		{
			for(y=t; y<b; y += bmy)
			{
				BitBlt(hdc, x,y, r-x,b-y, shdc, 0, 0, SRCCOPY);
			}
		}

		DeleteObject(hbmp);
		DeleteDC(shdc);
	}
}
