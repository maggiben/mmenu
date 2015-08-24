///////////////////////////////////////////////////////////////////////////////////
// Título:      		xmleng.h                                                 //
// Author:		        Benjamin Maggi                                           //
// Descripcion: 		XML Handling functions                                   //
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
// either version 2 of the License, or (at your option) any          	         //
// later version.                                                    	         //
//                                                                    	         //
// This program is distributed in the hope that it will be useful,   	         //
// but WITHOUT ANY WARRANTY; without even the implied warranty of    	         //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the     	         //
// GNU General Public License for more details.                      	         //
//                                                                               //
///////////////////////////////////////////////////////////////////////////////////

#ifndef __XMLENG_H__
#define __XMLENG_H__

///////////////////////////////////////////////////////////////////////////////////
// Module Functions                                                              //
///////////////////////////////////////////////////////////////////////////////////
IXMLDOMDocument*	loadXML				(char * fname);
void				unloadXML			(void);
BOOL				populateNode		(IXMLDOMElement *node);
BOOL				populateAttributes	(IXMLDOMElement *node);
HRESULT				getAttributeValue	(IXMLDOMElement *node, BSTR attribute,VARIANT* nodeValue);

///////////////////////////////////////////////////////////////////////////////////
// Type Conversion                                                               //
///////////////////////////////////////////////////////////////////////////////////
HRESULT		ConvertAnsiStrToBStrx		(LPCSTR szAnsiIn, BSTR * lpBstrOut);
HRESULT		convertWcharToBstr			(LPWSTR lpWstrIn, BSTR * lpBstrOut);
HRESULT		ConvertBStrToAnsiStrx		(BSTR bstrIn, LPSTR * lpszOut);
HRESULT		AnsiToUnicode				(LPCSTR pszA, LPOLESTR* ppszW);

#endif

