#if !defined(AFX_GDITRACE_H__7EFE4975_85DA_11D2_B10E_000000000000__INCLUDED_)
#define AFX_GDITRACE_H__7EFE4975_85DA_11D2_B10E_000000000000__INCLUDED_

#if _MSC_VER > 1000
   #pragma once
#endif // _MSC_VER > 1000


#ifndef __AFXWIN_H__
   #error include 'stdafx.h' before including this file for PCH
#endif



#include "resource.h"      // main symbols
#include "GDIReflect.h"
#include "..\Common\GdiResourcesDlg.h"


class CGDITraceApp : public CWinApp, public INotificationCallBack
{
//
// constructor
//
public:
   CGDITraceApp();


// C++ interface
//
public:
   void SetSharedVariables(DWORD ProcessID, DWORD NotifThreadID);
   void ResetSharedVariables();
   BOOL GetDataFromRequest(LPVOID pData, DWORD cbSize);
   BOOL SetDataForRequest(LPVOID pData, DWORD cbSize);
   LPVOID GetDataPointer();


// CResourcesDlg callbacks
// 
public:
   virtual void OnDoubleClick(HWND hWnd, DWORD hObject);
   virtual void OnSelect(HWND hWnd, DWORD hObject);


// implementation details
// 
private:
   // GDI information
   CGDIReflect m_GDISpy;

   // shared memory mapped file
   LPVOID m_lpvMem;
   HANDLE m_hMapObject;
   
   CTypedPtrMap<CMapPtrToPtr, HMODULE, HMODULE> m_LoadedModules;

   static HHOOK  s_hHook;  // instanciated in the debugger context only
   static HANDLE s_hEvent; // instanciated in the debuggee context only


// internal helpers
//
private:
   void OnNewDLL(HMODULE hModule);
   void OnFreeDLL(HMODULE hModule);
   void OnGetList(DWORD Notification);
   void OnShowList(DWORD Count, HWND hWnd);
   BOOL InstallHook();
   BOOL UninstallHook();
   void HookAlreadyLoadedDLLs();
   BOOL StartInfiltratedThread();
   BOOL StopInfiltratedThread();


// static methods
//
private:
   static LRESULT WINAPI GetMessageHookProc(int iCode, WPARAM wParam, LPARAM lParam);
   static unsigned __stdcall InfiltratedThreadProc(LPVOID pData);



//
// MFC stuff
//
//
// Overrides
// ClassWizard generated virtual function overrides
//{{AFX_VIRTUAL(CGDITraceApp)
public:
   virtual BOOL InitInstance();
   virtual int ExitInstance();
//}}AFX_VIRTUAL

//{{AFX_MSG(CGDITraceApp)
   // NOTE - the ClassWizard will add and remove member functions here.
   //    DO NOT EDIT what you see in these blocks of generated code !
//}}AFX_MSG

   DECLARE_MESSAGE_MAP()
};




//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GDITRACE_H__7EFE4975_85DA_11D2_B10E_000000000000__INCLUDED_)
