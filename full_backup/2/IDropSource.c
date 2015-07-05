// IDropSource.c

#include "IDragDrop.h"

//-----------------------------------------------------------------
// vtable is an array of pointers to the functions of the interface
//-----------------------------------------------------------------
//--------------------------------------------------------------
// IDropSource vtable
//--------------------------------------------------------------
IDropSourceVtbl dropSourceVtbl = {
	DropSource_QueryInterface,
	DropSource_AddRef,
	DropSource_Release,
	DropSource_QueryContinueDrag,
	DropSource_GiveFeedback
};
IDropSource DropSource = {&dropSourceVtbl};

//-----------------------------------------------------------------
// IDropSource Ex constructor
//-----------------------------------------------------------------
IDropSource * DropSource_CDropSource(void)
{
	 // alloc enough mem for interface and any class vars
	IDropSource * pIDropSource = malloc(sizeof(DropSourceEx));
	if(!pIDropSource)
		return NULL;

	// copy the interface into the DropSourceEx
	memcpy(pIDropSource, &DropSource, sizeof(dropSourceVtbl));

	// cast the local DropSourceEx pointer
	DropSourceEx * pDSEx = (DropSourceEx *) pIDropSource;
	pDSEx->m_cRefCount = 0;
	// LoadCursor only loads the cursor resource if it has not been loaded;
	// otherwise, it retrieves the handle of the existing resource.
	pDSEx->m_hCur = LoadCursor(GetModuleHandle(NULL), MAKEINTRESOURCE(8002));
	// increment the class reference count
	pIDropSource->lpVtbl->AddRef(pIDropSource);
	return pIDropSource;
}

//-----------------------------------------------------------------
// IUnknown Interface
//-----------------------------------------------------------------
STDMETHODIMP DropSource_QueryInterface(IDropSource * this, REFIID riid, LPVOID* ppvObject)
{
    if(IsEqualIID(riid, &IID_IUnknown) || IsEqualIID(riid , &IID_IDropSource))
    {
		*ppvObject = (void *)this;
		this->lpVtbl->AddRef(this);
		return S_OK;
    }
    *ppvObject = NULL;
    return E_NOINTERFACE;
}

STDMETHODIMP_(ULONG) DropSource_AddRef(IDropSource *this)
{
	// cast to get a pointer to the class
	DropSourceEx * pDSEx = (DropSourceEx *)this;
	// increase reference count
	return ++pDSEx->m_cRefCount;
}

STDMETHODIMP_(ULONG) DropSource_Release(IDropSource *this)
{
	DropSourceEx * pDSEx = (DropSourceEx *)this;

	// decrease reference count
	if(--pDSEx->m_cRefCount == 0){
		// free the memory if ref count is zero
		free(this);
		return 0;
	}
	return pDSEx->m_cRefCount;
}

//--------------------------------------------------------------
// IDropSource Interface
//--------------------------------------------------------------
STDMETHODIMP DropSource_GiveFeedback(IDropSource *this, DWORD dwEffect)
{
	// cast the local DropSourceEx pointer
	DropSourceEx * pDSEx = (DropSourceEx *)this;
	SetCursor(pDSEx->m_hCur);
	return S_OK; //DRAGDROP_S_USEDEFAULTCURSORS;
}

STDMETHODIMP DropSource_QueryContinueDrag(IDropSource *this, BOOL fEscapePressed, DWORD grfKeyState)
{
	if(fEscapePressed)
		return DRAGDROP_S_CANCEL;

	if(!(grfKeyState & MK_LBUTTON))
		return DRAGDROP_S_DROP;

	return S_OK;
}



