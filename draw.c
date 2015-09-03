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

#include <windows.h>		// windows standar
#include <windowsx.h>       // Extra features 
#include <tchar.h>          
#include <wchar.h>			// Unicode support
#include "main_menu.h"
#include "graphic.h"
#include "draw.h"

extern MENU_THEME main_menu_theme;
extern HWND main_menu;


static void _drawMenuArrow(HDC inHDC, RECT *inDestR, BOOL inIsEnabled);

///////////////////////////////////////////////////////////////////////////////////
//                                                                               //
///////////////////////////////////////////////////////////////////////////////////
BOOL DrawMenuChild(DRAWITEMSTRUCT *lpDrawItem, MENU_DATA *lpMenuData)
{
	RECT		rcSep;
	HBRUSH		hBrush;
	COLORREF	gradient;	// Used to store the gradient value
	HPEN		colorPen;	
	int			point_x		= 0;
	int			point_y 	= 0;

	if(lpDrawItem->itemState & ODS_SELECTED)
	{
		/////////////////////////////////////////////////////////////////
		//	Sidebar Fillup use solid color                             //
		/////////////////////////////////////////////////////////////////
		rcSep.top 		= lpDrawItem->rcItem.top;
		rcSep.left		= lpDrawItem->rcItem.left;
		rcSep.bottom	= lpDrawItem->rcItem.bottom;
		rcSep.right		= main_menu_theme.menu_theme_sidebar_width;

		hBrush = CreateSolidBrush(main_menu_theme.menu_theme_sidebarColor);

		FillRect(lpDrawItem->hDC, &rcSep, hBrush);
		DeleteObject(hBrush);
		/////////////////////////////////////////////////////////////////
		//	Fill the Selected Menu Item with solid color               //
		/////////////////////////////////////////////////////////////////
		rcSep.top 		= lpDrawItem->rcItem.top    ;
		rcSep.left		= lpDrawItem->rcItem.left + 2;
		rcSep.bottom	= lpDrawItem->rcItem.bottom ;
		rcSep.right		= lpDrawItem->rcItem.right  ;

		/////////////////////////////////////////////////////////////////
		//	Menu Item Border                                           //
		/////////////////////////////////////////////////////////////////
		rcSep.top 		= lpDrawItem->rcItem.top;
		rcSep.left		= lpDrawItem->rcItem.left + 1; // Small separation from the sidebar
		rcSep.bottom	= lpDrawItem->rcItem.bottom;
		rcSep.right		= lpDrawItem->rcItem.right;

		hBrush = CreateSolidBrush(lpMenuData->menu_color); // Border Color
		HBRUSH hOldBrush = SelectObject(lpDrawItem->hDC, hBrush);
		RoundRect(lpDrawItem->hDC, rcSep.left, rcSep.top, rcSep.right, rcSep.bottom, 4, 4);
		SelectObject(lpDrawItem->hDC, hOldBrush);
		DeleteObject(hBrush);


		/////////////////////////////////////////////////////////////////
		//	Put the Icon                                               //
		/////////////////////////////////////////////////////////////////
		BITMAP bm;
		GetObject(lpMenuData->menu_imageOvr, sizeof(BITMAP), (LPSTR)&bm);
		point_y = lpDrawItem->rcItem.top + ((lpDrawItem->rcItem.bottom - lpDrawItem->rcItem.top) / 2) - (bm.bmHeight/2); // Y position
		point_x = lpDrawItem->rcItem.left + (((lpDrawItem->rcItem.left + main_menu_theme.menu_theme_sidebar_width) / 2) - (bm.bmWidth/2));	// X position of the icon

		HDC     hMemDC = CreateCompatibleDC(lpDrawItem->hDC);
		HGDIOBJ hOld   = SelectObject(hMemDC, lpMenuData->menu_imageOvr);
		BITMAP bmp;
		GetObject(lpMenuData->menu_imageOut, sizeof(BITMAP), (LPSTR) &bmp);
	
		BLENDFUNCTION pixelblend = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };

    	AlphaBlend(lpDrawItem->hDC, point_x, point_y, bmp.bmWidth, bmp.bmHeight, hMemDC, 0, 0, bmp.bmWidth, bmp.bmHeight, pixelblend); // blend with per-pixel alpha
	
		SelectObject(hMemDC, hOld);
		DeleteObject(hMemDC);

		/////////////////////////////////////////////////////////////////
		//	Finally Draw the caption                                   //
		/////////////////////////////////////////////////////////////////
		rcSep.top 		= lpDrawItem->rcItem.top;
		rcSep.left		= lpDrawItem->rcItem.left  + (main_menu_theme.menu_theme_sidebar_width + 4);
		rcSep.bottom	= lpDrawItem->rcItem.bottom;
		rcSep.right		= lpDrawItem->rcItem.right;
				
		int PointSize = lpMenuData->menu_font_size;
		int DPI = 72;
		int lfHeight = -MulDiv(PointSize, GetDeviceCaps(lpDrawItem->hDC, LOGPIXELSY), DPI);

		COLORREF prevColor = SetTextColor(lpDrawItem->hDC, lpMenuData->menu_fontColor); //font color
		HFONT hfnt = CreateAngledFont(lfHeight, 0, lpMenuData->menu_font_name,NULL);
		
		HFONT hfntPrev = SelectObject(lpDrawItem->hDC, hfnt);

		SetBkMode(lpDrawItem->hDC,TRANSPARENT);
		DrawText(lpDrawItem->hDC,lpMenuData->menu_capti, -1, &rcSep,DT_SINGLELINE | DT_LEFT | DT_VCENTER);
		SelectObject(lpDrawItem->hDC, hfntPrev);
		SetTextColor(lpDrawItem->hDC, prevColor);
		DeleteObject(hfnt);
	}
	else
	{
		///////////////////////////////////////////////////////
		//	Fill in the menu window                          //
		///////////////////////////////////////////////////////
		rcSep.top 	= lpDrawItem->rcItem.top;
		rcSep.left	= lpDrawItem->rcItem.left;
		rcSep.bottom	= lpDrawItem->rcItem.bottom;
		rcSep.right	= lpDrawItem->rcItem.right;


		HBRUSH hBrush = CreateSolidBrush(main_menu_theme.menu_theme_bgColor);		// Menuitem Bg color
		FillRect(lpDrawItem->hDC, &rcSep, hBrush);
		DeleteObject(hBrush);

		///////////////////////////////////////////////////////
		//	barra al costado solo en los childs              //
		///////////////////////////////////////////////////////
		rcSep.top 		= lpDrawItem->rcItem.top;
		rcSep.left		= lpDrawItem->rcItem.left;
		rcSep.bottom	= lpDrawItem->rcItem.bottom;
		rcSep.right		= rcSep.left + main_menu_theme.menu_theme_sidebar_width;

		hBrush = CreateSolidBrush(main_menu_theme.menu_theme_sidebarColor);				// Sidebar color
		FillRect(lpDrawItem->hDC, &rcSep, hBrush);
		DeleteObject(hBrush);

		///////////////////////////////////////////////////////
		//	draws a line betwen the sidebar & the menuitem   //
		///////////////////////////////////////////////////////
		int y_t = lpDrawItem->rcItem.top;
		int x_t = main_menu_theme.menu_theme_sidebar_width;
		HPEN saveObject = SelectObject(lpDrawItem->hDC,CreatePen(PS_SOLID, 1, main_menu_theme.menu_theme_borderColor));
		MoveToEx(lpDrawItem->hDC, x_t, y_t, NULL);
		LineTo  (lpDrawItem->hDC, x_t, y_t);
		LineTo  (lpDrawItem->hDC, x_t ,lpDrawItem->rcItem.bottom);
		DeleteObject(SelectObject(lpDrawItem->hDC, saveObject));

		///////////////////////////////////////////////////////
		//	Put the icon                                     //
		///////////////////////////////////////////////////////		
		BITMAP bm;
		GetObject(lpMenuData->menu_imageOut, sizeof(BITMAP), (LPSTR)&bm);
		point_y = lpDrawItem->rcItem.top + ((lpDrawItem->rcItem.bottom - lpDrawItem->rcItem.top) / 2) - (bm.bmHeight/2); // Y position
		point_x = lpDrawItem->rcItem.left + (((lpDrawItem->rcItem.left + main_menu_theme.menu_theme_sidebar_width) / 2) - (bm.bmWidth/2));	// X position of the icon
		
		HDC     hMemDC = CreateCompatibleDC(lpDrawItem->hDC);
		HGDIOBJ hOld   = SelectObject(hMemDC, lpMenuData->menu_imageOut);
		BITMAP bmp;
		GetObject(lpMenuData->menu_imageOut, sizeof(BITMAP), (LPSTR) &bmp);
	
		BLENDFUNCTION pixelblend = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };

    	AlphaBlend(lpDrawItem->hDC, point_x, point_y, bmp.bmWidth, bmp.bmHeight, hMemDC, 0, 0, bmp.bmWidth, bmp.bmHeight, pixelblend); // blend with per-pixel alpha
	
		SelectObject(hMemDC, hOld);
		DeleteObject(hMemDC);
		///////////////////////////////////////////////////////
		//	Draw the item caption                            //
		///////////////////////////////////////////////////////	
		rcSep.top 		= lpDrawItem->rcItem.top;
		rcSep.left		= lpDrawItem->rcItem.left  + (main_menu_theme.menu_theme_sidebar_width + 4); //lpMenuData->menu_imgSize.right + 18; //+ text_off - 45 ;
		rcSep.bottom	= lpDrawItem->rcItem.bottom;
		rcSep.right		= lpDrawItem->rcItem.right;
				
		int PointSize = lpMenuData->menu_font_size;
		int DPI = 72;
		int lfHeight = -MulDiv(PointSize, GetDeviceCaps(lpDrawItem->hDC, LOGPIXELSY), DPI);

		COLORREF prevColor = SetTextColor(lpDrawItem->hDC, lpMenuData->menu_fontColor); //font color
		HFONT hfnt = CreateAngledFont(lfHeight, 0, lpMenuData->menu_font_name,NULL);
		
		HFONT hfntPrev = SelectObject(lpDrawItem->hDC, hfnt);

		SetBkMode(lpDrawItem->hDC,TRANSPARENT);
		DrawText (lpDrawItem->hDC,lpMenuData->menu_capti, -1, &rcSep,DT_SINGLELINE | DT_LEFT | DT_VCENTER);
		SelectObject(lpDrawItem->hDC, hfntPrev);
		SetTextColor(lpDrawItem->hDC, prevColor);
		DeleteObject(hfnt);

	}
	if(lpMenuData->menu_type == MF_POPUP)
	{
		#define OBM_MNARROW 32739
		HBITMAP hBitmap = (HBITMAP)LoadImage(NULL,MAKEINTRESOURCE(OBM_MNARROW), IMAGE_BITMAP, 0, 0,LR_DEFAULTSIZE | LR_SHARED);
		BITMAP bmp;
		GetObject(hBitmap, sizeof(BITMAP), (LPSTR) &bmp);

		RECT arrowDest;

		arrowDest.top = lpDrawItem->rcItem.top + ((lpDrawItem->rcItem.bottom - lpDrawItem->rcItem.top) / 2) - (bmp.bmHeight / 2);
		arrowDest.bottom = arrowDest.top + bmp.bmHeight;
		arrowDest.left = lpDrawItem->rcItem.right - (bmp.bmWidth + 0); // 1 = right margin
		arrowDest.right = arrowDest.left + bmp.bmWidth;

		//Draw the arrow
		_drawMenuArrow(lpDrawItem->hDC, &arrowDest, (lpDrawItem->itemState & ODS_SELECTED)?TRUE:FALSE);
		ExcludeClipRect(lpDrawItem->hDC, lpDrawItem->rcItem.left, lpDrawItem->rcItem.top, lpDrawItem->rcItem.right, lpDrawItem->rcItem.bottom);
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////
//                                                                               //
///////////////////////////////////////////////////////////////////////////////////
BOOL DrawMenuRoot(DRAWITEMSTRUCT *lpDrawItem, MENU_DATA *lpMenuData)
{
	RECT				rcSep;
	HBRUSH				hBrush		= NULL;
	COLORREF			gradient;
	HPEN				hpen		= NULL;
	HPEN				hpenOld		= NULL;
	int					point_x		= 0;
	int					point_y 	= 0;
	MENUITEMINFO		mii;
	MENU_DATA			menu_data;
	int					itemMargin  = 2;
	int                 icoMargin   = itemMargin + 2;
	
	if(!lpMenuData) { return FALSE; }
	if(lpDrawItem->itemState & ODS_SELECTED)
	{

		/////////////////////////////////////////////////////////////////
		//	Menu Item Border                                           //
		/////////////////////////////////////////////////////////////////
		rcSep.top 		= lpDrawItem->rcItem.top;
		rcSep.left		= lpDrawItem->rcItem.left + itemMargin; // Small separation from the sidebar
		rcSep.bottom	= lpDrawItem->rcItem.bottom;
		rcSep.right		= lpDrawItem->rcItem.right;

		// Create a green pen.
		hBrush = CreateSolidBrush(lpMenuData->menu_color); // Border Color
		HBRUSH hOldBrush = SelectObject(lpDrawItem->hDC, hBrush);
		RoundRect(lpDrawItem->hDC, rcSep.left, rcSep.top, rcSep.right, rcSep.bottom, 4, 4);
		SelectObject(lpDrawItem->hDC, hOldBrush);
		DeleteObject(hBrush);
		
		if(lpMenuData->menu_imageOvr)
		{
		BITMAP bm;
		GetObject(lpMenuData->menu_imageOvr, sizeof(BITMAP), (LPSTR)&bm);
		point_y = lpDrawItem->rcItem.top + ((lpDrawItem->rcItem.bottom - lpDrawItem->rcItem.top) / 2) - (bm.bmHeight/2); // Y position
		point_x = (lpDrawItem->rcItem.left) + icoMargin;	// X position of the icon
	
		HDC     hMemDC = CreateCompatibleDC(lpDrawItem->hDC);
		HGDIOBJ hOld   = SelectObject(hMemDC, lpMenuData->menu_imageOvr);
		BITMAP bmp;
		GetObject(lpMenuData->menu_imageOut, sizeof(BITMAP), (LPSTR) &bmp);
	
		BLENDFUNCTION pixelblend = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };

    	AlphaBlend(lpDrawItem->hDC, point_x, point_y, bmp.bmWidth, bmp.bmHeight, hMemDC, 0, 0, bmp.bmWidth, bmp.bmHeight, pixelblend); // blend with per-pixel alpha
	
		SelectObject(hMemDC, hOld);
		DeleteObject(hMemDC);
		}

		/////////////////////////////////////////////////////////////////
		//	text caption                                               //
		/////////////////////////////////////////////////////////////////
		SIZE extent;
		char* captionText = (char*)lpDrawItem->itemData;
		GetTextExtentPoint32(lpDrawItem->hDC,captionText, lstrlen(captionText), &extent);
		point_y = ((lpDrawItem->rcItem.bottom - lpDrawItem->rcItem.top) / 2) - (extent.cy/2);
		point_y += lpDrawItem->rcItem.top;

		int PointSize = lpMenuData->menu_font_size;
		int DPI = 72;
		int lfHeight = -MulDiv(PointSize, GetDeviceCaps(lpDrawItem->hDC, LOGPIXELSY), DPI);

		HFONT hfnt = CreateAngledFont(lfHeight, 0, lpMenuData->menu_font_name,NULL);

		HFONT hfntPrev = SelectObject(lpDrawItem->hDC, hfnt);

		if(lpMenuData->menu_imageOvr)
		{
		rcSep.top 		= lpDrawItem->rcItem.top;
		rcSep.left		= lpDrawItem->rcItem.left + lpMenuData->menu_imgSize.right + 12; // Icon + border
		rcSep.bottom	= lpDrawItem->rcItem.bottom;
		rcSep.right		= lpDrawItem->rcItem.right;
		}
		else
		{
			CopyRect(&rcSep, &lpDrawItem->rcItem);
			rcSep.left += 12; // 12 = text left padding
		}

		COLORREF prevColor = SetTextColor(lpDrawItem->hDC, main_menu_theme.menu_theme_font_hilite_color); //font color
		SetBkMode(lpDrawItem->hDC,TRANSPARENT);
		DrawText (lpDrawItem->hDC,captionText, -1, &rcSep,DT_SINGLELINE | DT_LEFT | DT_VCENTER);
		SelectObject(lpDrawItem->hDC, hfntPrev);
		SetTextColor(lpDrawItem->hDC, prevColor);
		DeleteObject(hfnt);

	}
	else
	{
		///////////////////////////////////////////////////////
		//	Fill in the menu window                          //
		///////////////////////////////////////////////////////
		rcSep.top 	= lpDrawItem->rcItem.top    ;
		rcSep.left	= lpDrawItem->rcItem.left;
		rcSep.bottom	= lpDrawItem->rcItem.bottom ;
		rcSep.right	= lpDrawItem->rcItem.right  + itemMargin;


		HBRUSH hBrush = CreateSolidBrush(main_menu_theme.menu_theme_bgColor);
		FillRect(lpDrawItem->hDC, &rcSep, hBrush);
		DeleteObject(hBrush);

		////////////////////////////////////////////////////////////////////
		//	Draws the icon holder & the separator For Main Menu           //
		////////////////////////////////////////////////////////////////////
		if(lpMenuData->menu_imageOut)
		{
			BITMAP bm;
			GetObject(lpMenuData->menu_imageOut, sizeof(BITMAP), (LPSTR)&bm);
			point_y = lpDrawItem->rcItem.top + ((lpDrawItem->rcItem.bottom - lpDrawItem->rcItem.top) / 2) - (bm.bmHeight/2); // Y position
			point_x = (lpDrawItem->rcItem.left) + icoMargin;	// X position of the icon
		
			//DrawTransparentBitmap(lpDs->hDC,menu_data.menu_imageOut,point_x,point_y,RGB(170,170,170));
			HDC     hMemDC = CreateCompatibleDC(lpDrawItem->hDC);
			HGDIOBJ hOld   = SelectObject(hMemDC, lpMenuData->menu_imageOut);
			BITMAP bmp;
			GetObject(lpMenuData->menu_imageOut, sizeof(BITMAP), (LPSTR) &bmp);
	
			BLENDFUNCTION pixelblend = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };

			AlphaBlend(lpDrawItem->hDC, point_x, point_y, bmp.bmWidth, bmp.bmHeight, hMemDC, 0, 0, bmp.bmWidth, bmp.bmHeight, pixelblend); // blend with per-pixel alpha
	
			SelectObject(hMemDC, hOld);
			DeleteObject(hMemDC);
		}


		///////////////////////////////////////////////////////
		//	Text & Captions goes here                        //
		///////////////////////////////////////////////////////
		SIZE extent;
		TCHAR *captionText = (TCHAR*)lpDrawItem->itemData;
		GetTextExtentPoint32(lpDrawItem->hDC,captionText, lstrlen(captionText), &extent);
		point_y = ((lpDrawItem->rcItem.bottom - lpDrawItem->rcItem.top) / 2) - (extent.cy/2);
		point_y += lpDrawItem->rcItem.top;
		
		if(lpMenuData->menu_imageOut)
		{
			rcSep.top 		= lpDrawItem->rcItem.top;
			rcSep.left		= lpDrawItem->rcItem.left + lpMenuData->menu_imgSize.right + 12; // Icon + border
			rcSep.bottom	= lpDrawItem->rcItem.bottom;
			rcSep.right		= lpDrawItem->rcItem.right;
		}
		else
		{
			CopyRect(&rcSep, &lpDrawItem->rcItem);
			rcSep.left += 12; // 12 = text left padding
		}

		int PointSize = lpMenuData->menu_font_size;
		int DPI = 72;
		int lfHeight = -MulDiv(PointSize, GetDeviceCaps(lpDrawItem->hDC, LOGPIXELSY), DPI);
		HFONT hfnt = CreateAngledFont(lfHeight, 0, lpMenuData->menu_font_name, NULL);
		HFONT hfntPrev = SelectObject(lpDrawItem->hDC, hfnt);

		COLORREF prevColor = SetTextColor(lpDrawItem->hDC, lpMenuData->menu_fontColor); //font color
		SetBkMode(lpDrawItem->hDC,TRANSPARENT);
		DrawText (lpDrawItem->hDC,captionText, -1, &rcSep,DT_SINGLELINE | DT_LEFT | DT_VCENTER);
		SelectObject(lpDrawItem->hDC, hfntPrev);
		SetTextColor(lpDrawItem->hDC, prevColor);
		DeleteObject(hfnt);

	}
	return TRUE;
}

