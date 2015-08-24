//------------------------------------------------------------------------
// IDropTarget Interface
//------------------------------------------------------------------------
#include "IDragDrop.h"

IDropTargetVtbl dropTargetVtbl = {
	DropTarget_QueryInterface,
	DropTarget_AddRef,
	DropTarget_Release,
	DropTarget_DragEnter,
	DropTarget_DragOver,
	DropTarget_DragLeave,
	DropTarget_Drop
};

IDropTarget DropTarget = {&dropTargetVtbl};

BOOL QueryDrop(IDropTarget *this, DWORD grfKeyState, LPDWORD pdwEffect);
void DisplayFileNames(IDropTarget *this, HGLOBAL hgFiles);

extern HWND hwndMain;

IDropTarget * DropTarget_CDropTarget(void)
{
	 // alloc enough mem for interface and any class vars
	IDropTarget * pIDropTarget = malloc(sizeof(DropTargetEx));
	if(!pIDropTarget)
		return NULL;

	// copy the interface into the DropSourceEx
	memcpy(pIDropTarget, &DropTarget, sizeof(dropTargetVtbl));

	// cast the local DropTargetEx pointer
	DropTargetEx * pDTEx = (DropTargetEx *) pIDropTarget;
	pDTEx->m_cRefCount = 0;
	pDTEx->m_fAcceptFmt = FALSE;
	pDTEx->m_pDropTargetHelper = NULL;

	CoCreateInstance(&CLSID_DragDropHelper,
                  NULL,
                  CLSCTX_INPROC_SERVER,
                  &IID_IDropTargetHelper,
                  (LPVOID*)&pDTEx->m_pDropTargetHelper);

	// increment the class reference count
	pIDropTarget->lpVtbl->AddRef(pIDropTarget);
	return pIDropTarget;
}

STDMETHODIMP DropTarget_QueryInterface(IDropTarget *this, REFIID riid, void ** ppvObject)
{
    if(IsEqualIID(riid, &IID_IUnknown) || IsEqualIID(riid , &IID_IDropTarget))
    {
		*ppvObject = (void*)&DropTarget;
		DropTarget_AddRef(this);
		return S_OK;
    }
    *ppvObject = NULL;
    return E_NOINTERFACE;
}


STDMETHODIMP_(ULONG) DropTarget_AddRef(IDropTarget *this)
{
	// cast the local DropTargetEx pointer
	DropTargetEx * pDTEx = (DropTargetEx *)this;
	return ++pDTEx->m_cRefCount;
}

STDMETHODIMP_(ULONG) DropTarget_Release(IDropTarget *this)
{
	DropTargetEx * pDTEx = (DropTargetEx *)this;
	if(--pDTEx->m_cRefCount == 0)
	{
		if(pDTEx->m_pDropTargetHelper)
		{
			pDTEx->m_pDropTargetHelper->lpVtbl->Release(pDTEx->m_pDropTargetHelper);
			pDTEx->m_pDropTargetHelper = NULL;
		}
		free(this);
		return 0;
	}

	return pDTEx->m_cRefCount;
}

STDMETHODIMP DropTarget_DragEnter(IDropTarget *this, LPDATAOBJECT pDataObj, DWORD grfKeyState, POINTL pt, LPDWORD pdwEffect)
{
	DropTargetEx * pDTEx = (DropTargetEx *)this;
	if(pDTEx->m_pDropTargetHelper)
	   pDTEx->m_pDropTargetHelper->lpVtbl->DragEnter(pDTEx->m_pDropTargetHelper, hwndMain, pDataObj, (LPPOINT)&pt, *pdwEffect);

	FORMATETC fe;

	fe.cfFormat = CF_HDROP;
	fe.ptd      = NULL;
	fe.dwAspect = DVASPECT_CONTENT;
	fe.lindex   = -1;
	fe.tymed    = TYMED_HGLOBAL;

	// Does the drag source provide the clipboard format we are looking for?
	pDTEx->m_fAcceptFmt = (S_OK == pDataObj->lpVtbl->QueryGetData(pDataObj, &fe)) ? TRUE : FALSE;

	QueryDrop(this, grfKeyState, pdwEffect);

	return S_OK;
}

STDMETHODIMP DropTarget_DragOver(IDropTarget *this, DWORD grfKeyState, POINTL pt, LPDWORD pdwEffect)
{
	DropTargetEx * pDTEx = (DropTargetEx *)this;
	if(pDTEx->m_pDropTargetHelper)
	   pDTEx->m_pDropTargetHelper->lpVtbl->DragOver(pDTEx->m_pDropTargetHelper, (LPPOINT)&pt, *pdwEffect);

	QueryDrop(this, grfKeyState, pdwEffect);


			//SendMessage(pDTEx->m_hwnd, WM_COMMAND, (WPARAM)330, (LPARAM)0);
			//MessageBox(pDTEx->m_hwnd,pszTemp,"Error!", IDOK);
	return S_OK;
}

STDMETHODIMP DropTarget_DragLeave(IDropTarget *this)
{
	DropTargetEx * pDTEx = (DropTargetEx *)this;
	if(pDTEx->m_pDropTargetHelper)
	   pDTEx->m_pDropTargetHelper->lpVtbl->DragLeave(pDTEx->m_pDropTargetHelper);

	pDTEx->m_fAcceptFmt = FALSE;
	return S_OK;
}

