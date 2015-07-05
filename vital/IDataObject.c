// IDataObject.c

#include "IDragDrop.h"

//--------------------------------------------------------------
// IDataObject vtable
//--------------------------------------------------------------
IDataObjectVtbl dataObjectVtbl = {
	DataObject_QueryInterface,
	DataObject_AddRef,
	DataObject_Release,
	DataObject_GetData,
	DataObject_GetDataHere,
	DataObject_QueryGetData,
	DataObject_GetCanonicalFormatEtc,
	DataObject_SetData,
	DataObject_EnumFormatEtc,
	DataObject_DAdvise,
	DataObject_DUnadvise,
	DataObject_EnumDAdvise
};
IDataObject DataObject = {&dataObjectVtbl};

//--------------------------------------------------------------
// IDataObject Ex constructor
//--------------------------------------------------------------
IDataObject * DataObject_CDataObject(void)
{
	 // alloc enough mem for interface and the class variables
	IDataObject * pIDataObject = malloc(sizeof(DataObjectEx));
	if(!pIDataObject)
		return NULL;

	// cast the local DropSourceEx pointer
	DataObjectEx * pDOEx = (DataObjectEx *) pIDataObject;

	// copy the interface into the DropSourceEx
	memcpy(pIDataObject, &DataObject, sizeof(dataObjectVtbl));

	// init the vars
	pDOEx->m_cRefCount = 0;
	pDOEx->m_pFormatEtc = NULL;
	pDOEx->m_pStgMedium = NULL;
	pDOEx->m_cDataCount = 0;

	// increment the class reference count
	pIDataObject->lpVtbl->AddRef(pIDataObject);
	return pIDataObject;
}

//-----------------------------------------------------------------
// IUnknown Interface
//-----------------------------------------------------------------
STDMETHODIMP DataObject_QueryInterface(IDataObject *this, REFIID riid, LPVOID *ppvObject)
{
    if(IsEqualIID(riid, &IID_IUnknown) || IsEqualIID(riid , &IID_IDataObject))
	{
		*ppvObject = (void*)this;
		this->lpVtbl->AddRef(this);
		return S_OK;
    }
	*ppvObject = NULL;
    return E_NOINTERFACE;
}

STDMETHODIMP_(ULONG) DataObject_AddRef(IDataObject *this)
{
	// cast to get a pointer to the dataobject class
	DataObjectEx * pDOEx = (DataObjectEx *)this;

	// increase reference count
	return ++pDOEx->m_cRefCount;
}

STDMETHODIMP_(ULONG) DataObject_Release(IDataObject *this)
{
	DataObjectEx * pDOEx = (DataObjectEx *)this;

	if(--pDOEx->m_cRefCount == 0){
		free(this);
		return 0;
	}
	return pDOEx->m_cRefCount;
}

//--------------------------------------------------------------
// IDataObject Interface
//--------------------------------------------------------------
STDMETHODIMP DataObject_GetData(IDataObject *this, LPFORMATETC pFE, LPSTGMEDIUM pSM)
{
	DataObjectEx * pDOEx = (DataObjectEx *)this;

	if(pFE == NULL || pSM == NULL)
		return E_INVALIDARG;

	memset(pSM, 0, sizeof(STGMEDIUM));

	DWORD i;

	for(i = 0; i < pDOEx->m_cDataCount; i++){
		if((pFE->tymed & pDOEx->m_pFormatEtc[i].tymed) &&
		  (pFE->dwAspect == pDOEx->m_pFormatEtc[i].dwAspect) &&
          (pFE->cfFormat == pDOEx->m_pFormatEtc[i].cfFormat) &&
		  (pFE->lindex   == pDOEx->m_pFormatEtc[i].lindex))
   		{
			HRESULT  hr;

			hr = CopyStgMedium(&pDOEx->m_pStgMedium[i], pSM);

			if(pSM->tymed == TYMED_HGLOBAL){
				//this tell's the caller not to free the global memory
				DataObject_QueryInterface(this, &IID_IUnknown, (LPVOID*)&pSM->pUnkForRelease);
			}

			return hr;
		}
	}

	return DV_E_FORMATETC;
}

STDMETHODIMP DataObject_GetDataHere(IDataObject *this, LPFORMATETC pFE, LPSTGMEDIUM pSM)
{
	return E_NOTIMPL;
}