BOOL DrawMenuSeparator(DRAWITEMSTRUCT *lpDrawItem, MENU_DATA *lpMenuData)
{
	RECT				rcSep;
	HBRUSH				hBrush		= NULL;
	COLORREF			gradient;
	HPEN				hpen		= NULL;
	HPEN				hpenOld		= NULL;
	POINT				mPnt;
	int					point_x		= 0;
	int					point_y 	= 0;
	MENUITEMINFO		mii;

	//////////////////////////////////////////////////////////////
	// MF_SEPARATOR for Root menu                               //
	//////////////////////////////////////////////////////////////
	if(lpDrawItem->hwndItem == (HWND)main_menu)
	{
		//////////////////////////////////////////////////////////////
		// a thin line of 2 pixels the height is                    //
		// bigger than 2 pixels so we need to calculate the middle  //
		//////////////////////////////////////////////////////////////
		hBrush = CreateSolidBrush(main_menu_theme.menu_theme_bgColor);		// Menu BG Color
		FillRect(lpDrawItem->hDC, &lpDrawItem->rcItem, hBrush);
		DeleteObject(hBrush);

		int middle 		= (lpDrawItem->rcItem.bottom - lpDrawItem->rcItem.top) / 2 ;
		rcSep.top 		= lpDrawItem->rcItem.top + middle ;
		rcSep.left		= lpDrawItem->rcItem.left + 8;
		rcSep.bottom	= lpDrawItem->rcItem.bottom - middle +1;
		rcSep.right		= lpDrawItem->rcItem.right;

		hBrush = CreateSolidBrush(main_menu_theme.menu_theme_3dLight);		// Light Color
		FillRect(lpDrawItem->hDC, &rcSep, hBrush);
		DeleteObject(hBrush);
				
		rcSep.top -=1;
		rcSep.bottom -= 1;
				
		hBrush = CreateSolidBrush(main_menu_theme.menu_theme_3dDark);		// Shadow Color
		FillRect(lpDrawItem->hDC, &rcSep, hBrush);
		DeleteObject(hBrush);
	}		
	//////////////////////////////////////////////////////////////
	// MF_SEPARATOR for Child menus                             //
	//////////////////////////////////////////////////////////////
	else
	{
		//////////////////////////////////////////////////////////////
		// a thin line of 2 pixels the height is                    //
		// bigger than 2 pixels so we need to calculate the middle  //
		//////////////////////////////////////////////////////////////
		RECT rcChild;
		CopyRect(&rcChild, &lpDrawItem->rcItem);
		rcChild.left += main_menu_theme.menu_theme_sidebar_width;
		hBrush = CreateSolidBrush(main_menu_theme.menu_theme_bgColor);		//menu_color_menu_background
		FillRect(lpDrawItem->hDC, &rcChild, hBrush);
		DeleteObject(hBrush);


		CopyRect(&rcChild, &lpDrawItem->rcItem);
		rcChild.right = rcChild.left + main_menu_theme.menu_theme_sidebar_width;
		hBrush = CreateSolidBrush(main_menu_theme.menu_theme_sidebarColor);		//menu_color_sidebar_background
		FillRect(lpDrawItem->hDC, &rcChild, hBrush);
		DeleteObject(hBrush);

		CopyRect(&rcChild, &lpDrawItem->rcItem);
		rcChild.right = rcChild.left + main_menu_theme.menu_theme_sidebar_width;
		HPEN saveObject = SelectObject(lpDrawItem->hDC,CreatePen(PS_SOLID, 1, main_menu_theme.menu_theme_borderColor));   //menu_color_border
		MoveToEx(lpDrawItem->hDC,rcChild.right, rcChild.top, NULL);
		LineTo(lpDrawItem->hDC, rcChild.right, rcChild.bottom);
		DeleteObject(SelectObject(lpDrawItem->hDC, saveObject));


		int middle 		= (lpDrawItem->rcItem.bottom - lpDrawItem->rcItem.top) / 2 ;
		rcSep.top 		= lpDrawItem->rcItem.top + middle ;
		rcSep.left		= lpDrawItem->rcItem.left + main_menu_theme.menu_theme_sidebar_width + 8;
		rcSep.bottom	= lpDrawItem->rcItem.bottom - middle +1;
		rcSep.right		= lpDrawItem->rcItem.right;

		hBrush = CreateSolidBrush(main_menu_theme.menu_theme_3dLight);		//menu_color_separator_3d_light
		FillRect(lpDrawItem->hDC, &rcSep, hBrush);
		DeleteObject(hBrush);
	
		rcSep.top -=1;
		rcSep.bottom -= 1;
			
		hBrush = CreateSolidBrush(main_menu_theme.menu_theme_3dDark);		//menu_color_separator_3d_shadow
		FillRect(lpDrawItem->hDC, &rcSep, hBrush);
		DeleteObject(hBrush);
	}
}

