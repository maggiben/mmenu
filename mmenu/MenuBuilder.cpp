///////////////////////////////////////////////////////////////////////////////////
// Título:      		MenuBuilder.cpp                                          //
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
// either version 2 of the License, or (at your option) any                      //
// later version.                                                                //
//                                                                               //
// This program is distributed in the hope that it will be useful,               //
// but WITHOUT ANY WARRANTY; without even the implied warranty of                //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                 //
// GNU General Public License for more details.                                  //
//                                                                               //
///////////////////////////////////////////////////////////////////////////////////

#define _CRT_SECURE_NO_WARNINGS

#include <msxml.h>			// MS XML Parser.
#include <map>

#include "MenuBuilder.h"
#include "main_menu.h"
#include "graphic.h"

using namespace std;
#define ISOWNERDRAW TRUE

MenuBuilder::MenuBuilder()
{
}
MenuBuilder::~MenuBuilder()
{
}

HMENU MenuBuilder::Load(LPWSTR fname, BOOL bOptimizeMemory)
{
	IXMLDOMDocument		*document = NULL;
	IXMLDOMParseError	*parseError = NULL;
	IXMLDOMElement		*element = NULL;
	BSTR				bstr = NULL;
	VARIANT_BOOL		status = VARIANT_FALSE;
	VARIANT				vSrc;
	MENUINFO			mi = { sizeof(mi), MIM_STYLE, MNS_DRAGDROP };

	if (menu) {
		return menu;
	}

	HRESULT hr;
	hr = CoInitialize(NULL);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Could not CoInitialize()", L"Error !", MB_ICONWARNING);
		return FALSE;
	}

	hr = CoCreateInstance(CLSID_DOMDocument, NULL, CLSCTX_INPROC_SERVER | CLSCTX_LOCAL_SERVER, IID_IXMLDOMDocument, (LPVOID*)&document);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Failed to CoCreate an instance of an XML DOM", L"Error !", MB_ICONWARNING);
	}

	document->put_async(VARIANT_FALSE);
	VariantInit(&vSrc);
	V_BSTR(&vSrc) = SysAllocString(fname);
	V_VT(&vSrc) = VT_BSTR;

	hr = document->load(vSrc, &status);

	if (status != VARIANT_TRUE)
	{
		long line, linePos;
		BSTR reason = NULL;
		WCHAR errMsg[1024];

		hr = document->get_parseError(&parseError);
		hr = parseError->get_reason(&bstr);
		hr = parseError->get_errorCode(&hr);
		hr = parseError->get_line(&line);
		hr = parseError->get_linepos(&linePos);

		wsprintf(errMsg, L"Error 0x%.8X on line %d, position %d\r\nReason: %s", hr, line, linePos, reason);
		MessageBox(NULL, errMsg, L"Load Error !", MB_ICONWARNING);
		goto clean;
	}

	hr = document->get_documentElement(&element);
	if (FAILED(hr) || element == NULL)
	{
		MessageBox(NULL, L"Empty document!", L"Error Loading XML", MB_ICONWARNING);
		goto clean;
	}
	IXMLDOMNodeList* childList;
	hr = element->get_childNodes(&childList);
	if (FAILED(hr) || element == NULL)
	{
		MessageBox(NULL, L"Empty document!", L"Error Loading XML", MB_ICONWARNING);
		goto clean;
	}
	HMENU menu = CreatePopupMenu();
	SetMenuInfo(menu, &mi);
	this->build(childList, menu, 1900);

clean:
	if (bstr) SysFreeString(bstr);
	if (&vSrc) VariantClear(&vSrc);
	if (parseError) parseError->Release();
	if (document) document->Release();
	return menu;
}

