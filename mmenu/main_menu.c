///////////////////////////////////////////////////////////////////////////////////
// Título:      		main_menu.c                                              //
// Author:		        Benjamin Maggi                                           //
// Descripcion: 		A customizable Menu                                      //
// Org. Date:           17/01/2005                                               //
// Last Modification:   11/05/2009                                               //
// Ver:                 0.9.5                                                    //
// compiler:            uses ansi-C / C99 tested with LCC & Pellesc              //
// Author mail:         benjaminmaggi@gmail.com                                  //
// License:             GNU                                                      //
//                                                                               //
// ***************************************************************************** //
// * Detais & compilating information                                          * //
// * I've compiled and linked this using lcc but should work in other c/c++    * //
// * compiler.                                                                 * //
// * be carful debugging the hooked function it may cause a GPF                * //
// * you can turn hooks off 									               * //
// ***************************************************************************** //
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
// ***************************************************************************** //
//                                                                               //
// CHANGE LOG:                                                                   //
//                                                                               //
// * Entry for: 4/12/2006                                                        //
//    Located some bugs free(menus) caused errros                                //
//    Working on MFT_SEPARATOR                                                   //
//    Heavy testing under XP moving from w98                                     //
//    Maybe Vista ?                                                              //
//    New features transparency ?                                                //
//    new icons are 24bpp + 256b alpha those need testing                        //
//    A faulty casting caused some bugs (HMENU) casted to -> (UINT) shit !       //
//                                                                               //
// * Entry for 11/DIC/2006                                                       //
//   * I've been thinking this a lot, and this proget needs to support more      //
//   image formats, the best lib i could find if freeImage, wich supports        //
//   more than 80 image formats, the pros are obious the bad thing is that       //
//   now we need to carry this heavy +1mb dll every were, it was not an easy     //
//   desition but finally i thought it was nessesary                             //
//   and in order to mantein compatibility i choose to write a function          //
//   to switch between components                                                //
//                                                                               //
//   * menu_imageOut & menu_imageOvr two new type of images one is for           //
//   menu items: selected and not.                                               //
//   * WM_MENUDRAG and WM_MENUGETOBJECT                                          //
//                                                                               //
// * Entry for 14/DIC/2006                                                       //
//	 * The fixed some GDI leaking problems                                       //
//                                                                               //
// * Entry for 17/DIC/2006                                                       //
//   * Added two new functions:                                                  //
//     insert_menuItem                                                           //
//	   append_menuItem                                                           //
//	   delete_menuItem                                                           //
//     These functions provide a standard framework to the application           //
//                                                                               //
// * Entry for 13/03/2008                                                        //
//   * Research on Drag&Drop                                                     //
//                                                                               //
// * Entry for 14/03/2008                                                        //
//   * Added CS_DROPSHADOW to MainForm                                           //
//                                                                               //
// * Entry for 18/03/2008                                                        //
//   * Working on MF_MENUBARBREAK                                                //
//                                                                               //
// * Entry for 20/03/2008                                                        //
//   * Working on MEASUREITEM                                                    //
//   * Rewrite the Sidebar code & draw functions                                 //
//   * Worked on Parse XML to proper detect item type                            //
//                                                                               //
// * Entry for 21/03/2008                                                        //
//   * Complete rewrite of the MSXML wrapper, drop disphelper                    //
//                                                                               //
// * Entry for 29/03/2008                                                        //
//   * Started creating the Menu Manager                                         //
//                                                                               //
// * Entry for 02/04/2008                                                        //
//   * ROOT Menu requires a separate drawing function (cosmetics are different)  //
//                                                                               //
// * Entry for 11/05/2009                                                        //
//   * Fixed some issues with MF_SIDEBAR & MF_MENUBREAK menu type handling       //
//                                                                               //
// * Entry for 29/05/2009                                                        //
//   * Constant theme data moved into MENU_THEME structure                       //
//   * rewrite XML code                                                          //
///////////////////////////////////////////////////////////////////////////////////

#define VERSION 0.9.5		// 29/05/2009
#define _CRT_SECURE_NO_WARNINGS
#define ISOWNERDRAW FALSE

///////////////////////////////////////////////////////////////////////////////////
//	Includes                                                                     //
///////////////////////////////////////////////////////////////////////////////////

#include <windows.h>		// windows standar
#include <windowsx.h>       // Extra features 
#include <winuser.h>		// Windows User Module definitions
#include <commctrl.h>       // Connmon controls
#include <string.h>         //
#include <stdio.h>			// You know what this is for
#include <shellapi.h>       // For shell interaction
#include <tchar.h>          
#include <wchar.h>			// Unicode support
#include <ole2.h>           // Drag & Drop operations and more
#include <msxml.h>			// MS XML Parser.

#include "IDragDrop.h"

#include "disphelper.h"     // interface with COM MSXML 
#include "main_menures.h"   // Resources
#include "manage_menu.h"	// Menu Manager
#include "graphic.h"		// Graphic Functions draw masks, bitmaps etc..
#include "shellHelper.h"	// Shell & Connmon Dlgs helpers
#include "xmleng.h"			// XML Helper Functions
#include "main_menu.h"      // Proto

#include "graphic.h"

#include "draw.h"

#include "FreeImage.h"      // FreeImage protos & delares

#pragma comment( lib, "Msimg32" )
#pragma comment( lib, "Urlmon" )
#pragma comment( lib, "FreeImage" )
#pragma comment( lib, "shell32" )
#pragma comment( lib, "ole32" )
#pragma comment( lib, "oleaut32" )

//#define MF_OWNERDRAW 0

///////////////////////////////////////////////////////////////////////////////////
// For HEX to DEC func                                                           //
///////////////////////////////////////////////////////////////////////////////////
#define xtod(c) ((c>='0' && c<='9') ? c-'0' : ((c>='A' && c<='F') ? c-'A'+10 : ((c>='a' && c<='f') ? c-'a'+10 : 0)))
int HextoDec(LPWSTR hex);
int xstrtoi(LPWSTR hex);
///////////////////////////////////////////////////////////////////////////////////
// #defines aqui                                                                 //
///////////////////////////////////////////////////////////////////////////////////
#define HR_TRY(func) if (FAILED(func)) { goto cleanup; }

LPMENU_DATA	primary_menu_data;
LPMENU_DATA	first_menu_data;

///////////////////////////////////////////////////////////////////////////////////
//	Global variables                                                             //
///////////////////////////////////////////////////////////////////////////////////

HINSTANCE	hInst;				// Instance handle
HWND		hwndMain;			// Main window handle
HWND		main_window;		// Another Main window Handle
HWND		menu_button;
int			menu_objects;
HWND		opti_button;
HWND		plus_button;
HMENU		main_menu;
WNDPROC		buttons_proc[10];
MENU_THEME	main_menu_theme;	// Menu theme

///////////////////////////////////////////////////////////////////////////////////
//	BMP de los botones                                                           //
//  1° Normal 2° Abajo 3° HotTracking                                            //
///////////////////////////////////////////////////////////////////////////////////
HBITMAP 	g_hBmpUp;
HBITMAP 	g_hBmpDown;
HBITMAP 	g_hBmpHot;

///////////////////////////////////////////////////////////////////////////////////
//	Para plus                                                                    //
///////////////////////////////////////////////////////////////////////////////////
HBITMAP		plus_up;
HBITMAP		plus_dw;
HBITMAP		plus_ht;

///////////////////////////////////////////////////////////////////////////////////
//	Para opciones                                                                //
///////////////////////////////////////////////////////////////////////////////////
HBITMAP		opti_up;
HBITMAP		opti_dw;
HBITMAP		opti_ht;

///////////////////////////////////////////////////////////////////////////////////
//Bitmaps de uso global y sistemas de redibujo                                   //
///////////////////////////////////////////////////////////////////////////////////
LPBITMAPINFO	lpBitmap;
LPTSTR			lpBits;
HBITMAP			MainBitmap;


#define RT_BITMAP		MAKEINTRESOURCE(2)
#define MAIN_BUT		33033;

#define MF_SIDEBAR		0x0FFFF1F1L
#define SIDEBAR_WIDTH	30

#ifndef CS_DROPSHADOW
#define CS_DROPSHADOW	0x20000	// This window style can make us drop a shadow under our frame
#endif
#ifndef MAX_CLASS_NAME
#define MAX_CLASS_NAME	256
#endif

#define OEMRESOURCE				// Used to load windows bitmaps 


int	g_BmpWidth;
int	g_BmpHeight;


///////////////////////////////////////////////////////////////////////////////////
//	Menu Globals & funcs                                                         //
///////////////////////////////////////////////////////////////////////////////////
static 	MENU_DATA 	get_menu_formWID				(HMENU,UINT);
static  BOOL        _GetMenu                        (MENU_DATA *MenuPtr, UINT m_wid);
static 	LPMENU_DATA redim_items						(LPMENU_DATA data_pointer, int reason);
static  LPMENU_DATA delete_item                     (LPMENU_DATA);
HMENU 				LoadXmlMenu					    (HWND);
int 				draw_buttons					(HWND,UINT,WPARAM,LPARAM);
///////////////////////////////////////////////////////////////////////////////////
//	2006 get image types inside HBITMAP                                          //
///////////////////////////////////////////////////////////////////////////////////
HBITMAP				imageOptions					(LPMENU_DATA menu_data_ptr, LPWSTR image_path, COLORREF bgColor);

static HMENU 		FindMenuFromID					(HMENU,UINT);
int 				MenuType						(HMENU hMenu, UINT id);
UINT 				IsRootMenu						(UINT);
HCURSOR				CreateAlphaCursor				(void);
///////////////////////////////////////////////////////////////////////////////////
// Create a bitmap mask by masking the selected color                            //
///////////////////////////////////////////////////////////////////////////////////


void SetWindowTransparentMode(HWND hwnd,BOOL bTransparentMode);
void UpdateAppearance(HWND hWnd, HDC dibitmapDC);
void cacaKK();
///////////////////////////////////////////////////////////////////////////////////
//	XML Loader & Parser Uses COM MSXML                                           //
///////////////////////////////////////////////////////////////////////////////////
static int 			LoadXML							(char *,int);
static int 			parse_XML						(IDispatch*,HMENU);
static HRESULT 		CreateAndAppendNode				(IDispatch * xmlDoc, IDispatch * xmlParent, LPCWSTR szNodeName, LPCWSTR szNodeText);
// New XML without using disphelper
static HMENU		xmlInitnstance					(LPWSTR fname, BOOL bOptimizeMemory);
static int			buildMenu						(IXMLDOMNodeList *node, HMENU rootMenu);
///////////////////////////////////////////////////////////////////////////////////
//	Drag & Drop helpers and wrappers                                             //
///////////////////////////////////////////////////////////////////////////////////
void				startDragDrop					(HWND hwndList);
///////////////////////////////////////////////////////////////////////////////////
// sub & super classing helpers functions
///////////////////////////////////////////////////////////////////////////////////
void 				initHook						(void);
void				unitHook						(void);
LRESULT CALLBACK 	install_Hook					(int code, WPARAM wParam, LPARAM lParam);
//LRESULT CALLBACK 	Sub_Class_MenuProc				(HWND,UINT,WPARAM,LPARAM);
LRESULT CALLBACK 	menudraw_sclass					(HWND,UINT,WPARAM,LPARAM);
LRESULT CALLBACK 	menuproc_sclass					(HWND,UINT,WPARAM,LPARAM);

///////////////////////////////////////////////////////////////////////////////////
//	core window                                                                  //
///////////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK 	MainWndProc						(HWND,UINT,WPARAM,LPARAM);
int					MagnetWindow					(LPARAM *lParam);

///////////////////////////////////////////////////////////////////////////////////
//	Core Hooking functions proto & variables                                     //
///////////////////////////////////////////////////////////////////////////////////
HHOOK	_hHook;
WNDPROC old_menu_proc;
HANDLE  mnu_hwnd;
HWND    damm_handle;
const TCHAR _WndPropName_OldProc[]	= _T("XPWndProp_OldProc");
const TCHAR _WndPropName_MenuXP[]	= _T("XPWndProp_MenuXP");
///////////////////////////////////////////////////////////////////////////////////
// VARIABLES FOR DEBUGGING ONLY !!!                                              //
///////////////////////////////////////////////////////////////////////////////////

void AlphaDraw(HDC hDC, int x, int y, int width, int height, HBITMAP hBmp);

