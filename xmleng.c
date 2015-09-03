///////////////////////////////////////////////////////////////////////////////////
// Título:              xmleng.c                                                 //
// Author:              Benjamin Maggi                                           //
// Descripcion:         XML Handling functions                                   //
// Org. Date:           13/03/2008                                               //
// Last Modification:   29/03/2008                                               //
// Ver:                 0.0.1                                                    //
// compiler:            uses ansi-C / C99 tested with LCC & Pellesc              //
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

#include <windows.h>		// Windows
#include <msxml.h>			// MSXML
#include <wchar.h>			// Unicode Support
#include <stdio.h>

#include "xmleng.h"


///////////////////////////////////////////////////////////////////////////////////
// Purpose:       load an XML file                                               //
//                                                                               //
// Input:         char * fname file name                                         //
//                IXMLDOMDocument* document in no error a valid IXMLDOMDocument  //
// Output:        On success return true                                         //
// Errors:        If the function succeeds, return = TRUE.                       //
//                If the function fails, return = FALSE.                         //
// Notes:		  .                                                              //
// Author:        Benjamin Maggi 2008                                            //
///////////////////////////////////////////////////////////////////////////////////
IXMLDOMDocument* loadXML(char * fname)
{
	static	IXMLDOMDocument*	document 	= NULL;
			IXMLDOMParseError*	parseError	= NULL;
			IXMLDOMElement*		element		= NULL;
			BSTR				bstr		= NULL;
			VARIANT_BOOL		status;
			VARIANT				vSrc;
			BSTR			fileName;

	HRESULT hr = CoInitialize(NULL);
	if (FAILED(hr))
	{
		MessageBox(NULL,"Could not CoInitialize()","Error !",MB_ICONWARNING);
		return FALSE;
	}

	hr = CoCreateInstance(&CLSID_DOMDocument, NULL, CLSCTX_INPROC_SERVER | CLSCTX_LOCAL_SERVER, &IID_IXMLDOMDocument, (LPVOID*)&document);
	if (FAILED(hr)) 
	{
		MessageBox(NULL,"Failed to CoCreate an instance of an XML DOM","Error !",MB_ICONWARNING);
		goto cleanup;
	}
	document->lpVtbl->put_async(document,VARIANT_FALSE);
	VariantInit(&vSrc);
	AnsiToUnicode(fname,&fileName);
	//ConvertAnsiStrToBStrx(fname,&fileName);
	V_BSTR(&vSrc) = SysAllocString(fileName);
	V_VT(&vSrc) = VT_BSTR;

	hr = document->lpVtbl->load(document, vSrc, &status);

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
        goto cleanup;
    }

	hr = document->lpVtbl->get_documentElement(document,&element);
	if (FAILED(hr) || element == NULL)
	{
		MessageBox(NULL,"Empty document!", "Error Loading XML", MB_ICONWARNING);
        goto cleanup;
	}

noerror:
    if (bstr) SysFreeString(bstr);
    if (&vSrc) VariantClear(&vSrc);
	//if (fileName) CoTaskMemFree((void*)fileName);
    if (parseError) parseError->lpVtbl->Release(parseError);
	VariantClear(&vSrc);
	return document;
