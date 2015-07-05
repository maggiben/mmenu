#ifndef __GDINJECT_H
#define __GDINJECT_H


#ifndef _GDINJECT_EXPORT
   #define DLL_SPEC __declspec(dllimport)
#else
   #define DLL_SPEC __declspec(dllexport)
#endif


#ifdef	__cplusplus
   extern "C" {
#endif	


// functions interface 
//
   DLL_SPEC BOOL WINAPI ExecuteRemoteAction(DWORD ProcessID, DWORD Action);
   DLL_SPEC LPVOID WINAPI GetDataPointer();
   DLL_SPEC BOOL WINAPI GetDataFromRequest(LPVOID pData, DWORD cbSize);
   DLL_SPEC BOOL WINAPI SetDataForRequest(LPVOID pData, DWORD cbSize);


// notification messages values
//
   const DWORD TM_SHOW_LIST   = WM_APP + 101;

// communication messages description
//
// ID             wParam      lParam      Description 
// -------------------------------------------------------------------------------------
// TM_SHOW_LIST   Obj count   hWnd        show the list with "hWnd" as parent
//                                        and the shared memory mapped file contains the 
//                                        list of GDI objects to display


#ifdef	__cplusplus
   }
#endif	



#endif