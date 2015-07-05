/*-------------------------------------------------------------
  fileselect.c : open dialog boxes to select file / folder
  (C) 1997-2003 Kazuto Sato
  Please read readme.txt about the license.
  
  Written by Kazubon, Nanashi-san
---------------------------------------------------------------*/

#include "common.h"
#include <shlobj.h>

#ifndef BIF_NEWDIALOGSTYLE
#define BIF_NEWDIALOGSTYLE     0x0040
#endif

/* Globals */

BOOL SelectMyFile(HINSTANCE hInst, HWND hDlg,
	const char *filter, DWORD nFilterIndex,
	const char *deffile, char *retfile);
BOOL SelectFolder(HWND hDlg, const char *def, char *ret);

/* Statics */

static int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg,
	LPARAM lParam, LPARAM lpData);

/*------------------------------------------------
   select file
--------------------------------------------------*/
BOOL SelectMyFile(HINSTANCE hInst, HWND hDlg,
	const char *filter, DWORD nFilterIndex,
	const char *deffile, char *retfile)
{
	OPENFILENAME ofn;
	char fname[MAX_PATH], ftitle[MAX_PATH], initdir[MAX_PATH];
	BOOL r;
	
	memset(&ofn, '\0', sizeof(OPENFILENAME));
	
	GetModuleFileName(hInst, initdir, MAX_PATH);
	del_title(initdir);
	if(deffile[0])
	{
		WIN32_FIND_DATA fd;
		HANDLE hfind;
		hfind = FindFirstFile(deffile, &fd);
		if(hfind != INVALID_HANDLE_VALUE)
		{
			FindClose(hfind);
			strcpy(initdir, deffile);
			del_title(initdir);
		}
	}
	
	fname[0] = 0;
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hDlg;
	ofn.hInstance = hInst;
	ofn.lpstrFilter = filter;
	ofn.nFilterIndex = nFilterIndex;
	ofn.lpstrFile= fname;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFileTitle = ftitle;
	ofn.nMaxFileTitle = MAX_PATH;
	ofn.lpstrInitialDir = initdir;
	ofn.Flags = OFN_HIDEREADONLY|OFN_EXPLORER|OFN_FILEMUSTEXIST;
	// ofn.FlagsEx = 0;
	
	r = GetOpenFileName(&ofn);
	if(!r) return r;
	
	strcpy(retfile, ofn.lpstrFile);
	
	return r;
}

/*------------------------------------------------
   select folder
--------------------------------------------------*/
BOOL SelectFolder(HWND hDlg, const char *def, char *ret)
{
	BROWSEINFO bi;
	LPITEMIDLIST pidl;
	
	memset(&bi, 0, sizeof(BROWSEINFO));
	bi.hwndOwner = hDlg;
	bi.ulFlags = BIF_RETURNONLYFSDIRS|BIF_NEWDIALOGSTYLE;
	bi.lpfn = (BFFCALLBACK)BrowseCallbackProc;
	bi.lParam = (LPARAM)def;
	pidl = SHBrowseForFolder(&bi);
	if(!pidl) return FALSE;
	
	SHGetPathFromIDList(pidl, ret);
	return TRUE;
}

/*------------------------------------------
  callback procedure: select default folder
--------------------------------------------*/
int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg,
	LPARAM lParam, LPARAM lpData)
{
	if(uMsg == BFFM_INITIALIZED)
	{
		if(lpData && *(char *)lpData)
			SendMessage(hwnd, BFFM_SETSELECTION, TRUE, (LPARAM)lpData);
	}
	return 0;
}