STDMETHODIMP DataObject_QueryGetData(IDataObject *this, LPFORMATETC pFE)
{
	DataObjectEx * pDOEx = (DataObjectEx *)this;

	if(!pFE)
		return DV_E_FORMATETC;

	if(!(DVASPECT_CONTENT & pFE->dwAspect))
		return DV_E_DVASPECT;

	//now check for an appropriate TYMED.
	DWORD    i;
	HRESULT  hr = DV_E_TYMED;

	for(i = 0; i < pDOEx->m_cDataCount; i++){
		if(pFE->tymed & pDOEx->m_pFormatEtc[i].tymed){
			if(pFE->cfFormat == pDOEx->m_pFormatEtc[i].cfFormat)
				return S_OK;
			else
				hr = DV_E_CLIPFORMAT;
		}else
			hr = DV_E_TYMED;
	}

	return hr;
}

STDMETHODIMP DataObject_GetCanonicalFormatEtc(IDataObject *this, LPFORMATETC pFE1, LPFORMATETC pFE2)
{
	return DATA_S_SAMEFORMATETC;
}

STDMETHODIMP DataObject_SetData(IDataObject *this, LPFORMATETC pFE , LPSTGMEDIUM pSM, BOOL fRelease)
{
	DataObjectEx * pDOEx = (DataObjectEx *)this;

	LPFORMATETC pfeNew;
	LPSTGMEDIUM psmNew;

	pDOEx->m_cDataCount++;

	pfeNew = calloc(1, sizeof(FORMATETC)*pDOEx->m_cDataCount);
	psmNew = calloc(1, sizeof(STGMEDIUM)*pDOEx->m_cDataCount);

	//copy the existing data
	if(pDOEx->m_pFormatEtc){
		memcpy(pfeNew, pDOEx->m_pFormatEtc, sizeof(FORMATETC) * (pDOEx->m_cDataCount - 1));
	}
	if(pDOEx->m_pStgMedium){
		memcpy(psmNew, pDOEx->m_pStgMedium, sizeof(STGMEDIUM) * (pDOEx->m_cDataCount - 1));
	}

	//add the new data
	pfeNew[pDOEx->m_cDataCount - 1] = *pFE;
	if(fRelease){
		psmNew[pDOEx->m_cDataCount - 1] = *pSM;
	}else{
		CopyStgMedium(pSM, &psmNew[pDOEx->m_cDataCount - 1]);
	}

	LPFORMATETC pfeTemp = pDOEx->m_pFormatEtc;
	pDOEx->m_pFormatEtc = pfeNew;
	pfeNew = pfeTemp;

	LPSTGMEDIUM psmTemp = pDOEx->m_pStgMedium;
	pDOEx->m_pStgMedium = psmNew;
	psmNew = psmTemp;

	if(pfeNew) free(pfeNew);
	if(psmNew) free(psmNew);

	if(pDOEx->m_pFormatEtc && pDOEx->m_pStgMedium)
		return S_OK;

	return E_OUTOFMEMORY;
}

STDMETHODIMP DataObject_EnumFormatEtc(IDataObject *this, DWORD dwDir, LPENUMFORMATETC *ppEnum)
{
	DataObjectEx * pDOEx = (DataObjectEx *)this;

	*ppEnum = NULL;

	switch(dwDir){
	case DATADIR_GET:
		*ppEnum = EnumFORMATETC_CEnumFormatEtc(pDOEx->m_pFormatEtc, pDOEx->m_cDataCount);

		if(*ppEnum)
			return S_OK;
		else
			return E_OUTOFMEMORY;

		break;

	default:
		return OLE_S_USEREG;
	}
}

STDMETHODIMP DataObject_DAdvise(IDataObject *this, LPFORMATETC pFE, DWORD advf, LPADVISESINK pAdvSink, LPDWORD pdwConnection)
{
	return E_NOTIMPL;
}

STDMETHODIMP DataObject_DUnadvise(IDataObject *this, DWORD dwConnection)
{
	return E_NOTIMPL;
}

STDMETHODIMP DataObject_EnumDAdvise(IDataObject *this, LPENUMSTATDATA *ppenumAdvise)
{
	return OLE_E_ADVISENOTSUPPORTED;
}

