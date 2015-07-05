#ifndef __GDITRACE_H
#define __GDITRACE_H

#ifndef _GDITRACE_EXPORT
   #define DLL_SPEC __declspec(dllimport)
#else
   #define DLL_SPEC __declspec(dllexport)
#endif


#ifdef	__cplusplus
   extern "C" {
#endif	


// functions interface 
//
   DLL_SPEC void WINAPI StartTraceGDI(DWORD ProcessID, DWORD NotifThreadID);
   DLL_SPEC void WINAPI StopTraceGDI();
   DLL_SPEC BOOL WINAPI PatchModule(HMODULE hModule);
   DLL_SPEC BOOL WINAPI UnpatchModule(HMODULE hModule);
   DLL_SPEC BOOL WINAPI RequestList(DWORD Notification);
   DLL_SPEC BOOL WINAPI ShowRemoteList(DWORD Count, HWND hWnd);
   DLL_SPEC BOOL WINAPI GetDataFromRequest(LPVOID pData, DWORD cbSize);
   DLL_SPEC BOOL WINAPI SetDataForRequest(LPVOID pData, DWORD cbSize);
   DLL_SPEC LPVOID WINAPI GetDataPointer();


// notification messages values
//
   const DWORD TM_NEW_DLL   = WM_APP+100;
   const DWORD TM_FREE_DLL  = WM_APP+101;
   const DWORD TM_GET_LIST  = WM_APP+102;
   const DWORD TM_SHOW_LIST = WM_APP+103;
//
// communication messages description
//
// ID             wParam      lParam      Description 
// -------------------------------------------------------------------------------------
// TM_NEW_DLL     0           hModule     notif the engine to spy this DLL too
//
// TM_FREE_DLL    0           hModule     notif the engine that the DLL is unloaded
//
// TM_GET_LIST    Notif ID    0           fill a shared memory mapped file with the list
//                                        and notify the debugger thread with Notif ID when
//                                        the list is ready
//
// TM_SHOW_LIST   Obj count   hWnd        show the list with "hWnd" as parent
//                                        and the shared memory mapped file contains the 
//                                        list of GDI objects to display



#ifdef	__cplusplus
   }
#endif	




// structures & types
//

// for GET_LIST message, see GdiResources.h
//


// for NEW_DLL message
//
// warning C4200: nonstandard extension used : zero-sized array in struct/union
#pragma warning( disable : 4200 )
   typedef struct 
   {
      DWORD    dwCount;
      HMODULE  hModules[]; 
   } NEWMODULELIST;
#pragma warning( default : 4200 )



#endif