STDMETHODIMP DropTarget_Drop(IDropTarget *this, LPDATAOBJECT pDataObj, DWORD grfKeyState, POINTL pt, LPDWORD pdwEffect)
{
	DropTargetEx * pDTEx = (DropTargetEx *)this;
	if(pDTEx->m_pDropTargetHelper)
	   pDTEx->m_pDropTargetHelper->lpVtbl->Drop(pDTEx->m_pDropTargetHelper, pDataObj, (LPPOINT)&pt, *pdwEffect);

	FORMATETC   fe;
	STGMEDIUM   sm;
	HRESULT     hr = E_FAIL;

	if(QueryDrop(this, grfKeyState, pdwEffect)){
	   fe.cfFormat  = CF_HDROP;
	   fe.ptd 		= NULL;
	   fe.dwAspect  = DVASPECT_CONTENT;
	   fe.lindex 	= -1;
	   fe.tymed 	= TYMED_HGLOBAL;

	   // User has dropped on us. Get the data from drag source
	   hr = pDataObj->lpVtbl->GetData(pDataObj, &fe, &sm);
	   if(SUCCEEDED(hr)){
	      // Display the data and release it.
	      DisplayFileNames(this, sm.hGlobal);
		MessageBeep(MB_ICONINFORMATION);
	      ReleaseStgMedium(&sm);
		}
	}

	*pdwEffect = DROPEFFECT_NONE;

	return hr;
}

/* OleStdGetDropEffect
** -------------------
**
** Convert a keyboard state into a DROPEFFECT.
**
** returns the DROPEFFECT value derived from the key state.
**    the following is the standard interpretation:
**          no modifier -- Default Drop     (0 is returned)
**          CTRL        -- DROPEFFECT_COPY
**          SHIFT       -- DROPEFFECT_MOVE
**          CTRL-SHIFT  -- DROPEFFECT_LINK
**
**    Default Drop: this depends on the type of the target application.
**    this is re-interpretable by each target application. a typical
**    interpretation is if the drag is local to the same document
**    (which is source of the drag) then a MOVE operation is
**    performed. if the drag is not local, then a COPY operation is
**    performed.
*/
#define OleStdGetDropEffect(grfKeyState)    \
    ( (grfKeyState & MK_CONTROL) ?          \
        ( (grfKeyState & MK_SHIFT) ? DROPEFFECT_LINK : DROPEFFECT_COPY ) :  \
        ( (grfKeyState & MK_SHIFT) ? DROPEFFECT_MOVE : 0 ) )

BOOL QueryDrop(IDropTarget *this, DWORD grfKeyState, LPDWORD pdwEffect)
{
	DropTargetEx * pDTEx = (DropTargetEx *)this;
	DWORD dwOKEffects = *pdwEffect;

	if(!pDTEx->m_fAcceptFmt){
		*pdwEffect = DROPEFFECT_NONE;
		return FALSE;
	}

	*pdwEffect = OleStdGetDropEffect(grfKeyState);
	if(*pdwEffect == 0){
		// No modifier keys used by user while dragging.
		if (DROPEFFECT_COPY & dwOKEffects)
			*pdwEffect = DROPEFFECT_COPY;
		else if (DROPEFFECT_MOVE & dwOKEffects)
			*pdwEffect = DROPEFFECT_MOVE;
		else if (DROPEFFECT_LINK & dwOKEffects)
			*pdwEffect = DROPEFFECT_LINK;
		else{
			*pdwEffect = DROPEFFECT_NONE;
		}
	}else{
		// Check if the drag source application allows the drop effect desired by user.
		// The drag source specifies this in DoDragDrop
		if(!(*pdwEffect & dwOKEffects))
			*pdwEffect = DROPEFFECT_NONE;

		// We don't accept links
		if(*pdwEffect == DROPEFFECT_LINK)
			*pdwEffect = DROPEFFECT_NONE;
	}

	return (DROPEFFECT_NONE == *pdwEffect) ? FALSE : TRUE;
}

void DisplayFileNames(IDropTarget *this, HGLOBAL hgFiles)
{
	DropTargetEx * pDTEx = (DropTargetEx *)this;

	UINT	i, nFiles;
	LPWSTR	pszFiles;
	UINT	dwSize;
	LPWSTR 	pszTemp;

	// the num of file names
	nFiles = DragQueryFile((HDROP)hgFiles, 0xFFFFFFFF, NULL, 0);

	// empty the list box
	//SendMessage(pDTEx->m_hListBox, LB_RESETCONTENT, 0, 0);

	dwSize = nFiles * MAX_PATH;
	pszFiles = (LPWSTR)malloc(sizeof(WCHAR) * dwSize);

	if(pszFiles){
		for(i = 0, pszTemp = pszFiles; i < nFiles; i++, pszTemp += lstrlen(pszTemp)){
			DragQueryFile((HDROP)hgFiles, i, pszTemp, MAX_PATH);
			//SendMessage(pDTEx->m_hListBox, LB_ADDSTRING, 0, (LPARAM)pszTemp);
			MessageBox(pDTEx->m_hwnd, pszTemp, L"Error!", IDOK);
		}
	}
}

