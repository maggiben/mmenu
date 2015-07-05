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
LPWSTR cip = L"helllo";
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
		//return 0;
	}

	hwnd = CreateWindowEx(
			WS_EX_TOOLWINDOW,
			"manager_menu_windowClass",
			"The title of my window",
			WS_POPUP|WS_CAPTION,
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
	static HBITMAP cDesk = NULL;
	static HBITMAP g_hbmBall = NULL;

	switch(msg)
	{
		case WM_CREATE:
		{	
			LPCREATESTRUCT lpCreate = (LPCREATESTRUCT)lParam;
			initBtnClass(lpCreate->hInstance);
			
			g_hbmBall = LoadBitmap(GetModuleHandle(NULL),MAKEINTRESOURCE(8003));
			if(g_hbmBall == NULL)
			{
				MessageBox(hwnd, "Could not load IDB_BALL!", "Error", MB_OK | MB_ICONEXCLAMATION);
			}
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
			crColor	= RGB(0,0,0);		//window background
			hBrush  = CreateSolidBrush(crColor);
   			HDC hDC = BeginPaint(hwnd, &pPaint);

			//wallpaper(hDC,&rct,8020);
			//rcPaint

			//blurBitmap(hdc,cDesk,1);
			MessageBeep(MB_OK);
			//if(cDesk == NULL)
			cDesk = CaptureScreen();
			wallpaperFromHbitmap(hDC,&rct,cDesk);

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
			rct.right	= rcOrg.right;

			FrameRect(hDC, &rct, hBrush);				// it will draw a 1px border only
			DeleteObject(hBrush);
			hBrush  = CreateSolidBrush(RGB(25,25,25));
			InflateRect(&rct,-1,-1);
			FillRect(hDC, &rct, hBrush);
			DeleteObject(hBrush);

			///////////////////////////////////////////////////////////////////////////
			//	be shure to clean this up                                            //
			///////////////////////////////////////////////////////////////////////////
			ReleaseDC(hwnd, hDC);
    		EndPaint(hwnd, &pPaint);
			break;
		}
		case WM_WINDOWPOSCHANGED:
		{
			WINDOWPOS* lpwinpos = (LPWINDOWPOS)lParam;
			DefWindowProc(hwnd, msg, wParam, lParam);
			HDC hdc;
			RECT rct;

			hdc = GetWindowDC(hwnd);
			GetWindowRect(hwnd, &rct);

			//blurBitmap(hdc,cDesk,1);
			//cDesk = CaptureScreen();

			BITMAP	bmp;
			GetObject(cDesk, sizeof(bmp), &bmp);

			HDC hDC = CreateCompatibleDC(hdc);				// Create a memory DC
			SelectObject(hDC, cDesk);
			//BitBlt(hdc, 0, 0, bmp.bmWidth, GetSystemMetrics(SM_CXFIXEDFRAME) + 18, hDC, lpwinpos->x, 0, SRCCOPY);
			POINT pp;
			ClientToScreen(hwnd, &pp);
			HBRUSH hBr = CreateSolidBrush(RGB(255, 0, 0));
			rct.left = 0;
			rct.top = 0;
			rct.bottom = GetSystemMetrics(SM_CXFIXEDFRAME) + 18;
			FillRect(hdc,&rct,hBr);
			BitBlt(hdc, 2, 2, 16, 16, hDC, lpwinpos->x, 0, SRCCOPY);
			DeleteObject(cDesk);
			return 0;
		}
		case WM_NCACTIVATE:
		{
			HDC hdc;
			RECT rct;
			MessageBeep(MB_ICONEXCLAMATION);
			hdc = GetWindowDC(hwnd);
			GetWindowRect(hwnd, &rct);

			//HBITMAP cDesk = CaptureScreen();
			if(cDesk == NULL)
			cDesk = CaptureScreen();
			//blurBitmap(hdc,cDesk,2);
			//blurBitmap(hdc,cDesk,2);
			//blurBitmap(hdc,cDesk,2);

			BITMAP	bmp;
			GetObject(cDesk, sizeof(bmp), &bmp);

			HDC hDC = CreateCompatibleDC(hdc);				// Create a memory DC
			SelectObject(hDC, cDesk);
			BitBlt(hdc, 0, 0, bmp.bmWidth, bmp.bmHeight, hDC, rct.left, rct.top, SRCCOPY);
			DeleteObject(cDesk);
			DeleteDC(hDC);
			DeleteDC(hdc);
			break;
		}
		case WM_NCPAINT:
		{
			DefWindowProc(hwnd, msg, wParam, lParam);
			int i ;
			char sCaption[200];

			HDC hdc;
			RECT rc, wrc;
			HFONT origFont;
			HPEN hPen, origPen;

			DWORD dwStyle = GetWindowLong(hwnd, GWL_STYLE);
			DWORD dwExStyle = GetWindowLong(hwnd, GWL_EXSTYLE);

			hdc = GetWindowDC(hwnd);
			GetWindowRect(hwnd, &wrc);
			//HDC memDc = CreateCompatibleDC(hdc);

			// DrawFrame
			HBRUSH blueBrh;
			//if(w.bActive){
				blueBrh = CreateSolidBrush(RGB(0, 0, 0));
			//}else{
			//	blueBrh = CreateSolidBrush(RGB(10, 90, 160));
			//}
		
			//HBITMAP cDesk = CaptureScreen();


			//blurBitmap(hdc,cDesk,2);
			//blurBitmap(hdc,cDesk,2);
			/*
			if(dwStyle & WS_BORDER)
			{
				// draw borders
				SetRect(&rc, 0, 0, wrc.right - wrc.left, GetSystemMetrics(SM_CXFIXEDFRAME) );
				FillRect(hdc, &rc, blueBrh);
				SetRect(&rc, 0, 0, GetSystemMetrics(SM_CXFIXEDFRAME), wrc.bottom - wrc.top);
				FillRect(hdc, &rc, blueBrh);
				SetRect(&rc, wrc.right - wrc.left - GetSystemMetrics(SM_CXFIXEDFRAME), 0, wrc.right - wrc.left, wrc.bottom - wrc.top);
				FillRect(hdc, &rc, blueBrh);
				SetRect(&rc, GetSystemMetrics(SM_CXFIXEDFRAME), wrc.bottom - wrc.top - GetSystemMetrics(SM_CXFIXEDFRAME), wrc.right - wrc.left, wrc.bottom - wrc.top );
				FillRect(hdc, &rc, blueBrh);
			}
			*/
				
			DeleteObject(blueBrh);

			BITMAP	bmp;
			GetObject(cDesk, sizeof(bmp), &bmp);

			HDC hDC = CreateCompatibleDC(hdc);				// Create a memory DC
			SelectObject(hDC, cDesk);
			//BitBlt(hdc, 0, 0, bmp.bmWidth, bmp.bmHeight, hDC, wrc.left, wrc.top, SRCCOPY);
			//BitBlt(hdc, 0, 0, bmp.bmWidth, 30, hDC, wrc.left, wrc.top, SRCCOPY);
			if(dwStyle & WS_BORDER)
			{
				RECT rc;
				HBRUSH hBr = CreateSolidBrush(RGB(255, 0, 0));
				// Bottom
				SetRect(&rc, GetSystemMetrics(SM_CXFIXEDFRAME), wrc.bottom - wrc.top - GetSystemMetrics(SM_CXFIXEDFRAME), wrc.right - wrc.left, wrc.bottom - wrc.top );
				FillRect(hdc, &rc, hBr);
				DeleteObject(hBr);
				hBr = CreateSolidBrush(RGB(0, 255, 0));
				// right
				SetRect(&rc, wrc.right - wrc.left - GetSystemMetrics(SM_CXFIXEDFRAME), 0, wrc.right - wrc.left, wrc.bottom - wrc.top);
				FillRect(hdc, &rc, hBr);
				DeleteObject(hBr);

				hBr = CreateSolidBrush(RGB(0, 0, 255));
				// Left
				SetRect(&rc, 0, 0, GetSystemMetrics(SM_CXFIXEDFRAME), wrc.bottom - wrc.top);
				FillRect(hdc, &rc, hBr);
				DeleteObject(hBr);

				hBr = CreateSolidBrush(RGB(0, 128, 0));
				// Top
				SetRect(&rc, 0, 0, wrc.right - wrc.left, GetSystemMetrics(SM_CXFIXEDFRAME) + 18);
				//FillRect(hdc, &rc, hBr);
				BitBlt(hdc, 0, 0, bmp.bmWidth, GetSystemMetrics(SM_CXFIXEDFRAME) + 18, hDC, wrc.left, wrc.top, SRCCOPY);
				DeleteObject(hBr);
				DeleteObject(cDesk);

			}
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
					DrawButtons(hwnd, msg, wParam, lParam);
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
	HBITMAP hBmp;

	HWND minimBtn = CreateWindowEx(WS_EX_WINDOWEDGE|WS_EX_NOPARENTNOTIFY,"ButtonCtrl32", NULL, WS_CHILD | WS_VISIBLE | BS_PUSHLIKE |BS_OWNERDRAW,  //BS_OWNERDRAW
					rcOrg.right - 100,	// x
					rcOrg.top + 1,		// y
				    29,					// width
				    12,					// height
					hwndParent, (HMENU)(1330), hInstance, NULL);
	hBmp = LoadBitmap(GetModuleHandle(NULL),MAKEINTRESOURCE(8003));;
	SetProp(minimBtn, "PROP_UP", hBmp); 
	hBmp = LoadBitmap(GetModuleHandle(NULL),MAKEINTRESOURCE(8004));;
	SetProp(minimBtn, "PROP_DOWN", hBmp); 
	hBmp = LoadBitmap(GetModuleHandle(NULL),MAKEINTRESOURCE(8005));;
	SetProp(minimBtn, "PROP_HOT", hBmp); 


	HWND maximBtn = CreateWindowEx(WS_EX_WINDOWEDGE|WS_EX_NOPARENTNOTIFY,"ButtonCtrl32", NULL, WS_CHILD | WS_VISIBLE | BS_PUSHLIKE |BS_OWNERDRAW,  //BS_OWNERDRAW
					rcOrg.right - 70,	// x
					rcOrg.top + 1,		// y
				    29,					// width
				    12,					// height
					hwndParent, (HMENU)(1331), hInstance, NULL);
	hBmp = LoadBitmap(GetModuleHandle(NULL),MAKEINTRESOURCE(8006));;
	SetProp(maximBtn, "PROP_UP", hBmp); 
	hBmp = LoadBitmap(GetModuleHandle(NULL),MAKEINTRESOURCE(8004));;
	SetProp(maximBtn, "PROP_DOWN", hBmp); 
	hBmp = LoadBitmap(GetModuleHandle(NULL),MAKEINTRESOURCE(8005));;
	SetProp(maximBtn, "PROP_HOT", hBmp); 


	HWND closeBtn = CreateWindowEx(WS_EX_WINDOWEDGE|WS_EX_NOPARENTNOTIFY,"ButtonCtrl32", NULL, WS_CHILD | WS_VISIBLE | BS_PUSHLIKE |BS_OWNERDRAW,  //BS_OWNERDRAW
					rcOrg.right - 40,	// x
					rcOrg.top + 1,		// y
				    29,					// width
				    12,					// height
					hwndParent, (HMENU)(1332), hInstance, NULL);
	hBmp = LoadBitmap(GetModuleHandle(NULL),MAKEINTRESOURCE(8006));;
	SetProp(closeBtn, "PROP_UP", hBmp); 
	hBmp = LoadBitmap(GetModuleHandle(NULL),MAKEINTRESOURCE(8007));;
	SetProp(closeBtn, "PROP_DOWN", hBmp); 
	hBmp = LoadBitmap(GetModuleHandle(NULL),MAKEINTRESOURCE(8008));;
	SetProp(closeBtn, "PROP_HOT", hBmp); 
	
	
	
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
			LPSTR * cap;

			tvinsert.item.pszText	= "NODE";//(LPWSTR)malloc(sizeof(WCHAR) * wcslen(caption));
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
			tvinsert.item.pszText	= "CHILD";//(LPSTR)malloc(sizeof(char) * 1024);
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

int DrawButtons(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)//(LPARAM lParam)
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
			/*
			switch(lpDs->CtlID)
			{
				case 1330: //main_menu
				temp_bmp[0] = LoadBitmap(GetModuleHandle(NULL),MAKEINTRESOURCE(8003));;
				temp_bmp[1] = LoadBitmap(GetModuleHandle(NULL),MAKEINTRESOURCE(8004));;
				temp_bmp[2] = LoadBitmap(GetModuleHandle(NULL),MAKEINTRESOURCE(8005));;
				GetObject(temp_bmp[0], sizeof(bmp), &bmp);
				break;
				case 1332:

				temp_bmp[0] = GetProp(lpDs->hwndItem,"PROP_UP"); //LoadBitmap(GetModuleHandle(NULL),MAKEINTRESOURCE(8006));
				temp_bmp[1] = GetProp(lpDs->hwndItem,"PROP_DOWN"); //LoadBitmap(GetModuleHandle(NULL),MAKEINTRESOURCE(8007));
				
				//temp_bmp[2] = LoadBitmap(GetModuleHandle(NULL),MAKEINTRESOURCE(8005));;
				//GetObject(temp_bmp[0], sizeof(bmp), &bmp);
				
				GetObject(temp_bmp[0], sizeof(bmp), &bmp);
				break;
			}
			*/
			temp_bmp[0] = GetProp(lpDs->hwndItem,"PROP_UP");
			temp_bmp[1] = GetProp(lpDs->hwndItem,"PROP_DOWN");
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
			HBITMAP		hBmp = (HBITMAP) GetProp(hWnd,"PROP_UP");
			BITMAP		bmp;

			BeginPaint(hWnd, &ps);
			crColor	= RGB(0,0,0);
			hBrush  = CreateSolidBrush (crColor);
			
			GetWindowRect(hWnd,&rct);
			FillRect(ps.hdc, &ps.rcPaint, hBrush);					// it will draw a 1px border only
			
			HDC hDC = CreateCompatibleDC(ps.hdc);
			GetObject(hBmp, sizeof(bmp), &bmp);
			HBITMAP saveObject = SelectObject(hDC,hBmp);
			BitBlt(ps.hdc, 0, 0, bmp.bmWidth, bmp.bmHeight, hDC, 0, 0, SRCCOPY);
			DeleteObject(saveObject);
			DeleteDC(hDC);
			EndPaint(hWnd, &ps);
			break;
		}
		case WM_MOUSEMOVE:
		{
			int xPos = GET_X_LPARAM(lParam);
			int yPos = GET_Y_LPARAM(lParam);
			///////////////////////////////////////////////////////////////////////////
			// Dont draw if we'r holding the left mouse key down                     //
			///////////////////////////////////////////////////////////////////////////
			if(wParam != MK_LBUTTON)
			{	
				BITMAP	bmp;
				HBITMAP temp_bmp;
				temp_bmp = (HBITMAP)GetProp(hWnd,"PROP_HOT");
				//temp_bmp = LoadBitmap(GetModuleHandle(NULL),MAKEINTRESOURCE(8008));
				GetObject(temp_bmp, sizeof(bmp), &bmp);

				HDC hDC = GetDC(hWnd);
				HDC mDC = CreateCompatibleDC(hDC);
				HBITMAP oldHbmp = SelectObject(mDC, temp_bmp);
				//blurBitmap( HDC hSrcDc, HBITMAP hSrcBmp, BYTE bDeltaMax );
				blurBitmap(mDC,temp_bmp,1);
				BitBlt(hDC, 0, 0, bmp.bmWidth, bmp.bmHeight, mDC, 0, 0, SRCCOPY);

				SelectObject(mDC,oldHbmp);
				DeleteDC(mDC);
				ReleaseDC(hWnd, hDC);
			}
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
			BITMAP	bmp;
			HBITMAP temp_bmp;
			//temp_bmp = LoadBitmap(GetModuleHandle(NULL),MAKEINTRESOURCE(8003));
			temp_bmp = (HBITMAP) GetProp(hWnd,"PROP_UP");
			GetObject(temp_bmp, sizeof(bmp), &bmp);

			HDC hDC = GetDC(hWnd);
			HDC mDC = CreateCompatibleDC(hDC);
			HBITMAP oldHbmp = SelectObject(mDC, temp_bmp);
			BitBlt(hDC, 0, 0, bmp.bmWidth, bmp.bmHeight, mDC, 0, 0, SRCCOPY);

			SelectObject(mDC,oldHbmp);
			DeleteDC(mDC);
			ReleaseDC(hWnd, hDC);

			break;
		}
		case WM_DESTROY:
		{	
			EnumPropsEx(hWnd, DelPropProc, 0);
			break;
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
void wallpaperFromHbitmap(HDC hdc,RECT *lprc,HBITMAP hbmp)
{
	HDC shdc = NULL;
	HINSTANCE hinst = NULL;
	BITMAP bmp;

	long bmx, bmy;

	shdc	= CreateCompatibleDC(hdc);
	SelectObject(shdc, hbmp);
	GetObject(hbmp, sizeof(BITMAP), &bmp);
	bmx = bmp.bmWidth;
	bmy = bmp.bmHeight;

	BitBlt(hdc, 0,0, (lprc->right - lprc->left),(lprc->bottom - lprc->top), shdc, 0, 0, SRCCOPY);

	DeleteObject(hbmp);
	DeleteDC(shdc);
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
BOOL CALLBACK DelPropProc(HWND hwndSubclass, LPCSTR lpszString, LPARAM lParam)
{
	HANDLE hData = RemoveProp(hwndSubclass, lpszString);
	return TRUE; 
}
BOOL DrawMenuShadow(HWND hWnd, HDC hDC, long xOrg, long yOrg)
{
	HDC hDcDsk;
    RECT rct;
    long winW, winH;
    long X,Y; //Dim X As Long, Y As Long, c As Long 
	COLORREF color;
  	//Get the size of the menu...
    GetWindowRect(hWnd, &rct);
    winW = rct.right - rct.left;
    winH = rct.bottom - rct.top;

	hDcDsk = GetWindowDC(GetDesktopWindow());

	// Simulate a shadow on right edge...
	for(int x = 1; x <= 4; x++)
	{
        // For Y = 0 To 3
		for(int y = 0; y <= 3; y++)
		{
            color = GetPixel(hDcDsk, xOrg + winW - x, yOrg + y);
            SetPixel(hDC, winW - x, y, color);
		}
        //For Y = 4 To 7
		for(int y = 4; y <= 7; y++)
		{
            color = GetPixel(hDcDsk, xOrg + winW - x, yOrg + y);
            SetPixel(hDC, winW - x, y, pMask(3 * X * (Y - 3), color));
		}
        //For Y = 8 To winH - 5
		for(int y = 4; y <= (winW - 5); y++)
		{
            color = GetPixel(hDcDsk, xOrg + winW - X, yOrg + Y);
            SetPixel(hDC, winW - x, y, pMask(15 * x, color));
		}
        //For Y = winH - 4 To winH - 1
		for(int y = (winH - 4); y <= (winH - 1); y++)
		{
			color = GetPixel(hDcDsk, xOrg + winW - X, yOrg + Y);
            SetPixel(hDC, winW - x, y, pMask(3 * x * -(y - winH), color));
		}
	}
}
COLORREF pMask(long lScale, COLORREF lColor)
{
    BYTE R;
    BYTE G;
    BYTE B;
     
    R = lColor - (GetRValue(lColor) * lScale / 255); //pTransform(lScale, R)
    G = lColor - (GetGValue(lColor) * lScale / 255);
    B = lColor - (GetBValue(lColor) * lScale / 255);
    
    return RGB(R, G, B);
}
HBITMAP CaptureScreen(void)
{
    int nScreenWidth = GetSystemMetrics(SM_CXSCREEN);
    int nScreenHeight = GetSystemMetrics(SM_CYSCREEN);
    HWND hDesktopWnd = GetDesktopWindow();
    HDC hDesktopDC = GetDC(hDesktopWnd);
    HDC hCaptureDC = CreateCompatibleDC(hDesktopDC);
    HBITMAP hCaptureBitmap =CreateCompatibleBitmap(hDesktopDC, 
                            nScreenWidth, nScreenHeight);
    SelectObject(hCaptureDC,hCaptureBitmap); 
    BitBlt(hCaptureDC,0,0,nScreenWidth,nScreenHeight,
           hDesktopDC,0,0,SRCCOPY|CAPTUREBLT); 
    //SaveCapturedBitmap(hCaptureBitmap); //Place holder - Put your code

                                //here to save the captured image to disk
    ReleaseDC(hDesktopWnd,hDesktopDC);
    DeleteDC(hCaptureDC);

	return hCaptureBitmap;

    DeleteObject(hCaptureBitmap);
}
/////////////////////////////////////////////////////////////////////
//
// Input  :  Source DC
//           Source Bitmap
//           Color Delta
//
// Output :  None
// return :  None
//
/////////////////////////////////////////////////////////////////////
//
// History : 05/2002 Jean-Rémi PINNA (jrpinna@noos.fr) Wrote it.
//
/////////////////////////////////////////////////////////////////////


void blurBitmap( HDC hSrcDc, HBITMAP hSrcBmp, BYTE bDeltaMax )
{

	BITMAPINFO  sBmpInfo;
	BYTE *      pBmpBytes;

	memset( &sBmpInfo, 0, sizeof(BITMAPINFO) );

	BITMAP SrcBmp;
	GetObject(hSrcBmp, sizeof(BITMAP), &SrcBmp);

	if( (SrcBmp.bmBitsPixel != 24) && (SrcBmp.bmBitsPixel != 32) )
    return;

  sBmpInfo.bmiHeader.biSize         = sizeof(BITMAPINFOHEADER);
  sBmpInfo.bmiHeader.biWidth        = SrcBmp.bmWidth;
  sBmpInfo.bmiHeader.biHeight       = SrcBmp.bmHeight;
  sBmpInfo.bmiHeader.biBitCount     = SrcBmp.bmBitsPixel;
  sBmpInfo.bmiHeader.biPlanes       = 1;
  sBmpInfo.bmiHeader.biCompression  = BI_RGB;
  sBmpInfo.bmiHeader.biSizeImage    =
        SrcBmp.bmBitsPixel*SrcBmp.bmWidth*SrcBmp.bmHeight/8;

  pBmpBytes = malloc(sBmpInfo.bmiHeader.biSizeImage);//new BYTE[sBmpInfo.bmiHeader.biSizeImage];

  // Fill the buffer with a copy of the bitmap's bits

  GetDIBits(hSrcDc, hSrcBmp, 0, sBmpInfo.bmiHeader.biHeight, pBmpBytes, &sBmpInfo, DIB_RGB_COLORS );


  DWORD dwLineWidth, dwByteCount, dwBytesPixel, dwImageSize;

  dwLineWidth   = SrcBmp.bmBitsPixel*SrcBmp.bmWidth/8;
  dwBytesPixel  = SrcBmp.bmBitsPixel/8;
  dwImageSize   = sBmpInfo.bmiHeader.biSizeImage;
  dwByteCount   = dwBytesPixel;
  // Start to second pixel to avoid bug on first pixel
  // (Previous bottom point coordinate is 1276)

  // Blending Matrix ---- This matrix correspond to the Blur More
  //                         blending method
  //                      The middle pixel value is the average
  //                        of all environnent pixels
  //                      Change value of an element modify his
  //                        weight in the computation of the average
  //
  // BYTE bBlurMoreBlendingMatrix[9] = { 1, 1, 1,
  //          1, 1, 1,
  //          1, 1, 1 };
  //
  // BYTE bHalfBlendingMatrix[9] = { 0, 1, 0,
  //          1, 1, 1,
  //          0, 1, 0 };


  // Blending Matrix ---- This matrix correspond to the Blur
  //                         blending method
  //                      The sum of the elements of the array
  //                         must not exceed 256
  //

  BYTE bBlurBlendingMatrix[9] = { 1, 2, 1,
          2, 4, 2,
          1, 2, 1 };

  // This matrix contains the values needed to point on
  // corresponding bit
  //

  long nPosRange[9] = { - (long)(dwLineWidth-dwBytesPixel) ,
                        - (long)(dwLineWidth) ,
                        - (long)(dwLineWidth+dwBytesPixel),
                        - (long)(dwBytesPixel) ,
                        0 ,
                        +dwBytesPixel,
                        + (dwLineWidth-dwBytesPixel) ,
                        + (dwLineWidth) ,
                        + (dwLineWidth+dwBytesPixel) };


  // Sum Blending Matrix Elements
  //

  BYTE bMatrixSum = 0;

  for( int iSum = 0 ; iSum<9 ; iSum++ )
    bMatrixSum += bBlurBlendingMatrix[iSum];

  while( dwByteCount < dwImageSize )
  {
    DWORD bDelta, dwAverage;

    // Create array of pixel's positions
    //
    long nPosNewRange[9]= { (long)(dwByteCount+nPosRange[0]),
          (long)(dwByteCount+nPosRange[1]),
          (long)(dwByteCount+nPosRange[2]),
          (long)(dwByteCount+nPosRange[3]),
          (long)(dwByteCount+nPosRange[4]),
          (long)(dwByteCount+nPosRange[5]),
          (long)(dwByteCount+nPosRange[6]),
          (long)(dwByteCount+nPosRange[7]),
          (long)(dwByteCount+nPosRange[8]) };

    // Check point validity : ensure points are in the bit array
    //
    for( int iPos = 0 ; iPos < 9 ; iPos++ )
    {
      nPosNewRange[iPos] = nPosNewRange[iPos] < 0 ? 0 :
              nPosNewRange[iPos] >
              (long)dwImageSize ? dwByteCount : nPosNewRange[iPos];
    }

    // Compute method on each RGB color of the pixel
    //
    for( int iColor = 0 ; iColor < 3 ; iColor++ )
    {
      dwAverage = 0;

      // Compute Color average
      //
      BYTE bColorValue[9] = { pBmpBytes[nPosNewRange[0]+iColor],
                              pBmpBytes[nPosNewRange[1]+iColor],
                              pBmpBytes[nPosNewRange[2]+iColor],
                              pBmpBytes[nPosNewRange[3]+iColor],
                              pBmpBytes[nPosNewRange[4]+iColor],
                              pBmpBytes[nPosNewRange[5]+iColor],
                              pBmpBytes[nPosNewRange[6]+iColor],
                              pBmpBytes[nPosNewRange[7]+iColor],
                              pBmpBytes[nPosNewRange[8]+iColor] };

      // Apply Blendind Matrix to compute color average
      //
      for( int iPos = 0 ; iPos < 9 ; iPos++ )
        dwAverage += bColorValue[iPos]*bBlurBlendingMatrix[iPos];

      BYTE bAverage = (BYTE)(dwAverage/bMatrixSum);

      bDelta = abs( pBmpBytes[dwByteCount]-bAverage );
      pBmpBytes[dwByteCount+iColor] =
           bDelta > bDeltaMax ? bAverage :
                                  pBmpBytes[dwByteCount+iColor];
    }

    // Move to next pixel
    //
    dwByteCount+=dwBytesPixel;
  }


  SetDIBits(hSrcDc, hSrcBmp, 0, sBmpInfo.bmiHeader.biHeight, pBmpBytes, &sBmpInfo, DIB_RGB_COLORS );

  free(pBmpBytes);
}
