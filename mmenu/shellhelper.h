///////////////////////////////////////////////////////////////////////////////////
// Título:      		shellHelper.h                                            //
// Author:		        Benjamin Maggi                                           //
// Descripcion: 		XML Handling functions                                   //
// Org. Date:           06/04/2008                                               //
// Last Modification:   06/04/2008                                               //
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

#ifndef __SHELLHELPER_H__
#define __SHELLHELPER_H__

///////////////////////////////////////////////////////////////////////////////////
//	Select Icon dialog                                                           //
///////////////////////////////////////////////////////////////////////////////////
#ifdef _UNICODE
#define SelectIcon SelectIconW
#else
#define SelectIcon SelectIconA
#endif
BOOL SelectIconW(HWND hWndParent, LPWSTR lpszFilename, DWORD dwBufferSize, DWORD *pdwIndex);
BOOL SelectIconA(HWND hWndParent, LPSTR lpszFilename, DWORD dwBufferSize, DWORD *pdwIndex);



#endif