///////////////////////////////////////////////////////////////////////////////////
//	Program code                                                                 //
///////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////
//	Registro de la clase para la ventana                                         //
///////////////////////////////////////////////////////////////////////////////////
static BOOL InitApplication(void)
{
	WNDCLASS wc;

	memset(&wc,0,sizeof(WNDCLASS));

	wc.style			= CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS|CS_DROPSHADOW;
	wc.lpfnWndProc		= (WNDPROC)MainWndProc;
	wc.hInstance		= hInst;
	wc.hbrBackground	= (HBRUSH)(COLOR_WINDOW);
	wc.lpszClassName	= L"main_menu_WndClass";
	wc.lpszMenuName		= NULL; //MAKEINTRESOURCE (IDMAINMENU);
	wc.hCursor			= LoadCursor(NULL,IDC_ARROW);
	wc.hIcon			= LoadIcon(NULL,MAKEINTRESOURCE(8001));
    wc.hIcon			= LoadImage(hInst, MAKEINTRESOURCE(8001), IMAGE_ICON, 32, 32, LR_CREATEDIBSECTION);
    //wc.hIconSm  = LoadImage(hInst, MAKEINTRESOURCE(8001), IMAGE_ICON, 16, 16, LR_CREATEDIBSECTION);

	if (!RegisterClass(&wc))
		return 0;

	///////////////////////////////////////////////////////////////////////////////////
	//	Load resurces                                                                //
	//	Setup Themes                                                                 //
	///////////////////////////////////////////////////////////////////////////////////

	main_menu_theme.menu_theme_bgColor       = RGB(055,055,055);
	main_menu_theme.menu_theme_fgColor       = RGB(000,000,000);
	main_menu_theme.menu_theme_sidebarColor  = RGB(025,025,025);
	main_menu_theme.menu_theme_borderColor   = RGB(000,000,000);
	main_menu_theme.menu_theme_3dLight       = RGB(075,075,057);
	main_menu_theme.menu_theme_3dDark		 = RGB(035,035,035);
	main_menu_theme.menu_theme_alpha		 = 250;
	main_menu_theme.menu_theme_btnUp		 = LoadImage(hInst, MAKEINTRESOURCE(ID_BMPUP), IMAGE_BITMAP,0, 0, LR_LOADMAP3DCOLORS);
	main_menu_theme.menu_theme_btnDw		 = LoadImage(hInst, MAKEINTRESOURCE(ID_BMPDOWN), IMAGE_BITMAP,0, 0, LR_LOADMAP3DCOLORS);
	main_menu_theme.menu_theme_btnHt		 = LoadImage(hInst, MAKEINTRESOURCE(ID_BMPHOT), IMAGE_BITMAP,0, 0, LR_LOADMAP3DCOLORS);
	main_menu_theme.menu_theme_background    = LoadImage(hInst, MAKEINTRESOURCE(12346), IMAGE_BITMAP,0, 0, LR_LOADMAP3DCOLORS);

	main_menu_theme.menu_theme_font_color    = RGB(128,128,128);
	main_menu_theme.menu_theme_font_hilite_color    = RGB(248,128,10);
	main_menu_theme.menu_theme_font_height   = 9;
	main_menu_theme.menu_theme_font			 = _T("PF Tempesta Five");
	main_menu_theme.menu_theme_sidebar_width = 30;

	///////////////////////////////////////////////////////////////////////////////////
	//	Load resurces                                                                //
	//	can be edited externaly using a resource hacker like tool                    //
	///////////////////////////////////////////////////////////////////////////////////

	g_hBmpUp    = LoadImage(hInst, MAKEINTRESOURCE(ID_BMPUP), IMAGE_BITMAP,0, 0, LR_LOADMAP3DCOLORS);
	g_hBmpDown  = LoadImage(hInst, MAKEINTRESOURCE(ID_BMPDOWN), IMAGE_BITMAP,0, 0, LR_LOADMAP3DCOLORS);
	g_hBmpHot   = LoadImage(hInst, MAKEINTRESOURCE(ID_BMPHOT), IMAGE_BITMAP,0, 0, LR_LOADMAP3DCOLORS);
	MainBitmap  = LoadImage(hInst, MAKEINTRESOURCE(12346), IMAGE_BITMAP,0, 0, LR_LOADMAP3DCOLORS);

	///////////////////////////////////////////////////////////////////////////////////
	//	Boton de Plus                                                                //
	///////////////////////////////////////////////////////////////////////////////////
	plus_up 	= LoadImage(hInst, MAKEINTRESOURCE(ID_PLUS1), IMAGE_BITMAP,0, 0, LR_LOADMAP3DCOLORS);
	plus_ht 	= LoadImage(hInst, MAKEINTRESOURCE(ID_PLUS2), IMAGE_BITMAP,0, 0, LR_LOADMAP3DCOLORS);
	plus_dw 	= LoadImage(hInst, MAKEINTRESOURCE(ID_PLUS3), IMAGE_BITMAP,0, 0, LR_LOADMAP3DCOLORS);
	opti_up		= LoadImage(hInst, MAKEINTRESOURCE(ID_OPTI1), IMAGE_BITMAP,0, 0, LR_LOADMAP3DCOLORS);
	opti_ht		= LoadImage(hInst, MAKEINTRESOURCE(ID_OPTI2), IMAGE_BITMAP,0, 0, LR_LOADMAP3DCOLORS);
	opti_dw		= LoadImage(hInst, MAKEINTRESOURCE(ID_OPTI3), IMAGE_BITMAP,0, 0, LR_LOADMAP3DCOLORS);

	BITMAP bm;

	///////////////////////////////////////////////////////////////////////////////////
	// Get width & height                                                            //
	///////////////////////////////////////////////////////////////////////////////////
	GetObject(g_hBmpUp, sizeof(BITMAP), &bm);

	g_BmpWidth  = bm.bmWidth;
	g_BmpHeight = bm.bmHeight;

	FreeImage_Initialise(FALSE);

	return 1;

}

///////////////////////////////////////////////////////////////////////////////////
//	Create the main window                                                       //
///////////////////////////////////////////////////////////////////////////////////
HWND Createmain_menu_WndClassWnd(void)
{

	BITMAP bmp;
	GetObject(main_menu_theme.menu_theme_background, sizeof(BITMAP), (LPSTR)&bmp);
				
	return CreateWindowEx
	(
		WS_EX_LEFT|WS_EX_TOOLWINDOW,	// WS_EX_TOOLWINDOW prevent our icon to be in ALT+TAB
		L"main_menu_WndClass",			// Class
		L"Título",						// Title
		WS_POPUP,						// style
		100,							// pos x of win
		100,							// pos y of win,
		bmp.bmWidth,					// Width
		bmp.bmHeight,  					// Height
		NULL,
		NULL,
		hInst,							// instance
		NULL
	);
}

///////////////////////////////////////////////////////////////////////////////////
//	Handle command subroutine                                                    //
///////////////////////////////////////////////////////////////////////////////////
void MainWndProc_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	HMENU		hMenu_ret;
	POINT		point;
	RECT		rc_1;
	MENU_DATA	menu_data;

	switch(id) 
	{
		case IDM_EXIT:
			PostMessage(hwnd, WM_CLOSE, 0, 0);
		break;

		case 330:
		{
			//hMenu_ret = LoadXmlMenu(hwnd);
			hMenu_ret = xmlInitnstance(L"menu.xml", TRUE);
			RECT rcDesktop;
			if(FALSE == SystemParametersInfo(SPI_GETWORKAREA, 0, &rcDesktop, 0))
			break;
			ClientToScreen(menu_button, &point);
			GetWindowRect(hwnd,&rc_1);
			
			int halfScreenHeight = (rcDesktop.bottom - rcDesktop.top) / 2;
			int halfScreenWidth = (rcDesktop.left - rcDesktop.right) / 2;

			POINT pt;
			UINT uFlags;
			if(rc_1.top <= halfScreenHeight)
			{
				pt.y = rc_1.top + (rc_1.bottom - rc_1.top) + 3;
				uFlags = TPM_BOTTOMALIGN;
			}
			else
			{
				pt.y = rc_1.top - 4;
				uFlags = TPM_BOTTOMALIGN;
			}
			if(rc_1.left <= halfScreenWidth)
			{
				pt.x = rc_1.left;
				uFlags = TPM_LEFTALIGN;
			}
			else
			{
				pt.x = rc_1.left;
				uFlags = TPM_LEFTALIGN;
			}

			BOOL result = TrackPopupMenu(hMenu_ret, uFlags, pt.x, pt.y, 0, main_window, 0);
			// This enable the menu to pass its messages to the mainHwnd proc if  | TPM_RETURNCMD | TPM_NONOTIFY
			// SendMessage(main_window, WM_COMMAND, result, (LPARAM)0);

			break;
		}
		case 331:
			PostMessage(hwnd,WM_CLOSE,0,0);;
		break;
		case 332:
		{
			// Creates the menu
			/*
			MENU_DATA newMenu;
			
			newMenu.menu_image		= NULL;
			newMenu.menu_handle		= main_menu;
			newMenu.menu_popup		= 0;
			newMenu.menu_capti		= "myDrive";
			newMenu.menu_exec		= "C:";
			newMenu.menu_color		= RGB(128,255,255);
			newMenu.menu_font_name	= "verdana";
			newMenu.wID				= (HMENU)20010;

			append_menuItem(&newMenu,main_menu,MF_STRING,20010,newMenu.menu_capti);
			*/
			TCHAR m_pszCurrentFile[MAX_PATH];
			DWORD m_dwCurrentIndex = 0;
			GetModuleFileName(GetModuleHandle(NULL), m_pszCurrentFile, MAX_PATH);
			//SelectIcon(hwnd, m_pszCurrentFile, MAX_PATH, &m_dwCurrentIndex);

			unitHook();
			ClientToScreen(menu_button, &point);
			GetWindowRect(hwnd,&rc_1);
			//BOOL result = TrackPopupMenu(test_menu,TPM_LEFTALIGN, rc_1.left, (rc_1.top + 42 ), 0, main_window, 0);
			break;
		}
		default:
		{
			break;
		}
	}
	///////////////////////////////////////////////////////////////////////////
	// Handle menu commands, menu id's start at 1900                         //
	// Notes: 2008 ver > 0.8                                                 //
	// Need work to handle more complex tasks like loading dlls              //
	///////////////////////////////////////////////////////////////////////////
	// HARDCODED ID VALUES WRONG WRONG fixme                                 //
	///////////////////////////////////////////////////////////////////////////
	if((id >= 1900) && (id <= 2000))
	{
		menu_data = get_menu_formWID(main_menu,id);
		ShellExecute(0, L"open", menu_data.menu_exec, NULL, NULL, SW_SHOW);
	}
}