cleanup:
    if (bstr) SysFreeString(bstr);
    if (&vSrc) VariantClear(&vSrc);
    if (parseError) parseError->lpVtbl->Release(parseError);
    if (document) document->lpVtbl->Release(document);
	VariantClear(&vSrc);
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////////
// Purpose:       Populate through the XML tree                                  //
//                                                                               //
// Input:         IXMLDOMElement* node source to ilterate.                       //
// Output:        IXMLDOMElement* node        .                                  //
// Errors:        If the function succeeds, return = TRUE.                       //
//                If the function fails, return = FALSE.                         //
// Notes:		  .                                                              //
// Author:        Benjamin Maggi 2008                                            //
///////////////////////////////////////////////////////////////////////////////////
BOOL populateNode(IXMLDOMElement *node)
{
	HRESULT		hr = S_OK;
	BSTR		nodeName;
	DOMNodeType	nodeType;


	node->lpVtbl->get_nodeType(node,&nodeType);
	switch(nodeType)
	{
		case NODE_INVALID:
			break;
    	case NODE_ELEMENT:
			node->lpVtbl->get_nodeName(node,&nodeName);
			//MessageBoxW(NULL,nodeName,L"NODE_ELEMENT",MB_ICONINFORMATION);
			populateAttributes(node);
			break;
    	case NODE_ATTRIBUTE:
			break;
    	case NODE_TEXT:
			node->lpVtbl->get_text(node,&nodeName);
			//MessageBoxW(NULL,nodeName,L"NODE_TEXT",MB_ICONINFORMATION);
			break;
    	case NODE_CDATA_SECTION:
			break;
    	case NODE_ENTITY_REFERENCE:
			break;
    	case NODE_ENTITY:
			break;
    	case NODE_PROCESSING_INSTRUCTION:
			break;
    	case NODE_COMMENT:
			node->lpVtbl->get_nodeName(node,&nodeName);
			//MessageBoxW(NULL,nodeName,L"NODE_COMMENT",MB_ICONINFORMATION);
			break;
    	case NODE_DOCUMENT:
			break;
    	case NODE_DOCUMENT_TYPE:
			break;
    	case NODE_DOCUMENT_FRAGMENT:
			break;
    	case NODE_NOTATION:
			break;
		default:
		{
			node->lpVtbl->get_nodeName(node,&nodeName);
			break;
		}
	}

	IXMLDOMNode* nextSibling = NULL;
	hr = node->lpVtbl->get_nextSibling(node,&nextSibling);
	if (SUCCEEDED(hr) && nextSibling != NULL)
	{
		populateNode((IXMLDOMElement*)nextSibling);
	}
	if (nodeName) SysFreeString(nodeName);
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////
// Purpose:       Populate through the XML attr list                             //
//                                                                               //
// Input:         IXMLDOMElement* node source to ilterate.                       //
// Output:        IXMLDOMElement* node        .                                  //
// Errors:        If the function succeeds, return = TRUE.                       //
//                If the function fails, return = FALSE.                         //
// Notes:         .                                                              //
// Author:        Benjamin Maggi 2008                                            //
///////////////////////////////////////////////////////////////////////////////////
BOOL populateAttributes(IXMLDOMElement *node)
{
	HRESULT hr = S_OK;
	BSTR	nodeName;
	VARIANT	vSrc;

	IXMLDOMNamedNodeMap* namedNodeMap = NULL;
	hr = node->lpVtbl->get_attributes(node,&namedNodeMap);
	if (SUCCEEDED(hr) && namedNodeMap != NULL)
	{
		long listLength;
		hr = namedNodeMap->lpVtbl->get_length(namedNodeMap,&listLength);
		for(long i = 0; i < listLength; i++)
		{
			IXMLDOMNode* listItem = NULL;
			hr = namedNodeMap->lpVtbl->get_item(namedNodeMap,i, &listItem);
			listItem->lpVtbl->get_nodeName(listItem,&nodeName);
			//listItem->lpVtbl->get_text(listItem,&nodeName);
			//MessageBoxW(NULL,nodeName,L"Error",MB_ICONINFORMATION);
		}
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////
// Purpose:       Populate through the XML                                       //
//                                                                               //
// Input:         IXMLDOMElement* node source to ilterate                        //
//                BSTR attribute Attribute name                                  //
//                VARIANT* nodeValue, return value will be placed here           //
// Output:        IXMLDOMElement* node        .                                  //
// Errors:        If the function succeeds, return = NOERROR.                    //
//                If the function fails, return = HRESULT from last call.        //
// Notes:                                                                        //
// Author:        Benjamin Maggi 2008                                            //
///////////////////////////////////////////////////////////////////////////////////
HRESULT getAttributeValue(IXMLDOMElement *node, BSTR attribute,VARIANT* nodeValue)
{
	HRESULT	hr = S_OK;
	BSTR	nodeName;
	long	listLength;

	IXMLDOMNamedNodeMap* namedNodeMap = NULL;
	hr = node->lpVtbl->get_attributes(node,&namedNodeMap);
	if (SUCCEEDED(hr) && namedNodeMap != NULL)
	{
		hr = namedNodeMap->lpVtbl->get_length(namedNodeMap,&listLength);
		for(long i = 0; i < listLength; i++)
		{
			IXMLDOMNode* attrNode = NULL;
			hr = namedNodeMap->lpVtbl->get_item(namedNodeMap,i, &attrNode);
			hr = attrNode->lpVtbl->get_nodeName(attrNode,&nodeName);
			// buble search for the node name
			if(!wcscmp(nodeName,attribute))
			{
				hr = attrNode->lpVtbl->get_nodeValue(attrNode,nodeValue);
				return NOERROR;
			}
		}
	}
	return hr;
}

///////////////////////////////////////////////////////////////////////////////////
// How To Convert from ANSI to Unicode & Unicode to ANSI for OLE                 //
// At MSDN Article ID:  138813                                                   //
//                                                                               //
// AnsiToUnicode converts the ANSI string pszA to a Unicode string               //
// and returns the Unicode string through ppszW. Space for the                   //
// the converted string is allocated by AnsiToUnicode.                           //
///////////////////////////////////////////////////////////////////////////////////
HRESULT AnsiToUnicode(LPCSTR pszA, LPOLESTR* ppszW)
{

    ULONG cCharacters;
    DWORD dwError;

    // If input is null then just return the same.
    if (NULL == pszA)
    {
        *ppszW = NULL;
        return NOERROR;
    }

    // Determine number of wide characters to be allocated for the
    // Unicode string.
    cCharacters =  strlen(pszA)+1;

    // Use of the OLE allocator is required if the resultant Unicode
    // string will be passed to another COM component and if that
    // component will free it. Otherwise you can use your own allocator.
    *ppszW = (LPOLESTR) CoTaskMemAlloc(cCharacters*2);
    if (NULL == *ppszW)
        return E_OUTOFMEMORY;

    // Covert to Unicode.
    if (0 == MultiByteToWideChar(CP_ACP, 0, pszA, cCharacters,
                  *ppszW, cCharacters))
    {
        dwError = GetLastError();
        CoTaskMemFree(*ppszW);
        *ppszW = NULL;
        return HRESULT_FROM_WIN32(dwError);
    }

    return NOERROR;
}

HRESULT ConvertAnsiStrToBStrx(LPCSTR szAnsiIn, BSTR * lpBstrOut)
{
	DWORD dwSize;

	if (lpBstrOut == NULL) return E_INVALIDARG;
	if (szAnsiIn == NULL) { *lpBstrOut = NULL; return NOERROR; }

	dwSize = MultiByteToWideChar(CP_ACP, 0, szAnsiIn, -1, NULL, 0);
	if (dwSize == 0) return HRESULT_FROM_WIN32( GetLastError() );

	*lpBstrOut = SysAllocStringLen(NULL, dwSize - 1);
	if (*lpBstrOut == NULL) return E_OUTOFMEMORY;

	if ( !MultiByteToWideChar(CP_ACP, 0, szAnsiIn, -1, *lpBstrOut, dwSize) )
	{
		SysFreeString(*lpBstrOut);
		return HRESULT_FROM_WIN32( GetLastError() );
	}

	return NOERROR;
}

///////////////////////////////////////////////////////////////////////////////////
// How To Convert from OLESTR & BSTR to ANSI string                              //
//                                                                               //
///////////////////////////////////////////////////////////////////////////////////
HRESULT ConvertBStrToAnsiStrx(BSTR bstrIn, LPSTR * lpszOut)
{
	DWORD dwSize;

	if (lpszOut == NULL) return E_INVALIDARG;
	if (bstrIn == NULL) { *lpszOut = NULL; return NOERROR; }

	dwSize = WideCharToMultiByte(CP_ACP, 0, bstrIn, -1, NULL, 0, NULL, NULL);
	if (dwSize == 0) return HRESULT_FROM_WIN32( GetLastError() );

	*lpszOut = (LPSTR) SysAllocStringByteLen(NULL, dwSize - 1);
	if (*lpszOut == NULL) return E_OUTOFMEMORY;

	if ( !WideCharToMultiByte(CP_ACP, 0, bstrIn, -1, *lpszOut, dwSize, NULL, NULL) )
	{
		SysFreeString((BSTR) *lpszOut);
		return HRESULT_FROM_WIN32( GetLastError() );
	}

	return NOERROR;
}


///////////////////////////////////////////////////////////////////////////////////
// Closes the COM library on the current thread                                  //
///////////////////////////////////////////////////////////////////////////////////
void unloadXML(void)
{
	CoUninitialize();
}