static void _drawMenuArrow(HDC inHDC, RECT *inDestR, BOOL inIsEnabled)
{
   //Create the DCs and Bitmaps we will need
   HDC arrowDC = CreateCompatibleDC(inHDC);
   HDC fillDC  = CreateCompatibleDC(inHDC);
   int arrowW  = inDestR->right - inDestR->left;
   int arrowH  = inDestR->bottom - inDestR->top;
   
   HBITMAP arrowBitmap    = CreateCompatibleBitmap(inHDC, arrowW, arrowH);//CreateDIBBitmap(inHDC, arrowW, arrowH);
   HBITMAP oldArrowBitmap = (HBITMAP)SelectObject(arrowDC, arrowBitmap);
   HBITMAP fillBitmap     = CreateCompatibleBitmap(inHDC, arrowW, arrowH);//CreateDIBBitmap(inHDC, arrowW, arrowH);
   HBITMAP oldFillBitmap  = (HBITMAP)SelectObject(fillDC, fillBitmap);

   //Set the offscreen arrow rect
   RECT tmpArrowR;
   SetRect(&tmpArrowR, 0, 0, arrowW, arrowH);

   //Draw the frame control arrow (The OS draws this as a black on
   //                              white bitmap mask)
   DrawFrameControl(arrowDC, &tmpArrowR, DFC_MENU, DFCS_MENUARROW);

   //Set the arrow color
   HBRUSH arrowBrush = inIsEnabled ? CreateSolidBrush(main_menu_theme.menu_theme_font_hilite_color) :
                                     CreateSolidBrush(main_menu_theme.menu_theme_borderColor);

   //Fill the fill bitmap with the arrow color
   FillRect(fillDC, &tmpArrowR, arrowBrush);

   //Blit the items in a masking fashion
   BitBlt(inHDC, inDestR->left, inDestR->top, arrowW, arrowH, fillDC, 0, 0, SRCINVERT);
   BitBlt(inHDC, inDestR->left, inDestR->top, arrowW, arrowH, arrowDC, 0, 0, SRCAND);
   BitBlt(inHDC, inDestR->left, inDestR->top, arrowW, arrowH, fillDC, 0, 0, SRCINVERT);

   //Clean up
   SelectObject(fillDC, oldFillBitmap);
   DeleteObject(fillBitmap);
   SelectObject(arrowDC, oldArrowBitmap);
   DeleteObject(arrowBitmap);
   DeleteDC(fillDC);
   DeleteDC(arrowDC);
}