///////////////////////////////////////////////////////////////////////////////////
// Main window procedure                                                         //
///////////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HANDLE 	hRes;
	HDC 			hdc;
	HDC     		mDC;
	RECT 			rd;
	BOOL   def_result = 0;

	switch (msg) 
	{
		//HANDLE_MSG(hwnd, WM_MEASUREITEM, OnMeasureItem);
	case WM_MENUSELECT:
	{
		///////////////////////////////////////////////////////////////////////////
		// Special things to do when we select a menu item                       //
		///////////////////////////////////////////////////////////////////////////
		break;
	}
	case WM_TIMER:
	{
		break;
	}
	case WM_CONTEXTMENU :
	{
		//TrackPopupMenuEx(main_menu, TPM_RIGHTBUTTON, LOWORD(lParam), HIWORD(lParam), hwnd, NULL);
		break;
	}
	case WM_COMMAND:

		HANDLE_WM_COMMAND(hwnd, wParam, lParam, MainWndProc_OnCommand);
		break;

	case WM_CREATE:
	{	
		LPCREATESTRUCT lpCreate = (LPCREATESTRUCT)lParam;
		///////////////////////////////////////////////////////////////////////////////////
		// Load resources                                                                //
		///////////////////////////////////////////////////////////////////////////////////
		hRes = LoadResource(hInst, FindResource(hInst, MAKEINTRESOURCE(ID_BMPFONDO), RT_BITMAP));
		lpBitmap	= (LPBITMAPINFO) LockResource(hRes);
 		lpBits		= (LPTSTR) lpBitmap;
	   	lpBits		+= lpBitmap->bmiHeader.biSize + (256 * sizeof(RGBQUAD));

		///////////////////////////////////////////////////////////////////////////////////
		// Register drag and drop operations                                             //
		///////////////////////////////////////////////////////////////////////////////////
		IDropTarget * pDropTarget = DropTarget_CDropTarget();
		DropTargetEx * pDTEx = (DropTargetEx *)pDropTarget;
		pDTEx->m_hwnd = hwnd;

		if(pDropTarget)
			//RegisterDragDrop(hwnd, pDropTarget);
		///////////////////////////////////////////////////////////////////////////
		//	Creo los botones de control                                          //
		///////////////////////////////////////////////////////////////////////////
		def_result = create_Controls(hInst, hwnd, 1);

		///////////////////////////////////////////////////////////////////////////
		// Screen size                                                           //
		///////////////////////////////////////////////////////////////////////////
		int cx = GetSystemMetrics( SM_CXSCREEN );
		int cy = GetSystemMetrics( SM_CYSCREEN );

		// Set top-most window
		SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
		//SetWindowTransparentMode(hwnd,TRUE);
		//UpdateAppearance(hwnd, GetDC(hwnd));
		break;
	}
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
		HBRUSH		hbrush;

		crColor	= RGB(0,0,0);
		hbrush  = CreateSolidBrush(crColor);
   		hdc 	= BeginPaint(hwnd, &pPaint);

		rd	= MapDialogControlRect(hwnd, 0, FALSE);
		mDC	=  CreateCompatibleDC(hdc);
		SelectObject(mDC, main_menu_theme.menu_theme_background);
		BitBlt(hdc, 0, 0, lpBitmap->bmiHeader.biWidth, lpBitmap->bmiHeader.biHeight, mDC, 0, 0, SRCCOPY);

		DeleteDC(mDC);
		SetBkMode(hdc, TRANSPARENT);
		ReleaseDC(hwnd, hdc);
		DeleteObject(hbrush);

		///////////////////////////////////////////////////////////////////////////
		//	be shure to clean this up                                            //
		///////////////////////////////////////////////////////////////////////////
    	EndPaint(hwnd, &pPaint);
		break;
	}
	case WM_INITMENU:
	{
		HMENU hmenuInit = (HMENU) wParam;			// manipulador de menú a inicializar
		break;		
	}
	case WM_INITMENUPOPUP:
	{
		HMENU hmenuPopup = (HMENU)wParam;			// manipulador de submenú
		UINT uPos = (UINT)LOWORD(lParam);			// posición del ítem del submenú
		BOOL fSystemMenu = (BOOL)HIWORD(lParam);	// bandera de menú de ventana

		MENUINFO mnfo;
		mnfo.cbSize = sizeof(mnfo);
		mnfo.fMask = MIM_STYLE;
		mnfo.dwStyle = MNS_DRAGDROP | MNS_AUTODISMISS;

		SetMenuInfo(hmenuPopup, &mnfo);

		break;
	}
	case WM_MENUGETOBJECT:
	{
		//new Feb 2008
		MENUGETOBJECTINFO *menuObject = (MENUGETOBJECTINFO*)lParam;
		HRESULT hr = MNGO_NOINTERFACE;

		//MessageBeep(0);

		break;
	}
	case WM_MENUDRAG:
	{
		UINT itemPos = (UINT)wParam;
		HWND menuH	 = (HWND)lParam;
		
		///////////////////////////////////////////////////////////////////////////
		// Register drag and drop operations                                     //
		///////////////////////////////////////////////////////////////////////////
		IDropTarget * pDropTarget = DropTarget_CDropTarget();
		DropTargetEx * pDTEx = (DropTargetEx *)pDropTarget;
		pDTEx->m_hwnd = menuH;

		if(pDropTarget)
			//RegisterDragDrop(menuH, pDropTarget);

		startDragDrop(menuH);

		// Menu should remain active. If the mouse is released, it should be ignored.
		return MND_CONTINUE;
	}
	case WM_MOVING:
	{
		///////////////////////////////////////////////////////////////////////////
		// make this window magnetic                                             //
		///////////////////////////////////////////////////////////////////////////
		return MagnetWindow(&lParam);
		break;
	}
	/*case WM_MEASUREITEM:
	{
		///////////////////////////////////////////////////////////////////////////
		//                                                                       //
		// We need to calculate OWNERDRAW items size                             //
		//                                                                       //
		///////////////////////////////////////////////////////////////////////////
		TEXTMETRIC          tm;
		HDC                 hdc = NULL;
		SIZE				sz;
		BOOL				hResult;
		LPMEASUREITEMSTRUCT pmis = (LPMEASUREITEMSTRUCT)lParam;
		
		if (pmis->CtlType != ODT_MENU)
		break;

		///////////////////////////////////////////////////////////////////////////
		//  Seteo medidas w y h del elemento del menu (puede ser independietnte) //
		//  chequear tamano en pixels de la fuente                               //
		///////////////////////////////////////////////////////////////////////////
		if (pmis->CtlType == ODT_MENU)
		{
			MENU_DATA menu_data = get_menu_formWID(main_menu, pmis->itemID);
			hdc = GetDC(hwnd);
			GetTextMetrics(hdc, &tm);
			
			///////////////////////////////////////////////////////////////////////////
			// nota: usar select object para seleccionar el tipo de fuente en el DC  //
			// 2009 Correct fix for calculating the correct size:                    //
			// First select font into DC and then call GetTextExtentPoint32 :)       //
			///////////////////////////////////////////////////////////////////////////
			int szText;
			int PointSize = menu_data.menu_font_size;
			int DPI = 72;
			int lfHeight = -MulDiv(PointSize, GetDeviceCaps(hdc, LOGPIXELSY), DPI);

			HFONT hfnt = CreateAngledFont(lfHeight, 0, menu_data.menu_font_name, NULL);
		
			HFONT hfntPrev = SelectObject(hdc, hfnt);
			szText = lstrlen(menu_data.menu_capti);
			hResult = GetTextExtentPoint32(hdc, menu_data.menu_capti, szText, &sz);
			SelectObject(hdc, hfntPrev);
			DeleteObject(hfnt);
			if(!hResult)
			{	
				MessageBox(hwnd, L"Error GetTextExtentPoint32() in gdi32.dll", L"Error!", IDOK);
				break;
			}			
			///////////////////////////////////////////////////////////////////////////
			// Look for root menu because we want to drawit differently from child   //
			// Windows will accomodate the menu width according to the biggest       //
			// menu item                                                             //
			///////////////////////////////////////////////////////////////////////////
			BITMAP bmp;
			ZeroMemory(&bmp, sizeof(BITMAP));
			if(menu_data.menu_imageOut)
			{
				GetObject(menu_data.menu_imageOut, sizeof(BITMAP), (LPSTR) &bmp);
			}
			UINT is_root = IsRootMenu(pmis->itemID);
			if(is_root == 1)
			{
				pmis->itemHeight  = (sz.cy > bmp.bmHeight) ? (sz.cy + 8):bmp.bmHeight + 8;// 8 = icon padding
				pmis->itemWidth   = sz.cx + bmp.bmWidth + 32; // Add Icon Size to menu width
			}
			else
			{
				pmis->itemHeight  = (sz.cy > bmp.bmHeight) ? (sz.cy + 8):bmp.bmHeight + 8;// 8 = icon padding
				pmis->itemWidth   = sz.cx + bmp.bmWidth + 32; // sidebar + icon + arrow = padding			
			}

			switch(menu_data.menu_type)
			{
				///////////////////////////////////////////////////////////////////////////
				// Set MF_SEPARATOR Measures                                             //
				///////////////////////////////////////////////////////////////////////////
				case MF_SEPARATOR:
				{
					pmis->itemHeight = 8; //GetSystemMetrics(SM_CYMENUCHECK)>>1;
					pmis->itemWidth  = 0;
					break;
				}
				///////////////////////////////////////////////////////////////////////////
				// Set MF_MENUBARBREAK Measures                                          //
				///////////////////////////////////////////////////////////////////////////
				case MF_MENUBARBREAK:
				{

					break;
				}
				///////////////////////////////////////////////////////////////////////////
				// Set MF_MENUBARBREAK Measures                                          //
				///////////////////////////////////////////////////////////////////////////
				case MF_MENUBREAK:
				{
					pmis->itemHeight  = 0;
					break;
				}
				///////////////////////////////////////////////////////////////////////////
				// Set MF_SIDEBAR Measures                                               //
				///////////////////////////////////////////////////////////////////////////
				case MF_SIDEBAR:
				{
					pmis->itemHeight  = 0;
					pmis->itemWidth   = SIDEBAR_WIDTH - 15;
					break;
				}
				default:
					break;
			}
		}
		ReleaseDC(hwnd, hdc);
		break;

		// whatever value I return in lpms->itemWidth, Windows will add the
		// width of a menu checkmark, so I must subtract to defeat Windows. Argh.
		//
		// pmis->itemWidth -= GetSystemMetrics(SM_CXMENUCHECK)-1;
	}*/
	case WM_DRAWITEM:
	{
		///////////////////////////////////////////////////////////////////////////
		// Each ownerdraw control has it's own draw function                     //
		///////////////////////////////////////////////////////////////////////////
		LPDRAWITEMSTRUCT lpDs = (LPDRAWITEMSTRUCT)lParam;
		MENU_DATA menuData;
		switch(lpDs->CtlType)
		{
			case ODT_BUTTON:
				draw_buttons(hwnd, msg, wParam, lParam);
			break;
			case ODT_MENU:
				menuData = get_menu_formWID((HMENU)lpDs->hwndItem, lpDs->itemID);
				DrawMenu(lpDs, &menuData);
			break;
		}
		//end switch
		break;
	}
	case WM_DESTROY:
	{
		unitHook();
		PostQuitMessage(0);
		break;
	}
	default:
		return DefWindowProc(hwnd,msg,wParam,lParam);  //let windows process other messages
	}
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////
//	Process windows message queque                                               //
///////////////////////////////////////////////////////////////////////////////////
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, INT nCmdShow)
{
	MSG msg;
	HANDLE hAccelTable;

	hInst = hInstance;
	menu_objects = 0;
	
	if(OleInitialize(NULL) != S_OK) // important
	return FALSE;

	///////////////////////////////////////////////////////////////////////////////////
	// Turn it off in debugging instances                                            //
	// other wise you will get GPF's a lot of times                                  //
	// specialy LCC whos debbugger is a bit messy                                    //
	///////////////////////////////////////////////////////////////////////////////////
	//initHook();

	if (!InitApplication())
	{
		return 0;
	}
	hAccelTable = LoadAccelerators(hInst,MAKEINTRESOURCE(IDACCEL));

	if ((hwndMain = Createmain_menu_WndClassWnd()) == (HWND)0)
	{
		return 0;
	}

	ShowWindow(hwndMain,SW_SHOW);

	///////////////////////////////////////////////////////////////////////////
	//	copia doble del puntero                                              //
	///////////////////////////////////////////////////////////////////////////

	main_window = hwndMain;
	///////////////////////////////////////////////////////////////////////////
	//dont fuck this pool windows doesnt like it                             //
	//don put shit inside this look                                          //
	///////////////////////////////////////////////////////////////////////////
	while (GetMessage(&msg,NULL,0,0)) 
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	OleUninitialize();
	return msg.wParam;
}

