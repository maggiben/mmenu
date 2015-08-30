///////////////////////////////////////////////////////////////////////////////////
// Título:      		MenuBuilder.h                                            //
// Author:		        Benjamin Maggi                                           //
// Descripcion: 		XML Handling functions                                   //
// Org. Date:           28/08/2015                                               //
// Last Modification:   28/08/2015                                               //
// Ver:                 0.0.1                                                    //
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

#ifndef __MENUBUILDER_H__
#define __MENUBUILDER_H__

#include <windows.h>
#include "graphic.h"

HRESULT oleStrToAnsi(BSTR bstrIn, LPSTR * lpszOut);

class MenuItem : public Graphics {
private:
	Graphics graphics;
	MENUITEMINFO menuItemInfo;
public:
	//////////////////////////////////////////////////////////////////////////
	// List managment propertys                                             //
	//////////////////////////////////////////////////////////////////////////
	HMENU				handle;
	HMENU				wID;
	IXMLDOMElement *	xmlMenuItem;
	//////////////////////////////////////////////////////////////////////////
	// Drawing & extra information                                          //
	//////////////////////////////////////////////////////////////////////////
	LPWSTR				label;				// Menu Label 2015
	LPWSTR				executable;			// Executable Path
	LPWSTR				imagePath;			// Image Path 2015
	HBITMAP				image;				// Image
	HFONT				font;				// Menu font
	COLORREF			color;				// Foreground Color
	COLORREF			backgroundColor;	// Background Color
	COLORREF			borderColor;
	UINT				type;				// Menu item thye MF_SEPERATOR popup etc... added 2006

	HRESULT draw(DRAWITEMSTRUCT * lpDs);
	MenuItem(IXMLDOMNode *node, HMENU hMenu, UINT wID, UINT uPos, HMENU hSubMenu);
	~MenuItem() {}
};

class MenuBuilder
{
private:
	int build(IXMLDOMNodeList *node, HMENU rootMenu, UINT startIdsAt);
	HMENU menu;
public:
	HMENU Load(LPWSTR fname, BOOL bOptimizeMemory);
	HMENU getMenuHandle() {
		return this->menu;
	}
	MenuBuilder();
	~MenuBuilder();
};

class Pepe
{
private:
	LPWSTR message;
public:
	Pepe() {
		this->message = L"Hello";
	}
	LPWSTR GetMesg() {
		return this->message;
	}
};
#endif