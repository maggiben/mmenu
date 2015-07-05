///////////////////////////////////////////////////////////////////////////////////
// Título:      		main_menu.c                                              //
// Author:		        Benjamin Maggi                                           //
// Descripcion: 		A customizable Menu                                      //
// Org. Date:           17/01/2005                                               //
// Last Modification:   02/04/2008                                               //
// Ver:                 0.9.2                                                    //
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
///////////////////////////////////////////////////////////////////////////////////

#define VERSION 0.9.3		// 06/04/2008

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
//#import "msxml.dll" raw_interfaces_only, no_implementation, named_guids,raw_native_types

#include "IDragDrop.h"

#include "disphelper.h"     // interface with COM MSXML 
#include "main_menures.h"   // Resources
#include "manage_menu.h"	// Menu Manager
#include "graphic.h"		// Graphic Functions draw masks, bitmaps etc..
#include "shellHelper.h"	// Shell & Connmon Dlgs helpers
#include "xmleng.h"			// XML Helper Functions
#include "main_menu.h"      // Proto

#include "FreeImage.h"      // FreeImage protos & delares


///////////////////////////////////////////////////////////////////////////////////
// For HEX to DEC func                                                           //
///////////////////////////////////////////////////////////////////////////////////
#define xtod(c) ((c>='0' && c<='9') ? c-'0' : ((c>='A' && c<='F') ? c-'A'+10 : ((c>='a' && c<='f') ? c-'a'+10 : 0)))
int HextoDec(char *hex);
int xstrtoi(char *hex) ;
///////////////////////////////////////////////////////////////////////////////////
// #defines aqui                                                                 //
///////////////////////////////////////////////////////////////////////////////////
#define HR_TRY(func) if (FAILED(func)) { goto cleanup; }

///////////////////////////////////////////////////////////////////////////////////
// C99 Preprocessor directives                                                   //
///////////////////////////////////////////////////////////////////////////////////
#pragma warn(disable: 2115 2114)


LPMENU_DATA	primary_menu_data;
LPMENU_DATA	first_menu_data;

///////////////////////////////////////////////////////////////////////////////////
//	Global variables                                                             //
///////////////////////////////////////////////////////////////////////////////////

HINSTANCE	hInst;			// Instance handle
HWND		hwndMain;			// Main window handle
HWND		main_window;		// Another Main window Handle
HWND		menu_button;
int			menu_objects;
UINT		menu_msg;
HWND		opti_button;
HWND		plus_button;
HMENU		main_menu;
HMENU		test_menu = NULL;
WNDPROC		buttons_proc[10];


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
#define CS_DROPSHADOW	0x20000	// This window style can make us drop a shadow under our frame
#define MF_SIDEBAR		0x0FFFF1F1L
#define SIDEBAR_WIDTH	30
int  MAX_CLASS_NAME =	256;

int	g_BmpWidth;
int	g_BmpHeight;