///////////////////////////////////////////////////////////////////////////
//	Create the buttons :)                                                //
///////////////////////////////////////////////////////////////////////////
BOOL create_Controls(HINSTANCE main_inst, HWND hwnd_parent, int skin_tpy)
{
	 int 		i = 0;

	///////////////////////////////////////////////////////////////////////////////////
	// 330 simbolo inicial comando boton                                             //
	///////////////////////////////////////////////////////////////////////////////////
	BITMAP bmp;
	POINT pt;
	GetObject(main_menu_theme.menu_theme_background, sizeof(BITMAP), (LPSTR)&bmp);
	pt.x = bmp.bmWidth;
	pt.y = bmp.bmHeight;
	GetObject(g_hBmpUp, sizeof(BITMAP), (LPSTR)&bmp);
	
	pt.x = 5;//(pt.x / 2 ) - (bmp.bmWidth / 2);
	pt.y = (pt.y / 2 ) - (bmp.bmHeight / 2);
	menu_button = CreateWindow(L"button", NULL, WS_CHILD | WS_VISIBLE | BS_PUSHLIKE | BS_OWNERDRAW ,
					pt.x,
					pt.y,
				    bmp.bmWidth,
				    bmp.bmHeight,
					hwnd_parent, (HMENU)(330 + i), main_inst, NULL);

	i++;
	opti_button = CreateWindow(L"button", NULL, WS_CHILD | WS_VISIBLE | BS_PUSHLIKE | BS_OWNERDRAW,
				  104,
					2,
					9,
					9,
					hwnd_parent, (HMENU)(330 + i), main_inst, NULL);
	i++;
    plus_button = CreateWindow(L"button", NULL, WS_CHILD | WS_VISIBLE | BS_PUSHLIKE | BS_OWNERDRAW,
				  104,
				   12,
					9,
					9,
					hwnd_parent, (HMENU)(330 + i), main_inst, NULL);

	///////////////////////////////////////////////////////////////////////////////////
	// Subclasing elements (keep orden)                                              //
	///////////////////////////////////////////////////////////////////////////////////
	buttons_proc[1] = (WNDPROC)SetWindowLong(menu_button,GWL_WNDPROC, (DWORD)ButtonProc) ;
	
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////
// Buttons subclased goes here                                                   //
///////////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK ButtonProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	///////////////////////////////////////////////////////////////////////////////////
	// WM_MLEAVE message dispacher api need this struct to know what                 //
	// window to track !!!                                                           //
	// bmaggi 2003 for lcc-win32                                                     //
	///////////////////////////////////////////////////////////////////////////////////
	switch (message)
	{
		case WM_CREATE:
			break;
		case WM_SETCURSOR:
			break;
		case WM_MEASUREITEM:
			break;
		case WM_MOUSEMOVE:
		{
			int xPos = GET_X_LPARAM(lParam); 
			int yPos = GET_Y_LPARAM(lParam); 
			
			///////////////////////////////////////////////////////////////////////////
			// Dont draw if we'r holding the left mouse key down                     //
			///////////////////////////////////////////////////////////////////////////
			if(wParam != MK_LBUTTON)
			{	
				HDC hDC = GetDC(menu_button);					// create a memory DC
				HDC	mDC = CreateCompatibleDC(hDC);				// Temporary DC
				SelectObject(mDC, g_hBmpHot);
			
				BitBlt(hDC, 0, 0, g_BmpWidth, g_BmpHeight, mDC, 0, 0, SRCCOPY);

				DeleteDC(mDC);
				ReleaseDC(menu_button, hDC);
			}
			///////////////////////////////////////////////////////////////////////////
			//	Enable Hot Traking for this button                                   //
			///////////////////////////////////////////////////////////////////////////
			TRACKMOUSEEVENT     stTRACKMOUSEEVENT;

			typedef BOOL (CALLBACK * TRACKMOUSE) (LPTRACKMOUSEEVENT );

			stTRACKMOUSEEVENT.cbSize 		=   sizeof(stTRACKMOUSEEVENT);
			stTRACKMOUSEEVENT.hwndTrack		=	hWnd;
			stTRACKMOUSEEVENT.dwFlags		=	TME_LEAVE;
			stTRACKMOUSEEVENT.dwHoverTime 	=   HOVER_DEFAULT;


			TRACKMOUSE	TrackMouseEvent = NULL;
			HMODULE		LibUser32;
			LibUser32 = LoadLibrary(L"user32.dll");

			if (LibUser32) {
	    		TrackMouseEvent = (TRACKMOUSE)GetProcAddress(LibUser32, "TrackMouseEvent");
			}
			else
			{
				MessageBox(hWnd, L"Error loading user32.dll", L"Oops Error !", IDOK);
			}

	    	if (TrackMouseEvent == NULL) 
			{
	    		FreeLibrary(LibUser32);
				MessageBox(hWnd, L"Error TrackMouseEvent() in user32.dll, wrong windows version?", L"Oops Error !", IDOK);
				break;
			}
			
			TrackMouseEvent(&stTRACKMOUSEEVENT);
			break;
		}
		case WM_MOUSELEAVE:
		{
			HDC hDC = GetDC(menu_button);
			HDC mDC = CreateCompatibleDC(hDC);
			SelectObject(mDC, g_hBmpUp);
			
			BitBlt(hDC, 0, 0, g_BmpWidth, g_BmpHeight, mDC, 0, 0, SRCCOPY);
			
			DeleteDC(mDC);
			ReleaseDC(menu_button, hDC);
			break;
		}
		case WM_PAINT:
		{
			break;
		}
		///////////////////////////////////////////////////////////////////////////////////
		//	Cant cast drawitem here ???                                                  //
		//	el subclass parece no funcionar y el mensaje lo recibe la veentana (parent)  //
		///////////////////////////////////////////////////////////////////////////////////
		case WM_DRAWITEM:
		{
			//  Handles the ownerdraw button
			LPDRAWITEMSTRUCT lpDs = (LPDRAWITEMSTRUCT)lParam;
			HDC hDC = CreateCompatibleDC(lpDs->hDC);				// create a memory DC

			if(hDC)
			{
				if(lpDs->itemState & ODS_SELECTED)					//  if button is down
				{
					SelectObject(hDC, g_hBmpUp);
				}
				else
				{
					SelectObject(hDC, g_hBmpDown);
				}

				BitBlt(lpDs->hDC, 0, 0, g_BmpWidth, g_BmpHeight, hDC, 0, 0, SRCCOPY);
				DeleteDC(hDC);
			}
		    break;
		}

		default:
			return CallWindowProc(buttons_proc[1], hWnd, message, wParam, lParam) ;
	}

	return CallWindowProc(buttons_proc[1], hWnd, message, wParam, lParam) ;
}

///////////////////////////////////////////////////////////////////////////////////
//	Helper func provided (?)                                                     //
///////////////////////////////////////////////////////////////////////////////////
RECT MapDialogControlRect(HWND hWnd, int id, BOOL flag)
{
	int w, h;
	RECT rw, rc, rect;

	if (GetWindowLong(hWnd, GWL_STYLE) & WS_SYSMENU)
	{
		w = GetSystemMetrics(SM_CXDLGFRAME);
		h = GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYDLGFRAME);
	}
	else if (GetWindowLong(hWnd, GWL_STYLE) & WS_DLGFRAME)
	{
		w = GetSystemMetrics(SM_CXDLGFRAME);
		h = GetSystemMetrics(SM_CYDLGFRAME);
	}
	else  if (GetWindowLong(hWnd, GWL_STYLE) & WS_BORDER)
	{
		w = GetSystemMetrics(SM_CXBORDER);
		h = GetSystemMetrics(SM_CYBORDER);
	}

	GetWindowRect(hWnd, &rw);
	GetWindowRect(GetDlgItem(hWnd, id), &rc);

	if (id == 0)
	{
		GetClientRect(hWnd, &rect);
		rect.left = 0;
	    rect.top  = 0;
	}
	else
	{
		GetClientRect(GetDlgItem(hWnd, id), &rect);
		rect.left = rc.left - rw.left - w;
		rect.top  = rc.top - rw.top - h;
		if (flag)
		{
			rect.right  = rect.left + rect.right;
			rect.bottom = rect.top + rect.bottom;
		}
	}

	return (rect);
}

///////////////////////////////////////////////////////////////////////////////////
// Initzialice the xml parser and create the main menu                           //
///////////////////////////////////////////////////////////////////////////////////
HMENU LoadXmlMenu(HWND parent)
{
	int			h_ref;
	static BOOL menu_is_done = FALSE;
	
	if(!menu_is_done)
	{
		main_menu = CreatePopupMenu();
		dhInitialize(TRUE);
		dhToggleExceptions(TRUE);

		h_ref = LoadXML("menu.xml", 256);

		dhUninitialize(TRUE);
		menu_is_done = TRUE;
	}
	return main_menu;
}

///////////////////////////////////////////////////////////////////////////////////
// CreateAndAppendNode:                                                          //
// Helper function that wraps up the functions needed to create an element,      //
// set its text, and add it to the DOM as a child of xmlParent.                  //
///////////////////////////////////////////////////////////////////////////////////
static HRESULT CreateAndAppendNode(IDispatch * xmlDoc, IDispatch * xmlParent,LPCWSTR szNodeName, LPCWSTR szNodeText)
{
	DISPATCH_OBJ(xmlNewNode);
	HRESULT hr;

	if (SUCCEEDED(hr = dhGetValue(L"%o", &xmlNewNode, xmlDoc, L".createElement(%S)", szNodeName)) &&
	    SUCCEEDED(hr = dhPutValue(xmlNewNode, L".text = %S", szNodeText)))
	{
		hr = dhCallMethod(xmlParent, L".appendChild(%o)", xmlNewNode);
	}

	SAFE_RELEASE(xmlNewNode);

	return hr;
}

///////////////////////////////////////////////////////////////////////////////////
// Load the XML file                                                             //
// Puts the XML data into the container                                          //
///////////////////////////////////////////////////////////////////////////////////
static int LoadXML(char * fname, int flags)
{
	DISPATCH_OBJ(xmlDoc);
	DISPATCH_OBJ(NodeList);

	HR_TRY( dhCreateObject(L"MSXML.DOMDocument", NULL, &xmlDoc) );
	HR_TRY( dhPutValue(xmlDoc, L".Async = %b", FALSE) );
	HR_TRY( dhCallMethod(xmlDoc, L".Load(%s)", fname) );

	dhGetValue(L"%o", &NodeList, xmlDoc, L".documentElement.childNodes");
	parse_XML(NodeList, main_menu);
	return 1;

cleanup:
	SAFE_RELEASE(xmlDoc);
	SAFE_RELEASE(NodeList);
	return -1;
}

///////////////////////////////////////////////////////////////////////////////////
//                                                                               //
// Notes: 2008 for ver > 0.8                                                     //
// Needs rework because menu container now properly sets all menu types          //
//                                                                               //
///////////////////////////////////////////////////////////////////////////////////
static MENU_DATA get_menu_formWID(HMENU hMenu, UINT m_wid)
{
	MENU_DATA	ptr1;
	MENU_DATA	ptr2;
	MENU_DATA	return_val;


	LPMENU_DATA pointer_holder	= &ptr1;
	LPMENU_DATA next_pointer	= &ptr2;
	int count					= 0;

	CopyMemory(next_pointer, first_menu_data, sizeof(MENU_DATA));
	int menuType = MenuType(hMenu, m_wid);

	if(menuType == MF_STRING)
	{	
		while(pointer_holder->wID != (HMENU)m_wid )
		{
			CopyMemory(pointer_holder, next_pointer->nextItem, sizeof(MENU_DATA));
			next_pointer = pointer_holder;
			if(count > menu_objects -1) {break;}
			count++;	
		}
	}
	///////////////////////////////////////////////////////////////////////////////////
	//Handle the case POPUP, need to ilterate the tree                               //
	//Popups have a dynamic wID which makes detection more messy                     //
	///////////////////////////////////////////////////////////////////////////////////
	if(menuType == MF_POPUP)
	{
		while(pointer_holder->wID != (HMENU)m_wid )
		{
			
			CopyMemory(pointer_holder, next_pointer->nextItem, sizeof(MENU_DATA));
			next_pointer = pointer_holder;
			if(count > menu_objects -1) {break;}
			count++;
		}	
	}
	//free(pointer_holder);
	//free(next_pointer);
	CopyMemory(&return_val, pointer_holder, sizeof(MENU_DATA));
	return return_val;
}

