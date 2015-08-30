///////////////////////////////////////////////////////////////////////////////////
// Título:      		draw.h                                                   //
// Author:		        Benjamin Maggi                                           //
// Descripcion: 		XML Handling functions                                   //
// Org. Date:           30/05/2009                                               //
// Last Modification:   30/05/2009                                               //
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

#pragma once

#ifndef __DRAW_H__
#define __DRAW_H__

#include <windows.h>
#include "MenuBuilder.h"

#define MF_SIDEBAR		0x0FFFF1F1L
#define SIDEBAR_WIDTH	30

class DrawMenuX
{
private:

	HWND MainWindowHandle;
	//DrawMenuX() { } // private default constructor
	BOOL DrawMenuChild(DRAWITEMSTRUCT *lpDrawItem, MENU_DATA *lpMenuData);
	BOOL DrawMenuRoot(DRAWITEMSTRUCT *lpDrawItem, MENU_DATA *lpMenuData);
	BOOL DrawMenuSeparator(DRAWITEMSTRUCT *lpDrawItem, MENU_DATA *lpMenuData);
	BOOL DrawMenuSidebar(DRAWITEMSTRUCT *lpDrawItem, MENU_DATA *lpMenuData);
	BOOL DrawRootMenuItem(DRAWITEMSTRUCT *lpDrawItem, MENU_DATA *lpMenuData);
	BOOL DrawMenuArrow(HDC inHDC, RECT *inDestR, BOOL inIsEnabled);

public:
	DrawMenuX(HWND hWnd) {
		this->MainWindowHandle = hWnd;
	}
	BOOL DrawMenu(DRAWITEMSTRUCT *lpDrawItem, MENU_DATA *lpMenuData);
};

BOOL DrawMenu			(DRAWITEMSTRUCT *lpDrawItem, MENU_DATA *lpMenuData);
BOOL DrawMenuChild		(DRAWITEMSTRUCT *lpDrawItem, MENU_DATA *lpMenuData);
BOOL DrawMenuRoot		(DRAWITEMSTRUCT *lpDrawItem, MENU_DATA *lpMenuData);
BOOL DrawMenuSeparator	(DRAWITEMSTRUCT *lpDrawItem, MENU_DATA *lpMenuData);
BOOL DrawMenuSidebar	(DRAWITEMSTRUCT *lpDrawItem, MENU_DATA *lpMenuData);
BOOL DrawRootMenuItem	(DRAWITEMSTRUCT *lpDrawItem, MENU_DATA *lpMenuData);

// 
void OnNcPaint			(HWND hWnd, HRGN hRgn);
#endif
