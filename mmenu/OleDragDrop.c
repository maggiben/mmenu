// oledragdrop.c

#include "IDragDrop.h"

//////////////////////////////////////////////////////////////
//get the ID for the File Group Descriptor clipboard format
unsigned short cfFileGroupDescriptor = 0;

//get the ID for the File Contents clipboard format
unsigned short cfFileContents = 0;

char * GetNextName(char * pszFiles)
{
	char * pszTemp = pszFiles;
	while(*pszTemp){
		pszTemp++;
	}

	pszTemp++;

	return *pszTemp ? pszTemp : NULL;
}

HGLOBAL CreateFileGroupDescriptor(LPCTSTR pszFiles)
{
	HGLOBAL              hg;
	FILEGROUPDESCRIPTOR  *pfgd;
	char *               pszNext;
	DWORD                dwItems;

	//get the number of items
	for(dwItems = 0, pszNext = (char *)pszFiles; pszNext; pszNext = GetNextName(pszNext)){
		dwItems++;
	}

	hg = GlobalAlloc(GMEM_FIXED, sizeof(FILEGROUPDESCRIPTOR) + (dwItems - 1) * sizeof(FILEDESCRIPTOR));

	if(hg){
		pfgd = (FILEGROUPDESCRIPTOR*)GlobalLock(hg);
		if(pfgd){
			int   i;
			pfgd->cItems = dwItems;

			for(i = 0, pszNext = (char *)pszFiles; pszNext; pszNext = GetNextName(pszNext), i++){
				pfgd->fgd[i].dwFlags = FD_ATTRIBUTES;
				pfgd->fgd[i].dwFileAttributes = FILE_ATTRIBUTE_NORMAL;
				lstrcpyn(pfgd->fgd[i].cFileName, pszNext, sizeof(pfgd->fgd[i].cFileName));
			}

			GlobalUnlock(hg);
		}
	}
	return hg;
}

HGLOBAL CreateFileContents(LPCTSTR pszFiles, int nItem)
{
	HGLOBAL  hg = NULL;
	TCHAR    szText[MAX_PATH];
	char *   pszNext;
	int      i;
	char * psz;

	for(i = 0, pszNext = (char *)pszFiles; i < nItem; pszNext = GetNextName(pszNext), i++)
	{
	}

	wsprintf(szText, TEXT("This file is %s created from CFSTR_FILECONTENTS"), pszNext);

	hg = GlobalAlloc(GMEM_FIXED, (strlen(szText) + 1) * sizeof(TCHAR));

	if(hg){
		psz = (LPSTR)GlobalLock(hg);

		if(psz){
			strcpy_s(psz, strlen(szText), szText);
			GlobalUnlock(hg);
		}
	}

	return hg;
}
extern HWND hwndMain;

void HandleDragDrop(HWND hwndList)
{
	DWORD       dwEffect;
	HRESULT 	hr;

	IDataObject * pDataObject = DataObject_CDataObject();
	IDropSource * pDropSource = DropSource_CDropSource();
	if(!pDataObject || !pDropSource)
		return;

	//get the ID for the File Group Descriptor clipboard format
	cfFileGroupDescriptor = (unsigned short)RegisterClipboardFormat(CFSTR_FILEDESCRIPTOR);

	//get the ID for the File Contents clipboard format
	cfFileContents = (unsigned short)RegisterClipboardFormat(CFSTR_FILECONTENTS);

	int nItems = ListView_GetSelectedCount(hwndList);
	if(!nItems)
		return;

	FORMATETC   fe;
	STGMEDIUM   sm;
	int         i, nStart;
	DWORD       dwChars;

	//get the indexes of the selected items
	int * anItems = (int *)malloc(sizeof(int) * nItems);

	for(i = 0, nStart = -1; i < nItems; i++){
		nStart = anItems[i] = ListView_GetNextItem(hwndList, nStart, LVNI_SELECTED);
	}

	char szText[MAX_PATH];
	//get the length of the text for each selected item
	for(i = 0, dwChars = 0; i < nItems; i++){
		ListView_GetItemText(hwndList, anItems[i], 0, szText, MAX_PATH); // filename
		dwChars += lstrlen(szText) + 1;
	}

	//add one for the double-NULL terminator
	dwChars += 1;
	char * pszFiles = (char *)malloc(sizeof(char) * dwChars);
	char * pszThis;

	//get the text for each selected item
	for(i = 0, pszThis = pszFiles; i < nItems; i++){
		ListView_GetItemText(hwndList, anItems[i], 0, pszThis, dwChars - (DWORD)(pszThis - pszFiles));
		pszThis += lstrlen(pszThis) + 1;
	}

	//double NULL terminate the list
	*pszThis = 0;

	//initialize the data object
	fe.cfFormat 	= cfFileGroupDescriptor;
	fe.ptd 			= NULL;
	fe.dwAspect 	= DVASPECT_CONTENT;
	fe.lindex 		= -1;
	fe.tymed 		= TYMED_HGLOBAL;

	sm.tymed 		  = TYMED_HGLOBAL;
	sm.pUnkForRelease = NULL;
	sm.hGlobal 		  = CreateFileGroupDescriptor(pszFiles);

	pDataObject->lpVtbl->SetData(pDataObject, &fe, &sm, TRUE);

	fe.cfFormat = cfFileContents;
	fe.ptd 		= NULL;
	fe.dwAspect = DVASPECT_CONTENT;
	fe.tymed 	= TYMED_HGLOBAL;

	sm.tymed = TYMED_HGLOBAL;
	sm.pUnkForRelease = NULL;

	for(i = 0; i < nItems; i++){
		fe.lindex = i;
		sm.hGlobal = CreateFileContents(pszFiles, i);
		pDataObject->lpVtbl->SetData(pDataObject, &fe, &sm, TRUE);
	}

	// do the drag and drop
	hr = DoDragDrop(pDataObject, pDropSource, DROPEFFECT_COPY, &dwEffect);

	pDataObject->lpVtbl->Release(pDataObject);
	pDropSource->lpVtbl->Release(pDropSource);

	//-----------------------------------------------
	// Example of not using the lpVtbl
	//-----------------------------------------------
	// one could cast pDropSource to (DropSourceEx *)
	// and use the functions without the ->lpVtbl->
	// DropSourceEx * pDSEx = (DropSourceEx *) pDropSource;
	// pDSEx->Release(pDropSource);
	//--------------------------------------------
}

