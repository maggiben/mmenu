///////////////////////////////////////////////////////////////////////////////////
// Título:      		main_menu.c                                              //
// Author:		        Benjamin Maggi                                           //
// Descripcion: 		A customizable Menu                                      //
// Org. Date:           17/01/2005                                               //
// Last Modification:   13/03/2008                                               //
// ver:                 0.8                                                      //
// compiler:            LCC-Win32 (uses ansi-C standar only / C99)               //
// Author mail:         benjaminmaggi@gmail.com                                  //
// License:             GNU                                                      //
// ----------------------------------------------------------------------------- //
//	                       								                         //
// Detais & compilating information                                              //
// ver 0.1                                                                       //
// I've compiled and linked this using lcc but should work in other c/c++        //
// compiler.                                                                     //
// be carful debugging the hooked function it may cause a GPF                    //
// you can turn hooks off 									                     // 
// ----------------------------------------------------------------------------- //
//                                                                               //
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
// ----------------------------------------------------------------------------- //
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
//	* Research on Drag&Drop                                                      //
//                                                                               //
// * Entry for 14/03/2008                                                        //
//	* Added CS_DROPSHADOW to MainForm                                            //
//                                                                               //
// * Entry for 18/03/2008                                                        //
//	* Working on MF_MENUBARBREAK                                                 //
//                                                                               //
///////////////////////////////////////////////////////////////////////////////////
//	Includes                                                                     //
///////////////////////////////////////////////////////////////////////////////////

#include <windows.h>		// windows standar
#include <windowsx.h>       // Extra features 
#include <commctrl.h>       // Connmon controls
#include <string.h>         //
#include <stdio.h>			// You know what this is for
#include <shellapi.h>       // For shell interaction
#include <tchar.h>          
#include <wchar.h>
#include <ole2.h>           // Drag & Drop operations and more

#include "IDragDrop.h"

#include "disphelper.h"     // interface with COM MSXML 
#include "main_menures.h"   // Resources
#include "main_menu.h"      // Proto

#include "FreeImage.h"      // FreeImage protos & delares



///////////////////////////////////////////////////////////////////////////////////
// #defines aqui                                                                 //
///////////////////////////////////////////////////////////////////////////////////
#define HR_TRY(func) if (FAILED(func)) { goto cleanup; }

///////////////////////////////////////////////////////////////////////////////////
// C99 Preprocessor directives                                                   //
///////////////////////////////////////////////////////////////////////////////////
#pragma warn(disable: 2115 2114)


///////////////////////////////////////////////////////////////////////////////////
// Main menu structure it handles all the nessesary data                         //
// We will make a linked list of items                                           //
///////////////////////////////////////////////////////////////////////////////////
typedef struct _MENU_DATA
{
		///////////////////////////////////////////////////////////////////////////////////
		// List managment propertys                                                      //
		///////////////////////////////////////////////////////////////////////////////////
		void *    nextItem;				//point to next MENU_DATA
		void * 	  prevItem;				//point to prev MENU_DATA
		HMENU     menu_handle;
		HMENU	  wID;

		///////////////////////////////////////////////////////////////////////////////////
		// Drawing & extra information                                                   //
		///////////////////////////////////////////////////////////////////////////////////
		LPSTR		menu_capti;			// Menu Caption
		LPSTR		menu_exec;			// Executable Path
		HBITMAP		menu_image;			// Plain bitmap
		HBITMAP		menu_imageOvr;		// Image to show when menu is selected.
		HBITMAP		menu_imageOut;		// Image to show when menu is not selected.
		UINT		menu_imgType;		// Type of image Icom or Bitmap
		BOOL		menu_imask;			//
		HFONT		menu_font;			// Menu font
		LPSTR		menu_font_name;		// Menu Item Font name
		HFONT		menu_fsize;			
		COLORREF	menu_color;			//
		COLORREF	menu_selor;			
		int			menu_width;			// menu item width
		int			menu_height;		// menu item height
		int			menu_popup;
		UINT		menu_type;			// Menu item thye MF_SEPERATOR popup etc... added 2006
}MENU_DATA, *LPMENU_DATA;

LPMENU_DATA	primary_menu_data;
LPMENU_DATA	first_menu_data;

///////////////////////////////////////////////////////////////////////////////////
//	Global variables                                                             //
///////////////////////////////////////////////////////////////////////////////////

HINSTANCE hInst;			// Instance handle
HWND      hwndMain;			// Main window handle
HWND	  main_window;		// Another Main window Handle
HWND      menu_button;
int 	  menu_objects;
UINT      menu_msg;
HWND      opti_button;
HWND      plus_button;
HMENU     main_menu;
WNDPROC	  buttons_proc[10];


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

void 				DrawTransparentBitmap			(HDC,HBITMAP,short,short,COLORREF );
void 				drawIconToBitmap				(HBITMAP bmSrc,char * FileName,int size,COLORREF compositeColor);
///////////////////////////////////////////////////////////////////////////////////
// Create a bitmap mask by masking the selected color                            //
///////////////////////////////////////////////////////////////////////////////////
HBITMAP 			CreateBitmapMask				(HBITMAP,COLORREF);
static HFONT 		CreateAngledFont				(int,int,LPCTSTR);


///////////////////////////////////////////////////////////////////////////////////
//	XML Loader & Parser Uses COM MSXML                                           //
///////////////////////////////////////////////////////////////////////////////////
static int 			LoadXML							(char *,int);
static int 			parse_XML						(IDispatch*,HMENU);
static BOOL			deserializeStruct				(LPMENU_DATA,LPSTR);
static HRESULT 		CreateAndAppendNode				(IDispatch * xmlDoc, IDispatch * xmlParent,LPCWSTR szNodeName, LPCWSTR szNodeText);
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
LRESULT CALLBACK 	Sub_Class_MenuProc				(HWND,UINT,WPARAM,LPARAM);
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

UINT Spacial_Case;

