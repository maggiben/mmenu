// APIReflect.h: interface for the CAPIReflect class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_APIREFLECT_H__DAEE1AC4_8600_11D2_B10E_000000000000__INCLUDED_)
#define AFX_APIREFLECT_H__DAEE1AC4_8600_11D2_B10E_000000000000__INCLUDED_

#if _MSC_VER > 1000
   #pragma once
#endif // _MSC_VER > 1000


#include "HookAPI.h"


// initialization steps
//
const DWORD APIR_STATE_INIT   = 0;
const DWORD APIR_STATE_ENABLE = 1;


// trace level
//
const DWORD API_TRACE_LEVEL_NONE                   =  0;
const DWORD API_TRACE_LEVEL_GETPROCADDRESS         =  1;
const DWORD API_TRACE_LEVEL_GETMODULEHANDLE        =  2;
const DWORD API_TRACE_LEVEL_REFLECT_CALL           =  4;
const DWORD API_TRACE_LEVEL_DOREFLECT              =  8;
const DWORD API_TRACE_LEVEL_INIT                   = 16;
const DWORD API_TRACE_LEVEL_HOOK_IMPORTED_FUNCTION = 32;



// declaration of two static and a type corresponding to the function prototype
//
#define BEGIN_REFLECT_LIST() \
   static void TraceReflectCall(LPCTSTR FunctionName, DWORD Value)\
   {                                                              \
      if (API_TRACE_LEVEL_REFLECT_CALL & m_dwTraceLevel)          \
      {                                                           \
         CString szInfo;                                          \
         szInfo.Format(_T("%s() -> %08x\n"), FunctionName, Value);\
         TRACE(szInfo);                                           \
      }                                                           \
   }


// macros for API reflection
//
// APIR_STATE_ENABLE : reflect the API for the given module
// APIR_STATE_INIT   : get default value via ::GetProcAddress()
//
#define DEFINE_API_REFLECT(Class, Name) \
   Class##::Name##Proc Class##::__##Name = NULL 

#define BEGIN_IMPLEMENT_API_REFLECT() \
   PROC pfnOld; \
   BOOL bReturn = TRUE;

#define IMPLEMENT_API_REFLECT(hModule, From, Name)                         \
   if (APIR_STATE_ENABLE == dwState)                                       \
   {                                                                       \
      pfnOld = HookImportedFunction(hModule, From, #Name, (PROC)_##Name);  \
      if (pfnOld != NULL)                                                  \
      {                                                                    \
         TCHAR szModuleName[_MAX_PATH+1];                                  \
         ::GetModuleFileName(hModule, szModuleName, _MAX_PATH);            \
         if (API_TRACE_LEVEL_HOOK_IMPORTED_FUNCTION & m_dwTraceLevel)      \
            TRACE(                                                         \
               _T("HookImportedFunction(%s, %s)\n"),                       \
               szModuleName,                                               \
               #Name                                                       \
               );                                                          \
      }                                                                    \
   }                                                                       \
   else                                                                    \
   if (APIR_STATE_INIT == dwState)                                         \
   {                                                                       \
      __##Name =                                                           \
       (Name##Proc)(FARPROC)_GetProcAddress(_GetModuleHandle(From),#Name); \
   }

#define END_IMPLEMENT_API_REFLECT() \
   return(bReturn);




class CAPIReflect
{
// construction/destruction
// 
public:
	CAPIReflect();
	virtual ~CAPIReflect();


// C++ interface
// 
public:
   DWORD SetTraceLevel(DWORD dwTraceLevel);


// overridable methods
// 
public:
   virtual BOOL Init();
   virtual BOOL DoReflect(HMODULE hModule);
   virtual BOOL FillStubs(DWORD dwState, HMODULE hModule) = 0;


// internal helpers
// 
protected:
   HMODULE _GetModuleHandle(LPCTSTR szModule);
   FARPROC _GetProcAddress(HMODULE hModule, LPCTSTR szName);


// static members
// 
protected:
   static DWORD m_dwTraceLevel;
};


#endif // !defined(AFX_APIREFLECT_H__DAEE1AC4_8600_11D2_B10E_000000000000__INCLUDED_)