static BOOL _GetMenu(MENU_DATA *MenuPtr, UINT m_wid)
{

	MenuPtr	= first_menu_data;
	int count = 0;

	while(MenuPtr->wID != (HMENU)m_wid )
	{
		MenuPtr = MenuPtr->nextItem;
		if(count > menu_objects -1)
		{
			MenuPtr = NULL;
			return FALSE;
		}
		count++;	
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////
//	basic like redim func                                                        //
//	reason 1 creates a new element                                               //
//	reason 2 gives current ptr                                                   //
//	check cleans & memout                                                        //
///////////////////////////////////////////////////////////////////////////////////
static LPMENU_DATA redim_items(LPMENU_DATA data_pointer, int reason)
{
	LPMENU_DATA pointer_holder = NULL;

	//return pointer_holder;
	if(menu_objects == 0)
	{
		//init the object loop
		primary_menu_data = malloc(sizeof(MENU_DATA));
		ZeroMemory(primary_menu_data, sizeof(MENU_DATA));
		first_menu_data = primary_menu_data;
		menu_objects++;
		return 	primary_menu_data;
	}
	if(reason == 1)
	{
		//redim the structure
		primary_menu_data->nextItem = malloc(sizeof(MENU_DATA));
		//save this
		pointer_holder = primary_menu_data->nextItem;
		//copy this onto main pointer
		CopyMemory(pointer_holder,primary_menu_data, sizeof(MENU_DATA));
		primary_menu_data = pointer_holder;
		menu_objects++;
	}
	if(reason == 2)
	{
		//get current pointer
		pointer_holder = primary_menu_data;
	}
	return 	pointer_holder;
}

///////////////////////////////////////////////////////////////////////////////////
// Purpose:       parse the XML file                                             //
// Input:         case IDispatch *NodeList, HMENU holder (parent menu)           //
//                                                                               //
// Output:        create the menu and the list of items.                         //
//                constructs the menu by reclusing the XML tre                   //
// Errors:        If the function succeeds, the return value is 1.               //
//                If the function fails, the return value is 0.                  //
// Notes:		  .                                                              //
// Author:        Benjamin Maggi 2005-2006                                       //
// Last Modified: Dec 2006                                                       //
///////////////////////////////////////////////////////////////////////////////////
static int parse_XML(IDispatch *NodeList, HMENU holder)
{

	DISPATCH_OBJ(TempNode);				// temporary storage node
	DISPATCH_OBJ(NodeItem);				// Child node
	LPMENU_DATA menu_data = NULL;

	LPWSTR			caption;			//
	LPWSTR			image_path = NULL;	//
	LPWSTR			font_name;			// Menu item font name
	LPWSTR			menuType;			// Either MF_STRING MF_MENUBARBREAK MF_MENUBREAK 
	BOOL			apiResult;			// Generic result for api calls
	LPWSTR			exec_path;			//
	LPWSTR			hexVal;				//
	HBITMAP			loaded_bmp = NULL;	//
	COLORREF		menu_fg_color;		//
	COLORREF		menu_bg_color;		//
	MENUITEMINFO	mii;				// Menu Item structure
	SIZE			zSize = {0, 0};
	int				icoSize;			// Size of the icons
	
	
	static	int MMU_IDS		= 1900;		// Start menu ID's at 1900
	static	int reclusines	= 0;		// Reclusion Starts at 0
			int ilter		= 0;		// Ilterations Start at 0
			int cnt			= 0;
			int depth		= 0;	
	
	
	if (reclusines == 0) { 
		icoSize = GetSystemMetrics(SM_CXICON); 
	} else { 
		icoSize = GetSystemMetrics(SM_CXSMICON); 
	}

	menu_data = redim_items(menu_data, CUR_MENU);


	dhGetValue(L"%d", &ilter, NodeList, L".length");

	for(cnt = 0; cnt < ilter; cnt++)
	{
		dhGetValue(L"%o", &NodeItem			,NodeList, L".item(%d)", cnt);
		dhGetValue(L"%S", &caption			,NodeItem, L".selectSingleNode(%S).text", L"CAPTION");
		dhGetValue(L"%S", &image_path		,NodeItem, L".selectSingleNode(%S).text", L"IMAGE");
		dhGetValue(L"%S", &hexVal			,NodeItem, L".selectSingleNode(%S).text", L"COLOR");
		menu_fg_color = xstrtoi(hexVal);
		dhGetValue(L"%S", &hexVal			,NodeItem, L".selectSingleNode(%S).text", L"BGCOLOR");
		menu_bg_color = xstrtoi(hexVal);
		dhGetValue(L"%S", &exec_path		,NodeItem, L".selectSingleNode(%S).text", L"EXECUTE");
		dhGetValue(L"%S", &font_name		,NodeItem, L".selectSingleNode(%S).text", L"FONT");
		dhGetValue(L"%S", &menuType			,NodeItem, L".selectSingleNode(%S).text", L"MENUTYPE");
		///////////////////////////////////////////////////////////////////////////////////
		// search for any for more objects                                               //
		///////////////////////////////////////////////////////////////////////////////////
		dhGetValue(L"%o", &TempNode, NodeItem, L".selectNodes(%S)",L"MENUITEM");
		dhGetValue(L"%d", &depth, TempNode, L".length");

	if(depth > 0)
		{
			// Create popups
			menu_data = redim_items(menu_data, NEW_MENU);

			// It must be a shell object if there is no icon find get windows
			// default icon for this item
			if(image_path[0] == '\0' )
			{
				///////////////////////////////////////////////////////////////////////////////
                //                                      
				///////////////////////////////////////////////////////////////////////////////
				HDC hDCDesktop = GetDC(0);
				loaded_bmp = CreateCompatibleBitmap(hDCDesktop, icoSize, icoSize);
  				ReleaseDC(NULL, hDCDesktop);
  				drawIconToBitmap(loaded_bmp, exec_path, icoSize, RGB(128, 128, 128));
  				menu_data->menu_imgSize.left = 0;
				menu_data->menu_imgSize.top = 0;
				menu_data->menu_imgSize.bottom = icoSize;
				menu_data->menu_imgSize.right = icoSize;
			}
			else
			{
			///////////////////////////////////////////////////////////////////////////////
			// Get The menu icon pass the data container                                 //
			///////////////////////////////////////////////////////////////////////////////
				loaded_bmp					= imageOptions(menu_data, image_path, menu_bg_color);
				menu_data->menu_imageOut	= imageOptions(menu_data, image_path, menu_bg_color);
				menu_data->menu_imageOvr	= imageOptions(menu_data, image_path, menu_fg_color);
			}
			//menu_data->menu_width = width;
			//menu_data->menu_height non staticaly set


			///////////////////////////////////////////////////////////////////////////////
			// Bug info:                                                                 //
			// Aparentemente cuando el ID es dinamico es decir atachado como un submenu  //
			// new_data se pierde y chau ......                                          //
			///////////////////////////////////////////////////////////////////////////////

			HMENU newPop				= CreatePopupMenu();
			menu_data->menu_image		= loaded_bmp;
			menu_data->menu_handle		= newPop;
			menu_data->menu_popup		= 1;
			menu_data->menu_capti		= caption;
			menu_data->menu_exec		= exec_path;
			menu_data->menu_color		= menu_fg_color;
			menu_data->menu_fontColor	= RGB(128,128,128);
			menu_data->menu_font_name	= font_name;
			menu_data->menu_font_size   = 9;
			menu_data->wID 				= newPop; //(UINT)menu_data->menu_handle; //aca pasa una cagada
			menu_data->menu_type		= MF_POPUP;

			//holder esta aca y el el puntero al menu en el que se esta trabajando
			apiResult = AppendMenu(holder, MF_POPUP | MF_OWNERDRAW , (unsigned int)newPop, caption);
			GetMenuItemInfo(holder, 1, TRUE, &mii); 

			reclusines = depth;
			
			//force reclusion item is a new item container we need to expand her childs
			parse_XML(TempNode,newPop);
		}
		else
		{
			///////////////////////////////////////////////////////////
			// Build an isolated menu item (does not have any child) //
			///////////////////////////////////////////////////////////
			menu_data = redim_items(menu_data, NEW_MENU);
			// set data for this item
			// If there's no image we get the icon from the executable (shell items also work)
			if(image_path[0] == '\0' )
			{
				HDC hDCDesktop = GetDC(0);
				menu_data->menu_imageOut = CreateCompatibleBitmap(hDCDesktop, icoSize,icoSize);
				menu_data->menu_imageOvr = CreateCompatibleBitmap(hDCDesktop, icoSize,icoSize);

  				ReleaseDC(NULL, hDCDesktop );

				drawIconToBitmap(menu_data->menu_imageOut, exec_path, icoSize, menu_bg_color);
				drawIconToBitmap(menu_data->menu_imageOvr, exec_path, icoSize ,menu_fg_color);
  				menu_data->menu_imgSize.left = 0;
				menu_data->menu_imgSize.top = 0;
				menu_data->menu_imgSize.bottom = icoSize;
				menu_data->menu_imgSize.right = icoSize;
			}
			else
			{
				loaded_bmp = imageOptions(menu_data, image_path, menu_fg_color);
				menu_data->menu_imageOut	= imageOptions(menu_data, image_path, menu_bg_color);
				menu_data->menu_imageOvr	= imageOptions(menu_data, image_path, menu_fg_color);
			}


			if(menu_data->menu_image != NULL)
			{
				//MessageBox(main_window,image_path,"Error de tipo:",MB_OK);
			}

			menu_data->menu_image		= loaded_bmp;
			menu_data->menu_handle		= holder;
			menu_data->menu_popup		= 0;
			menu_data->menu_capti		= caption;
			menu_data->menu_exec		= exec_path;
			menu_data->menu_color		= menu_fg_color;
			menu_data->menu_fontColor	= RGB(128,128,128);
			menu_data->menu_font_name	= font_name;
			menu_data->menu_font_size   = 6;
			menu_data->wID				= (HMENU)MMU_IDS;
			menu_data->xmlMenuItem		= (IXMLDOMElement*) NodeItem;
			//populateAttributes((IXMLDOMElement*)NodeItem);//menu_data->xmlMenuItem);
			
			menu_data->menu_width = 0;
			menu_data->menu_width = max(menu_data->menu_width, zSize.cx);

			if (!wcscmp(menuType, L"MF_STRING"))
			{
				if (wcslen(caption) >= 0)
				{
					menu_data->menu_capti = caption;
					menu_data->menu_type = MF_STRING;
					AppendMenu(holder, MF_STRING | MF_OWNERDRAW, MMU_IDS, caption);
				}
			}
			else if (!wcscmp(menuType, L"MF_MENUBARBREAK"))
			{
				menu_data->menu_capti = caption;
				menu_data->menu_type = MF_MENUBARBREAK;
				apiResult = AppendMenu(holder, MF_MENUBARBREAK | MF_OWNERDRAW, MMU_IDS, caption);
			}
			else if (!wcscmp(menuType, L"MF_MENUBREAK"))
			{
				menu_data->menu_capti = caption;
				menu_data->menu_type = MF_MENUBREAK;
				apiResult = AppendMenu(holder, MF_MENUBREAK | MF_OWNERDRAW, MMU_IDS, caption);
			}
			else if (!wcscmp(menuType, L"MF_SEPARATOR"))
			{
				menu_data->menu_capti = caption;
				menu_data->menu_type = MF_SEPARATOR;
				apiResult = AppendMenu(holder, MF_SEPARATOR | MF_OWNERDRAW, MMU_IDS, NULL);
			}
			else if (!wcscmp(menuType, L"MF_SIDEBAR"))
			{
				menu_data->menu_capti = caption;
				menu_data->menu_type = MF_SIDEBAR;
				apiResult = AppendMenu(holder, MF_OWNERDRAW, MMU_IDS, NULL);
			}
			MMU_IDS++;
		}

	}//NEXT

	//return 1;
cleanup:
	//dhFreeString(caption);
	//dhFreeString(exec_path);
	reclusines--;
	dhFreeString(image_path);
	SAFE_RELEASE(TempNode);
	SAFE_RELEASE(NodeItem);
	return 1;

}

///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK install_Hook (int code, WPARAM wParam, LPARAM lParam)
{
	CWPSTRUCT* pStruct = (CWPSTRUCT*)lParam;

	static HWND acthMenu = NULL;
    static HWND acthWnd	 = NULL;

	damm_handle = pStruct->hwnd;
	//return CallNextHookEx (_hHook, code, wParam, lParam);

	while ( code == HC_ACTION )
	{
		HWND hWnd = pStruct->hwnd;
		// Normal and special handling for menu 0x10012
		if ( pStruct->message != WM_CREATE && pStruct->message != 0x01E2 )
		{
			break;
		}

		//4402762 chorrus
		HWND pFrame = GetForegroundWindow();

		//si es que activamos el skin para esta ventana con handle
		//if ( pFrame == NULL || !GetXPLookNFeel (pFrame) )
		if ( pFrame == NULL )
		{
			break;
		}
		WCHAR sClassName[256];
		int Count = GetClassName(hWnd, sClassName, MAX_CLASS_NAME);
		// Check for the menu-class
		if (Count != 6 || wcscmp(sClassName, L"#32768") != 0)
		{
			acthWnd = hWnd;
		}
		else
		{
			acthMenu = hWnd;
			HWND hMenuWnd = pStruct->hwnd;
      		DWORD dwStyle =0;
      		dwStyle = GetWindowLong(hMenuWnd, GWL_EXSTYLE);
      		SetWindowLong(hMenuWnd, GWL_EXSTYLE, dwStyle|WS_EX_LAYERED);

			SetLayeredWindowAttributes(hMenuWnd, 0, main_menu_theme.menu_theme_alpha, LWA_ALPHA);
		}
		//hasta aca estamos seguros Safe point
		HANDLE resu = GetProp (pStruct->hwnd, _WndPropName_OldProc);
		if ( GetProp (pStruct->hwnd, _WndPropName_OldProc) != NULL )
		{
			// Already subclassed
			break;
		}
		// Subclass the window
        //get the code pointer

		WNDPROC oldWndProc = (WNDPROC)(LONG_PTR)GetWindowLong (pStruct->hwnd, GWL_WNDPROC);
		old_menu_proc = oldWndProc;
		if(oldWndProc == NULL)
 		{
  			//oops GPF we'r on the edge !!!
			break;
		}

		mnu_hwnd = pStruct->hwnd;
		if ( !SetProp (pStruct->hwnd, _WndPropName_OldProc, oldWndProc) )
        {
            break;
        }
		long set_wl = SetWindowLong (pStruct->hwnd, GWL_WNDPROC,(DWORD)(DWORD_PTR)menuproc_sclass);
		if(!set_wl)
		{
			RemoveProp (pStruct->hwnd, _WndPropName_OldProc);
			break;
		}


		// Success !
		break;
	}
	return CallNextHookEx (_hHook, code, wParam, lParam);
}

///////////////////////////////////////////////////////////////////////////////////
// This is where the hook is going to come                                       //
// It's verry important to keep this as clean as possible, if by some reason     //
// you can return form this code the app will crash                              //
// It's also required that you call the original window proc after done          //
//                                                                               //
///////////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK menuproc_sclass(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	WNDPROC oldWndProc = (WNDPROC)GetProp(hWnd, _WndPropName_OldProc);

	switch ( uMsg )
	{
		case WM_DRAWITEM:
		{
			// Not used.
			LPDRAWITEMSTRUCT lpDs = (LPDRAWITEMSTRUCT)lParam;
		}
		case WM_PRINT:
		{
			break;
		}
		/*
		case WM_NCCALCSIZE:
		{
			NCCALCSIZE_PARAMS* lpncsp = (NCCALCSIZE_PARAMS*)lParam;
			lpncsp->rgrc[0].left +=2;
			lpncsp->rgrc[0].top +=2;
			lpncsp->rgrc[0].right -=2;
			lpncsp->rgrc[0].bottom -=2;
			return WVR_REDRAW;
		}
		*/
		case WM_WINDOWPOSCHANGING:
		{
			WINDOWPOS* lpwinpos = (LPWINDOWPOS)lParam;

	 		if( (lpwinpos->flags & SWP_NOSIZE) == 0) 
			{
				lpwinpos->cx += 0;	//Menu Width
				lpwinpos->cy += 0;	//bugfix: -= 3 will make the last menu item's bottom border invisible.
			}
			if((lpwinpos->flags & SWP_NOMOVE) == 0)
			{
				lpwinpos->x += 0;	//Move the sub-menu's window right
				lpwinpos->y += 0;	//Move the sub-menu's window down
			}
			return 0;
		}
		case WM_ERASEBKGND:
		{
			//MessageBeep(MB_ICONEXCLAMATION);
			return TRUE;
		}
		case WM_NCPAINT:
		{
			//return 0;
			HDC hDC;
			RECT rcTwo;
			RECT rcSep;
			RECT rcOrg;
			int bdr_w = SIDEBAR_WIDTH + 4;   //main menu sidebar w 4 is the border width missing
			int bdr_h = 150;

			hDC = GetWindowDC (hWnd);
			GetWindowRect(hWnd, &rcTwo);

			GetClipBox(hDC,&rcTwo);
			rcOrg = rcTwo;

			/////////////////////////////////////////////////////////////////
			// Draw Menu border                                            //
			/////////////////////////////////////////////////////////////////
			rcSep.top 		= rcTwo.top 	+2;		// Fixed calculation of 3d border
			rcSep.left		= rcTwo.left	-2;
			rcSep.bottom	= rcTwo.bottom  -2;
			rcSep.right		= rcTwo.right   +2;

			HBRUSH hBrush = CreateSolidBrush(main_menu_theme.menu_theme_borderColor); // Menu Border color
			FrameRect(hDC, &rcTwo, hBrush);					// it will draw a 1px border only

			/////////////////////////////////////////////////////////////////
			// Fill the rest of the menu border with color                 //
			/////////////////////////////////////////////////////////////////
			/*		
			rcTwo.top		= rcTwo.top		+1;
			rcTwo.left		= rcTwo.left	+1;
			rcTwo.bottom	= rcTwo.bottom	-1;
			rcTwo.right		= rcTwo.right	-1;
			*/
			InflateRect(&rcTwo,-1,-1);
			DeleteObject(hBrush);

			hBrush = CreateSolidBrush(main_menu_theme.menu_theme_bgColor);	// Menu Color
			FrameRect(hDC, &rcTwo, hBrush);					// it will draw a 1px border only

			/////////////////////////////////////////////////////////////////
			// Sice the 3D border is 3px make 2 rects                      //
			/////////////////////////////////////////////////////////////////
			InflateRect(&rcTwo,-1,-1);
			FrameRect(hDC, &rcTwo, hBrush);
			DeleteObject(hBrush);

			/////////////////////////////////////////////////////////////////
			// Fill the sidebar with bar                                   //
			// Try not to paint over any live item or it will cause flickr //
			/////////////////////////////////////////////////////////////////
			hBrush = CreateSolidBrush(main_menu_theme.menu_theme_sidebarColor);	// Sidebar Color
			rcTwo.top		= rcOrg.top 	+1;
			rcTwo.left		= rcOrg.left	+1;
			rcTwo.bottom	= rcOrg.bottom  -1;
			rcTwo.right		= 3;

			FillRect(hDC, &rcTwo, hBrush);		// it will draw a 1px border only

			rcTwo.top		= rcOrg.bottom	-3;
			rcTwo.left		= rcOrg.left	+1;
			rcTwo.bottom	= rcOrg.bottom	-1;
			rcTwo.right		= bdr_w;

			FillRect(hDC, &rcTwo, hBrush);		// it will draw a 1px border only

			rcTwo.top		= rcOrg.top		-1;
			rcTwo.left		= rcOrg.left	+1;
			rcTwo.bottom	= rcOrg.top		+3;
			rcTwo.right		= bdr_w;

			FillRect(hDC, &rcTwo, hBrush);		// it will draw a 1px border only

			DeleteObject(hBrush);

			/////////////////////////////////////////////////////////////////
			// Sidebar Border                                              //
			/////////////////////////////////////////////////////////////////
			rcTwo.top 	 = rcOrg.top 	;
			rcTwo.left	 = rcOrg.left	;
			rcTwo.bottom 	 = rcOrg.bottom ;
			rcTwo.right	 = 0;
			rcTwo.right = bdr_w;
	
			HPEN hPen = CreatePen(PS_SOLID, 1, main_menu_theme.menu_theme_borderColor);
			HPEN hPenOld = SelectObject(hDC,hPen);

			MoveToEx(hDC, rcTwo.left, rcTwo.top, NULL);
			LineTo  (hDC, rcTwo.right, rcTwo.top);
			MoveToEx(hDC, rcTwo.right-1, rcTwo.top, NULL);
			LineTo  (hDC, rcTwo.right-1, rcTwo.top + 3);
			MoveToEx(hDC, rcTwo.left, rcTwo.bottom-1, NULL);
			LineTo  (hDC, rcTwo.right, rcTwo.bottom-1);
			MoveToEx(hDC, rcTwo.right-1, rcTwo.bottom, NULL);
			LineTo  (hDC, rcTwo.right-1, rcTwo.bottom - 4);

			SelectObject(hDC, hPenOld);
			DeleteObject(hPen);
	
			// Restore & DC's and objects state
			ReleaseDC(hWnd, hDC);
			return 0;
		}
		case WM_SHOWWINDOW:
		{
			BOOL bShow = wParam != 0;
			if (!bShow)
			{
				SetWindowLong(hWnd, GWL_WNDPROC, (DWORD)(DWORD_PTR)oldWndProc);
				RemoveProp(hWnd, _WndPropName_OldProc);
			}
			break;
		}
		case WM_DESTROY:
		{
			SetWindowLong(hWnd, GWL_WNDPROC, (DWORD)(DWORD_PTR)oldWndProc);
			RemoveProp(hWnd, _WndPropName_OldProc);
			break;
		}
		case WM_CLOSE:
		{
			SetWindowLong(hWnd, GWL_WNDPROC, (DWORD)(DWORD_PTR)oldWndProc);
			RemoveProp(hWnd, _WndPropName_OldProc);
			break;
		}
		case WM_NCDESTROY:
		{
			if ( oldWndProc != NULL )
			{
				WNDPROC NewWndProc = (WNDPROC)(LONG_PTR)GetWindowLong (hWnd, GWL_WNDPROC);
				SetWindowLong (hWnd, GWL_WNDPROC, (DWORD)(DWORD_PTR)oldWndProc);
				RemoveProp (hWnd, _WndPropName_OldProc);
				NewWndProc = (WNDPROC)(LONG_PTR)GetWindowLong (hWnd, GWL_WNDPROC);
			}
    		break;
		}
		default:
		return CallWindowProc (oldWndProc, hWnd, uMsg, wParam, lParam);
	}
	//End case
	return CallWindowProc (oldWndProc, hWnd, uMsg, wParam, lParam);
}

///////////////////////////////////////////////////////////////////////////////////
// Here you start the hook to the windows procedure of the menu                  //
///////////////////////////////////////////////////////////////////////////////////
void initHook(void)
{
	_hHook = (HHOOK) SetWindowsHookEx(WH_CALLWNDPROC, &install_Hook, hInst, GetCurrentThreadId());
}

///////////////////////////////////////////////////////////////////////////////////
void unitHook(void)
{
	WNDPROC NewWndProc = (WNDPROC)(LONG_PTR)GetWindowLong (mnu_hwnd, GWL_WNDPROC);
	SetWindowLong (mnu_hwnd, GWL_WNDPROC,(DWORD)(DWORD_PTR)old_menu_proc);
	NewWndProc = (WNDPROC)(LONG_PTR)GetWindowLong (mnu_hwnd, GWL_WNDPROC);
	RemoveProp (mnu_hwnd, _WndPropName_OldProc);
	UnhookWindowsHookEx(_hHook);
}


///////////////////////////////////////////////////////////////////////////////////
int draw_buttons(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	LPDRAWITEMSTRUCT lpDs = (LPDRAWITEMSTRUCT)lParam;
	HDC 	hDC;
	HBITMAP temp_bmp[3];
	BITMAP	bmp;

	hDC = CreateCompatibleDC(lpDs->hDC);	// create a memory DC

	switch(lpDs->CtlID)
	{
		case 330: //main_menu
			temp_bmp[0] = g_hBmpUp;
			temp_bmp[1] = g_hBmpDown;
			temp_bmp[2] = g_hBmpHot;
			GetObject(g_hBmpUp, sizeof(bmp), &bmp);
			break;
		case 331: //opti_button:
			temp_bmp[0] = plus_up;
			temp_bmp[1] = plus_dw;
			temp_bmp[2] = plus_ht;
			GetObject(plus_up, sizeof(bmp), &bmp);
			break;
		case 332: //plus_button:
			temp_bmp[0] = opti_up;
			temp_bmp[1] = opti_dw;
			temp_bmp[2] = opti_ht;
			GetObject(plus_up, sizeof(bmp), &bmp);
			break;

	}
	//end switch
	if(hDC)
	{
		HGDIOBJ saveObj;
		if(lpDs->itemState & ODS_SELECTED) //  if button is down
		{
			saveObj = SelectObject(hDC, temp_bmp[1]);
		}
		else
		{
			saveObj = SelectObject(hDC, temp_bmp[0]);
		}

		BitBlt(lpDs->hDC, 0, 0, bmp.bmWidth, bmp.bmHeight, hDC, 0, 0, SRCCOPY);

		SelectObject(hDC, saveObj);

		if(lpDs->CtlID == 330)
		{

		}

		DeleteDC(hDC);
	}
	return 0;
}
///////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////

static HMENU FindMenuFromID(HMENU hMenu, UINT id)
{
	HMENU 			hSubMenu = NULL;
	MENUITEMINFO	mii;
	int				isDone = 0;
	int				iCount = GetMenuItemCount(hMenu);


	for ( int i = 0; i < iCount; i++) 
	{
		if ( id == GetMenuItemID(hMenu, i) ) 
		{
			//wID found we'r on a normal menuitem
			GetMenuItemInfo(hMenu, i, TRUE, &mii);
			return hMenu;
		}
		else 
		{
			hSubMenu = GetSubMenu(hMenu, i);
			if(hSubMenu == (HMENU)id)
			{
				GetMenuItemInfo(hSubMenu, 0, TRUE, &mii);
				isDone = id;
				return hSubMenu;
			}
			if ( NULL != hSubMenu ) 
			{
				// recurse
				hSubMenu = FindMenuFromID(hSubMenu, id);
				if ( NULL != hSubMenu ) 
					{
						return hSubMenu;
					}
			}
		}
	}
	// no match
	return NULL;
}

////////////////////////////////////////////////////////////////////////////////////
// This function will return the menu type of a given ID searches the entire tree //
////////////////////////////////////////////////////////////////////////////////////
int MenuType(HMENU hMenu, UINT id)
{
	HMENU hSubMenu	= NULL;
	HMENU hSubChild	= NULL;
	int isDone = 0;
	int iCount = GetMenuItemCount(hMenu);
	for ( int i = 0; i < iCount; i++) 
	{
		if ( id == GetMenuItemID(hMenu, i) ) 
		{
			// wID found we'r on a normal menuitem
			// Normal menu item
			return MF_STRING;
		}
		else
		{
			hSubMenu = GetSubMenu(hMenu, i);
			if(hSubMenu == (HMENU)id)
			{
				return MF_POPUP;
			}
			if ( NULL != hSubMenu ) 
			{
				hSubMenu = FindMenuFromID(hSubMenu, id);
				if ( hSubMenu != NULL) 
				{
					return MF_STRING;
				}
			}
		}
	}

	return -1;
}

///////////////////////////////////////////////////////////////////////////////////
// This function will return true if a menu item is part of the root menu        //
//                                                                               //
///////////////////////////////////////////////////////////////////////////////////
UINT IsRootMenu(UINT id)
{
	HMENU hSubMenu = NULL;
	int iCount = GetMenuItemCount(main_menu);
	for ( int i = 0; i < iCount; i++)
	{
		if ( id == GetMenuItemID(main_menu, i) )
		{
			return 1;
		}
		else
		{
			hSubMenu = GetSubMenu(main_menu, i);
			if((UINT)hSubMenu == id)
			{
				return 1;
			}
		}
	}
	return 0;
}

///////////////////////////////////////////////////////////////////////////////
// Start drag and drop operations from the HWDN source                       //
///////////////////////////////////////////////////////////////////////////////
void startDragDrop(HWND hwndList)
{
	DWORD       dwEffect;
	HRESULT 	hr;
	//////////////////////////////////////////////////////////////
	//get the ID for the File Group Descriptor clipboard format
	unsigned short cfFileGroupDescriptor = 0;

	//get the ID for the File Contents clipboard format
	unsigned short cfFileContents = 0;

	IDataObject * pDataObject = DataObject_CDataObject();
	IDropSource * pDropSource = DropSource_CDropSource();
	if(!pDataObject || !pDropSource)
		return;

	//get the ID for the File Group Descriptor clipboard format
	cfFileGroupDescriptor = (unsigned short)RegisterClipboardFormat(CFSTR_FILEDESCRIPTOR);

	//get the ID for the File Contents clipboard format
	cfFileContents = (unsigned short)RegisterClipboardFormat(CFSTR_FILECONTENTS);

	FORMATETC fe;
	
	//initialize the data object
	fe.cfFormat 	= cfFileGroupDescriptor;
	fe.ptd 			= NULL;
	fe.dwAspect 	= DVASPECT_CONTENT;
	fe.lindex 		= -1;
	fe.tymed 		= TYMED_HGLOBAL;

	STGMEDIUM sm;
	sm.tymed 		  = TYMED_HGLOBAL;
	sm.pUnkForRelease = NULL;
	//sm.hGlobal      = MessageBeep(0); //CreateFileGroupDescriptor(pszFiles);

	pDataObject->lpVtbl->SetData(pDataObject, &fe, &sm, TRUE);

	fe.cfFormat = cfFileContents;
	fe.ptd 		= NULL;
	fe.dwAspect = DVASPECT_CONTENT;
	fe.tymed 	= TYMED_HGLOBAL;

	sm.tymed = TYMED_HGLOBAL;
	sm.pUnkForRelease = NULL;

	// do the drag and drop
	hr = DoDragDrop(pDataObject, pDropSource, DROPEFFECT_MOVE, &dwEffect);

	pDataObject->lpVtbl->Release(pDataObject);
	pDropSource->lpVtbl->Release(pDropSource);

	return;
}

///////////////////////////////////////////////////////////////////////////////////
// This function will take acctions regarding each image format                  //
// Bye example PNG can be transparents so we will enable this feature            //
// Also we provide support for other formats as well a nice improvement that     //
// The program needed                                                            //
// It also sets the menu_imgSize value for this menu                             //
///////////////////////////////////////////////////////////////////////////////////
HBITMAP	imageOptions(LPMENU_DATA menu_data_ptr, LPWSTR image_path ,COLORREF bgColor)
{
	
	HBITMAP tmpBMP = NULL;

	HDC hDCDesktop = GetDC(0);
	FREE_IMAGE_FORMAT fif = FreeImage_GetFileTypeU(image_path, 0);
	//MessageBox(NULL, image_path, L"Error !", MB_ICONWARNING);
	FIBITMAP *image = FreeImage_LoadU(fif, image_path, 0);
	if (!image)
	{
		// Could not load image
		return NULL;
	}

	static int img_width = 0;
	static int img_height = 0;
	img_width	= FreeImage_GetWidth(image);
	img_height	= FreeImage_GetHeight(image);
	
	FreeImage_Unload(image);

	menu_data_ptr->menu_imgSize.left = 0;
	menu_data_ptr->menu_imgSize.top = 0;
	menu_data_ptr->menu_imgSize.right = img_width;
	menu_data_ptr->menu_imgSize.bottom = img_height;

	switch(fif)
	{
		case FIF_UNKNOWN:
		break;
		case FIF_BMP:
			tmpBMP = LoadImage(hInst,image_path,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
			return tmpBMP;
			break;
		break;
		case FIF_ICO:
		{
			tmpBMP = LoadImage(hInst,image_path,IMAGE_ICON,0,0,LR_LOADFROMFILE);
			
			FIBITMAP *image = FreeImage_LoadU(fif, image_path, 0);
			if(img_width > 32 || img_height > 32 || img_width != img_height)
			{
				//image = FreeImage_Rescale(image,16,16,FILTER_BICUBIC);
			}
			FreeImage_PreMultiplyWithAlpha(image);
			tmpBMP = CreateDIBitmap(hDCDesktop, FreeImage_GetInfoHeader(image),CBM_INIT, FreeImage_GetBits(image), FreeImage_GetInfo(image), DIB_RGB_COLORS);
			FreeImage_Unload(image);
			return tmpBMP;
		}
		case FIF_JPEG:
			break;
		case FIF_JNG:
			break;
		case FIF_KOALA:
			break;
		case FIF_LBM:
			break;
		case FIF_MNG:
			break;
		case FIF_PBM:
			break;
		case FIF_PBMRAW:
			break;
		case FIF_PCD:
			break;
		case FIF_PCX:
			break;
		case FIF_PGM:
			break;
		case FIF_PGMRAW:
			break;
		case FIF_PNG:
		{
	
			FIBITMAP *image = FreeImage_LoadU(fif, image_path, PNG_DEFAULT);
			//image = FreeImage_GetChannel(image,FICC_ALPHA); // Get Alpha Mask
			if(img_width > 32 || img_height > 32 || img_width != img_height)
			{
				//image = FreeImage_Rescale(image,16,16,FILTER_BICUBIC);
			}
			BOOL bHasBackground = FreeImage_HasBackgroundColor(image);
			BOOL bIsTransparent = FreeImage_IsTransparent(image);
			// image (c) : use a user specified solid background 
			RGBQUAD appColor; // = { 128, 128, 128, 0 }; 
				appColor.rgbRed			= GetRValue(bgColor);
				appColor.rgbGreen		= GetGValue(bgColor);
				appColor.rgbBlue		= GetBValue(bgColor);
				appColor.rgbReserved	= 0;
			//Use checkboard
			//FIBITMAP *display_dib = FreeImage_Composite(image, FALSE, NULL,NULL);// &appColor,NULL);
			//Use Solid Color
			FreeImage_PreMultiplyWithAlpha(image);
			// 2009 FIBITMAP *display_dib = FreeImage_Composite(image, FALSE, &appColor,NULL);
			//Use a file
			//FIBITMAP *bg = NULL;
			//FIBITMAP * display_dib = NULL;
			//bg = FreeImage_Load(FIF_BMP, "bgr.bmp", BMP_DEFAULT); 
			//display_dib = FreeImage_Composite(image,TRUE, NULL,bg);
			// Use file
			//display_dib = FreeImage_Composite(image, TRUE,NULL,bg);  laque anda
			// Copy to an HBITMAP
			// Release all GDI objects
			// 2009 tmpBMP = CreateDIBitmap(hDCDesktop, FreeImage_GetInfoHeader(display_dib),CBM_INIT, FreeImage_GetBits(display_dib), FreeImage_GetInfo(display_dib), DIB_RGB_COLORS);
			tmpBMP = CreateDIBitmap(hDCDesktop, FreeImage_GetInfoHeader(image),CBM_INIT, FreeImage_GetBits(image), FreeImage_GetInfo(image), DIB_RGB_COLORS);
			FreeImage_Unload(image);
			//FreeImage_Unload(bg);
			ReleaseDC(NULL, hDCDesktop);
			return tmpBMP;
			break;
		}
		case FIF_PPM:
			break;
		case FIF_PPMRAW:
			break;
		case FIF_RAS:
			break;
		case FIF_TARGA:
			break;
		case FIF_TIFF:
			break;
		case FIF_WBMP:
			break;
		case FIF_PSD:
			break;
		case FIF_CUT:
			break;
		case FIF_XBM:
			break;
		case FIF_XPM:
			break;
		case FIF_DDS:
			break;
		case FIF_GIF:
			break;
		case FIF_HDR:
			break;
		case FIF_FAXG3:
			break;
		case FIF_SGI:
			break;
	}		
	ReleaseDC(NULL, hDCDesktop );
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////////
// Purpose:       Make windows magnetic                                          //
// Input:         case WM_MOVING:                                                //
//                return MagnetWindow(wParam,lParam);                            //
// Output:        Change windows coordinates.                                    //
// Errors:        If the function succeeds, the return value is true.            //
//                If the function fails, the return value is false.              //
// Notes:		  This function can only handle single window.                   //
// Author:        tuukka.lehtinen@koti.soon.fi                                   //
///////////////////////////////////////////////////////////////////////////////////
BOOL MagnetWindow(LPARAM *lParam)
{
	RECT *pRect = (LPRECT) *lParam;				// screen coordinates
	static RECT rcOld = { 0, 0, 0, 0 };
	RECT rcDesktop;								// dimension of desktop work area
	INT	 iPower = 20;							// how long distance windows snap (in pixels)
	SIZE szSize;								// size of window

	///////////////////////////////////////////////////////////////////////////////////
	// get the desktop work area dimension                                           //
	///////////////////////////////////////////////////////////////////////////////////
	if(FALSE == SystemParametersInfo(SPI_GETWORKAREA, 0, &rcDesktop, 0))
		return FALSE;

	szSize.cx = pRect->right -  pRect->left;
	szSize.cy = pRect->bottom - pRect->top;


	if(pRect->top < iPower && rcOld.top > pRect->top)
	{
		pRect->top = 0;
		pRect->bottom = szSize.cy;
	}
	else
	if(pRect->bottom > (rcDesktop.bottom - iPower) &&
	   rcOld.bottom  < pRect->bottom)
	{
		pRect->bottom = rcDesktop.bottom;
		pRect->top = rcDesktop.bottom - szSize.cy;
	}

	if(pRect->left < iPower && rcOld.left > pRect->left)
	{
		pRect->left = 0;
		pRect->right = szSize.cx;
	}
	else
	if(pRect->right > (rcDesktop.right - iPower) &&
		rcOld.right < pRect-> right)
	{
		pRect->right = rcDesktop.right;
		pRect->left =  rcDesktop.right - szSize.cx;
	}

	rcOld.top = pRect -> top;
	rcOld.bottom = pRect -> bottom;
	rcOld.left = pRect -> left;
	rcOld.right = pRect -> right;

	return TRUE;
}

int HextoDec(LPWSTR hex)
{
	if (*hex==0) return 0;
	return  HextoDec(hex-1)*16 +  xtod(*hex) ;
}
int xstrtoi(LPWSTR hex)    // hex string to integer
{
	return HextoDec(hex + wcslen(hex) - 1);
}


HMENU xmlInitnstance(LPWSTR fname, BOOL bOptimizeMemory)
{
	IXMLDOMDocument	*	document	= NULL;
	IXMLDOMParseError*	parseError	= NULL;
	IXMLDOMElement*		element		= NULL;	
	BSTR				bstr		= NULL;
	VARIANT_BOOL		status		= VARIANT_FALSE;
	VARIANT				vSrc;
	BSTR				fileName	= NULL;
	HMENU				menu		= NULL;

	HRESULT hr;
	hr = CoInitialize(NULL);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Could not CoInitialize()", L"Error !",MB_ICONWARNING);
		return FALSE;
	}

	hr = CoCreateInstance(&CLSID_DOMDocument, NULL, CLSCTX_INPROC_SERVER | CLSCTX_LOCAL_SERVER, &IID_IXMLDOMDocument, (LPVOID*)&document);
	if (FAILED(hr)) 
	{
		MessageBox(NULL, L"Failed to CoCreate an instance of an XML DOM", L"Error !",MB_ICONWARNING);
	}

	document->lpVtbl->put_async(document, VARIANT_FALSE);
	VariantInit(&vSrc);
	//ConvertAnsiStrToBStrx(fname, &fileName);
	//convertWcharToBstr(fname, &fileName)
	V_BSTR(&vSrc) = SysAllocString(fname);
	V_VT(&vSrc) = VT_BSTR;

	hr = document->lpVtbl->load(document, vSrc, &status);

	if (status != VARIANT_TRUE)
	{
		long line, linePos;
		BSTR reason = NULL;
		WCHAR errMsg[1024];

		hr = document->lpVtbl->get_parseError(document,&parseError);
		hr = parseError->lpVtbl->get_reason(parseError,&bstr);
		hr = parseError->lpVtbl->get_errorCode(parseError,&hr);
		hr = parseError->lpVtbl->get_line(parseError,&line);
		hr = parseError->lpVtbl->get_linepos(parseError,&linePos);

		wsprintf(errMsg, L"Error 0x%.8X on line %d, position %d\r\nReason: %s", hr, line, linePos, reason);
		MessageBox(NULL, errMsg, L"Load Error !", MB_ICONWARNING);
        goto clean;
    }

	BSTR nodeName;
	hr = document->lpVtbl->get_documentElement(document,&element);
	if (FAILED(hr) || element == NULL)
	{
		MessageBox(NULL, L"Empty document!", L"Error Loading XML", MB_ICONWARNING);
        goto clean;
	}
	IXMLDOMNodeList* childList;
	hr = element->lpVtbl->get_childNodes(element,&childList);
	if (FAILED(hr) || element == NULL)
	{
		MessageBox(NULL, L"Empty document!", L"Error Loading XML", MB_ICONWARNING);
		goto clean;
	}
	menu = CreatePopupMenu();
	buildMenu(childList, menu);
	
	
	element->lpVtbl->get_nodeName(element,&nodeName);
clean:
    if (bstr) SysFreeString(bstr);
    if (&vSrc) VariantClear(&vSrc);
	if (fileName) CoTaskMemFree((void*)fileName);
    if (parseError) parseError->lpVtbl->Release(parseError);
    if (document) document->lpVtbl->Release(document);
	return menu;
}

///////////////////////////////////////////////////////////////////////////////////
// Build Menu XML                                                                //
///////////////////////////////////////////////////////////////////////////////////
static int buildMenu(IXMLDOMNodeList *node, HMENU rootMenu)
{
	HRESULT 			hr = S_OK;
	long				listLength;
	VARIANT_BOOL		hasChild = VARIANT_FALSE;
	IXMLDOMNode*		nodeList = NULL;
	IXMLDOMNode*		resultNode = NULL;

	static	int		MMU_IDS		= 2900;		// Start menu ID's at 2900
	static	int 	reclusines	= 0;		// Reclusion Starts at 0
			int 	ilter		= 0;		// Ilterations Start at 0
			int		cnt			= 0;
			long	depth		= 0;	

	hr = node->lpVtbl->get_length(node,&listLength);
	if (FAILED(hr))
	{
		goto cleanup;
	}
	for(long i = 0; i < listLength; i++)
	{
		MENU_DATA *menuData = (MENU_DATA *)malloc(sizeof(MENU_DATA));

		hr = node->lpVtbl->get_item(node, i, &nodeList);
		hr = nodeList->lpVtbl->selectSingleNode(nodeList, L"CAPTION", &resultNode);
		BSTR caption;
		resultNode->lpVtbl->get_text(resultNode, &caption);
		resultNode->lpVtbl->get_text(resultNode, &menuData->label);
		resultNode->lpVtbl->Release(resultNode);
		hr = nodeList->lpVtbl->selectSingleNode(nodeList, L"IMAGE" , &resultNode);
		resultNode->lpVtbl->get_text(resultNode, &menuData->imagePath);
		resultNode->lpVtbl->Release(resultNode);
		hr = nodeList->lpVtbl->selectSingleNode(nodeList, L"COLOR", &resultNode);
		resultNode->lpVtbl->Release(resultNode);
		hr = nodeList->lpVtbl->selectSingleNode(nodeList, L"BGCOLOR", &resultNode);
		resultNode->lpVtbl->Release(resultNode);
		hr = nodeList->lpVtbl->selectSingleNode(nodeList, L"EXECUTE", &resultNode);
		resultNode->lpVtbl->Release(resultNode);
		hr = nodeList->lpVtbl->selectSingleNode(nodeList, L"FONT", &resultNode);
		resultNode->lpVtbl->Release(resultNode);
		hr = nodeList->lpVtbl->selectSingleNode(nodeList, L"MENUTYPE", &resultNode);
		resultNode->lpVtbl->Release(resultNode);
		hr = nodeList->lpVtbl->selectSingleNode(nodeList, L"MENUTYPE", &resultNode);
		BSTR menuType;
		resultNode->lpVtbl->get_text(resultNode, &menuType);
		resultNode->lpVtbl->Release(resultNode);

		///////////////////////////////////////////////////////////////////////////////////
		// search for nested objects                                                     //
		///////////////////////////////////////////////////////////////////////////////////
		IXMLDOMNodeList *menuChild;
		hr = nodeList->lpVtbl->selectNodes(nodeList, L"MENUITEM", &menuChild);
		hr = menuChild->lpVtbl->get_length(menuChild, &depth);
		
		if (FAILED(hr))
		{
			MessageBoxW(NULL, L"selectSingleNode()", L"Error", MB_ICONINFORMATION);
			return hr;
		}
		
		MENUITEMINFO menuiteminfo;
		ZeroMemory(&menuiteminfo, sizeof(menuiteminfo));
		menuiteminfo.cbSize = sizeof(menuiteminfo);
		menuiteminfo.fMask = MIIM_FTYPE | MIIM_ID | MIIM_SUBMENU | MIIM_DATA | MIIM_STRING | MIIM_BITMAP;
		menuiteminfo.fType = MFT_STRING;
		menuiteminfo.wID = MMU_IDS;
		menuiteminfo.dwTypeData = menuData->label;
		menuiteminfo.cch = (UINT)min(wcslen(menuiteminfo.dwTypeData), UINT_MAX);
		menuiteminfo.dwItemData = menuData;
		menuiteminfo.hbmpItem = LoadImageSmart(menuData->imagePath);//HBMMENU_MBAR_CLOSE;
		
		/////////////////////////////////////////////////////////////////////////////
		// For Items with nested nodes build a popup menu                          //
		/////////////////////////////////////////////////////////////////////////////
		if(depth > 0)
		{
			HMENU popup = CreatePopupMenu();
			//AppendMenu(rootMenu, MF_POPUP, (unsigned int)newPop, caption);
			InsertMenu(rootMenu, i, MF_POPUP, (unsigned int)popup, menuData->label);
			buildMenu(menuChild, popup);
		}
		else
		{
			if (!wcscmp(menuType, L"MF_STRING"))
			{
				menuiteminfo.fType = MFT_STRING;
				InsertMenuItem(rootMenu, i, TRUE, &menuiteminfo);
			}
			else if (!wcscmp(menuType, L"MF_MENUBARBREAK"))
			{
				menuiteminfo.fType = MF_MENUBARBREAK;
				InsertMenuItem(rootMenu, i, TRUE, &menuiteminfo);
			}
			else if (!wcscmp(menuType, L"MF_MENUBREAK"))
			{
				menuiteminfo.fType = MF_MENUBARBREAK;
				InsertMenuItem(rootMenu, i, TRUE, &menuiteminfo); 
			}
			else if (!wcscmp(menuType, L"MF_SEPARATOR"))
			{
				menuiteminfo.fType = MF_SEPARATOR;
				InsertMenuItem(rootMenu, i, TRUE, &menuiteminfo);
			}
			else if (!wcscmp(menuType, L"MF_SIDEBAR"))
			{
				//AppendMenu(rootMenu ,MF_STRING, MMU_IDS, caption);
				menuiteminfo.fType = NULL;
				InsertMenuItem(rootMenu, i, TRUE, &menuiteminfo);
			}
		}
		//if (caption) SysFreeString(caption);
		if (menuType) SysFreeString(menuType);
		MMU_IDS++;
	}

cleanup:
	//SAFE_RELEASE(resultNode);
	if (nodeList)	nodeList->lpVtbl->Release(nodeList);
	return 0;
}

void UpdateAppearance(HWND hWnd, HDC dibitmapDC)
{

	DWORD exStyle = GetWindowLong(hWnd, GWL_EXSTYLE);
	if ((exStyle & WS_EX_LAYERED) == 0)
	{
		SetWindowLong(hWnd, GWL_EXSTYLE, exStyle | WS_EX_LAYERED);
	}	
	HDC dcScreen, dcMemory;
	dcScreen = GetDC(NULL);
	dcMemory = CreateCompatibleDC(dcScreen);

	BITMAP bmp;
	HBITMAP hSkinBmp = (HBITMAP)LoadImage(0, L"splash.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	SelectObject(dcMemory, hSkinBmp);
	GetObject(hSkinBmp, sizeof(bmp), &bmp);

	RECT	rt;
	POINT	ptSrc = {0, 0};
	SIZE	size;
	GetWindowRect(hWnd, &rt);
	ptSrc.x = rt.left;
	ptSrc.y = rt.top;

	size.cx = rt.right - rt.left;
	size.cy = rt.bottom - rt.top;

	RECT rcWindow;
        GetWindowRect(hWnd, &rcWindow);

        POINT windowTopLeft;
        windowTopLeft.x = rcWindow.left;
        windowTopLeft.y = rcWindow.top;

        SIZE windowSize;
        windowSize.cx = rcWindow.right - rcWindow.left;
        windowSize.cy = rcWindow.bottom - rcWindow.top;

        POINT surfaceTopLeft;
        surfaceTopLeft.x = surfaceTopLeft.y = 0;

        BLENDFUNCTION bf;
        bf.BlendOp = AC_SRC_OVER;
        bf.BlendFlags = 0;
        bf.SourceConstantAlpha = 128;
        bf.AlphaFormat = AC_SRC_OVER;

	UpdateLayeredWindow(hWnd, dcScreen, NULL, &size, dcMemory, &ptSrc, 0, &bf, ULW_ALPHA);
        //const WinLayeredWindowAPI* api = WinCoveredCalcApp::GetInstance()->GetLayeredWindowAPI();
        //UpdateLayeredWindow(hWnd, NULL, &windowTopLeft, &windowSize, dibitmapDC, &surfaceTopLeft, 0, &bf, ULW_ALPHA);
	DeleteDC(dcMemory);
}
//=============================================================================
//
//  SetWindowTransparentMode()
//
void SetWindowTransparentMode(HWND hwnd,BOOL bTransparentMode)
{
  FARPROC fp;
  int  iAlphaPercent;
  BYTE bAlpha;

  if (bTransparentMode) {
    if (fp = GetProcAddress(GetModuleHandle(L"User32"), "SetLayeredWindowAttributes")) {
      SetWindowLong(hwnd, GWL_EXSTYLE,
        GetWindowLong(hwnd, GWL_EXSTYLE) | /*WS_EX_LAYERED*/0x00080000);

      // get opacity level from registry
      iAlphaPercent = 128;
      if (iAlphaPercent < 0 || iAlphaPercent > 100)
        iAlphaPercent = 75;
      bAlpha = iAlphaPercent * 255 / 100;

      fp(hwnd,0,bAlpha,/*LWA_ALPHA*/0x00000002);
    }
  }

  else
    SetWindowLong(hwnd,GWL_EXSTYLE,
      GetWindowLong(hwnd,GWL_EXSTYLE) & ~/*WS_EX_LAYERED*/0x00080000);
}


void AlphaDraw(HDC hDC, int x, int y, int width, int height, HBITMAP hBmp)
{
    HDC     hMemDC = CreateCompatibleDC(hDC);
    HGDIOBJ hOld   = SelectObject(hMemDC, hBmp);

   	
    HBRUSH hBrush = CreateSolidBrush(RGB(0xFF, 0xFF, 0));
    SelectObject(hDC, hBrush);
    Ellipse(hDC, x, y, width*2, height*2);                  // a yellow circle in the background 
    SelectObject(hDC, GetStockObject(WHITE_BRUSH));
    DeleteObject(hBrush);

    BitBlt(hDC, x, y, width, height, hMemDC, 0, 0, SRCCOPY);  // display the bitmap

    BLENDFUNCTION pixelblend = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };

    AlphaBlend(hDC, x, y+height, width, height, hMemDC, 0, 0, width, height, pixelblend); // blend with per-pixel alpha

    BLENDFUNCTION blend50 = { AC_SRC_OVER, 0, 128, 0 };

    AlphaBlend(hDC, x+width, y, width, height, hMemDC, 0, 0, width, height, blend50); // 50% blending

    SelectObject(hMemDC, hOld);
    DeleteObject(hMemDC);
}

// End of file main_menu.c