int MenuBuilder::build(IXMLDOMNodeList *node, HMENU rootMenu, UINT startIdsAt)
{
	HRESULT 			hr = S_OK;
	long				listLength;
	VARIANT_BOOL		hasChild = VARIANT_FALSE;
	IXMLDOMNode*		nodeList = NULL;
	long				depth = 0;
	static	int			MMU_IDS = startIdsAt;

	hr = node->get_length(&listLength);
	if (FAILED(hr))
	{
		goto cleanup;
	}
	for (long i = 0; i < listLength; i++)
	{
		///////////////////////////////////////////////////////////////////////////////////
		// search for nested objects                                                     //
		///////////////////////////////////////////////////////////////////////////////////
		hr = node->get_item(i, &nodeList);
		IXMLDOMNodeList *menuChild;
		hr = nodeList->selectNodes(L"MENUITEM", &menuChild);
		hr = menuChild->get_length(&depth);

		if (FAILED(hr))
		{
			MessageBoxW(NULL, L"selectNodes()", L"Error", MB_ICONINFORMATION);
			return hr;
		}
		
		/////////////////////////////////////////////////////////////////////////////
		// For Items with nested nodes build a popup menu                          //
		/////////////////////////////////////////////////////////////////////////////
		if (depth > 0)
		{
			HMENU hSubMenu = CreatePopupMenu();
			MenuItem *menuItem = new MenuItem(nodeList, rootMenu, MMU_IDS, i, hSubMenu);
			this->build(menuChild, hSubMenu, MMU_IDS);
		}
		else {
			MenuItem *menuItem = new MenuItem(nodeList, rootMenu, MMU_IDS, i, NULL);
		}
		if (menuChild)	menuChild->Release();
		MMU_IDS++;
	}

cleanup:
	//SAFE_RELEASE(resultNode);
	if (nodeList) nodeList->Release();
	return 0;
}


MenuItem::MenuItem(IXMLDOMNode *node, HMENU hMenu, UINT wID, UINT uPos, HMENU hSubMenu)
{
	HRESULT 		hr = S_OK;
	IXMLDOMNode*	resultNode = NULL;
	BSTR			imagePath;
	BSTR			menuType;

	struct comparator {
		bool operator()(const wchar_t * lhs, const wchar_t * rhs) const {
			return lstrcmp(lhs, rhs) < 0;
		}
	};

	std::pair<const wchar_t *, UINT> map_data[] = {
		std::make_pair(L"MF_STRING",		MF_STRING),
		std::make_pair(L"MF_MENUBARBREAK",	MF_MENUBARBREAK),
		std::make_pair(L"MF_MENUBREAK",		MF_MENUBREAK),
		std::make_pair(L"MF_SEPARATOR",		MF_SEPARATOR),
		std::make_pair(L"MF_SIDEBAR",		MF_STRING)
	};

	std::map<const wchar_t *, UINT, comparator>my_map(map_data, map_data + sizeof map_data / sizeof map_data[0]);

	if (SUCCEEDED(node->selectSingleNode(L"CAPTION", &resultNode)))
	{
		resultNode->get_text(&this->label);
		IXMLDOMNamedNodeMap *pNodeAttributeMap;
		if (SUCCEEDED(resultNode->get_attributes(&pNodeAttributeMap)))
		{
			if (pNodeAttributeMap) {
				VARIANT fontFamily;
				VARIANT fontSize;
				IXMLDOMNode* pXMLNode;
				LONG PointSize = 0;
				if (SUCCEEDED(pNodeAttributeMap->getNamedItem(L"font-family", &pXMLNode)))
				{
					if (pXMLNode) 
					{
						pXMLNode->get_nodeValue(&fontFamily);
						pXMLNode->Release();
					}
				}
				if (SUCCEEDED(pNodeAttributeMap->getNamedItem(L"font-size", &pXMLNode)))
				{
					if (pXMLNode)
					{
						pXMLNode->get_nodeValue(&fontSize);
						VarI4FromStr(V_BSTR(&fontSize), 0, LOCALE_NOUSEROVERRIDE, &PointSize);
						pXMLNode->Release();
					}
				}

				if (PointSize > 0 && fontFamily.vt == VT_BSTR && SysStringLen(V_BSTR(&fontSize)) > 0)
				{
					this->graphics.CreateAngledFont(PointSize, 0, V_BSTR(&fontFamily));
				}

				VariantClear(&fontFamily);
				VariantClear(&fontSize);
				pNodeAttributeMap->Release();
			}
		}
		resultNode->Release();
	}

	if (SUCCEEDED(node->selectSingleNode(L"IMAGE", &resultNode)))
	{
		resultNode->get_text(&imagePath);
		resultNode->Release();
		if (SysStringLen(imagePath) > 0)
		{
			this->image = LoadImageSmart(imagePath);
			SysFreeString(imagePath);
		}
	}
	
	if (SUCCEEDED(node->selectSingleNode(L"MENUTYPE", &resultNode)))
	{
		resultNode->get_text(&menuType);
		resultNode->Release();
	}
	
	/*AllocConsole();
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);
	printf("%S\t\t%u\n", this->label, my_map[menuType]);*/

	this->color = RGB(255, 0, 0);
	this->type = my_map[menuType];

	ZeroMemory(&this->menuItemInfo, sizeof(menuItemInfo));
	this->menuItemInfo.cbSize = sizeof(menuItemInfo);
	this->menuItemInfo.fMask = MIIM_FTYPE | MIIM_ID | MIIM_DATA | MIIM_STRING | MIIM_BITMAP | MIIM_SUBMENU;
	this->menuItemInfo.fType = this->type | MF_OWNERDRAW;
	this->menuItemInfo.fState = MFS_UNHILITE;
	this->menuItemInfo.wID = wID;
	this->menuItemInfo.dwTypeData = this->label;
	this->menuItemInfo.cch = (UINT)min(lstrlen(menuItemInfo.dwTypeData), UINT_MAX);
	this->menuItemInfo.dwItemData = (ULONG_PTR)this;
	this->menuItemInfo.hbmpItem = this->image;

	if (IsMenu(hSubMenu)) {
		this->menuItemInfo.hSubMenu = hSubMenu;
	}
	InsertMenuItem(hMenu, wID, TRUE, &this->menuItemInfo);
}