///////////////////////////////////////////////////////////////////////////////////
//	Menu Globals & funcs                                                         //
///////////////////////////////////////////////////////////////////////////////////
static 	MENU_DATA 	get_menu_formWID				(HMENU,UINT);
static 	LPMENU_DATA redim_items						(LPMENU_DATA,int);
static  LPMENU_DATA delete_item                     (LPMENU_DATA);
HMENU 				menu_builder					(HWND);
int 				draw_buttons					(HWND,UINT,WPARAM,LPARAM);
int 				draw_menu						(HWND,UINT,WPARAM,LPARAM);
BOOL				drawChildMenuItem				(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL				drawRootMenuItem				(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
///////////////////////////////////////////////////////////////////////////////////
//	2006 get image types inside HBITMAP                                          //
///////////////////////////////////////////////////////////////////////////////////
HBITMAP				imageOptions					(LPMENU_DATA menu_data_ptr,LPSTR image_path,COLORREF bgColor);

static HMENU        insert_menuItem					(LPMENU_DATA menu_data_ptr,HMENU hMenu,UINT uPosition,UINT uFlags,UINT_PTR uIDNewItem,LPCSTR lpNewItem );
static BOOL			append_menuItem					(LPMENU_DATA menu_data_ptr,HMENU hMenu,UINT uFlags,UINT uIDNewItem,LPCTSTR lpNewItem);
//static BOOL			delete_menuItem

static HMENU 		FindMenuFromID					(HMENU,UINT);
int 				menuType_fromID					(HMENU,UINT);
UINT 				ISmenu_root						(UINT);

HCURSOR				CreateAlphaCursor				(void);
///////////////////////////////////////////////////////////////////////////////////
// Create a bitmap mask by masking the selected color                            //
///////////////////////////////////////////////////////////////////////////////////

static HFONT 		CreateAngledFont				(int,int,LPCTSTR,LOGFONT*);


///////////////////////////////////////////////////////////////////////////////////
//	XML Loader & Parser Uses COM MSXML                                           //
///////////////////////////////////////////////////////////////////////////////////
static int 			LoadXML							(char *,int);
static int 			parse_XML						(IDispatch*,HMENU);
static HRESULT 		CreateAndAppendNode				(IDispatch * xmlDoc, IDispatch * xmlParent,LPCWSTR szNodeName, LPCWSTR szNodeText);
// New XML without using disphelper
BOOL				xmlInitnstance					(char * fname, BOOL bOptimizeMemory);
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
HHOOK	hoo_handle;
WNDPROC old_menu_proc;
HANDLE  mnu_hwnd;
HWND    damm_handle;
const TCHAR _WndPropName_OldProc[]	= "XPWndProp_OldProc";
const TCHAR _WndPropName_MenuXP[]	= "XPWndProp_MenuXP";
///////////////////////////////////////////////////////////////////////////////////
// VARIABLES FOR DEBUGGING ONLY !!!                                              //
///////////////////////////////////////////////////////////////////////////////////


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
	wc.lpszClassName	= "main_menu_WndClass";
	wc.lpszMenuName		= NULL; //MAKEINTRESOURCE (IDMAINMENU);
	wc.hCursor			= LoadCursor(NULL,IDC_ARROW);
	wc.hIcon			= LoadIcon(NULL,MAKEINTRESOURCE(8001));
    wc.hIcon			= LoadImage(hInst, MAKEINTRESOURCE(8001), IMAGE_ICON, 32, 32, LR_CREATEDIBSECTION);
    //wc.hIconSm  = LoadImage(hInst, MAKEINTRESOURCE(8001), IMAGE_ICON, 16, 16, LR_CREATEDIBSECTION);

	if (!RegisterClass(&wc))
		return 0;

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
	return CreateWindowEx
	(
		WS_EX_LEFT|WS_EX_TOOLWINDOW,	// WS_EX_TOOLWINDOW prevent our icon to be in ALT+TAB
		"main_menu_WndClass",			// Class
		"Título",						// Title
		WS_POPUP,						// style
		100,							// pos x of win
		100,							// pos y of win,
		115,							// Width
		40,         					// Height
       	NULL,       
		NULL,
		hInst,     						// instance
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

	char * texto = malloc(256);

	switch(id) 
	{
		case IDM_EXIT:
			PostMessage(hwnd,WM_CLOSE,0,0);
		break;

		case 330:
		{
			hMenu_ret = menu_builder(hwnd);
			ClientToScreen(menu_button, &point);
			GetWindowRect(hwnd,&rc_1);
			//HMENU menu = LoadMenu(hInst,MAKEINTRESOURCE(IDMAINMENU));
			
			//  set_newTheme(hWnd,TRUE);
			//HMENU hmenuPopup = (HMENU) wParam;          // manipulador de submenú
			//UINT uPos = (UINT) LOWORD(lParam);         // posición del ítem del submenú
			//BOOL fSystemMenu = (BOOL) HIWORD(lParam);  // bandera de menú de ventana
			/*
			MENUINFO mnfo;
			mnfo.cbSize = sizeof(mnfo);
			mnfo.fMask = MIM_STYLE;
			mnfo.dwStyle = MNS_DRAGDROP | MNS_AUTODISMISS;

			SetMenuInfo(hMenu_ret,&mnfo);
			*/

			BOOL result = TrackPopupMenu(hMenu_ret,TPM_LEFTALIGN, rc_1.left, (rc_1.top + 42 ), 0, main_window, 0);
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
			HICON m_hIcon;
			GetModuleFileName(GetModuleHandle(NULL), m_pszCurrentFile, MAX_PATH);
			//SelectIcon(hwnd, m_pszCurrentFile, MAX_PATH, &m_dwCurrentIndex);

			//unitHook();
			ClientToScreen(menu_button, &point);
			GetWindowRect(hwnd,&rc_1);
			//BOOL result = TrackPopupMenu(test_menu,TPM_LEFTALIGN, rc_1.left, (rc_1.top + 42 ), 0, main_window, 0);

			if(!test_menu)
			{
				//xmlInitnstance("menu.xml",FALSE);
				manage_menu(hInst,hwnd);
			}
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
		ShellExecute(0, "open", menu_data.menu_exec, "", "", SW_SHOW);
	}
}

///////////////////////////////////////////////////////////////////////////////////
// Main window procedure                                                         //
///////////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK MainWndProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	static HANDLE 	hRes;
	HDC 			hdc;
	HDC     		mDC;
	RECT 			rd;
	BOOL   def_result = 0;
	static BYTE gbAlpha = 25;

	switch (msg) 
	{
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

		HANDLE_WM_COMMAND(hwnd,wParam,lParam,MainWndProc_OnCommand);
		break;

	case WM_CREATE:
	{	
		LPCREATESTRUCT lpCreate = (LPCREATESTRUCT)lParam;
		///////////////////////////////////////////////////////////////////////////////////
		// Load resources                                                                //
		///////////////////////////////////////////////////////////////////////////////////
		hRes		= LoadResource(hInst,FindResource(hInst, "#12346", RT_BITMAP));
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
			RegisterDragDrop(hwnd, pDropTarget);
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
		hbrush  = CreateSolidBrush (crColor);
   		hdc 	= BeginPaint(hwnd, &pPaint);

		SetStretchBltMode(hdc, 4);
		rd	= MapDialogControlRect(hwnd, 0, FALSE);
		mDC	=  CreateCompatibleDC(hdc);
		SelectObject(mDC, MainBitmap);

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
		HMENU hmenuInit = (HMENU) wParam;  // manipulador de menú a inicializar
		break;		
	}
	case WM_INITMENUPOPUP:
	{
		HMENU hmenuPopup = (HMENU) wParam;          // manipulador de submenú
		UINT uPos = (UINT) LOWORD(lParam);         // posición del ítem del submenú
		BOOL fSystemMenu = (BOOL) HIWORD(lParam);  // bandera de menú de ventana

		MENUINFO mnfo;
		mnfo.cbSize = sizeof(mnfo);
		mnfo.fMask = MIM_STYLE;
		mnfo.dwStyle = MNS_DRAGDROP | MNS_AUTODISMISS;

		SetMenuInfo(hmenuPopup,&mnfo);

		break;
	}
	case WM_MENUGETOBJECT:
	{
		//new Feb 2008
		MENUGETOBJECTINFO *menuObject = (MENUGETOBJECTINFO*)lParam;
		HRESULT hr = MNGO_NOINTERFACE;
		HMENU   m_dstMenu;											// destination menu handle
		int     m_dstPos;											// destination menu item

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
			RegisterDragDrop(menuH, pDropTarget);

		startDragDrop(menuH);


		return MND_CONTINUE;
		break;
	}
	case WM_MOVING:
	{
		///////////////////////////////////////////////////////////////////////////
		// make this window magnetic                                             //
		///////////////////////////////////////////////////////////////////////////
		return MagnetWindow(&lParam);
		break;
	}
	case WM_MEASUREITEM:
	{
		///////////////////////////////////////////////////////////////////////////
		//                                                                       //
		// We need to calculate OWNERDRAW items size                             //
		//                                                                       //
		///////////////////////////////////////////////////////////////////////////
		TEXTMETRIC          tm;
		HDC                 hdc;
		SIZE				sz;
		HFONT 				hfnt;
		BOOL				hResult;
		LPMEASUREITEMSTRUCT pmis = (LPMEASUREITEMSTRUCT)lParam;

		MENU_DATA menu_data = get_menu_formWID(main_menu,pmis->itemID);
		
		if (pmis->CtlType != ODT_MENU)
		break;

		///////////////////////////////////////////////////////////////////////////
		//  Seteo medidas w y h del elemento del menu (puede ser independietnte) //
		//  chequear tamano en pixels de la fuente                               //
		///////////////////////////////////////////////////////////////////////////
		if (pmis->CtlType == ODT_MENU)
		{
			hdc = GetDC(hwnd);
			GetTextMetrics(hdc, &tm);
			
			///////////////////////////////////////////////////////////////////////////
			// nota: usar select object para seleccionar el tipo de fuente en el DC  //
			///////////////////////////////////////////////////////////////////////////
			int szText;
			szText = lstrlen(menu_data.menu_capti);
			hResult = GetTextExtentPoint32(hdc, menu_data.menu_capti, szText, &sz);
			if(!hResult)
			{	
				MessageBox(hwnd,"Error GetTextExtentPoint32() in gdi32.dll","Error!", IDOK);
				break;
			}			
			///////////////////////////////////////////////////////////////////////////
			// Look for root menu because we want to drawit differently from child   //
			// Windows will accomodate the menu width according to the biggest       //
			// menu item                                                             //
			///////////////////////////////////////////////////////////////////////////
			UINT is_root = ISmenu_root(pmis->itemID);
			if(is_root == 1)
			{
				pmis->itemHeight  = menu_data.menu_imgSize.bottom + 8;//40;
				pmis->itemWidth   = sz.cx + 80; // Add Icon Size to menu width
			}
			else
			{
				pmis->itemHeight  = menu_data.menu_imgSize.bottom + 8;
				pmis->itemWidth   = sz.cx + 30;			
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
			draw_buttons(hwnd, msg, wParam, lParam);
			break;
		case ODT_MENU:
			draw_menu(hwnd, msg, wParam, lParam);
			break;
		}
		//end switch
		break;
	}
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		///////////////////////////////////////////////////////////////////////////////////
		// windows can handle this any way (i think) but never shure !!!                 //
		///////////////////////////////////////////////////////////////////////////////////
		unitHook();
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
	initHook();

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
		if (!TranslateAccelerator(msg.hwnd,hAccelTable,&msg))
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
	 POINT     	menu_bt;
	 POINT      clos_bt;
	 POINT      opti_bt;

	///////////////////////////////////////////////////////////////////////////////////
	// 330 simbolo inicial comando boton                                             //
	///////////////////////////////////////////////////////////////////////////////////
	menu_button = CreateWindow("button", NULL, WS_CHILD | WS_VISIBLE | BS_PUSHLIKE | BS_OWNERDRAW ,
					5,
					5,
				   75,
				   30,
					hwnd_parent, (HMENU)(330 + i), main_inst, NULL);

	i++;
	opti_button = CreateWindow("button", NULL, WS_CHILD | WS_VISIBLE | BS_PUSHLIKE | BS_OWNERDRAW,
				  104,
					2,
					9,
					9,
					hwnd_parent, (HMENU)(330 + i), main_inst, NULL);
	i++;
    plus_button = CreateWindow("button", NULL, WS_CHILD | WS_VISIBLE | BS_PUSHLIKE | BS_OWNERDRAW,
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
HMENU menu_builder(HWND parent)
{
	HMENU	hmenu_1;
	int     h_ref;

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

	dhGetValue(L"%o",&NodeList,xmlDoc, L".documentElement.childNodes");
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
	char		logMsg[256];


	LPMENU_DATA pointer_holder	= &ptr1;
	LPMENU_DATA next_pointer	= &ptr2;
	int count					= 0;

	CopyMemory(next_pointer, first_menu_data, sizeof(MENU_DATA));
	int menuType = menuType_fromID(hMenu, m_wid);

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
	//Popups have a dynamic wID which causes detection more messy                    //
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
///////////////////////////////////////////////////////////////////////////////////
//	basic like redim func                                                        //
//	reason 1 creates a new element                                               //
//	reason 2 gives current ptr                                                   //
//	check cleans & memout                                                        //
///////////////////////////////////////////////////////////////////////////////////
static LPMENU_DATA redim_items(LPMENU_DATA data_pointer, int reason)
{
	LPMENU_DATA pointer_holder;
	void * memory;

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
// Purpose:       Append a menu item the the menu                                //
//                                                                               //
// Output:        append a menu item to the menu       .                         //
// Errors:        If the function succeeds, the return value is 1.               //
//                If the function fails, the return value is 0.                  //
// Notes:		  .                                                              //
// Author:        Benjamin Maggi 2005-2006                                       //
///////////////////////////////////////////////////////////////////////////////////
static BOOL append_menuItem(LPMENU_DATA menu_data_ptr,HMENU hMenu,UINT uFlags,UINT uIDNewItem,LPCTSTR lpNewItem)
{
	///////////////////////////////////////////////////////////////////////////////
	// Request menu list header                                                  //
	///////////////////////////////////////////////////////////////////////////////
	LPMENU_DATA menu_data = redim_items(menu_data, 2);
	switch(uFlags)
	{
		case MF_POPUP:
			break;
		case MF_STRING:
		{
			// Request a new menu item in the list
			menu_data = redim_items(menu_data, 1);
			CopyMemory(menu_data,menu_data_ptr, sizeof(MENU_DATA));
			AppendMenu(hMenu,MF_STRING | MF_OWNERDRAW,uIDNewItem, menu_data->menu_capti);
			break;
		}
		case MF_MENUBARBREAK:
			break;
		case MF_SEPARATOR:
			break;
	}
	return TRUE;
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

	LPSTR			caption;			//
	LPSTR			image_path;			//
	LPSTR			font_name;			// Menu item font name
	LPSTR			menuType;			// Either MF_STRING MF_MENUBARBREAK MF_MENUBREAK 
	BOOL			apiResult;			// Generic result for api calls
	LPSTR			exec_path;			//
	LPSTR			hexVal;				//
	short			width,height;		//
	HBITMAP			loaded_bmp;			//
	COLORREF		menu_fg_color;		//
	COLORREF		menu_bg_color;		//
	BOOL        	bolVal;				//
	MENUITEMINFO	mii;				// Menu Item structure
	SIZE			zSize;
	HDC				nullDC;				// null pointer DC 
	int				icoSize;			// Size of the icons 
	char			logMsg[256];
	
	
	static	int MMU_IDS		= 1900;		// Start menu ID's at 1900
	static	int reclusines	= 0;		// Reclusion Starts at 0
			int ilter		= 0;		// Ilterations Start at 0
			int cnt			= 0;
			int depth		= 0;	
	
	
	if(reclusines == 0){ icoSize = 32; } else { icoSize = 16; }

	LPMENU_DATA menu_data = redim_items(menu_data, 2);


	dhGetValue(L"%d", &ilter, NodeList, L".length");

	for(cnt = 0;cnt < ilter; cnt++)
	{
		dhGetValue(L"%o", &NodeItem			,NodeList, L".item(%d)",cnt);
		dhGetValue(L"%s", &caption			,NodeItem, L".selectSingleNode(%S).text", L"CAPTION");
		dhGetValue(L"%s", &image_path		,NodeItem, L".selectSingleNode(%S).text", L"IMAGE");
		dhGetValue(L"%s", &hexVal			,NodeItem, L".selectSingleNode(%S).text", L"COLOR");
		menu_fg_color = xstrtoi(hexVal);
		dhGetValue(L"%s", &hexVal			,NodeItem, L".selectSingleNode(%S).text", L"BGCOLOR");
		menu_bg_color = xstrtoi(hexVal);
		dhGetValue(L"%s", &exec_path		,NodeItem, L".selectSingleNode(%S).text", L"EXECUTE");
		dhGetValue(L"%s", &font_name		,NodeItem, L".selectSingleNode(%S).text", L"FONT");
		dhGetValue(L"%s", &menuType			,NodeItem, L".selectSingleNode(%S).text", L"MENUTYPE");
		///////////////////////////////////////////////////////////////////////////////////
		// search for any for more objects                                               //
		///////////////////////////////////////////////////////////////////////////////////
		dhGetValue(L"%o", &TempNode, NodeItem, L".selectNodes(%S)",L"MENUITEM");
		dhGetValue(L"%d", &depth, TempNode, L".length");

	if(depth > 0)
		{		
			//Create popups
			menu_data = redim_items(menu_data, 1);

			if(image_path[0] == '\0' )
			{
				///////////////////////////////////////////////////////////////////////////////
                //                                      
				///////////////////////////////////////////////////////////////////////////////
				HDC hDCDesktop = GetDC(0);
				loaded_bmp = CreateCompatibleBitmap( hDCDesktop, icoSize,icoSize);
  				ReleaseDC(NULL, hDCDesktop );
  				drawIconToBitmap( loaded_bmp, exec_path, icoSize,RGB(128, 128, 128));
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
				menu_data->menu_capti	= "hola";
				loaded_bmp					= imageOptions(menu_data,image_path,menu_bg_color);
				menu_data->menu_imageOut	= imageOptions(menu_data,image_path,menu_bg_color);
				menu_data->menu_imageOvr	= imageOptions(menu_data,image_path,menu_fg_color);
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
			menu_data->menu_font_name	= font_name;
			menu_data->wID 				= newPop; //(UINT)menu_data->menu_handle; //aca pasa una cagada
			menu_data->menu_type		= MF_POPUP;

			//holder esta aca y el el puntero al menu en el que se esta trabajando
			apiResult = AppendMenu(holder,MF_POPUP | MF_OWNERDRAW ,(unsigned int)newPop, caption);
			GetMenuItemInfo(holder, 1, TRUE, &mii); 

			reclusines = depth;
			
			//wsprintf(logMsg,"Popup depth: \t\t[ %d ] \n Caption: \t\t\t[ %s ] \n real HANDLE: \t\t[ %d ] \n storage HANDLE: \t\t[ %d ] \n Stored ID: \t\t[ %d ]", depth,menu_data->menu_capti,menu_data->menu_handle,menu_data->wID);
			//wsprintf(logMsg,"Caption: \t\t[ %s ] \n Real Hanlde: \t[ %d ] \n My Handle: \t[ %d ] \n My wID: \t\t[ %d ]",menu_data->menu_capti,newPop,menu_data->menu_handle,menu_data->wID);
			//MessageBox(main_window,logMsg,"CreatePopupMenu(XXX)",MB_OK);
			//force reclusion item is a new item container we need to expand her childs
			parse_XML(TempNode,newPop);
		}
		else
		{
			///////////////////////////////////////////////////////////
			// Build an isolated menu item (does not have any child) //
			///////////////////////////////////////////////////////////
			menu_data = redim_items(menu_data, 1);
			//set data for this item
			// If there's no image we get the icon from the executable (shell items also work)
			if(image_path[0] == '\0' )
			{
				HDC hDCDesktop = GetDC(0);
				//loaded_bmp = CreateCompatibleBitmap( hDCDesktop, icoSize,icoSize);
				menu_data->menu_imageOut = CreateCompatibleBitmap( hDCDesktop, icoSize,icoSize);
				menu_data->menu_imageOvr = CreateCompatibleBitmap( hDCDesktop, icoSize,icoSize);

  				ReleaseDC(NULL, hDCDesktop );

				drawIconToBitmap( menu_data->menu_imageOut, exec_path, icoSize,menu_bg_color);
				drawIconToBitmap( menu_data->menu_imageOvr, exec_path, icoSize,menu_fg_color);
  				menu_data->menu_imgSize.left = 0;
				menu_data->menu_imgSize.top = 0;
				menu_data->menu_imgSize.bottom = icoSize;
				menu_data->menu_imgSize.right = icoSize;
			}
			else
			{
				loaded_bmp = imageOptions(menu_data,image_path,menu_fg_color);
				
				menu_data->menu_imageOut	= imageOptions(menu_data,image_path,menu_bg_color);
				menu_data->menu_imageOvr	= imageOptions(menu_data,image_path,menu_fg_color);
				
				//loaded_bmp = LoadImage(hInst,image_path,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
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
			menu_data->menu_font_name	= font_name;
			menu_data->wID				= (HMENU)MMU_IDS;
			menu_data->xmlMenuItem		= (IXMLDOMElement*) NodeItem;
			//populateAttributes((IXMLDOMElement*)NodeItem);//menu_data->xmlMenuItem);
			
			BSTR nodeName;
			/*
			hr = document->lpVtbl->get_documentElement(document,&element);
			if (FAILED(hr) || element == NULL)
			{
				MessageBox(NULL,"Empty document!", "Error Loading XML", MB_ICONWARNING);
	   	     goto clean;
			}
			*/
			//menu_data->xmlMenuItem->lpVtbl->get_nodeName(menu_data->xmlMenuItem,&nodeName);
			//MessageBoxW(NULL,nodeName,L"Error",MB_ICONINFORMATION);

			menu_data->menu_width = 0;
			menu_data->menu_width = max(menu_data->menu_width,zSize.cx);
			
			//SelectObject(nullDC, oldFt);
			//DeleteDC(nullDC);

			if (!strcmp(menuType, "MF_STRING"))
			{
				if(strlen(caption) >= 0)
				{
					menu_data->menu_capti = caption;
					menu_data->menu_type = MF_STRING;
					AppendMenu(holder,MF_STRING | MF_OWNERDRAW,MMU_IDS, caption );
				}
			}
			else if (!strcmp(menuType, "MF_MENUBARBREAK"))
			{
				menu_data->menu_capti	= caption;
				menu_data->menu_type = MF_MENUBARBREAK;
				apiResult = AppendMenu(holder,MF_MENUBARBREAK|MF_OWNERDRAW,MMU_IDS,caption);
			}
			else if (!strcmp(menuType, "MF_MENUBREAK"))
			{
				menu_data->menu_capti	= caption;
				menu_data->menu_type = MF_MENUBREAK;
				apiResult = AppendMenu(holder,MF_MENUBREAK|MF_OWNERDRAW,MMU_IDS,caption);
			}
			else if (!strcmp(menuType, "MF_SEPARATOR"))
			{
				menu_data->menu_capti	= caption;
				menu_data->menu_type = MF_SEPARATOR;
				apiResult = AppendMenu(holder,MF_SEPARATOR|MF_OWNERDRAW,MMU_IDS,NULL);
			}
			else if (!strcmp(menuType, "MF_SIDEBAR"))
			{
				menu_data->menu_capti	= caption;
				menu_data->menu_type = MF_SIDEBAR;
				apiResult = AppendMenu(holder,MF_OWNERDRAW,MMU_IDS,NULL);
			}
			/*
			if(strlen(caption) <= 1)
			{
				switch(caption[0])
				{
					case 'B':
					{
						//menu_data->menu_capti	= "B";
						menu_data->menu_capti	= caption;
						menu_data->menu_type	= MF_MENUBARBREAK;
						//apiResult = AppendMenu(holder,MF_MENUBARBREAK | MF_OWNERDRAW,MMU_IDS, NULL );
						break;
					}
					case 'R':
					{
						break;
					}
					case '-':
					{
						menu_data->menu_capti	= caption;
						menu_data->menu_type	= MF_SEPARATOR;
						//apiResult = AppendMenu(holder,MF_SEPARATOR | MF_OWNERDRAW,MMU_IDS, NULL );
				
						break;
					}
					case '\0':
					{
						break;
					}
				}
			}
			else
			{	
				//Normal case
				//menu_data->menu_capti	= caption;
				//menu_data->menu_type	= MF_STRING;
				//AppendMenu(holder,MF_STRING | MF_OWNERDRAW,MMU_IDS, caption );
			}
			*/
		//}//end if
	
		//AppendMenu(holder,MF_STRING,MMU_IDS, Menu_item.menu_capti );
		//menu_data->wID =  GetMenuItemID(holder, cnt);

		MMU_IDS++;

		char MSG[1024];
		wsprintf(MSG,"Menu Id(%d) Caption(%s)",MMU_IDS,caption);
		//MessageBox(NULL,MSG,"Alert!",MB_OK);
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
	//return CallNextHookEx (hoo_handle, code, wParam, lParam);

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
		char sClassName[256];
		int Count = GetClassName (hWnd, sClassName, MAX_CLASS_NAME);
		// Check for the menu-class
		if ( Count != 6 || strcmp (sClassName, "#32768") != 0 )
		{
			acthWnd = hWnd;
			break;
		}
		else
		{
			acthMenu = hWnd;
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
	return CallNextHookEx (hoo_handle, code, wParam, lParam);
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
		case WM_PRINT:
		{
			break;
		}
		case WM_WINDOWPOSCHANGING:
		{
			WINDOWPOS* lpwinpos = (LPWINDOWPOS)lParam;
			MENU_DATA  menu_data;

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
		case WM_NCPAINT:
		{
			//return 0;
			HDC hDC;
			RECT rcTwo;
			RECT rcSep;
			RECT rcOrg;
			int bdr_w = SIDEBAR_WIDTH + 4;   //main menu sidebar w 4 is the border width missing
			int bdr_h = 150;

			//static int main_wnd = 0;

			hDC = GetWindowDC (hWnd);
			GetWindowRect(hWnd,&rcTwo);

			//SetWindowPos(hWnd,hWnd,10,10,10,10,0);

			GetClipBox(hDC,&rcTwo);
			rcOrg = rcTwo;

			//return 0;
			//LPDRAWITEMSTRUCT	lpDs = (LPDRAWITEMSTRUCT)lParam;

			/////////////////////////////////////////////////////////////////
			// Draw Menu border                                            //
			/////////////////////////////////////////////////////////////////
			rcSep.top 		= rcTwo.top 	+2;		// Fixed calculation of 3d border
			rcSep.left		= rcTwo.left	-2;
			rcSep.bottom	= rcTwo.bottom  -2;
			rcSep.right		= rcTwo.right   +2;

			HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0)); // Menu Border color
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

			hBrush = CreateSolidBrush(RGB(195,195, 195));	// Menu Color
			FrameRect(hDC, &rcTwo, hBrush);					// it will draw a 1px border only

			/////////////////////////////////////////////////////////////////
			// Sice the 3D border is 3px make 2 rects                      //
			/////////////////////////////////////////////////////////////////
			/*
			rcTwo.top		= rcTwo.top 	+1;
			rcTwo.left		= rcTwo.left	+1;
			rcTwo.bottom	= rcTwo.bottom	-1;
			rcTwo.right		= rcTwo.right	-1;
			*/
			InflateRect(&rcTwo,-1,-1);
			FrameRect(hDC, &rcTwo, hBrush);
			DeleteObject(hBrush);

			/////////////////////////////////////////////////////////////////
			// Fill the sidebar with bar                                   //
			// Try not to paint over any live item or it will cause flickr //
			/////////////////////////////////////////////////////////////////
			hBrush = CreateSolidBrush(RGB(128,128, 128));	// Sidebar Color
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
	
			hBrush = CreateSolidBrush(RGB(0, 0, 0));	// Sidebar Border Color
			FrameRect(hDC, &rcTwo, hBrush);				// it will draw a 1px border only 
			DeleteObject(hBrush);
	
			// Restore & DC's and objects state
			DeleteDC(hDC);
			//HPEN saveObject = SelectObject(hDC,CreatePen(PS_SOLID, 1, RGB(0, 0, 0)));
 	  		//DeleteObject(SelectObject(hDC, saveObject));
			return 0;
		}
		case WM_SHOWWINDOW:
		{
			BOOL bShow = wParam != 0;
			if (!bShow)
			{
				SetWindowLong  (hWnd, GWL_WNDPROC, (DWORD)(DWORD_PTR)oldWndProc);
				RemoveProp (hWnd, _WndPropName_OldProc);
			}
			break;
		}
		case WM_DESTROY:
		{
			SetWindowLong  (hWnd, GWL_WNDPROC, (DWORD)(DWORD_PTR)oldWndProc);
			RemoveProp (hWnd, _WndPropName_OldProc);
			break;
		}
		case WM_CLOSE:
		{
			SetWindowLong  (hWnd, GWL_WNDPROC, (DWORD)(DWORD_PTR)oldWndProc);
			RemoveProp (hWnd, _WndPropName_OldProc);
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
	hoo_handle = (HHOOK) SetWindowsHookEx(WH_CALLWNDPROC,&install_Hook,hInst,GetCurrentThreadId());
}
///////////////////////////////////////////////////////////////////////////////////
void unitHook(void)
{
	WNDPROC NewWndProc = (WNDPROC)(LONG_PTR)GetWindowLong (mnu_hwnd, GWL_WNDPROC);
	SetWindowLong (mnu_hwnd, GWL_WNDPROC,(DWORD)(DWORD_PTR)old_menu_proc);
	NewWndProc = (WNDPROC)(LONG_PTR)GetWindowLong (mnu_hwnd, GWL_WNDPROC);
	RemoveProp (mnu_hwnd, _WndPropName_OldProc);
	UnhookWindowsHookEx(hoo_handle);
}
///////////////////////////////////////////////////////////////////////////////////






///////////////////////////////////////////////////////////////////////////////////
int draw_menu(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
		RECT		rcSep;
		BITMAP		bmp;
		HBRUSH		hBrush;
		COLORREF	gradient;	// Used to store the gradient value
		HPEN		colorPen;	

		//medidas
		int border_w	= SIDEBAR_WIDTH;
		//int text_off	= 78;
		//int inner_sq    = 4;

		int point_x		= 0;
		int point_y 	= 0;

		LPDRAWITEMSTRUCT lpDs = (LPDRAWITEMSTRUCT)lParam;
		MENUITEMINFO mii;
		MENU_DATA    rawi_data;
		LPMENU_DATA  menu_data;

		//copy before going reclusives search
		DRAWITEMSTRUCT lpCopy;
		CopyMemory(&lpCopy, lpDs, sizeof(DRAWITEMSTRUCT));

		rawi_data = get_menu_formWID((HMENU)lpDs->hwndItem,lpDs->itemID);


		menu_data = &rawi_data;

		lpDs = &lpCopy;
		
		if(menu_data->menu_type == MF_SIDEBAR)
		{
			RECT rct;
			GetClipBox(lpDs->hDC,&rct);
			
			HBRUSH hBrush = CreateSolidBrush(RGB(128, 128, 128));	//sidebar color
			FillRect(lpDs->hDC, &rct, hBrush);
			DeleteObject(hBrush);

			///////////////////////////////////////////////////////
			//	Put logo                                         //
			///////////////////////////////////////////////////////
			HBITMAP numBMP = LoadImage(hInst, MAKEINTRESOURCE(ID_LOGO), IMAGE_BITMAP,0, 0, LR_LOADTRANSPARENT);
			GetObject(numBMP, sizeof(BITMAP), &bmp);
			// if need to know bmp size use this
			int BmpWidth  = bmp.bmWidth;
			int BmpHeight = bmp.bmHeight;

			// get the drawing handle for the objects
			HDC hDC = CreateCompatibleDC(lpDs->hDC);
			SelectObject(hDC, numBMP);
			BitBlt(lpDs->hDC, 0, 0, 25, 25, hDC,0,0, SRCCOPY);
			DeleteDC(hDC);

			///////////////////////////////////////////////////////
			//	Side bar text & logo                             //
			///////////////////////////////////////////////////////
			int PointSize = 6;
			int DPI = 72;
			int lfHeight = -MulDiv(PointSize, GetDeviceCaps(lpDs->hDC, LOGPIXELSY), DPI);

			HFONT hfnt = CreateAngledFont(lfHeight, 90, "PF Tempesta Five",NULL);
			LPCTSTR lpszLable = "MENU HELPER";
			SetBkMode(lpDs->hDC, TRANSPARENT);
			COLORREF prevColor = SetTextColor(lpDs->hDC, RGB(90,90,90)); //font shadow
			HFONT hfntPrev = SelectObject(lpDs->hDC, hfnt);

			TextOut(lpDs->hDC, rct.left + ((SIDEBAR_WIDTH - 15) / 2), rct.bottom - 10, lpszLable, lstrlen(lpszLable));		//Restore defaults
			SelectObject(lpDs->hDC, hfntPrev);
			SetTextColor(lpDs->hDC, prevColor);
			
			prevColor = SetTextColor(lpDs->hDC, RGB(210,210,210)); //font color
			hfntPrev = SelectObject(lpDs->hDC, hfnt);

			TextOut(lpDs->hDC, rct.left + ((SIDEBAR_WIDTH - 14) / 2), rct.bottom - 9, lpszLable, lstrlen(lpszLable));		//Restore defaults
			SelectObject(lpDs->hDC, hfntPrev);
			SetTextColor(lpDs->hDC, prevColor);



			SetBkMode(lpDs->hDC, OPAQUE);

			//return 0;
		}
		if(lpDs->itemAction & ODA_DRAWENTIRE)
		{
			//Aparentemente con esto se soluciona la acomodando el pequeño
			//espacio que queda sin dibujar cuando drag and drop
			RECT rct;
			GetClipBox(lpDs->hDC,&rct);

			HBRUSH hBrush = CreateSolidBrush(RGB(195, 195, 195));
			//FillRect(lpDs->hDC, &rct, hBrush); //&lpDs->rcItem
			DeleteObject(hBrush);
			//MessageBeep(MB_ICONEXCLAMATION);
		}
		if(lpDs->itemState & ODS_SELECTED)
		{
			if (lpDs->hwndItem != (HWND)main_menu)
			{
				drawChildMenuItem(hWnd, uMsg, wParam, lParam);
				return 0;
			}
			if (lpDs->hwndItem == (HWND)main_menu)
			{
				drawRootMenuItem(hWnd, uMsg, wParam, lParam);
				//DrawAlphaBlend (hWnd, lpDs->hDC);
				return 0;
			}
			///////////////////////////////////////////////////////
			//	barra al costado                                 //
			///////////////////////////////////////////////////////
			rcSep.top 		= lpDs->rcItem.top    ;
			rcSep.left		= lpDs->rcItem.left + 3;
			rcSep.bottom	= lpDs->rcItem.bottom ;
			rcSep.right		= lpDs->rcItem.right  ;

			hBrush = CreateSolidBrush(menu_data->menu_color);
			FillRect(lpDs->hDC, &rcSep, hBrush);
			DeleteObject(hBrush);

			
			/////////////////////////////////////////////////////////////////
			//	Fill the Selected Menu Item with gradient color            //
			/////////////////////////////////////////////////////////////////

			gradientFill(lpDs->hDC,rcSep,RGB(255,0,0),RGB(0,255,0));
			/////////////////////////////////////////////////////////////////
			//	perimetro mas obscuro compartido por todos los menusitems  //
			/////////////////////////////////////////////////////////////////
			rcSep.top 		= lpDs->rcItem.top;
			rcSep.left		= lpDs->rcItem.left + 2; // Small separation from the sidebar
			rcSep.bottom	= lpDs->rcItem.bottom;
			rcSep.right		= lpDs->rcItem.right;

			hBrush = CreateSolidBrush(RGB(96, 96, 96));
			FrameRect(lpDs->hDC, &rcSep, hBrush);
			DeleteObject(hBrush);

			///////////////////////////////////////////////////////
			//	barra al costado box extra                       //
			///////////////////////////////////////////////////////
			if(lpDs->hwndItem != (HWND)main_menu)
			{
				rcSep.top 		= lpDs->rcItem.top;
				rcSep.left		= lpDs->rcItem.left;
				rcSep.bottom	= lpDs->rcItem.bottom;
				rcSep.right		= SIDEBAR_WIDTH;

				hBrush = CreateSolidBrush(RGB(128, 255, 128));
				FillRect(lpDs->hDC, &rcSep, hBrush);
				DeleteObject(hBrush);
			}

			/////////////////////////////////////////////////////////////////////////////////////
			//	draws a line betwen the sidebar & the menuitem  all menuitems share this color //
			/////////////////////////////////////////////////////////////////////////////////////
			if(lpDs->hwndItem == (HWND)main_menu)
			{
			int y_t = lpDs->rcItem.top;
			int x_t = border_w;
			HPEN saveObject = SelectObject(lpDs->hDC,CreatePen(PS_SOLID, 1, RGB(0, 0, 0)));
			MoveToEx(lpDs->hDC, x_t, y_t, NULL);
			LineTo  (lpDs->hDC, x_t, y_t);
			LineTo  (lpDs->hDC, x_t ,lpDs->rcItem.bottom);
			DeleteObject(SelectObject(lpDs->hDC, saveObject));
			}
			///////////////////////////////////////////////////////
			//	Icon Holder & mini sidedar                       //
			///////////////////////////////////////////////////////
			if(lpDs->hwndItem == (HWND)main_menu)
			{
				rcSep.top 		= lpDs->rcItem.top    +2;
				rcSep.left		= lpDs->rcItem.left   + border_w + 8;
				rcSep.bottom	= lpDs->rcItem.bottom -2;
				rcSep.right		= rcSep.left + 36 ;

			
				//DrawEdge(lpDs->hDC, &rcSep, BDR_SUNKENOUTER, BF_RECT);
				//InflateRect(&rcSep, -1,-1);
				hBrush = CreateSolidBrush(RGB(0, 0, 0));
				//FillRect(lpDs->hDC, &rcSep, hBrush);
				DeleteObject(hBrush);

		
				BITMAP bm;
				GetObject(menu_data->menu_image, sizeof(BITMAP), (LPSTR)&bm);
 				point_y = ((lpDs->rcItem.bottom - lpDs->rcItem.top) / 2) - (bm.bmHeight/2);
				point_y += lpDs->rcItem.top;
				//point_x = (lpDs->rcItem.left + border_w + 10) + (((lpDs->rcItem.left + bm.bmWidth) / 2 ) - (bm.bmWidth / 2));
				point_x = (lpDs->rcItem.left) + 4;
		
				DrawTransparentBitmap(lpDs->hDC,menu_data->menu_imageOvr,point_x,point_y,RGB(255,170,170));
				
				/*
				///////////////////////////////////////////////////////
				//	Separador obscuro                                //
				///////////////////////////////////////////////////////
				HPEN saveObject = SelectObject(lpDs->hDC,CreatePen(PS_SOLID, 1, RGB(100, 100, 100)));
				point_x = rcSep.right + 4;
				point_y = rcSep.top;
				MoveToEx(lpDs->hDC, point_x, point_y, NULL);
				LineTo  (lpDs->hDC, point_x, point_y);
				LineTo  (lpDs->hDC, point_x, rcSep.bottom);
				DeleteObject(SelectObject(lpDs->hDC, saveObject));
				///////////////////////////////////////////////////////
				// Separador claro                                   //
				///////////////////////////////////////////////////////
				saveObject = SelectObject(lpDs->hDC,CreatePen(PS_SOLID, 1, RGB(230, 230, 230)));
				point_x += 1;
				point_y = rcSep.top;
				MoveToEx(lpDs->hDC, point_x, point_y, NULL);
				LineTo  (lpDs->hDC, point_x, point_y);
				LineTo  (lpDs->hDC, point_x, rcSep.bottom);
				DeleteObject(SelectObject(lpDs->hDC, saveObject));
				*/
			}

			///////////////////////////////////////////////////////
			//	child drawind here                               //
			///////////////////////////////////////////////////////
			else
			{
					rcSep.top 		= lpDs->rcItem.top     +1;
					rcSep.left		= lpDs->rcItem.left    +1;
					rcSep.bottom	= lpDs->rcItem.bottom  -1;
					rcSep.right		= lpDs->rcItem.left   +24;

					// Rect around the icon
					hBrush = CreateSolidBrush(RGB(90, 90, 90));
					//FrameRect(lpDs->hDC, &rcSep, hBrush);
					DeleteObject(hBrush);

					InflateRect(&rcSep, -1,-1);
					hBrush = CreateSolidBrush(menu_data->menu_color);
					FillRect(lpDs->hDC, &rcSep, hBrush);
					DeleteObject(hBrush);

					BITMAP bm;
					GetObject(menu_data->menu_imageOvr, sizeof(BITMAP), (LPSTR)&bm);
 					point_y = ((lpDs->rcItem.bottom - lpDs->rcItem.top) / 2) - (bm.bmHeight/2);
					point_y += lpDs->rcItem.top;
					point_x = lpDs->rcItem.left + 5;

					DrawTransparentBitmap(lpDs->hDC,menu_data->menu_imageOvr,point_x,point_y,RGB(170,170,170));
			}
			///////////////////////////////////////////////////////
			//	texto caption                                    //
			///////////////////////////////////////////////////////
			if(lpDs->hwndItem == (HWND)main_menu)
			{
					SIZE extent;
					char* captionText = (char*)lpDs->itemData;
					GetTextExtentPoint32(lpDs->hDC,captionText, lstrlen(captionText), &extent);
					point_y = ((lpDs->rcItem.bottom - lpDs->rcItem.top) / 2) - (extent.cy/2);
					point_y += lpDs->rcItem.top;
		
					rcSep.top 		= lpDs->rcItem.top;
					rcSep.left		= lpDs->rcItem.left + menu_data->menu_imgSize.right + 12; // Icon + border
					rcSep.bottom	= lpDs->rcItem.bottom;
					rcSep.right		= lpDs->rcItem.right;
					SetBkMode(lpDs->hDC,TRANSPARENT);
					DrawText (lpDs->hDC,captionText, -1, &rcSep,DT_SINGLELINE | DT_LEFT | DT_VCENTER);
			}
			///////////////////////////////////////////////////////
			// Draw Text For child items 
			///////////////////////////////////////////////////////
			else
			{
					char* captionText = (char*)lpDs->itemData;	
					// 
					rcSep.top 		= lpDs->rcItem.top;
					rcSep.left		= lpDs->rcItem.left  + menu_data->menu_imgSize.right + 18; //+ text_off - 45 ;
					rcSep.bottom	= lpDs->rcItem.bottom;
					rcSep.right		= lpDs->rcItem.right;
				
					int PointSize = 8;
					int DPI = 72;
					int lfHeight = -MulDiv(PointSize, GetDeviceCaps(lpDs->hDC, LOGPIXELSY), DPI);

					HFONT hfnt = CreateAngledFont(lfHeight, 0, menu_data->menu_font_name,NULL);
		
					HFONT hfntPrev = SelectObject(lpDs->hDC, hfnt);
	
					SetBkMode(lpDs->hDC,TRANSPARENT);
					DrawText (lpDs->hDC,captionText, -1, &rcSep,DT_SINGLELINE | DT_LEFT | DT_VCENTER);
					SelectObject(lpDs->hDC, hfntPrev);
					DeleteObject(hfnt);

			}
		}
		///////////////////////////////////////////////////////
		//	************************************************ //
		//	State not selected no seleccionado               //
		//	menuitems disabed, separators, and bears go here //
		//	************************************************ //
		///////////////////////////////////////////////////////
		else
		{
			if (lpDs->hwndItem != (HWND)main_menu)
			{
				drawChildMenuItem(hWnd, uMsg, wParam, lParam);
				return 0;
			}
			///////////////////////////////////////////////////////
			//	Fill in the menu window                          //
			///////////////////////////////////////////////////////
			rcSep.top 	= lpDs->rcItem.top    ;
			rcSep.left	= lpDs->rcItem.left;
			rcSep.bottom	= lpDs->rcItem.bottom ;
			rcSep.right	= lpDs->rcItem.right  + 3;


			HBRUSH hBrush = CreateSolidBrush(RGB(195, 195, 195));
			FillRect(lpDs->hDC, &rcSep, hBrush);
			DeleteObject(hBrush);

			///////////////////////////////////////////////////////
			//	barra al costado solo en los childs              //
			///////////////////////////////////////////////////////
			if(lpDs->hwndItem != (HWND)main_menu)
			{
			rcSep.top 		= lpDs->rcItem.top    ;
			rcSep.left		= lpDs->rcItem.left   ;
			rcSep.bottom	= lpDs->rcItem.bottom ;
			rcSep.right		= rcSep.left + border_w  ;

			hBrush = CreateSolidBrush(RGB(128, 128, 128));
			FillRect(lpDs->hDC, &rcSep, hBrush);
			DeleteObject(hBrush);
			}
			///////////////////////////////////////////////////////
			//	draws a line betwen the sidebar & the menuitem   //
			///////////////////////////////////////////////////////
		
			if(lpDs->hwndItem != (HWND)main_menu)
			{
			//border_w = 30;
			}
				int y_t = lpDs->rcItem.top;
				int x_t = border_w;
				HPEN saveObject = SelectObject(lpDs->hDC,CreatePen(PS_SOLID, 1, RGB(0, 0, 0)));
				MoveToEx(lpDs->hDC, x_t, y_t, NULL);
				LineTo  (lpDs->hDC, x_t, y_t);
				LineTo  (lpDs->hDC, x_t ,lpDs->rcItem.bottom);
				DeleteObject(SelectObject(lpDs->hDC, saveObject));
			
			///////////////////////////////////////////////////////
			//	Text & Captions goes here                        //
			///////////////////////////////////////////////////////
			if(lpDs->hwndItem == (HWND)main_menu)
			{
					SIZE extent;
					char* captionText = (char*)lpDs->itemData;
					GetTextExtentPoint32(lpDs->hDC,captionText, lstrlen(captionText), &extent);
					point_y = ((lpDs->rcItem.bottom - lpDs->rcItem.top) / 2) - (extent.cy/2);
					point_y += lpDs->rcItem.top;
		
					rcSep.top 		= lpDs->rcItem.top;
					rcSep.left		= lpDs->rcItem.left + menu_data->menu_imgSize.right + 12; // Icon + border
					rcSep.bottom	= lpDs->rcItem.bottom;
					rcSep.right		= lpDs->rcItem.right;
					SetBkMode(lpDs->hDC,TRANSPARENT);
					DrawText (lpDs->hDC,captionText, -1, &rcSep,DT_SINGLELINE | DT_LEFT | DT_VCENTER);
			}

			////////////////////////////////////////////////////////////////////
			//	Draws the icon holder & the separator For Main Menu           //
			////////////////////////////////////////////////////////////////////
			if(lpDs->hwndItem == (HWND)main_menu && lpDs->itemData > 0)
			{
				rcSep.top 	= lpDs->rcItem.top    +2;
				rcSep.left	= lpDs->rcItem.left   + border_w + 8;
				rcSep.bottom	= lpDs->rcItem.bottom -2;
				rcSep.right	= rcSep.left + 36 ;

			
				//DrawEdge(lpDs->hDC, &rcSep, BDR_SUNKENOUTER, BF_RECT);
				//InflateRect(&rcSep, -1,-1);
				hBrush = CreateSolidBrush(RGB(0, 0, 0));
				//FillRect(lpDs->hDC, &rcSep, hBrush);
				DeleteObject(hBrush);
		
		
				BITMAP bm;
				GetObject(menu_data->menu_imageOut, sizeof(BITMAP), (LPSTR)&bm);
 				point_y = ((lpDs->rcItem.bottom - lpDs->rcItem.top) / 2) - (bm.bmHeight/2);
				point_y += lpDs->rcItem.top;
				point_x = (lpDs->rcItem.left + border_w + 10) + (((lpDs->rcItem.left + bm.bmWidth) / 2 ) - (bm.bmWidth / 2));
				point_x = (lpDs->rcItem.left) + 4;// + (((lpDs->rcItem.left + bm.bmWidth) / 2 ) - (bm.bmWidth / 2));
		
				DrawTransparentBitmap(lpDs->hDC,menu_data->menu_imageOut,point_x,point_y,RGB(170,170,170));

				/*
				///////////////////////////////////////////////////////	
				//	Separador obscuro                                //
				///////////////////////////////////////////////////////
				HPEN saveObject = SelectObject(lpDs->hDC,CreatePen(PS_SOLID, 1, RGB(128, 128, 128)));
				point_x = rcSep.right + 4;
				point_y = rcSep.top;
				MoveToEx(lpDs->hDC, point_x, point_y, NULL);
				LineTo  (lpDs->hDC, point_x, point_y);
				LineTo  (lpDs->hDC, point_x, rcSep.bottom);
				DeleteObject(SelectObject(lpDs->hDC, saveObject));
				///////////////////////////////////////////////////////
				// Separador claro                                   //
				///////////////////////////////////////////////////////
				saveObject = SelectObject(lpDs->hDC,CreatePen(PS_SOLID, 1, RGB(230, 230, 230)));
				point_x += 1;
				MoveToEx(lpDs->hDC, point_x, point_y, NULL);
				LineTo  (lpDs->hDC, point_x, point_y);
				LineTo  (lpDs->hDC, point_x, rcSep.bottom);
				DeleteObject(SelectObject(lpDs->hDC, saveObject));
				*/
			}
		
			///////////////////////////////////////////////////////
			//	Child menus draw here                            //
			///////////////////////////////////////////////////////
			if(lpDs->hwndItem != (HWND)main_menu && lpDs->itemData > 0)
			{

					BITMAP bm;

					rcSep.top 		= lpDs->rcItem.top     +1;
					rcSep.left		= lpDs->rcItem.left    +1;
					rcSep.bottom	= lpDs->rcItem.bottom  -1;
					rcSep.right		= lpDs->rcItem.left   +24;


					GetObject(menu_data->menu_imageOut, sizeof(BITMAP), (LPSTR)&bm);
 					//ptSize.x = bm.bmWidth;            // Get width of bitmap
   					//ptSize.y = bm.bmHeight;           // Get height of bitmap
					point_y = ((lpDs->rcItem.bottom - lpDs->rcItem.top) / 2) - (bm.bmHeight/2);
					point_y += lpDs->rcItem.top;

					//point_x = 15 - (bm.bmWidth/2);
					point_x = lpDs->rcItem.left + 5;
		
					DrawTransparentBitmap(lpDs->hDC,menu_data->menu_imageOut,point_x,point_y,RGB(170,170,170));
					
					///////////////////////////////////////////////////////
					//	MF_SEPARATOR                                     //
					//  Este codigo no es llamado nunca                  //
					///////////////////////////////////////////////////////

					if(lpDs->itemData == 0)
					{
						int y_t = (lpDs->rcItem.top + lpDs->rcItem.bottom - 1) / 2;
						int x_t = lpDs->rcItem.left + 32;
						HPEN saveObject = SelectObject(lpDs->hDC,CreatePen(PS_SOLID, 1, RGB(0, 0, 0))); //130
						MoveToEx(lpDs->hDC, x_t, y_t, NULL);
						LineTo  (lpDs->hDC, x_t, y_t);
						LineTo  (lpDs->hDC, x_t + lpDs->rcItem.right - 5, y_t);
						DeleteObject(SelectObject(lpDs->hDC, saveObject));
						saveObject = SelectObject(lpDs->hDC,CreatePen(PS_SOLID, 1, RGB(255, 0, 0))); //220

						MoveToEx(lpDs->hDC, x_t, y_t+1, NULL);
						LineTo  (lpDs->hDC, x_t, y_t+1);
						LineTo  (lpDs->hDC, x_t + lpDs->rcItem.right - 5, y_t+1);
						DeleteObject(SelectObject(lpDs->hDC, saveObject));
					}
					
					///////////////////////////////////////////////////////
					// finalemente si todo esta ok dibujo el texto si:   //
					// no es un separador y al futuro chequear           //
					// que no este tachado MF_CHEKED                     //
					///////////////////////////////////////////////////////
					else
					{

						if(lpDs->hwndItem == (HWND)main_menu)
						{
							rcSep.top 		= lpDs->rcItem.top    ;
							rcSep.left		= lpDs->rcItem.left + menu_data->menu_imgSize.right + 12; //+ text_off;
							rcSep.bottom	= lpDs->rcItem.bottom ;
							rcSep.right		= lpDs->rcItem.right   ;
							//////////////////////////////////////////////////////////////////////////////////
							// Backgound is transparent                                                     //
							//////////////////////////////////////////////////////////////////////////////////
							SetBkMode(lpDs->hDC,TRANSPARENT);
							char* statusText = (char*)lpDs->itemData;
							DrawText (lpDs->hDC, statusText, -1, &rcSep, DT_SINGLELINE | DT_LEFT | DT_VCENTER);
						}
						//////////////////////////////////////////////////////////////////////////////////
						// Child submenus caption draw                                                  //
						//////////////////////////////////////////////////////////////////////////////////
						else
						{
							char* captionText = (char*)lpDs->itemData;
							rcSep.top 		= lpDs->rcItem.top;
							rcSep.left		= lpDs->rcItem.left + menu_data->menu_imgSize.right + 18; //+ text_off - 45 ;
							rcSep.bottom	= lpDs->rcItem.bottom;
							rcSep.right		= lpDs->rcItem.right;
	
							int PointSize = 8;
							int DPI = 72;
							int lfHeight = -MulDiv(PointSize, GetDeviceCaps(lpDs->hDC, LOGPIXELSY), DPI);

							HFONT hfnt = CreateAngledFont(lfHeight, 0, menu_data->menu_font_name,NULL);
							HFONT hfntPrev = SelectObject(lpDs->hDC, hfnt);

							SetBkMode(lpDs->hDC,TRANSPARENT);
	
							DrawText (lpDs->hDC,captionText, -1, &rcSep,
							DT_SINGLELINE | DT_LEFT | DT_VCENTER);
							SelectObject(lpDs->hDC, hfntPrev);
							DeleteObject(hfnt);
						}


					}
			}
			
		if (lpDs->itemState & ODS_GRAYED )
		{

		}
	
		}
		/*
		if(menu_data->menu_type == MF_MENUBARBREAK) //|| lpDs->itemData == 0
		{
			//MessageBox(main_window,"MF_MENUBARBREAK","Error",MB_OK);
			int middle 	= (lpDs->rcItem.right - lpDs->rcItem.left) / 2 ;
			rcSep.top 		= lpDs->rcItem.top ;
			rcSep.left		= lpDs->rcItem.left;
			rcSep.bottom	= lpDs->rcItem.bottom;
			rcSep.right		= lpDs->rcItem.right;

			hBrush = CreateSolidBrush(RGB(180, 80, 80));
			FillRect(lpDs->hDC, &rcSep, hBrush);
			DeleteObject(hBrush);

		}
		*/
		if(menu_data->menu_type == MF_SEPARATOR) //|| lpDs->itemData == 0
		{
			if(lpDs->hwndItem == (HWND)main_menu)
			{
				//////////////////////////////////////////////////////////////
				// a thin line of 2 pixels the height is                    //
				// bigger than 2 pixels so we need to calculate the middle  //
				//////////////////////////////////////////////////////////////
				int middle 		= (lpDs->rcItem.bottom - lpDs->rcItem.top) / 2 ;
				rcSep.top 		= lpDs->rcItem.top + middle ;
				rcSep.left		= lpDs->rcItem.left + 8;
				rcSep.bottom	= lpDs->rcItem.bottom - middle +1;
				rcSep.right		= lpDs->rcItem.right;

				hBrush = CreateSolidBrush(RGB(210, 210, 210));
				FillRect(lpDs->hDC, &rcSep, hBrush);
				DeleteObject(hBrush);
				
				rcSep.top -=1;
				rcSep.bottom -= 1;
				
				hBrush = CreateSolidBrush(RGB(160, 160, 160));
				FillRect(lpDs->hDC, &rcSep, hBrush);
				//DrawEdge(lpDs->hDC, &rcSep, BDR_SUNKENOUTER, BF_RECT);
				DeleteObject(hBrush);
			}
			
			///////////////////////////////////////////////////////
			// MF_SEPARATOR for Child menus                      //
			///////////////////////////////////////////////////////
			else
			{
				//////////////////////////////////////////////////////////////
				// a thin line of 2 pixels the height is                    //
				// bigger than 2 pixels so we need to calculate the middle  //
				//////////////////////////////////////////////////////////////
				int middle 		= (lpDs->rcItem.bottom - lpDs->rcItem.top) / 2 ;
				rcSep.top 		= lpDs->rcItem.top + middle ;
				rcSep.left		= lpDs->rcItem.left + border_w + 8;
				rcSep.bottom	= lpDs->rcItem.bottom - middle +1;
				rcSep.right		= lpDs->rcItem.right;

				hBrush = CreateSolidBrush(RGB(210, 210, 210));
				FillRect(lpDs->hDC, &rcSep, hBrush);
				DeleteObject(hBrush);
				
				rcSep.top -=1;
				rcSep.bottom -= 1;
				
				hBrush = CreateSolidBrush(RGB(160, 160, 160));
				FillRect(lpDs->hDC, &rcSep, hBrush);
				DeleteObject(hBrush);
			}
		}
		
		if(lpDs->CtlType & MFT_SEPARATOR )
		{
			int b;
			b++;
		}
		// is this menu item disabled ?
		if(lpDs->itemState & ODS_GRAYED)
		{


		}
		return 0;
freeObjectsX:
//free(menu_data);
return 0;
}
///////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////
int draw_buttons(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	LPDRAWITEMSTRUCT lpDs = (LPDRAWITEMSTRUCT)lParam;
	HDC 	hDC;
	HWND	temp_hwnd;
	HBITMAP temp_bmp[3];
	BITMAP	bmp;

	temp_hwnd = lpDs->hwndItem;
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
		DeleteObject(saveObj);
		//DrawAlphaBlend (hWnd, hDC);
		DeleteDC(hDC);
	}
	return 0;
}
///////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////
static HFONT CreateAngledFont(int fontHeight, int fontAngle, LPCTSTR lpfontFace,LOGFONT * ftAttr)
{
	////////////////////////////////////////////////////////////////////////////
	// Prepare logical font                                                   //
	////////////////////////////////////////////////////////////////////////////
	if(!ftAttr)
	{
		LOGFONT lf;
		ZeroMemory(&lf, sizeof(lf));

		lf.lfHeight 		= fontHeight;
		lf.lfEscapement		= ( 10 * fontAngle );	// 90' ccw
		lf.lfOrientation	= ( 10 * fontAngle );	// 90' ccw
		lf.lfWeight 		= FW_NORMAL;			// 600
		lf.lfCharSet 		= ANSI_CHARSET;
		lf.lfQuality 		= ANTIALIASED_QUALITY;

		lstrcpy(lf.lfFaceName, lpfontFace);

		////////////////////////////////////////////////////////////////////////////
		// instance logical font                                                  //
		////////////////////////////////////////////////////////////////////////////
		return CreateFontIndirect(&lf);
	}
	else
	{
		return CreateFontIndirect(ftAttr);
	}
}
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
int menuType_fromID(HMENU hMenu, UINT id)
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
UINT ISmenu_root(UINT id)
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
	//sm.hGlobal      = CreateFileGroupDescriptor(pszFiles);

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
HBITMAP	imageOptions(LPMENU_DATA menu_data_ptr,LPSTR image_path,COLORREF bgColor)
{
	
	HBITMAP tmpBMP = NULL;

	HDC hDCDesktop = GetDC(0);
	FREE_IMAGE_FORMAT fif = FreeImage_GetFileType(image_path,0);
	FIBITMAP *image = FreeImage_Load(fif, image_path, 0);

	static int img_width;
	static int img_height;
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
			//tmpBMP = LoadImage(hInst,image_path,IMAGE_ICON,0,0,LR_LOADFROMFILE);
			FIBITMAP *image = FreeImage_Load(fif, image_path, 0);
			if(img_width > 32 || img_height > 32 || img_width != img_height)
			{
				//image = FreeImage_Rescale(image,16,16,FILTER_BICUBIC);
			}
			FIBITMAP *display_dib_a = FreeImage_Composite(image,FALSE,NULL,NULL); 
			// image (b) : use the image file background if there is one 
			FIBITMAP *display_dib_b = FreeImage_Composite(image, TRUE,NULL,NULL); 
			// image (c) : use a user specified background 
			RGBQUAD appColor; //= { 128, 128, 128, 0 }; 
				appColor.rgbRed			= GetRValue(bgColor);
				appColor.rgbGreen		= GetGValue(bgColor);
				appColor.rgbBlue		= GetBValue(bgColor);
				appColor.rgbReserved	= 0;
			FIBITMAP *display_dib_c = FreeImage_Composite(image, FALSE, &appColor,NULL);
			
			FIBITMAP *rescaledImg;
			if(img_width > 32 || img_height > 32 || img_width != img_height)
			{
				//rescaledImg = FreeImage_Rescale(display_dib_c,16,16,FILTER_BSPLINE);
			}
			tmpBMP = CreateDIBitmap(hDCDesktop, FreeImage_GetInfoHeader(display_dib_c),CBM_INIT, FreeImage_GetBits(display_dib_c), FreeImage_GetInfo(display_dib_c), DIB_RGB_COLORS);
			FreeImage_Unload(image);
			ReleaseDC(NULL, hDCDesktop );
			return tmpBMP;
			break;
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
			FIBITMAP *image = FreeImage_Load(fif, image_path, 0);
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
			FIBITMAP *display_dib = FreeImage_Composite(image, FALSE, &appColor,NULL);
			//Use a file
			//FIBITMAP *bg = NULL;
			//FIBITMAP * display_dib = NULL;
			//bg = FreeImage_Load(FIF_BMP, "bgr.bmp", BMP_DEFAULT); 
			//display_dib = FreeImage_Composite(image,TRUE, NULL,bg);
			// Use file
			//display_dib = FreeImage_Composite(image, TRUE,NULL,bg);  laque anda
			// Copy to an HBITMAP
			// Release all GDI objects
			tmpBMP = CreateDIBitmap(hDCDesktop, FreeImage_GetInfoHeader(display_dib),CBM_INIT, FreeImage_GetBits(display_dib), FreeImage_GetInfo(display_dib), DIB_RGB_COLORS);
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

int HextoDec(char *hex)
{
	if (*hex==0) return 0;
	return  HextoDec(hex-1)*16 +  xtod(*hex) ;
}
int xstrtoi(char *hex)    // hex string to integer
{
	return HextoDec(hex+strlen(hex)-1);
}


BOOL xmlInitnstance(char * fname, BOOL bOptimizeMemory)
{
	IXMLDOMDocument*	document	= NULL;
	IXMLDOMParseError*	parseError	= NULL;
	IXMLDOMElement*		element		= NULL;	
	BSTR				bstr		= NULL;
	VARIANT_BOOL		status		= VARIANT_FALSE;
	VARIANT				vSrc;
	BSTR				fileName	= NULL;

	HRESULT hr;
	hr = CoInitialize(NULL);
	if (FAILED(hr))
	{
		MessageBox(NULL,"Could not CoInitialize()","Error !",MB_ICONWARNING);
		return FALSE;
	}

	hr = CoCreateInstance(&CLSID_DOMDocument, NULL, CLSCTX_INPROC_SERVER | CLSCTX_LOCAL_SERVER, &IID_IXMLDOMDocument, (LPVOID*)&document);
	if (FAILED(hr)) 
	{
		MessageBox(NULL,"Failed to CoCreate an instance of an XML DOM","Error !",MB_ICONWARNING);
	}

	document->lpVtbl->put_async(document,VARIANT_FALSE);
	VariantInit(&vSrc);
	//AnsiToUnicode(fname,&fileName);
	ConvertAnsiStrToBStrx(fname,&fileName);
	V_BSTR(&vSrc) = SysAllocString(fileName);
	V_VT(&vSrc) = VT_BSTR;

	hr = document->lpVtbl->load(document,vSrc, &status);

	if (status != VARIANT_TRUE)
	{
		long line, linePos;
		BSTR reason = NULL;
		char errMsg[1024];

		hr = document->lpVtbl->get_parseError(document,&parseError);
		hr = parseError->lpVtbl->get_reason(parseError,&bstr);
		hr = parseError->lpVtbl->get_errorCode(parseError,&hr);
		hr = parseError->lpVtbl->get_line(parseError,&line);
		hr = parseError->lpVtbl->get_linepos(parseError,&linePos);

		wsprintf(errMsg,"Error 0x%.8X on line %d, position %d\r\nReason: %s",hr, line, linePos, reason);
		MessageBox(NULL,errMsg,"Load Error !",MB_ICONWARNING);
        goto clean;
    }

	BSTR nodeName;
	hr = document->lpVtbl->get_documentElement(document,&element);
	if (FAILED(hr) || element == NULL)
	{
		MessageBox(NULL,"Empty document!", "Error Loading XML", MB_ICONWARNING);
        goto clean;
	}
	IXMLDOMNodeList* childList;
	hr = element->lpVtbl->get_childNodes(element,&childList);
	test_menu = CreatePopupMenu();
	buildMenu(childList, test_menu);
	
	
	element->lpVtbl->get_nodeName(element,&nodeName);
	//MessageBoxW(NULL,nodeName,L"Error",MB_ICONINFORMATION);
	//populateAttributes(element);

	/*
    hr = document->lpVtbl->get_xml(document,&bstr);
	if (SUCCEEDED(hr))
	{
		MessageBoxW(NULL,bstr,L"Error",MB_ICONWARNING);
	}
	*/
	// create read struct

clean:
    if (bstr) SysFreeString(bstr);
    if (&vSrc) VariantClear(&vSrc);
	if (fileName) CoTaskMemFree((void*)fileName);
    if (parseError) parseError->lpVtbl->Release(parseError);
    if (document) document->lpVtbl->Release(document);
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////
// Build Menu XML                                                                //
///////////////////////////////////////////////////////////////////////////////////
static int buildMenu(IXMLDOMNodeList *node, HMENU rootMenu)
{
	HRESULT 			hr = S_OK;
	long				listLength;
	BSTR				nodeName;
	VARIANT				varNodeValue;
	VARTYPE				returnType;
	VARIANT_BOOL		hasChild = VARIANT_FALSE;
	IXMLDOMNodeList*	childList;
	IXMLDOMNode*		nodeList = NULL;
	IXMLDOMNode*		resultNode = NULL;
	char msg[1024];

	static	int		MMU_IDS		= 2900;		// Start menu ID's at 1900
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

		hr = node->lpVtbl->get_item(node,i, &nodeList);
		
		//hr = nodeList->lpVtbl->get_nodeName(nodeList,&nodeName);

		hr = nodeList->lpVtbl->selectSingleNode(nodeList,L"CAPTION",&resultNode);
		BSTR caption;
		resultNode->lpVtbl->get_text(resultNode,&caption);
		resultNode->lpVtbl->Release(resultNode);
		hr = nodeList->lpVtbl->selectSingleNode(nodeList,L"IMAGE",&resultNode);
		resultNode->lpVtbl->Release(resultNode);
		hr = nodeList->lpVtbl->selectSingleNode(nodeList,L"COLOR",&resultNode);
		resultNode->lpVtbl->Release(resultNode);
		hr = nodeList->lpVtbl->selectSingleNode(nodeList,L"BGCOLOR",&resultNode);
		resultNode->lpVtbl->Release(resultNode);
		hr = nodeList->lpVtbl->selectSingleNode(nodeList,L"EXECUTE",&resultNode);
		resultNode->lpVtbl->Release(resultNode);
		hr = nodeList->lpVtbl->selectSingleNode(nodeList,L"FONT",&resultNode);
		resultNode->lpVtbl->Release(resultNode);
		hr = nodeList->lpVtbl->selectSingleNode(nodeList,L"MENUTYPE",&resultNode);
		resultNode->lpVtbl->Release(resultNode);
		hr = nodeList->lpVtbl->selectSingleNode(nodeList,L"MENUTYPE",&resultNode);
		BSTR menuType;
		resultNode->lpVtbl->get_text(resultNode,&menuType);
		resultNode->lpVtbl->Release(resultNode);
		///////////////////////////////////////////////////////////////////////////////////
		// search for nested objects                                                     //
		///////////////////////////////////////////////////////////////////////////////////
		IXMLDOMNodeList*	menuChild;
		hr = nodeList->lpVtbl->selectNodes(nodeList,L"MENUITEM",&menuChild);
		hr = menuChild->lpVtbl->get_length(menuChild,&depth);
		
		//resultNode->lpVtbl->get_text(resultNode,&nodeName);
		if (FAILED(hr))
		{
			MessageBoxW(NULL,L"selectSingleNode()",L"Error",MB_ICONINFORMATION);
			return hr;
		}
		//nodeList->lpVtbl->hasChildNodes(nodeList,&hasChild);
		//if (hasChild == VARIANT_TRUE)
		/////////////////////////////////////////////////////////////////////////////
		// For Items with nested nodes build a popup menu                          //
		/////////////////////////////////////////////////////////////////////////////
		if(depth > 0)
		{
			//hr = nodeList->lpVtbl->get_text(nodeList,&nodeName);
			//hr = nodeList->lpVtbl->get_nodeName(nodeList,&nodeName);
			//MessageBoxW(NULL,nodeName,L"Error",MB_ICONINFORMATION);
			HMENU newPop = CreatePopupMenu();

			//holder esta aca y el el puntero al menu en el que se esta trabajando
			AppendMenuW(rootMenu,MF_POPUP,(unsigned int)newPop, caption);
			if (caption) SysFreeString(caption);
			buildMenu(menuChild,newPop);
		}
		else
		{
			if (!wcscmp(menuType, L"MF_STRING"))
			{
				if(wcslen(caption) >= 0)
				{
					//menu_data->menu_capti = caption;
					//menu_data->menu_type = MF_STRING;
					AppendMenuW(rootMenu,MF_STRING,MMU_IDS, caption);
					if (caption) SysFreeString(caption);
					if (menuType) SysFreeString(menuType);
				}
			}
			else if (!wcscmp(menuType, L"MF_MENUBARBREAK"))
			{
				//menu_data->menu_capti	= caption;
				//menu_data->menu_type = MF_MENUBARBREAK;
				AppendMenuW(rootMenu,MF_MENUBARBREAK,MMU_IDS,caption);
				if (caption) SysFreeString(caption);
				if (menuType) SysFreeString(menuType);
			}
			else if (!wcscmp(menuType, L"MF_MENUBREAK"))
			{
				//menu_data->menu_capti	= caption;
				//menu_data->menu_type = MF_MENUBREAK;
				AppendMenuW(rootMenu,MF_MENUBREAK,MMU_IDS,caption);
				if (caption) SysFreeString(caption);
				if (menuType) SysFreeString(menuType);
			}
			else if (!wcscmp(menuType, L"MF_SEPARATOR"))
			{
				//menu_data->menu_capti	= caption;
				//menu_data->menu_type = MF_SEPARATOR;
				AppendMenuW(rootMenu,MF_SEPARATOR,MMU_IDS,NULL);
				if (caption) SysFreeString(caption);
				if (menuType) SysFreeString(menuType);
			}
			else if (!wcscmp(menuType, L"MF_SIDEBAR"))
			{
				//menu_data->menu_capti	= caption;
				//menu_data->menu_type = MF_SIDEBAR;
				AppendMenuW(rootMenu,MF_STRING,MMU_IDS,caption);
				if (caption) SysFreeString(caption);
				if (menuType) SysFreeString(menuType);
			}
		}
		MMU_IDS++;
	}

cleanup:
	//if (resultNode)	resultNode->lpVtbl->Release(resultNode);
	//SAFE_RELEASE(resultNode);
	if (nodeList)	nodeList->lpVtbl->Release(nodeList);
	return 0;
}
///////////////////////////////////////////////////////////////////////////////////
// How To Convert from BSTR to ANSI string                                       //
//                                                                               //
///////////////////////////////////////////////////////////////////////////////////
HRESULT oleStrToAnsi(BSTR bstrIn, LPSTR * lpszOut)
{
	DWORD dwSize;

	if(lpszOut == NULL)
	{
		return E_INVALIDARG;
	}
	if(bstrIn == NULL)
	{
		*lpszOut = NULL;
		return NOERROR;
	}
	dwSize = WideCharToMultiByte(CP_ACP, 0, bstrIn, -1, NULL, 0, NULL, NULL);
	if(dwSize == 0)
	{
		return HRESULT_FROM_WIN32(GetLastError());
	}
	*lpszOut = (LPSTR) SysAllocStringByteLen(NULL, dwSize - 1);
	if (*lpszOut == NULL)
	{
		return E_OUTOFMEMORY;
	}
	if ( !WideCharToMultiByte(CP_ACP, 0, bstrIn, -1, *lpszOut, dwSize, NULL, NULL) )
	{
		SysFreeString((BSTR) *lpszOut);
		return HRESULT_FROM_WIN32(GetLastError());
	}

	return NOERROR;
}
///////////////////////////////////////////////////////////////////////////////////
// Hanldes the drawing operation of th Root Menu either state                    //
//                                                                               //
///////////////////////////////////////////////////////////////////////////////////
BOOL drawRootMenuItem(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LPDRAWITEMSTRUCT	lpDs 		= (LPDRAWITEMSTRUCT)lParam;
	RECT				rcSep;
	HBRUSH				hBrush		= NULL;
	COLORREF			gradient;
	HPEN				hpen		= NULL;
	HPEN				hpenOld		= NULL;
	POINT				mPnt;
	int					point_x		= 0;
	int					point_y 	= 0;
	MENUITEMINFO		mii;
	MENU_DATA			menu_data;

	menu_data = get_menu_formWID((HMENU)lpDs->hwndItem,lpDs->itemID);
		
	if(lpDs->itemState & ODS_SELECTED)
	{
		/////////////////////////////////////////////////////////////////
		//	Menu Item Border                                           //
		/////////////////////////////////////////////////////////////////
		rcSep.top 		= lpDs->rcItem.top;
		rcSep.left		= lpDs->rcItem.left + 2; // Small separation from the sidebar
		rcSep.bottom	= lpDs->rcItem.bottom;
		rcSep.right		= lpDs->rcItem.right;

		// Create a green pen.
		hpen = CreatePen(PS_SOLID, 1, RGB(15, 15, 15));
		hBrush = CreateSolidBrush(RGB(95, 95, 95)); // Border Color

		hpenOld = SelectObject(lpDs->hDC, hpen);

		FrameRect(lpDs->hDC, &rcSep, hBrush);

		SelectObject(lpDs->hDC, hpenOld);
		DeleteObject(hpen);
		DeleteObject(hBrush);

		/////////////////////////////////////////////////////////////////
		//	Fill the Selected Menu Item with gradient color            //
		/////////////////////////////////////////////////////////////////
		InflateRect(&rcSep,-1,-1);
		gradientFill(lpDs->hDC,rcSep,menu_data.menu_color,RGB(180,180,180));

		/////////////////////////////////////////////////////////////////
		//	Create HBITMAP image from this menuitem                    //
		/////////////////////////////////////////////////////////////////
		HDC hCaptureDC = CreateCompatibleDC(lpDs->hDC);
		HBITMAP hCaptureBitmap = CreateCompatibleBitmap(lpDs->hDC,(rcSep.right - rcSep.left), (rcSep.bottom - rcSep.top));
		SelectObject(hCaptureDC,hCaptureBitmap);

		BitBlt(hCaptureDC,0,0,(rcSep.right - rcSep.left),(rcSep.bottom - rcSep.top),lpDs->hDC,0,0,SRCCOPY);

   		DeleteDC(hCaptureDC);
		// TODO DO STUFF
		HBITMAP myIcon = compositeImage(menu_data.menu_imageOvr,hCaptureBitmap);
		DeleteObject(hCaptureBitmap);
		/////////////////////////////////////////////////////////////////
		//	Icon Holder & mini sidedar                                 //
		/////////////////////////////////////////////////////////////////

		BITMAP bm;
		GetObject(menu_data.menu_image, sizeof(BITMAP), (LPSTR)&bm);
		point_y = lpDs->rcItem.top + ((lpDs->rcItem.bottom - lpDs->rcItem.top) / 2) - (bm.bmHeight/2); // Y position
		point_x = (lpDs->rcItem.left) + 4;	// X position of the icon
	
		DrawTransparentBitmap(lpDs->hDC,menu_data.menu_imageOvr,point_x,point_y,RGB(255,170,170));
			
		/////////////////////////////////////////////////////////////////
		//	texto caption                                              //
		/////////////////////////////////////////////////////////////////
		SIZE extent;
		char* captionText = (char*)lpDs->itemData;
		GetTextExtentPoint32(lpDs->hDC,captionText, lstrlen(captionText), &extent);
		point_y = ((lpDs->rcItem.bottom - lpDs->rcItem.top) / 2) - (extent.cy/2);
		point_y += lpDs->rcItem.top;
		
		rcSep.top 		= lpDs->rcItem.top;
		rcSep.left		= lpDs->rcItem.left + menu_data.menu_imgSize.right + 12; // Icon + border
		rcSep.bottom	= lpDs->rcItem.bottom;
		rcSep.right		= lpDs->rcItem.right;
		SetBkMode(lpDs->hDC,TRANSPARENT);
		DrawText (lpDs->hDC,captionText, -1, &rcSep,DT_SINGLELINE | DT_LEFT | DT_VCENTER);
	}
	else
	{

	}

	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////
// Hanldes the drawing operation of SubMenus either state                        //
//                                                                               //
///////////////////////////////////////////////////////////////////////////////////
BOOL drawChildMenuItem(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LPDRAWITEMSTRUCT lpDs = (LPDRAWITEMSTRUCT)lParam;
	RECT		rcSep;
	HBRUSH		hBrush;
	COLORREF	gradient;	// Used to store the gradient value
	HPEN		colorPen;	
	int			point_x		= 0;
	int			point_y 	= 0;

	MENUITEMINFO mii;
	MENU_DATA  menu_data;

	menu_data = get_menu_formWID((HMENU)lpDs->hwndItem,lpDs->itemID);

	if(lpDs->itemState & ODS_SELECTED)
	{
		/////////////////////////////////////////////////////////////////
		//	Sidebar Fillup use solid color                             //
		/////////////////////////////////////////////////////////////////
		rcSep.top 		= lpDs->rcItem.top;
		rcSep.left		= lpDs->rcItem.left;
		rcSep.bottom	= lpDs->rcItem.bottom;
		rcSep.right		= SIDEBAR_WIDTH;		// Sidebar width

		hBrush = CreateSolidBrush(RGB(128, 128, 128));

		FillRect(lpDs->hDC, &rcSep, hBrush);
		DeleteObject(hBrush);
		/////////////////////////////////////////////////////////////////
		//	Fill the Selected Menu Item with solid color               //
		/////////////////////////////////////////////////////////////////
		rcSep.top 		= lpDs->rcItem.top    ;
		rcSep.left		= lpDs->rcItem.left + 2;
		rcSep.bottom	= lpDs->rcItem.bottom ;
		rcSep.right		= lpDs->rcItem.right  ;

		/*
		hBrush = CreateSolidBrush(menu_data.menu_color); // Use sidebar Color
		FillRect(lpDs->hDC, &rcSep, hBrush);
		DeleteObject(hBrush);
		*/
		/////////////////////////////////////////////////////////////////
		//	Fill the Selected Menu Item with gradient color            //
		/////////////////////////////////////////////////////////////////
		gradientFill(lpDs->hDC,rcSep,RGB(215,215,215),RGB(180,180,180));

		/*
		/////////////////////////////////////////////////////////////////
		//	Button From Bitmap                                         //
		/////////////////////////////////////////////////////////////////
		int offset =  0;
		HBITMAP hbmpLf = LoadImage(GetModuleHandle(NULL),"btnLf.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
		HBITMAP hbmpMd = LoadImage(GetModuleHandle(NULL),"btnMd.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
		HBITMAP hbmpRg = LoadImage(GetModuleHandle(NULL),"btnRg.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
		
		HBITMAP btn = skinedBox(hbmpLf, hbmpMd, hbmpRg, lpDs->rcItem.right - lpDs->rcItem.left, 21);

		HDC hTempDC = CreateCompatibleDC(lpDs->hDC);
		HBITMAP oldBmp = SelectObject(hTempDC,hbmpLf);
		BitBlt(lpDs->hDC,lpDs->rcItem.left,lpDs->rcItem.top,(rcSep.right - rcSep.left),(rcSep.bottom - rcSep.top),hTempDC,0,0,SRCCOPY);		
		
		oldBmp = SelectObject(hTempDC,hbmpMd);
		for(int x = 0; x < ((lpDs->rcItem.right - lpDs->rcItem.left) - 8); x++)
		{
			BitBlt(lpDs->hDC,lpDs->rcItem.left + x,lpDs->rcItem.top,1,21,hTempDC,0,0,SRCCOPY);		
			offset++;
		}
		SelectObject(hTempDC,oldBmp);
		
		oldBmp = SelectObject(hTempDC,hbmpRg);
		BitBlt(lpDs->hDC,((lpDs->rcItem.right - lpDs->rcItem.left) - 8),lpDs->rcItem.top,8,21,hTempDC,0,0,SRCCOPY);		
		
		SelectObject(hTempDC,oldBmp);
		DeleteDC(hTempDC);
		DeleteObject(btn);
		DeleteObject(hbmpRg);
		DeleteObject(hbmpMd);
		DeleteObject(hbmpLf);

		*/
		/////////////////////////////////////////////////////////////////
		//	Menu Item Border                                           //
		/////////////////////////////////////////////////////////////////
		rcSep.top 		= lpDs->rcItem.top;
		rcSep.left		= lpDs->rcItem.left + 1; // Small separation from the sidebar
		rcSep.bottom	= lpDs->rcItem.bottom;
		rcSep.right		= lpDs->rcItem.right;

		hBrush = CreateSolidBrush(RGB(95, 95, 95)); // Border Color
		FrameRect(lpDs->hDC, &rcSep, hBrush);
		DeleteObject(hBrush);

		/////////////////////////////////////////////////////////////////
		//	Put the Icon                                               //
		/////////////////////////////////////////////////////////////////
		BITMAP bm;
		GetObject(menu_data.menu_imageOvr, sizeof(BITMAP), (LPSTR)&bm);
		//point_y = ((lpDs->rcItem.bottom - lpDs->rcItem.top) / 2) - (bm.bmHeight/2);
		//point_y += lpDs->rcItem.top;
		//point_x = lpDs->rcItem.left + 5;
		point_y = lpDs->rcItem.top + ((lpDs->rcItem.bottom - lpDs->rcItem.top) / 2) - (bm.bmHeight/2); // Y position
		point_x = lpDs->rcItem.left + (((lpDs->rcItem.left + SIDEBAR_WIDTH) / 2) - (bm.bmWidth/2));	// X position of the icon

		DrawTransparentBitmap(lpDs->hDC,menu_data.menu_imageOvr,point_x,point_y,RGB(170,170,170));
			

		/////////////////////////////////////////////////////////////////
		//	Finally Draw the caption                                   //
		/////////////////////////////////////////////////////////////////

		//populateAttributes(menu_data.xmlMenuItem);//menu_data->xmlMenuItem);
		IXMLDOMNode* firstChild;
		menu_data.xmlMenuItem->lpVtbl->get_firstChild(menu_data.xmlMenuItem,&firstChild);
		//populateNode((IXMLDOMElement *)firstChild);
		//populateAttributes(menu_data.xmlMenuItem);
		VARIANT vrt;
		VariantInit(&vrt);
		HRESULT hr = getAttributeValue(menu_data.xmlMenuItem,L"font",&vrt);

		switch(V_VT(&vrt))
		{
			case VT_BSTR:
				//MessageBoxW(NULL,V_BSTR(&vrt),L"NODE_TEXT",MB_ICONINFORMATION);
			break;
			case VT_I4:
			//V_I4(pvArg)
			break;
			case VT_UI4:
			break;
			case VT_R8:
			break;
			case VT_BOOL:
			break;
		}
		VariantClear(&vrt);
		//V_VT(vrt)  = VT_UI4;
		/*
		BSTR		nodeName;
		menu_data.xmlMenuItem->lpVtbl->get_text(menu_data.xmlMenuItem,&nodeName);
		MessageBoxW(NULL,nodeName,L"NODE_TEXT",MB_ICONINFORMATION);
		if (nodeName) SysFreeString(nodeName);
		*/
		//BSTR nodeName;
		//menu_data.xmlMenuItem->lpVtbl->get_nodeName(menu_data.xmlMenuItem,&nodeName);
		//MessageBoxW(NULL,nodeName,L"Error",MB_ICONINFORMATION);

		rcSep.top 		= lpDs->rcItem.top;
		rcSep.left		= lpDs->rcItem.left  + (SIDEBAR_WIDTH + 4);
		rcSep.bottom	= lpDs->rcItem.bottom;
		rcSep.right		= lpDs->rcItem.right;
				
		int PointSize = 8;
		int DPI = 72;
		int lfHeight = -MulDiv(PointSize, GetDeviceCaps(lpDs->hDC, LOGPIXELSY), DPI);

		COLORREF prevColor = SetTextColor(lpDs->hDC, RGB(0, 0, 0)); //font color
		HFONT hfnt = CreateAngledFont(lfHeight, 0, menu_data.menu_font_name,NULL);
		
		HFONT hfntPrev = SelectObject(lpDs->hDC, hfnt);

		SetBkMode(lpDs->hDC,TRANSPARENT);
		DrawText (lpDs->hDC,menu_data.menu_capti, -1, &rcSep,DT_SINGLELINE | DT_LEFT | DT_VCENTER);
		SelectObject(lpDs->hDC, hfntPrev);
		SetTextColor(lpDs->hDC, prevColor);
		DeleteObject(hfnt);
	}
	else
	{
		///////////////////////////////////////////////////////
		//	Fill in the menu window                          //
		///////////////////////////////////////////////////////
		rcSep.top 	= lpDs->rcItem.top;
		rcSep.left	= lpDs->rcItem.left;
		rcSep.bottom	= lpDs->rcItem.bottom;
		rcSep.right	= lpDs->rcItem.right;


		HBRUSH hBrush = CreateSolidBrush(RGB(195, 195, 195));		// Menuitem Bg color
		FillRect(lpDs->hDC, &rcSep, hBrush);
		DeleteObject(hBrush);

		///////////////////////////////////////////////////////
		//	barra al costado solo en los childs              //
		///////////////////////////////////////////////////////
		rcSep.top 		= lpDs->rcItem.top;
		rcSep.left		= lpDs->rcItem.left;
		rcSep.bottom	= lpDs->rcItem.bottom;
		rcSep.right		= rcSep.left + SIDEBAR_WIDTH;

		hBrush = CreateSolidBrush(RGB(128, 128, 128));				// Sidebar color
		FillRect(lpDs->hDC, &rcSep, hBrush);
		DeleteObject(hBrush);

		///////////////////////////////////////////////////////
		//	draws a line betwen the sidebar & the menuitem   //
		///////////////////////////////////////////////////////
		int y_t = lpDs->rcItem.top;
		int x_t = SIDEBAR_WIDTH;
		HPEN saveObject = SelectObject(lpDs->hDC,CreatePen(PS_SOLID, 1, RGB(0, 0, 0)));
		MoveToEx(lpDs->hDC, x_t, y_t, NULL);
		LineTo  (lpDs->hDC, x_t, y_t);
		LineTo  (lpDs->hDC, x_t ,lpDs->rcItem.bottom);
		DeleteObject(SelectObject(lpDs->hDC, saveObject));

		///////////////////////////////////////////////////////
		//	Put the icon                                     //
		///////////////////////////////////////////////////////		
		BITMAP bm;
		GetObject(menu_data.menu_imageOut, sizeof(BITMAP), (LPSTR)&bm);
		point_y = lpDs->rcItem.top + ((lpDs->rcItem.bottom - lpDs->rcItem.top) / 2) - (bm.bmHeight/2); // Y position
		point_x = lpDs->rcItem.left + (((lpDs->rcItem.left + SIDEBAR_WIDTH) / 2) - (bm.bmWidth/2));	// X position of the icon
		
		DrawTransparentBitmap(lpDs->hDC,menu_data.menu_imageOut,point_x,point_y,RGB(170,170,170));

		///////////////////////////////////////////////////////
		//	Draw the item caption                            //
		///////////////////////////////////////////////////////	
		rcSep.top 		= lpDs->rcItem.top;
		rcSep.left		= lpDs->rcItem.left  + (SIDEBAR_WIDTH + 4); //menu_data.menu_imgSize.right + 18; //+ text_off - 45 ;
		rcSep.bottom	= lpDs->rcItem.bottom;
		rcSep.right		= lpDs->rcItem.right;
				
		int PointSize = 8;
		int DPI = 72;
		int lfHeight = -MulDiv(PointSize, GetDeviceCaps(lpDs->hDC, LOGPIXELSY), DPI);

		COLORREF prevColor = SetTextColor(lpDs->hDC, RGB(0, 0, 0)); //font color
		HFONT hfnt = CreateAngledFont(lfHeight, 0, menu_data.menu_font_name,NULL);
		
		HFONT hfntPrev = SelectObject(lpDs->hDC, hfnt);

		SetBkMode(lpDs->hDC,TRANSPARENT);
		DrawText (lpDs->hDC,menu_data.menu_capti, -1, &rcSep,DT_SINGLELINE | DT_LEFT | DT_VCENTER);
		SelectObject(lpDs->hDC, hfntPrev);
		SetTextColor(lpDs->hDC, prevColor);
		DeleteObject(hfnt);

	}
	return TRUE;
}

