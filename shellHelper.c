///////////////////////////////////////////////////////////////////////////////////
// Título:      		shellHelper.c                                            //
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

#include <windows.h>		// windows standar
#include <windowsx.h>       // Extra features 
#include <winuser.h>		// Windows User Module definitions
#include <stdio.h>
#include <wchar.h>			// WCHAR support
#include <tchar.h>			// TCHAR support
#include <shellapi.h>       // For shell interaction
#include "shellHelper.h"	// Shell Helper Functions

///////////////////////////////////////////////////////////////////////////////////
// SelectIconA & SelectIconW Open up the Select Icon connmon dialog for win32    //
///////////////////////////////////////////////////////////////////////////////////
BOOL SelectIconA(HWND hWndParent, LPSTR lpszFilename, DWORD dwBufferSize, DWORD *pdwIndex)
{
	BOOL result = FALSE;
	OSVERSIONINFO versioninfo;
	typedef WINSHELLAPI BOOL (WINAPI *fnPickIconDlg)(HWND hWndParent, LPTSTR pszFilename, LPDWORD pdwBufferSize, LPDWORD pdwIndex);

	versioninfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&versioninfo);
	HMODULE hShell32 = LoadLibrary(_T("shell32.dll"));
	if (hShell32)
	{
		fnPickIconDlg PickIconDlg = (fnPickIconDlg)GetProcAddress(hShell32, (LPCSTR)62);
		if (PickIconDlg)
		{
			if (versioninfo.dwPlatformId == VER_PLATFORM_WIN32_NT)
			{
				//LPWSTR pszWideName = new WCHAR[dwBufferSize];
				LPWSTR pszWideName = malloc(sizeof(WCHAR) * dwBufferSize);
				ZeroMemory(pszWideName, sizeof(WCHAR) * dwBufferSize);
				MultiByteToWideChar(CP_ACP, 0, lpszFilename, -1, pszWideName, dwBufferSize);
				result = PickIconDlg(hWndParent, (LPTSTR)pszWideName, &dwBufferSize, pdwIndex);
				WideCharToMultiByte(CP_ACP, 0, pszWideName, -1, lpszFilename, dwBufferSize, NULL, NULL);
				//delete[] pszWideName;
				free(pszWideName);
			}
			else
			{
				result = PickIconDlg(hWndParent, (LPTSTR)lpszFilename, &dwBufferSize, pdwIndex);
			}
		}
		FreeLibrary(hShell32);
	}
	return result;
}
///////////////////////////////////////////////////////////////////////////////////
// SelectIconA & SelectIconW Open up the Select Icon connmon dialog for win32    //
///////////////////////////////////////////////////////////////////////////////////
BOOL SelectIconW(HWND hWndParent, LPWSTR lpszFilename, DWORD dwBufferSize, DWORD *pdwIndex)
{
	BOOL result = FALSE;
	OSVERSIONINFO versioninfo;
	typedef WINSHELLAPI BOOL (WINAPI *fnPickIconDlg)(HWND hWndParent, LPTSTR pszFilename, LPDWORD pdwBufferSize, LPDWORD pdwIndex);

	versioninfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&versioninfo);
	HMODULE hShell32 = LoadLibrary(_T("shell32.dll"));
	if (hShell32)
	{
		fnPickIconDlg PickIconDlg = (fnPickIconDlg)GetProcAddress(hShell32, (LPCSTR)62);
		if (PickIconDlg)
		{
			if (versioninfo.dwPlatformId != VER_PLATFORM_WIN32_NT)
			{
				LPSTR pszMBName = malloc(sizeof(CHAR) * dwBufferSize);
				WideCharToMultiByte(CP_ACP, 0, lpszFilename, -1, pszMBName, dwBufferSize, NULL, NULL);
				result = PickIconDlg(hWndParent, (LPTSTR)pszMBName, &dwBufferSize, pdwIndex);
				MultiByteToWideChar(CP_ACP, 0, pszMBName, -1, lpszFilename, dwBufferSize);
				free(pszMBName);
			}
			else
			{
				result = PickIconDlg(hWndParent, (LPTSTR)lpszFilename, &dwBufferSize, pdwIndex);
			}
		}
		FreeLibrary(hShell32);
	}
	return result;
}