HRESULT MenuItem::draw(DRAWITEMSTRUCT * lpDs)
{
	switch (this->type)
	{
	case MF_STRING:
	{
		///////////////////////////////////////////////////////
		//	Fill in the menu window                          //
		///////////////////////////////////////////////////////
		RECT rcSep;
		CopyRect(&rcSep, &lpDs->rcItem);

		HBRUSH hBrush = CreateSolidBrush(RGB(0,255,0));
		FillRect(lpDs->hDC, &rcSep, hBrush);
		DeleteObject(hBrush);
		break;
	}
	case MF_SEPARATOR:
		break;
	}

	return S_OK;
}

///////////////////////////////////////////////////////////////////////////////////
// How To Convert from BSTR to ANSI string                                       //
///////////////////////////////////////////////////////////////////////////////////
HRESULT oleStrToAnsi(BSTR bstrIn, LPSTR * lpszOut)
{
	DWORD dwSize;

	if (lpszOut == NULL)
	{
		return E_INVALIDARG;
	}
	if (bstrIn == NULL)
	{
		*lpszOut = NULL;
		return NOERROR;
	}
	dwSize = WideCharToMultiByte(CP_ACP, 0, bstrIn, -1, NULL, 0, NULL, NULL);
	if (dwSize == 0)
	{
		return HRESULT_FROM_WIN32(GetLastError());
	}
	*lpszOut = (LPSTR)SysAllocStringByteLen(NULL, dwSize - 1);
	if (*lpszOut == NULL)
	{
		return E_OUTOFMEMORY;
	}
	if (!WideCharToMultiByte(CP_ACP, 0, bstrIn, -1, *lpszOut, dwSize, NULL, NULL))
	{
		SysFreeString((BSTR)*lpszOut);
		return HRESULT_FROM_WIN32(GetLastError());
	}

	return NOERROR;
}