///////////////////////////////////////////////////////////////////////////////////
// Título:      		manage_menu.h                                            //
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

#ifndef __MANAGE_MENU_H__
#define __MANAGE_MENU_H__

///////////////////////////////////////////////////////////////////////////////
// Constants                                                                 //
///////////////////////////////////////////////////////////////////////////////
#define CS_DROPSHADOW	0x20000	// This window style can make us drop a shadow under our frame
#define IDC_TREE1		91331
///////////////////////////////////////////////////////////////////////////////
// Menu Manager functions                                                    //
///////////////////////////////////////////////////////////////////////////////
//const char manager_menu_ClassName[] = "manager_menu_windowClass";

IXMLDOMDocument xmlTreeDoc;

BOOL manage_menu(HINSTANCE hInstance,HWND parent);
LRESULT CALLBACK main_menu_wndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

BOOL createControls(HINSTANCE hInstance, HWND hwndParent);
LRESULT CALLBACK treeWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

BOOL buildTree(IXMLDOMNodeList *node, HTREEITEM rootMenu, HWND parentHwnd);




///////////////////////////////////////////////////////////////////////////////
// Window Procedure here's where messages go 	                             //        
///////////////////////////////////////////////////////////////////////////////
LRESULT WINAPI	btnProc		(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL			initBtnClass(HINSTANCE hInst);

///////////////////////////////////////////////////////////////////////////////
// Button Superclass                                                         //
///////////////////////////////////////////////////////////////////////////////
#define BUTTON_CLASS "ButtonCtrl32" // Class Name
#define TOTAL_BUTTONS         3
WNDPROC btnOldProc;

typedef struct tagBUTTONDETAILS
{
	HWND	CtrHwnd;
	int		ResourceId;			// Holds button "UP" BMP image ID
    UINT	CtrId;				// HMENU value in CreateWindow()
    HBITMAP	hBm[4];				// Holds HBITMAP for each state

}BUTTONDETAILS, *LPBUTTONDETAILS;


int DrawButtons(LPARAM lParam);
BITMAP 	LoadOwnerDrawBitmaps(BUTTONDETAILS *BttnDetails);


///////////////////////////////////////////////////////////////////////////////
// Wallpaper                                                         //
///////////////////////////////////////////////////////////////////////////////

void wallpaper(HDC hdc,RECT *lprc,int idbitmap);
#endif
