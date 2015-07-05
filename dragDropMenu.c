DragDrop Menu
plik resource.h poniżej
===================================
#define IDD_MAIN                1000
#define IDC_INFO                1002
#define IDR_MENU1               10000
#define IDM_1                   10002
#define IDM_2                   10003
#define IDM_3                   10004
#define IDM_4                   10005
#define IDM_5                   10006
#define IDM_10                  10008
#define IDM_11                  10007
#define IDM_12                  10009
#define IDM_13                  10010
#define IDM_14                  10011
#define IDM_CUT                 10012
#define IDM_COPY                10013
#define IDM_PASTE               10014
#define IDM_DELETE              10017
#define IDM_FIND                10015
#define IDM_REPLACE             10016
#define IDM_UNDO                10018
#define IDM_REDO                10019
===================================
plik example.rc poniżej:
===================================
#include "resource.h"
IDD_MAIN DIALOGEX 6,6,217,56
CAPTION "IDD_DLG"
FONT 8,"MS Sans Serif",0,0
MENU IDR_MENU1
STYLE 0x10CF0800
EXSTYLE 0x00000008
BEGIN
  CONTROL "Drag & Drop Menu",1001,"Static",0x50001201,28,5,168,15
  CONTROL "",IDC_INFO,"Static",0x50000000,50,24,122,24,0x00000200
END
IDR_MENU1 MENU
BEGIN
  POPUP "File"
  BEGIN
    MENUITEM "item 0",IDM_1
    MENUITEM "item 1",IDM_2
    MENUITEM "item 2",IDM_3
    MENUITEM "item 3",IDM_4
    MENUITEM SEPARATOR
    MENUITEM "item 5",IDM_5
  END
  POPUP "View"
  BEGIN
    MENUITEM "item 0",IDM_10
    MENUITEM "item 1",IDM_11
    MENUITEM "item 2",IDM_12
    MENUITEM "item 3",IDM_13
    MENUITEM "item 4",IDM_14
  END
  POPUP "Edit"
  BEGIN
    MENUITEM "Cut\tCtrl+X",IDM_CUT
    MENUITEM "Copy\tCtrl+C",IDM_COPY
    MENUITEM "Paste\tCtrl+V",IDM_PASTE
    MENUITEM "Delete\tDEL",IDM_DELETE
    MENUITEM SEPARATOR
    MENUITEM "Find\tCtrl+F",IDM_FIND
    MENUITEM "Replace\tCtrl+G",IDM_REPLACE
    MENUITEM SEPARATOR
    MENUITEM "Undo\tCtrl+Z",IDM_UNDO
    MENUITEM "Redo\tCtrl+Y",IDM_REDO
  END
END
===================================
plik dragdrop.h poniżej
===================================
#include <ole2.h>

class CUnknown
{
public:
	virtual STDMETHODIMP QueryInterface(IID *riid, void **ppvObject);
	virtual STDMETHODIMP AddRef();
	virtual STDMETHODIMP Release();
};

class CDropSource : CUnknown
{
	virtual STDMETHODIMP QueryInterface(IID *riid, void **ppvObject);
	//
	virtual STDMETHODIMP QueryContinueDrag(BOOL fEscapePressed,DWORD grfKeyState);
	virtual STDMETHODIMP GiveFeedback(DWORD dwEffect);
};

class CDataObject : CUnknown
{
	virtual STDMETHODIMP QueryInterface(IID *riid, void **ppvObject);
	//
	virtual STDMETHODIMP GetData(FORMATETC *pformatetcIn,/*out*/STGMEDIUM *pmedium);
	virtual STDMETHODIMP GetDataHere(FORMATETC *pformatetc,/*out*/STGMEDIUM *pmedium);
	virtual STDMETHODIMP QueryGetData(FORMATETC *pformatetc);
	virtual STDMETHODIMP GetCanonicalFormatEtc(FORMATETC *pformatectIn,/*out*/FORMATETC *pformatetcOut);
	virtual STDMETHODIMP SetData(FORMATETC *pformatetc,STGMEDIUM *pmedium,BOOL fRelease);
	virtual STDMETHODIMP EnumFormatEtc(DWORD dwDirection,/*out*/IEnumFORMATETC **ppenumFormatEtc);
	virtual STDMETHODIMP DAdvise(FORMATETC *pformatetc,DWORD advf,IAdviseSink *pAdvSink,/*out*/DWORD *pdwConnection);
	virtual STDMETHODIMP DUnadvise(DWORD dwConnection);
	virtual STDMETHODIMP EnumDAdvise(/*out*/IEnumSTATDATA **ppenumAdvise);
};

