// IEnumFormatEtc.c

#include "IDragDrop.h"

//--------------------------------------------------------------
// IEnumFORMATETC vtable
//--------------------------------------------------------------
struct IEnumFORMATETCVtbl ienumFORMATETCVtbl = {
	EnumFORMATETC_QueryInterface,
	EnumFORMATETC_AddRef,
	EnumFORMATETC_Release,
	EnumFORMATETC_Next,
	EnumFORMATETC_Skip,
	EnumFORMATETC_Reset,
	EnumFORMATETC_Clone
};
IEnumFORMATETC EnumFORMATETC = {&ienumFORMATETCVtbl};

//--------------------------------------------------------------
// IEnumFORMATETC Ex constructor
//--------------------------------------------------------------
IEnumFORMATETC * EnumFORMATETC_CEnumFormatEtc(LPFORMATETC pFE, int nItems)
{
	 // alloc enough mem for interface and any class vars
	IEnumFORMATETC * pEnumFORMATETC = malloc(sizeof(EnumFORMATETCEx));
	if(!pEnumFORMATETC)
		return NULL;

	// copy the interface into the EnumFORMATETCEx
	memcpy(pEnumFORMATETC, &EnumFORMATETC, sizeof(ienumFORMATETCVtbl));

	// cast the local EnumFORMATETCEx pointer
	EnumFORMATETCEx * pEFEx = (EnumFORMATETCEx *)pEnumFORMATETC;

	// init the vars
	pEFEx->m_pStrFE    = NULL;
	pEFEx->m_iCur      = 0;
	pEFEx->m_cItems	   = nItems;
	pEFEx->m_cRefCount = 0;

	pEFEx->m_pStrFE = malloc(sizeof(FORMATETC)*nItems);
	if(!pEFEx->m_pStrFE)
		return NULL;

	memcpy(pEFEx->m_pStrFE, pFE, sizeof(FORMATETC) * nItems);

	// increment the class reference count
	pEnumFORMATETC->lpVtbl->AddRef(pEnumFORMATETC);
	return pEnumFORMATETC;
}

//-----------------------------------------------------------------
// IUnknown Interface
//-----------------------------------------------------------------
STDMETHODIMP EnumFORMATETC_QueryInterface(IEnumFORMATETC *this, REFIID riid, LPVOID *ppvOut)
{
	if(IsEqualIID(riid, &IID_IUnknown) || IsEqualIID(riid, &IID_IEnumFORMATETC)){
		*ppvOut = (void*)this;
		this->lpVtbl->AddRef(this);
		return S_OK;
	}
	*ppvOut = NULL;
	return E_NOINTERFACE;
}

STDMETHODIMP_(ULONG) EnumFORMATETC_AddRef(IEnumFORMATETC *this)
{
	// cast the local EnumFORMATETCEx pointer
	EnumFORMATETCEx * pEFEx = (EnumFORMATETCEx *)this;
	return ++pEFEx->m_cRefCount;
}

STDMETHODIMP_(ULONG) EnumFORMATETC_Release(IEnumFORMATETC *this)
{
	EnumFORMATETCEx * pEFEx = (EnumFORMATETCEx *)this;

	if(--pEFEx->m_cRefCount == 0){
		if(pEFEx->m_pStrFE)
			free(pEFEx->m_pStrFE);
		free(this);
		return 0;
	}
	return pEFEx->m_cRefCount;
}

//--------------------------------------------------------------
// IEnumFORMATETC Interface
//--------------------------------------------------------------
STDMETHODIMP EnumFORMATETC_Next(IEnumFORMATETC *this, ULONG celt, LPFORMATETC pFE, ULONG *puFetched)
{
	EnumFORMATETCEx * pEFEx = (EnumFORMATETCEx *)this;

	ULONG cReturn = 0L;

	if(celt <= 0 || pFE == NULL || pEFEx->m_iCur >= pEFEx->m_cItems)
		return S_FALSE;

	if(puFetched == NULL && celt != 1)
		return S_FALSE;

	if(puFetched != NULL)
		*puFetched = 0;

	while(pEFEx->m_iCur < pEFEx->m_cItems && celt > 0){
		*pFE++ = pEFEx->m_pStrFE[pEFEx->m_iCur++];
		cReturn++;
		celt--;
	}

	if(NULL != puFetched)
		*puFetched = (cReturn - celt);

	return S_OK;
}

STDMETHODIMP EnumFORMATETC_Skip(IEnumFORMATETC *this, ULONG celt)
{
	EnumFORMATETCEx * pEFEx = (EnumFORMATETCEx *)this;

	if((pEFEx->m_iCur + celt) >= pEFEx->m_cItems)
		return S_FALSE;

	pEFEx->m_iCur += celt;

	return S_OK;
}

STDMETHODIMP EnumFORMATETC_Reset(IEnumFORMATETC *this)
{
	EnumFORMATETCEx * pEFEx = (EnumFORMATETCEx *)this;

	pEFEx->m_iCur = 0;
	return S_OK;
}

STDMETHODIMP EnumFORMATETC_Clone(IEnumFORMATETC *this, IEnumFORMATETC **ppCloneEnumFormatEtc)
{
	EnumFORMATETCEx * pEFEx = (EnumFORMATETCEx *)this;

	IEnumFORMATETC * newEnum;

	if(NULL == ppCloneEnumFormatEtc)
		return S_FALSE;

	newEnum = EnumFORMATETC_CEnumFormatEtc(pEFEx->m_pStrFE, pEFEx->m_cItems);
	if(!newEnum){
		*ppCloneEnumFormatEtc = NULL;
		return E_OUTOFMEMORY;
	}

	*ppCloneEnumFormatEtc = newEnum;

	// cast another local EnumFORMATETCEx pointer to the new interface
	EnumFORMATETCEx * pEFExtmp = (EnumFORMATETCEx *)newEnum;

	pEFExtmp->m_iCur = pEFEx->m_iCur;

	return S_OK;
}


