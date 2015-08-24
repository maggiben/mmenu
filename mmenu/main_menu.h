///////////////////////////////////////////////////////////////////////////////////
// Título:      		main_menu.h                                              //
// Author:		        Benjamin Maggi                                           //
// Descripcion: 		A customizable Menu                                      //
// Org. Date:           17/01/2005                                               //
// Last Modification:   13/03/2008                                               //
// Ver:                 0.9.1                                                    //
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
///////////////////////////////////////////////////////////////////////////////////

#ifndef __MAIN_MENU_h__
#define __MAIN_MENU_h__

#ifndef __LCC__
#define __LCC__
#endif


#define CUR_MENU 2
#define NEW_MENU 1

typedef struct _MENU_THEME
{
	COLORREF	menu_theme_bgColor;
	COLORREF	menu_theme_fgColor;
	COLORREF	menu_theme_3dLight;
	COLORREF	menu_theme_3dDark;
	COLORREF	menu_theme_borderColor;
	COLORREF	menu_theme_sidebarColor;
	int			menu_theme_sidebar_width;
	HBITMAP		menu_theme_main_icon;
	TCHAR		*menu_theme_font;
	int			menu_theme_font_height;
	COLORREF	menu_theme_font_color;
	COLORREF	menu_theme_font_hilite_color;
	BYTE		menu_theme_alpha;
	HBITMAP		menu_theme_background;
	HBITMAP		menu_theme_btnUp;
	HBITMAP		menu_theme_btnDw;
	HBITMAP		menu_theme_btnHt;
	BOOL		menu_theme_border;
} MENU_THEME, *LPMENU_THEME;

///////////////////////////////////////////////////////////////////////////////////
// Main menu structure it handles all the nessesary data                         //
// We will make a linked list of items                                           //
///////////////////////////////////////////////////////////////////////////////////
typedef struct _MENU_DATA
{
	//////////////////////////////////////////////////////////////////////////
	// List managment propertys                                             //
	//////////////////////////////////////////////////////////////////////////
	void *				nextItem;				//point to next MENU_DATA
	void *				prevItem;				//point to prev MENU_DATA
	HMENU				menu_handle;
	HMENU				wID;
	IXMLDOMElement *	xmlMenuItem;
	//////////////////////////////////////////////////////////////////////////
	// Drawing & extra information                                          //
	//////////////////////////////////////////////////////////////////////////
	LPWSTR				label;				// Menu Label 2015
	LPWSTR				menu_capti;			// Menu Caption
	LPWSTR				menu_exec;			// Executable Path
	LPWSTR				imagePath;			// Image Path 2015
	HBITMAP				menu_image;			// Plain bitmap
	HBITMAP				menu_imageOvr;		// Image to show when menu is selected.
	HBITMAP				menu_imageOut;		// Image to show when menu is not selected.
	UINT				menu_imgType;		// Type of image Icom or Bitmap
	RECT				menu_imgSize;		// The image size
	BOOL				menu_imask;			//
	HFONT				menu_font;			// Menu font
	LPWSTR				menu_font_name;		// Menu Item Font name
	int					menu_font_size;     // font point size
	LOGFONT				menu_logFont;
	COLORREF			menu_fontColor;
	HFONT				menu_fsize;			
	COLORREF			menu_color;			//
	COLORREF			menu_selor;			
	int					menu_width;			// menu item width
	int					menu_height;		// menu item height
	int					menu_popup;
	UINT				menu_type;			// Menu item thye MF_SEPERATOR popup etc... added 2006
} MENU_DATA, *LPMENU_DATA;


BOOL create_Controls(HINSTANCE main_inst, HWND hwnd_parent, int skin_tpy);

LRESULT CALLBACK	ButtonProc					(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
RECT				MapDialogControlRect		(HWND, int, BOOL);

#endif