class CDropTarget : CUnknown
{
public:
	virtual STDMETHODIMP QueryInterface(IID *riid, void **ppvObject);
	//
	virtual STDMETHODIMP DragEnter(IDataObject *pDataObj,DWORD grfKeyState,POINTL pt,DWORD *pdwEffect);
	virtual STDMETHODIMP DragOver(DWORD grfKeyState,POINTL pt,DWORD *pdwEffect);
	virtual STDMETHODIMP DragLeave();
	virtual STDMETHODIMP Drop(IDataObject *pDataObj,DWORD grfKeyState,POINTL pt,DWORD *pdwEffect);
	//
	void SetSource(HWND hwnd, HMENU hMenu, int item);
	void SetDestination(HMENU hMenu, int item);

	HWND    m_srcHwnd; // source window handle
	HMENU   m_srcMenu; // source menu handle
	INT     m_srcPos;  // source menu item
	HMENU   m_dstMenu; // destination menu handle
	int     m_dstPos;  // destination menu item
};

#ifndef MENUGETOBJECTINFO // to dla dev-cpp
struct MENUGETOBJECTINFO {
  DWORD  dwFlags;
  UINT  uPos;
  HMENU  hmenu;
  PVOID  riid;
  PVOID  pvObj;
};
#define MNGO_NOINTERFACE     0x00000000
#define MNGO_NOERROR         0x00000001
#define MND_CONTINUE       0
#define MND_ENDMENU        1

#endif
===================================
plik main.cpp poniżej
===================================
#define WINVER 0x7777
#include <windows.h>
#include "dragdrop.h"
#include "resource.h"

BOOL CALLBACK DialogProcMain(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);


//--------------------------------------
class CApp
{
public:
	BOOL WndProc(UINT uMsg,WPARAM wParam,LPARAM lParam);

	CDropSource m_dropsource;
	CDropTarget m_droptarget;
	CDataObject m_dataobject;

	HWND m_hwnd;
};

//--------------------------------------
CApp *app;


int WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
{
	CApp _app;
	app = &_app;
	DialogBoxParam(hInstance, (TCHAR*)IDD_MAIN, 0, DialogProcMain, 0);
	return 0;
}



BOOL CApp::WndProc(UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_INITDIALOG:
		{
			OleInitialize(0);
			MENUINFO mi;
			mi.cbSize = sizeof(mi);
			mi.fMask = MIM_STYLE;
			mi.dwStyle = MNS_DRAGDROP;
			SetMenuInfo(GetMenu(m_hwnd), &mi);
			break;
		}
		case WM_CLOSE:
			OleUninitialize();
			EndDialog(m_hwnd, 0);
			break;

		case WM_MENUGETOBJECT:
		{
			#define pmgo ((MENUGETOBJECTINFO*)lParam)
			HRESULT hr = MNGO_NOINTERFACE;

			// zapisz cel drag-drop
			app->m_droptarget.SetDestination((HMENU)(pmgo->hmenu), (int)pmgo->uPos);

			if (!app->m_droptarget.QueryInterface((IID*)pmgo->riid, &pmgo->pvObj))
				hr = MNGO_NOERROR;

			SetWindowLong(m_hwnd, DWL_MSGRESULT, hr);
			break;
		}
		case WM_MENUDRAG:
		{
			// zapisz źródło drag-drop
			app->m_droptarget.SetSource(m_hwnd, (HMENU)lParam, (int)wParam);
			// zacznij drag-grop
			DWORD dwEffect;
			DoDragDrop((IDataObject*)&app->m_dataobject, (IDropSource*)&app->m_dropsource, DROPEFFECT_MOVE, &dwEffect);
			SetWindowLong(m_hwnd, DWL_MSGRESULT, MND_CONTINUE);
			break;
		}
		default:
			return false;
	}
	return true;
}



BOOL CALLBACK DialogProcMain(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	if (uMsg == WM_INITDIALOG) app->m_hwnd = hwnd;
	return app->WndProc(uMsg, wParam, lParam);
}
===================================
plik dragdrop.cpp poniżej
===================================
#include "dragdrop.h"
#include "resource.h"


//--------------------------------------------------------------------------------
// CUnknown
//--------------------------------------------------------------------------------
STDMETHODIMP CUnknown::QueryInterface(IID *riid, void **ppvObject)
{
	if (!riid || !ppvObject)
		return E_INVALIDARG;

	if (*riid == IID_IUnknown)
	{
		*ppvObject = (void*)this;
		return S_OK;
	}
	*ppvObject = NULL;
	return E_NOINTERFACE;
}
//--------------------------------------------------------------------------------
STDMETHODIMP CUnknown::AddRef()
{
	return 1;
}
//--------------------------------------------------------------------------------
STDMETHODIMP CUnknown::Release()
{
	return 1;
}

//--------------------------------------------------------------------------------
// CDropSource
//--------------------------------------------------------------------------------
STDMETHODIMP CDropSource::QueryInterface(IID *riid, void **ppvObject)
{
	if (*riid == IID_IDropSource) {
		riid = (IID*)&IID_IUnknown;
	}
	return CUnknown::QueryInterface(riid, ppvObject);
}