///////////////////////////////////////////////////////////////////////////////////
//	Program code
///////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////
//	Registro de la clase para la ventana                                         //
///////////////////////////////////////////////////////////////////////////////////
static BOOL InitApplication(void)
{
	WNDCLASS wc;

	memset(&wc,0,sizeof(WNDCLASS));

	wc.style         = CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS|CS_DROPSHADOW;
	wc.lpfnWndProc   = (WNDPROC)MainWndProc;
	wc.hInstance     = hInst;
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);
	wc.lpszClassName = "main_menu_WndClass";
	wc.lpszMenuName  = NULL; //MAKEINTRESOURCE (IDMAINMENU);
	wc.hCursor       = LoadCursor(NULL,IDC_ARROW);
	wc.hIcon         = 0;	//LoadIcon        (NULL,IDI_APPLICATION);

	if (!RegisterClass(&wc))
		return 0;

	//	Load resurces
	//	can be edited externaly using a resource hacker like tool

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
//	Crea la ventana                                                              //
///////////////////////////////////////////////////////////////////////////////////

HWND Createmain_menu_WndClassWnd(void)
{
		return CreateWindowEx
		(
			WS_EX_LEFT,
			"main_menu_WndClass",
			"Título",
			WS_POPUP,	// style
			100,		// pos x of win
			100,		// pos y of win,
			115,		// Width
			40,         // Height
        	NULL,       
			NULL,
			hInst,      // instance
			NULL
		);
}

///////////////////////////////////////////////////////////////////////////////////
//	Handle command subroutine                                                    //
///////////////////////////////////////////////////////////////////////////////////
void MainWndProc_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	HMENU	hMenu_ret;
	BOOL    ret_u;
	POINT   point;
	RECT    rc_1;
	//int	ret;

	MENU_DATA    menu_data;

	char * texto = malloc(256);


	switch(id) 
	{
		case IDM_EXIT:
			PostMessage(hwnd,WM_CLOSE,0,0);
		break;

		case 330:
			hMenu_ret = menu_builder(hwnd);
			ClientToScreen(menu_button, &point);
			GetWindowRect(hwnd,&rc_1);
			point = point;
			//HMENU menu = LoadMenu(hInst,MAKEINTRESOURCE(IDMAINMENU));
			
			//  set_newTheme(hWnd,TRUE);
			//HMENU hmenuPopup = (HMENU) wParam;          // manipulador de submenú
			//UINT uPos = (UINT) LOWORD(lParam);         // posición del ítem del submenú
			//BOOL fSystemMenu = (BOOL) HIWORD(lParam);  // bandera de menú de ventana

			MENUINFO mnfo;
			mnfo.cbSize = sizeof(mnfo);
			mnfo.fMask = MIM_STYLE;
			mnfo.dwStyle = MNS_DRAGDROP | MNS_AUTODISMISS;

			SetMenuInfo(hMenu_ret,&mnfo);

			ret_u = TrackPopupMenu(hMenu_ret,TPM_LEFTALIGN | TPM_RETURNCMD | TPM_NONOTIFY, rc_1.left, (rc_1.top + 42), 0, main_window, 0);
			SendMessage(main_window, WM_COMMAND, ret_u, (LPARAM)0);

		break;
		case 331:
		exit(1);
		break;
		case 332:
		{
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

			deserializeStruct(primary_menu_data,"mocoo");
			break;
		}
		default:
		{
			break;
		}
	}