//--------------------------------------------------------------------------------
STDMETHODIMP CDropSource::QueryContinueDrag(BOOL fEscapePressed,DWORD grfKeyState)
{
	if (fEscapePressed)
		return DRAGDROP_S_CANCEL;

	if (!(grfKeyState & VK_LBUTTON))
		return DRAGDROP_S_DROP;

	return S_OK;
}

//--------------------------------------------------------------------------------
STDMETHODIMP CDropSource::GiveFeedback(DWORD dwEffect)
{
	return DRAGDROP_S_USEDEFAULTCURSORS;
}



//--------------------------------------------------------------------------------
// CDropTarget
//--------------------------------------------------------------------------------
void CDropTarget::SetSource(HWND hwnd, HMENU hMenu, int item)
{
	m_srcHwnd  = hwnd;
	m_srcMenu = hMenu;
	m_srcPos  = item;
}

//--------------------------------------------------------------------------------
void CDropTarget::SetDestination(HMENU hMenu, int item)
{
	m_dstMenu = hMenu;
	m_dstPos = item;
}

//--------------------------------------------------------------------------------
STDMETHODIMP CDropTarget::QueryInterface(IID *riid, void **ppvObject)
{
	if (*riid == IID_IDropTarget) {
		riid = (IID*)&IID_IUnknown;
	}
	return CUnknown::QueryInterface(riid, ppvObject);
}

//--------------------------------------------------------------------------------
STDMETHODIMP CDropTarget::DragEnter(IDataObject *pDataObj,DWORD grfKeyState,POINTL pt,DWORD *pdwEffect)
{
	*pdwEffect = DROPEFFECT_MOVE;
	return S_OK;
}

//--------------------------------------------------------------------------------
STDMETHODIMP CDropTarget::DragOver(DWORD grfKeyState,POINTL pt,DWORD *pdwEffect)
{
	*pdwEffect = DROPEFFECT_MOVE;
	return S_OK;
}

//--------------------------------------------------------------------------------
STDMETHODIMP CDropTarget::DragLeave()
{
	return S_OK;
}

//--------------------------------------------------------------------------------
STDMETHODIMP CDropTarget::Drop(IDataObject *pDataObj,DWORD grfKeyState,POINTL pt,DWORD *pdwEffect)
{
	MessageBeep(MB_ICONINFORMATION);
	char info[256];
	wsprintf(info, "Move from	0x%X:%d\nto		0x%X:%d", m_srcMenu, m_srcPos, m_dstMenu, m_dstPos);
	SetDlgItemText(m_srcHwnd, IDC_INFO, info);

//	MoveMenuItem(m_srcHwnd, m_srcMenu, m_srcPos, m_dstMenu, m_dstPos);
	return S_OK;
}



//--------------------------------------------------------------------------------
// CDataObject
//--------------------------------------------------------------------------------
STDMETHODIMP CDataObject::QueryInterface(IID *riid, void **ppvObject)
{
	if (*riid == IID_IDataObject) {
		riid = (IID*)&IID_IUnknown;
	}
	return CUnknown::QueryInterface(riid, ppvObject);
}

//--------------------------------------------------------------------------------
STDMETHODIMP CDataObject::GetData(FORMATETC *pformatetcIn,/*out*/STGMEDIUM *pmedium)
{
	return E_NOTIMPL;
}

//--------------------------------------------------------------------------------
STDMETHODIMP CDataObject::GetDataHere(FORMATETC *pformatetc,/*out*/STGMEDIUM *pmedium)
{
	return E_NOTIMPL;
}

//--------------------------------------------------------------------------------
STDMETHODIMP CDataObject::QueryGetData(FORMATETC *pformatetc)
{
	return E_NOTIMPL;
}

//--------------------------------------------------------------------------------
STDMETHODIMP CDataObject::GetCanonicalFormatEtc(FORMATETC *pformatectIn,/*out*/FORMATETC *pformatetcOut)
{
	return E_NOTIMPL;
}

//--------------------------------------------------------------------------------
STDMETHODIMP CDataObject::SetData(FORMATETC *pformatetc,STGMEDIUM *pmedium,BOOL fRelease)
{
	return E_NOTIMPL;
}

STDMETHODIMP CDataObject::EnumFormatEtc(DWORD dwDirection,/*out*/IEnumFORMATETC **ppenumFormatEtc)
{
	return E_NOTIMPL;
}

//--------------------------------------------------------------------------------
STDMETHODIMP CDataObject::DAdvise(FORMATETC *pformatetc,DWORD advf,IAdviseSink *pAdvSink,/*out*/DWORD *pdwConnection)
{
	return E_NOTIMPL;
}

//--------------------------------------------------------------------------------
STDMETHODIMP CDataObject::DUnadvise(DWORD dwConnection)
{
	return E_NOTIMPL;
}

//--------------------------------------------------------------------------------
STDMETHODIMP CDataObject::EnumDAdvise(/*out*/IEnumSTATDATA **ppenumAdvise)
{
	return E_NOTIMPL;
}