///////////////////////////////////////////////////////////////////////////////////
// Handle menu commands, menu id's start at 1900                                 //
///////////////////////////////////////////////////////////////////////////////////
		if(id >= 1900)
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

	switch (msg) 
	{
	case WM_MENUSELECT:
	{
		///////////////////////////////////////////////////////////////////////////
		// Special things to do when we select a menu item                       //
		///////////////////////////////////////////////////////////////////////////
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
		///////////////////////////////////////////////////////////////////////////
		// Load resources                                                        //
		///////////////////////////////////////////////////////////////////////////
		hRes		= LoadResource(hInst,FindResource(hInst, "#12346", RT_BITMAP));

		lpBitmap	= (LPBITMAPINFO) LockResource(hRes);
 		lpBits		= (LPTSTR) lpBitmap;
	   	lpBits		+= lpBitmap->bmiHeader.biSize + (256 * sizeof(RGBQUAD));

		///////////////////////////////////////////////////////////////////////////
		// Register drag and drop operations                                     //
		///////////////////////////////////////////////////////////////////////////
		IDropTarget * pDropTarget = DropTarget_CDropTarget();
		DropTargetEx * pDTEx = (DropTargetEx *)pDropTarget;
		pDTEx->m_hwnd = hwnd;

		if(pDropTarget)
			RegisterDragDrop(hwnd, pDropTarget);
		///////////////////////////////////////////////////////////////////////////
		//	Creo los botones de control                                          //
		///////////////////////////////////////////////////////////////////////////
		def_result = create_Controls(hInst, hwnd, 1);
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
		HMENU   m_dstMenu; // destination menu handle
		int     m_dstPos;  // destination menu item

		int b;
		b = 1;

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
		// make this window magnetic
		///////////////////////////////////////////////////////////////////////////
		return MagnetWindow(&lParam);
		break;
	}
	case WM_MEASUREITEM:
	{
		///////////////////////////////////////////////////////////////////////////
		// We need to calculate OWNERDRAW items size                             //
		///////////////////////////////////////////////////////////////////////////
		TEXTMETRIC          tm;
		LPMEASUREITEMSTRUCT pmis;
		HDC                 hdc;
		SIZE				sz;
		HFONT 				hfnt;
		BOOL				hResult;

		//load strcture pointer 
		pmis = (LPMEASUREITEMSTRUCT)lParam;
		//TCS_OWNERDRAWFIXED
		MENU_DATA menu_data = get_menu_formWID(main_menu,pmis->itemID);
		
		///////////////////////////////////////////////////////////////////////////
		//  Seteo medidas w y h del elemento del menu (puede ser independietnte) //
		//  chequear tamano en pixels de la fuente                               //
		///////////////////////////////////////////////////////////////////////////
		if (pmis->CtlType == ODT_MENU)
		{
			hdc = GetDC(hwnd);
			GetTextMetrics(hdc, &tm);
			
			//
			// nota: usar select object para seleccionar el tipo de fuente en el DC
			//
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
				pmis->itemHeight  = 40;
				pmis->itemWidth   = sz.cx + 100;
			}
			else
			{
				pmis->itemHeight  = 26;
				pmis->itemWidth   = sz.cx + 35;			
			}
			///////////////////////////////////////////////////////////////////////////
			// Set MF_SEPARATOR height                                               //
			///////////////////////////////////////////////////////////////////////////
			if(pmis->itemData <= 0)
			{
				pmis->itemHeight  = 12;
			}
			ReleaseDC(hwnd, hdc);
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

	///////////////////////////////////////////////////////////////////////////
	//	330 simbolo inicial comando boton 
	///////////////////////////////////////////////////////////////////////////
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

	///////////////////////////////////////////////////////////////////////////
	// Subclasing elements (keep orden)                                      //
	///////////////////////////////////////////////////////////////////////////
	buttons_proc[1] = (WNDPROC)SetWindowLong(menu_button,GWL_WNDPROC, (DWORD)ButtonProc) ;

	 return 1;
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

		//	Cant cast drawitem here ???
		//	el subclass parece no funcionar y el mensaje lo recibe la veentana (parent)
		//
		case WM_DRAWITEM:
		{ //  Handles the ownerdraw button

			LPDRAWITEMSTRUCT lpDs = (LPDRAWITEMSTRUCT)lParam;
			HDC hDC = CreateCompatibleDC(lpDs->hDC);	// create a memory DC

			if(hDC)
			{
				if(lpDs->itemState & ODS_SELECTED) //  if button is down
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
//	Helper func provided (?)
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
//
///////////////////////////////////////////////////////////////////////////////////
static BOOL deserializeStruct(LPMENU_DATA menu_data_prt,LPSTR fileName)
{
	DISPATCH_OBJ(xmlDoc);
	HR_TRY( dhCreateObject(L"MSXML.DOMDocument", NULL, &xmlDoc) );
	HR_TRY(	dhPutValue(xmlDoc, L".Async = %b", FALSE) );
	
	WITH1(xmlNode, xmlDoc, L".CreateElement(%S)", L"CD")
	{
		/* Add the child elements with our helper function */
		CreateAndAppendNode(xmlDoc, xmlNode, L"TITLE",   L"DispHelper Soundtrack");
		CreateAndAppendNode(xmlDoc, xmlNode, L"ARTIST",  L"DispHelper Development Department");
		CreateAndAppendNode(xmlDoc, xmlNode, L"COUNTRY", L"Global");
		CreateAndAppendNode(xmlDoc, xmlNode, L"COMPANY", L"DispHelper");
		CreateAndAppendNode(xmlDoc, xmlNode, L"PRICE",   L"12.75");
		CreateAndAppendNode(xmlDoc, xmlNode, L"YEAR",    L"2003");

		/* Append our new node to the original document */
		dhCallMethod(xmlDoc, L".documentElement.appendChild(%o)", xmlNode);

	} END_WITH(xmlNode);

	dhCallMethod(xmlDoc, L"Save(%S)", L"cd_catalog2.xml");

	return TRUE;
cleanup:
	SAFE_RELEASE(xmlDoc);
	return FALSE;
}
///////////////////////////////////////////////////////////////////////////////////
// Load the XML file                                                             //

///////////////////////////////////////////////////////////////////////////////////
static int LoadXML(char * fname, int flags)
{
	DISPATCH_OBJ(xmlDoc);
	DISPATCH_OBJ(NodeList);

	HR_TRY( dhCreateObject(L"MSXML.DOMDocument", NULL, &xmlDoc) );
	HR_TRY( dhPutValue(xmlDoc, L".Async = %b", FALSE) );
	HR_TRY( dhCallMethod(xmlDoc, L".Load(%S)", L"menu.xml") );

	dhGetValue(L"%o",&NodeList,xmlDoc, L".documentElement.childNodes");
	AppendMenu(main_menu,MF_MENUBARBREAK | MF_OWNERDRAW,1800, NULL );
	parse_XML(NodeList, main_menu);
	return 1;
cleanup:
	SAFE_RELEASE(xmlDoc);
	SAFE_RELEASE(NodeList);
	return -1;
}


///////////////////////////////////////////////////////////////////////////////////
//
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

	
	//pointer_holder = first_menu_data;
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
		dhGetValue(L"%u", &menu_fg_color	,NodeItem, L".selectSingleNode(%S).text", L"COLOR");
		dhGetValue(L"%u", &menu_bg_color	,NodeItem, L".selectSingleNode(%S).text", L"BGCOLOR");
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
			}
			else
			{
			///////////////////////////////////////////////////////////////////////////////
			// Distinguish between normal BMP's JPG's or ICO                             //
			///////////////////////////////////////////////////////////////////////////////
				menu_data->menu_capti	= "hola";
				menu_data->menu_width	= 112;
				menu_data->menu_height	= 112;
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
			//Build an isolated menu
			menu_data = redim_items(menu_data, 1);

			//set data for this item
			if(image_path[0] == '\0' )
			{
				//MessageBox(main_window,"no path","Error de tipo:",MB_OK);
				HDC hDCDesktop = GetDC(0);
				//loaded_bmp = CreateCompatibleBitmap( hDCDesktop, icoSize,icoSize);
				menu_data->menu_imageOut = CreateCompatibleBitmap( hDCDesktop, icoSize,icoSize);
				menu_data->menu_imageOvr = CreateCompatibleBitmap( hDCDesktop, icoSize,icoSize);
  				ReleaseDC(NULL, hDCDesktop );

				drawIconToBitmap( menu_data->menu_imageOut, exec_path, icoSize,menu_bg_color);
				drawIconToBitmap( menu_data->menu_imageOvr, exec_path, icoSize,menu_fg_color);
  				//drawIconToBitmap( loaded_bmp, exec_path, icoSize,RGB(128, 128, 128));
				//menu_data->menu_imageOut = loaded_bmp;
				//menu_data->menu_imageOvr = loaded_bmp;
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

			
			menu_data->menu_width = 0;
			menu_data->menu_width = max(menu_data->menu_width,zSize.cx);
			
			//SelectObject(nullDC, oldFt);
			//DeleteDC(nullDC);

			if (!strcmp(menuType, "MF_STRING"))
			{
				if(strlen(caption) >= 1)
				{
					//Normal case
					menu_data->menu_capti = caption;
					menu_data->menu_type = MF_STRING;
					//AppendMenu(holder,MF_STRING|MF_OWNERDRAW,MMU_IDS, caption );
				}
			}
			else if (!strcmp(menuType, "MF_MENUBARBREAK"))
			{
				menu_data->menu_capti	= caption;
				menu_data->menu_type = MF_MENUBARBREAK;
				apiResult = AppendMenu(holder,MF_MENUBARBREAK|MF_OWNERDRAW,MMU_IDS, NULL );
			}
			else if (!strcmp(menuType, "MF_SEPARATOR"))
			{
				menu_data->menu_capti	= caption;
				menu_data->menu_type = MF_SEPARATOR;
				apiResult = AppendMenu(holder,MF_SEPARATOR|MF_OWNERDRAW,MMU_IDS, NULL );
			}
			
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
				menu_data->menu_capti	= caption;
				menu_data->menu_type	= MF_STRING;
				AppendMenu(holder,MF_STRING | MF_OWNERDRAW,MMU_IDS, caption );
			}
		
		//}//end if
	
		//AppendMenu(holder,MF_STRING,MMU_IDS, Menu_item.menu_capti );
		//menu_data->wID =  GetMenuItemID(holder, cnt);

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

	int x = 0;

	x++;
	damm_handle = pStruct->hwnd;

	//MessageBox(main_window,"hookinged  !!!","Error de tipo:",MB_OK);

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

	x = 10;
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
	x = 10;
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
	if ( oldWndProc == NULL )
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
	WNDPROC oldWndProc = (WNDPROC)GetProp (hWnd, _WndPropName_OldProc);

	switch ( uMsg )
	{
	case WM_DRAWITEM:
	{

	}


		/*
	case WM_NCCALCSIZE:
		{
		NCCALCSIZE_PARAMS* lpncsp = (NCCALCSIZE_PARAMS*)lParam;

		//lpncsp->lppos->x +=  30;
		lpncsp->rgrc[0].right  -= 1;
		lpncsp->rgrc[2].bottom -= 10;
		lpncsp->rgrc[0].top    += 1;
		lpncsp->rgrc[0].left   += 1;
		lpncsp->lppos->cx +=  10;
		return 0;
		}
		*/
	case WM_NCPAINT:
	{
		//return 0;
		HDC hDC;
		RECT rcTwo;
		RECT rcSep;
		RECT rcOrg;
		int bdr_w = 30;   //main menu sidebar w
		int bdr_h = 150;

		//static int main_wnd = 0;

		hDC = GetWindowDC (hWnd);
		GetWindowRect(hWnd,&rcTwo);

		//SetWindowPos(hWnd,hWnd,10,10,10,10,0);

		GetClipBox(hDC,&rcTwo);
		rcOrg = rcTwo;

		//return 0;
		//LPDRAWITEMSTRUCT	lpDs = (LPDRAWITEMSTRUCT)lParam;
		rcSep.top 		= rcTwo.top 	+2;
		rcSep.left		= rcTwo.left	-2;
		rcSep.bottom	= rcTwo.bottom  -2;
		rcSep.right		= rcTwo.right   +2;

		HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0));
		FrameRect(hDC, &rcTwo, hBrush);

		rcTwo.top 	 = rcTwo.top 	 +1;
		rcTwo.left	 = rcTwo.left	 +1;
		rcTwo.bottom 	 = rcTwo.bottom  -1;
		rcTwo.right	 = rcTwo.right	 -1;
		DeleteObject(hBrush);

		//cambiar color: borde mas fino
		hBrush = CreateSolidBrush(RGB(195,195, 195));
		FrameRect(hDC, &rcTwo, hBrush);

		rcTwo.top 	= rcTwo.top 	+1;
		rcTwo.left	= rcTwo.left	+1;
		rcTwo.bottom	= rcTwo.bottom	-1;
		rcTwo.right	= rcTwo.right	-1;

		FrameRect(hDC, &rcTwo, hBrush);
		DeleteObject(hBrush);

		//Side bar
		rcTwo.top 	 = rcOrg.top 	+1;
		rcTwo.left	 = rcOrg.left	+1;
		rcTwo.bottom 	 = rcOrg.bottom -1;
		rcTwo.right	 = 3;

		hBrush = CreateSolidBrush(RGB(128,128, 128));
		//FillRect generates the overlaping
		//by detecting painting all the time
		//is better to draw only where the menuitem
		//is not going to be drawing off
		//that leaves the perimeter of the windows
		FillRect(hDC, &rcTwo, hBrush);


		rcTwo.top 	 = rcOrg.bottom	-3;
		rcTwo.left	 = rcOrg.left	+1;
		rcTwo.bottom = rcOrg.bottom -1;
		rcTwo.right	 = 29;

		FillRect(hDC, &rcTwo, hBrush);

		rcTwo.top 	 = rcOrg.top	-1;
		rcTwo.left	 = rcOrg.left	+1;
		rcTwo.bottom = rcOrg.top    +3;
		rcTwo.right	 = 29;

		FillRect(hDC, &rcTwo, hBrush);

		DeleteObject(hBrush);

		//black perimeter
		rcTwo.top 	 = rcOrg.top 	;
		rcTwo.left	 = rcOrg.left	;
		rcTwo.bottom 	 = rcOrg.bottom ;
		rcTwo.right	 = bdr_w;
	
		hBrush = CreateSolidBrush(RGB(0,0, 0));
		FrameRect(hDC, &rcTwo, hBrush);
		DeleteObject(hBrush);

		HPEN saveObject = SelectObject(hDC,CreatePen(PS_SOLID, 1, RGB(255, 0, 0)));
   		DeleteObject(SelectObject(hDC, saveObject));
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
		RECT    rcSep;
		BITMAP  bmp;
		HBRUSH  hBrush;


		//medidas
		int border_w	= 26;
		int text_off	= 78;
		int inner_sq    = 4;

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


		menu_data = & rawi_data;

		lpDs = &lpCopy;


		if(lpDs->itemState & ODS_SELECTED)
		{
			///////////////////////////////////////////////////////
			//	barra al costado                                 //
			///////////////////////////////////////////////////////
			rcSep.top 		= lpDs->rcItem.top    ;
			rcSep.left		= lpDs->rcItem.left   + border_w + 3;
			rcSep.bottom	= lpDs->rcItem.bottom ;
			rcSep.right		= lpDs->rcItem.right  ;

			hBrush = CreateSolidBrush(menu_data->menu_color);
			FillRect(lpDs->hDC, &rcSep, hBrush);
			DeleteObject(hBrush);

			///////////////////////////////////////////////////////
			//	perimetro mas obscuro                            //
			///////////////////////////////////////////////////////
			rcSep.top 		= lpDs->rcItem.top;
			rcSep.left		= lpDs->rcItem.left   + border_w + 3;
			rcSep.bottom	= lpDs->rcItem.bottom;
			rcSep.right		= lpDs->rcItem.right;

			hBrush = CreateSolidBrush(RGB(128, 128, 128));
			FrameRect(lpDs->hDC, &rcSep, hBrush);
			DeleteObject(hBrush);

			///////////////////////////////////////////////////////
			//	barra al costado box extra                       //
			///////////////////////////////////////////////////////
			rcSep.top 		= lpDs->rcItem.top;
			rcSep.left		= lpDs->rcItem.left;
			rcSep.bottom	= lpDs->rcItem.bottom;
			rcSep.right		= rcSep.left + border_w;

			hBrush = CreateSolidBrush(RGB(128, 128, 128));
			FillRect(lpDs->hDC, &rcSep, hBrush);
			DeleteObject(hBrush);

			///////////////////////////////////////////////////////
			//	draws a line betwen the sidebar & the menuitem   //
			///////////////////////////////////////////////////////
			int y_t = lpDs->rcItem.top;
			int x_t = border_w;
			HPEN saveObject = SelectObject(lpDs->hDC,CreatePen(PS_SOLID, 1, RGB(0, 0, 0)));
			MoveToEx(lpDs->hDC, x_t, y_t, NULL);
			LineTo  (lpDs->hDC, x_t, y_t);
			LineTo  (lpDs->hDC, x_t ,lpDs->rcItem.bottom);
			DeleteObject(SelectObject(lpDs->hDC, saveObject));

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
				point_x = (lpDs->rcItem.left + border_w + 10) + (((lpDs->rcItem.left + bm.bmWidth) / 2 ) - (bm.bmWidth / 2));
				
		
				DrawTransparentBitmap(lpDs->hDC,menu_data->menu_imageOvr,point_x,point_y,RGB(170,170,170));
				
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

					hBrush = CreateSolidBrush(RGB(90, 90, 90));
					FrameRect(lpDs->hDC, &rcSep, hBrush);
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
			//	Put logo                                         //
			///////////////////////////////////////////////////////
			if(lpDs->hwndItem == (HWND)main_menu)
			{
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
					rcSep.left		= lpDs->rcItem.left   + text_off;
					rcSep.bottom	= lpDs->rcItem.bottom;
					rcSep.right		= lpDs->rcItem.right;
					SetBkMode(lpDs->hDC,TRANSPARENT);
					DrawText (lpDs->hDC,captionText, -1, &rcSep,DT_SINGLELINE | DT_LEFT | DT_VCENTER);
			}
			///////////////////////////////////////////////////////
			// 
			///////////////////////////////////////////////////////
			else
			{
					char* captionText = (char*)lpDs->itemData;	
					// 
					rcSep.top 		= lpDs->rcItem.top;
					rcSep.left		= lpDs->rcItem.left   + text_off - 45 ;
					rcSep.bottom	= lpDs->rcItem.bottom;
					rcSep.right		= lpDs->rcItem.right;
				
					int PointSize = 8;
					int DPI = 72;
					int lfHeight = -MulDiv(PointSize, GetDeviceCaps(lpDs->hDC, LOGPIXELSY), DPI);

					HFONT hfnt = CreateAngledFont(lfHeight, 0, menu_data->menu_font_name);
		
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
			///////////////////////////////////////////////////////
			//	Fill in the menu window                          //
			///////////////////////////////////////////////////////
			rcSep.top 	= lpDs->rcItem.top    ;
			rcSep.left	= lpDs->rcItem.left   ;
			rcSep.bottom	= lpDs->rcItem.bottom ;
			rcSep.right	= lpDs->rcItem.right  ;


			HBRUSH hBrush = CreateSolidBrush(RGB(195, 195, 195));
			FillRect(lpDs->hDC, &rcSep, hBrush);
			DeleteObject(hBrush);

			///////////////////////////////////////////////////////
			//	barra al costado                                 //
			///////////////////////////////////////////////////////
			rcSep.top 		= lpDs->rcItem.top    ;
			rcSep.left		= lpDs->rcItem.left   ;
			rcSep.bottom	= lpDs->rcItem.bottom ;
			rcSep.right		= rcSep.left + border_w  ;

			hBrush = CreateSolidBrush(RGB(128, 128, 128));
			FillRect(lpDs->hDC, &rcSep, hBrush);
			DeleteObject(hBrush);

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
					rcSep.left		= lpDs->rcItem.left   + text_off;
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
					//	Side bar text & logo                             //
					///////////////////////////////////////////////////////
					if(lpDs->hwndItem == (HWND)main_menu)
					{
						
					
						int PointSize = 7;
						int DPI = 72;
						int lfHeight = -MulDiv(PointSize, GetDeviceCaps(lpDs->hDC, LOGPIXELSY), DPI);

						HFONT hfnt = CreateAngledFont(lfHeight, 90, "Verdana");
						LPCTSTR lpszLable = "MENU HELPER";
						SetBkMode(lpDs->hDC, TRANSPARENT);
						COLORREF prevColor = SetTextColor(lpDs->hDC, RGB(190,190,190));
						HFONT hfntPrev = SelectObject(lpDs->hDC, hfnt);

						BOOL ok = TextOut(lpDs->hDC, 0 , 200 , lpszLable, lstrlen(lpszLable));		//Restore defaults
						SelectObject(lpDs->hDC, hfntPrev);
						SetTextColor(lpDs->hDC, prevColor);
						SetBkMode(lpDs->hDC, OPAQUE);
						///////////////////////////////////////////////////////
						//	Put logo                                         //
						///////////////////////////////////////////////////////
						HBITMAP BMPlogo = LoadImage(hInst, MAKEINTRESOURCE(ID_LOGO), IMAGE_BITMAP,0, 0, LR_LOADMAP3DCOLORS);

						HDC hDC = CreateCompatibleDC(lpDs->hDC);
    					SelectObject(hDC, BMPlogo);
    					BitBlt(lpDs->hDC, 0, 0, 25, 25, hDC,0, 0, SRCCOPY);

    					DeleteDC(hDC);
						
    				}
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
							rcSep.left		= lpDs->rcItem.left   + text_off;
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
							rcSep.left		= lpDs->rcItem.left   + text_off - 45 ;
							rcSep.bottom	= lpDs->rcItem.bottom;
							rcSep.right		= lpDs->rcItem.right;
	
							int PointSize = 8;
							int DPI = 72;
							int lfHeight = -MulDiv(PointSize, GetDeviceCaps(lpDs->hDC, LOGPIXELSY), DPI);

							HFONT hfnt = CreateAngledFont(lfHeight, 0, menu_data->menu_font_name);
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
				rcSep.left		= lpDs->rcItem.left + border_w + 8;
				rcSep.bottom	= lpDs->rcItem.bottom - middle +1;
				rcSep.right		= lpDs->rcItem.right;

				hBrush = CreateSolidBrush(RGB(222, 222, 222));
				FillRect(lpDs->hDC, &lpDs->rcItem, hBrush);
				DeleteObject(hBrush);
				
				rcSep.top -=1;
				rcSep.bottom -= 1;
				
				hBrush = CreateSolidBrush(RGB(80, 80, 80));
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

				hBrush = CreateSolidBrush(RGB(222, 222, 222));
				FillRect(lpDs->hDC, &rcSep, hBrush);
				DeleteObject(hBrush);
				
				rcSep.top -=1;
				rcSep.bottom -= 1;
				
				hBrush = CreateSolidBrush(RGB(80, 80, 80));
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
	int pic_w;
	int pic_h;
	//	copia

	temp_hwnd = lpDs->hwndItem;
	hDC = CreateCompatibleDC(lpDs->hDC);	// create a memory DC

	switch(lpDs->CtlID)
		{

		case 330: //main_menu
			temp_bmp[0] = g_hBmpUp;
			temp_bmp[1] = g_hBmpDown;
			temp_bmp[2] = g_hBmpHot;
			pic_w		= 75;
			pic_h		= 30;
			break;
		case 331: //opti_button:
			temp_bmp[0] = plus_up;
			temp_bmp[1] = plus_dw;
			temp_bmp[2] = plus_ht;
			pic_w		= 9;
			pic_h		= 9;
			break;
		case 332: //plus_button:
			temp_bmp[0] = opti_up;
			temp_bmp[1] = opti_dw;
			temp_bmp[2] = opti_ht;
			pic_w		= 9;
			pic_h		= 9;
			break;

		}
	//end switch
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
			
			BitBlt(lpDs->hDC, 0, 0, pic_w, pic_h, hDC, 0, 0, SRCCOPY);
			DeleteDC(hDC);
		}

	return 0;
}
///////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////
static HFONT CreateAngledFont(int fontHeight, int fontAngle, LPCTSTR lpfontFace)
{
	////////////////////////////////////////////////////////////////////////////
	// Prepare logical font                                                   //
	////////////////////////////////////////////////////////////////////////////

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
///////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////
int menuType_fromID(HMENU hMenu, UINT id)
{
	HMENU hSubMenu	= NULL;
	HMENU hSubChild	= NULL;
	int isDone = 0;
	/*
	fprintf(outfile,"-----------------------------------------------------------------------\n");
	fprintf(outfile,"PIDE MENUID:\t%d HWND:\t%d\n",id,hMenu);
	fprintf(outfile,"-----------------------------------------------------------------------\n");
	*/
	int iCount = GetMenuItemCount(hMenu);
	for ( int i = 0; i < iCount; i++) 
	{
		if ( id == GetMenuItemID(hMenu, i) ) 
		{
			//wID found we'r on a normal menuitem
			//Normal menu item
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
//                                                                               //
//                                                                               //
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

	if(img_width > 32 || img_height > 32 || img_width != img_height)
	{
		char errorMsg[1024];
		wsprintf(errorMsg,"The image: %s has one of the following errors: \nThe size is bigger than 128 pixels \nThe image width and height are not the same", image_path);
		MessageBox(main_window,errorMsg,"Image size error !",MB_OK);
		//return NULL;
	}
	else
	{
			menu_data_ptr->menu_width	= img_width;
			menu_data_ptr->menu_height	= 33;

	}
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
			FIBITMAP *fg = FreeImage_Load(fif, image_path, 0);
			// image (a) : use a checkerboard background
			FIBITMAP *display_dib_a = FreeImage_Composite(fg,FALSE,NULL,NULL); 
			// image (b) : use the image file background if there is one 
			FIBITMAP *display_dib_b = FreeImage_Composite(fg, TRUE,NULL,NULL); 
			// image (c) : use a user specified background 
			RGBQUAD appColor; // = { 128, 128, 128, 0 }; 
				appColor.rgbRed			= GetRValue(bgColor);
				appColor.rgbGreen		= GetGValue(bgColor);
				appColor.rgbBlue		= GetBValue(bgColor);
				appColor.rgbReserved	= 0;
			FIBITMAP *display_dib_c = FreeImage_Composite(fg, FALSE, &appColor,NULL);

			// Copy to an HBITMAP
			// Release all GDI objects
			tmpBMP = CreateDIBitmap(hDCDesktop, FreeImage_GetInfoHeader(display_dib_c),CBM_INIT, FreeImage_GetBits(display_dib_c), FreeImage_GetInfo(display_dib_c), DIB_RGB_COLORS);
			FreeImage_Unload(fg);
			ReleaseDC(NULL, hDCDesktop );
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
	/*
	FIBITMAP *fg = FreeImage_Load(fif, image_path, 0);
	// image (a) : use a checkerboard background 
	FIBITMAP *display_dib_a = FreeImage_Composite(fg,FALSE,NULL,NULL); 
	// image (b) : use the image file background if there is one 
	FIBITMAP *display_dib_b = FreeImage_Composite(fg, TRUE,NULL,NULL); 
	// image (c) : use a user specified background 
	RGBQUAD appColor = { 128, 128, 128, 0 }; 
	FIBITMAP *display_dib_c = FreeImage_Composite(fg, FALSE, &appColor,NULL);

	//loaded_bmp = LoadImage(hInst,image_path,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
			
	loaded_bmp = CreateDIBitmap(hDCDesktop, FreeImage_GetInfoHeader(display_dib_c),CBM_INIT, FreeImage_GetBits(display_dib_c), FreeImage_GetInfo(display_dib_c), DIB_RGB_COLORS);
	FreeImage_Unload(fg);
			//ok
		//loaded_bmp = CreateDIBitmap(hDCDesktop, FreeImage_GetInfoHeader(dib),CBM_INIT, FreeImage_GetBits(dib), FreeImage_GetInfo(dib), DIB_RGB_COLORS);
			
			//FreeImage_Unload(dib);
	ReleaseDC(NULL, hDCDesktop );
	*/
	ReleaseDC(NULL, hDCDesktop );
	return NULL;


}
///////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////
void DrawTransparentBitmap(HDC hdc, HBITMAP hBitmap, short xStart,short yStart, COLORREF cTransparentColor)
{

   BITMAP     bm;
   COLORREF   cColor;
   HBITMAP    bmAndBack, bmAndObject, bmAndMem, bmSave;
   HBITMAP    bmBackOld, bmObjectOld, bmMemOld, bmSaveOld;
   HDC        hdcMem, hdcBack, hdcObject, hdcTemp, hdcSave;
   POINT      ptSize;

   hdcTemp = CreateCompatibleDC(hdc);
   SelectObject(hdcTemp, hBitmap);   // Select the bitmap

   GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&bm);
   ptSize.x = bm.bmWidth;            // Get width of bitmap
   ptSize.y = bm.bmHeight;           // Get height of bitmap
   DPtoLP(hdcTemp, &ptSize, 1);      // Convert from device

                                     // to logical points

   // Create some DCs to hold temporary data.
   hdcBack   = CreateCompatibleDC(hdc);
   hdcObject = CreateCompatibleDC(hdc);
   hdcMem    = CreateCompatibleDC(hdc);
   hdcSave   = CreateCompatibleDC(hdc);

   // Create a bitmap for each DC. DCs are required for a number of
   // GDI functions.

   // Monochrome DC
   bmAndBack   = CreateBitmap(ptSize.x, ptSize.y, 1, 1, NULL);

   // Monochrome DC
   bmAndObject = CreateBitmap(ptSize.x, ptSize.y, 1, 1, NULL);

   bmAndMem    = CreateCompatibleBitmap(hdc, ptSize.x, ptSize.y);
   bmSave      = CreateCompatibleBitmap(hdc, ptSize.x, ptSize.y);

   // Each DC must select a bitmap object to store pixel data.
   bmBackOld   = SelectObject(hdcBack, bmAndBack);
   bmObjectOld = SelectObject(hdcObject, bmAndObject);
   bmMemOld    = SelectObject(hdcMem, bmAndMem);
   bmSaveOld   = SelectObject(hdcSave, bmSave);

   // Set proper mapping mode.
   SetMapMode(hdcTemp, GetMapMode(hdc));

   // Save the bitmap sent here, because it will be overwritten.
   BitBlt(hdcSave, 0, 0, ptSize.x, ptSize.y, hdcTemp, 0, 0, SRCCOPY);

   // Set the background color of the source DC to the color.
   // contained in the parts of the bitmap that should be transparent
   cColor = SetBkColor(hdcTemp, cTransparentColor);

   // Create the object mask for the bitmap by performing a BitBlt
   // from the source bitmap to a monochrome bitmap.
   BitBlt(hdcObject, 0, 0, ptSize.x, ptSize.y, hdcTemp, 0, 0,
          SRCCOPY);

   // Set the background color of the source DC back to the original
   // color.
   SetBkColor(hdcTemp, cColor);

   // Create the inverse of the object mask.
   BitBlt(hdcBack, 0, 0, ptSize.x, ptSize.y, hdcObject, 0, 0,
          NOTSRCCOPY);

   // Copy the background of the main DC to the destination.
   BitBlt(hdcMem, 0, 0, ptSize.x, ptSize.y, hdc, xStart, yStart,
          SRCCOPY);

   // Mask out the places where the bitmap will be placed.
   BitBlt(hdcMem, 0, 0, ptSize.x, ptSize.y, hdcObject, 0, 0, SRCAND);

   // Mask out the transparent colored pixels on the bitmap.
   BitBlt(hdcTemp, 0, 0, ptSize.x, ptSize.y, hdcBack, 0, 0, SRCAND);

   // XOR the bitmap with the background on the destination DC.
   BitBlt(hdcMem, 0, 0, ptSize.x, ptSize.y, hdcTemp, 0, 0, SRCPAINT);

   // Copy the destination to the screen.
   BitBlt(hdc, xStart, yStart, ptSize.x, ptSize.y, hdcMem, 0, 0,
          SRCCOPY);

   // Place the original bitmap back into the bitmap sent here.
   BitBlt(hdcTemp, 0, 0, ptSize.x, ptSize.y, hdcSave, 0, 0, SRCCOPY);

   // Delete the memory bitmaps.
   DeleteObject(SelectObject(hdcBack, bmBackOld));
   DeleteObject(SelectObject(hdcObject, bmObjectOld));
   DeleteObject(SelectObject(hdcMem, bmMemOld));
   DeleteObject(SelectObject(hdcSave, bmSaveOld));

   // Delete the memory DCs.
   DeleteDC(hdcMem);
   DeleteDC(hdcBack);
   DeleteDC(hdcObject);
   DeleteDC(hdcSave);
   DeleteDC(hdcTemp);
}

HBITMAP CreateBitmapMask(HBITMAP hbmColour, COLORREF crTransparent)
{

    HDC hdcMem, hdcMem2, hdcMem3;
    HBITMAP hbmMask;
    HBITMAP hbmBack;
    BITMAP bm;
	
	///////////////////////////////////////////////////////////////////////////////////
    // Create monochrome (1 bit) mask bitmap.
	///////////////////////////////////////////////////////////////////////////////////

    GetObject(hbmColour, sizeof(BITMAP), &bm);
    hbmMask = CreateBitmap(bm.bmWidth, bm.bmHeight, 1, 1, NULL);
    hbmBack = CreateBitmap(bm.bmWidth, bm.bmHeight, 1, 24, NULL);

	///////////////////////////////////////////////////////////////////////////////////
    // Get some HDCs that are compatible with the display driver
	///////////////////////////////////////////////////////////////////////////////////

    hdcMem	= CreateCompatibleDC(0);
    hdcMem2	= CreateCompatibleDC(0);
    hdcMem3	= CreateCompatibleDC(0);

    SelectBitmap(hdcMem, hbmColour);
    SelectBitmap(hdcMem2, hbmMask);
    SelectBitmap(hdcMem3, hbmBack);

	///////////////////////////////////////////////////////////////////////////////////
    //Copy of original !!!
	///////////////////////////////////////////////////////////////////////////////////
    BitBlt(hdcMem3, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);
    // Set the background colour of the colour image to the colour
    // you want to be transparent.
    SetBkColor(hdcMem, crTransparent);

    // Copy the bits from the colour image to the B+W mask... everything
    // with the background colour ends up white while everythig else ends up
    // black...Just what we wanted.

    BitBlt(hdcMem2, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem3, 0, 0, SRCCOPY);

    // Take our new mask and use it to turn the transparent colour in our
    // original colour image to black so the transparency effect will
    // work right.
    BitBlt(hdcMem, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem2, 0, 0, SRCINVERT);

    // Clean up.

    DeleteDC(hdcMem);
    DeleteDC(hdcMem2);

    return hbmMask;

}

///////////////////////////////////////////////////////////////////////////////////
// This draws a bitmap in a solid backgound color of the same                    //
// size than the icon and returns an HBITMAP                                     //
///////////////////////////////////////////////////////////////////////////////////
void drawIconToBitmap(HBITMAP bmSrc, char * FileName, int size, COLORREF compositeColor)
{
	SHFILEINFO sfi;
	HDC        hMemDC;
	HBITMAP    hBitmap, hbmOld,bufferh;
	HDC        hDCDesktop;
	HBRUSH     hOldBrush;
	RECT       rc_a;

	rc_a.top		= 0;
	rc_a.left		= 0;
	rc_a.bottom		= size;
	rc_a.right		= size;

	#define BASIC_SHGFI_FLAGS SHGFI_TYPENAME | SHGFI_SHELLICONSIZE | SHGFI_SYSICONINDEX | SHGFI_DISPLAYNAME | SHGFI_EXETYPE


	//MessageBox(main_window,FileName,"Error de tipo:",MB_OK);
	//LPSTR pFileName = "c:\windows\calc.exe";
	hDCDesktop = GetDC( GetDesktopWindow() );

	///////////////////////////////////////////////////////////////////////////////////
	// Create bitmap                                                                 //
	///////////////////////////////////////////////////////////////////////////////////
	hBitmap = bmSrc; //CreateCompatibleBitmap( hDCDesktop, 96, iNumFiles * 32 );

	ReleaseDC( GetDesktopWindow(), hDCDesktop );
	hMemDC	= CreateCompatibleDC( NULL );				// Create screen DC

	bufferh	= CreateCompatibleBitmap(hMemDC, size,size);
	hbmOld	= SelectObject( hMemDC, hBitmap );			// Select memory bitmap


	///////////////////////////////////////////////////////////////////////////////////
	// Select brush for deleting the memory bitmap                                   //
	///////////////////////////////////////////////////////////////////////////////////
	hOldBrush = CreateSolidBrush(compositeColor);
	///////////////////////////////////////////////////////////////////////////////////
	// Delete memory bitmap                                                          //
	///////////////////////////////////////////////////////////////////////////////////
	FillRect(hMemDC,&rc_a,hOldBrush);
	//Rectangle( hMemDC, 0, 0, 32,32 );
	SelectObject( hMemDC, hOldBrush );


	if(size == 16)
	{
		//Small icon
		SHGetFileInfo( 
					FileName,
					0,
					&sfi,
					sizeof( sfi ),
					SHGFI_ICON | SHGFI_SMALLICON 
					);

    	DrawIconEx(hMemDC,0,0,sfi.hIcon,size,size,0,NULL,DI_NORMAL);
		// Destroy icon   
		DestroyIcon(sfi.hIcon);
	}
	if(size == 32)
	{
    	//Large icon
    	SHGetFileInfo(
					FileName,
					0,
					&sfi,
					sizeof( sfi ),
					SHGFI_ICON | SHGFI_LARGEICON
					); 
		// SHGFI_LARGEICON = 0
    	DrawIconEx( hMemDC,	0, 0,sfi.hIcon, size, size, 0, NULL, DI_NORMAL );
		// Destroy icon
    	DestroyIcon( sfi.hIcon );                           
	}
	SelectObject( hMemDC, hbmOld );        // Old bitmap back to memory DC
	DeleteDC    ( hMemDC );                // Destroy memory DC
	//Preserve bitmap
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

