#include "stdafx.h"
#include "APIReflect.h"


#ifdef _DEBUG
   #undef THIS_FILE
   static char THIS_FILE[]=__FILE__;
   #define new DEBUG_NEW
#endif



// static members
//
DWORD CAPIReflect::m_dwTraceLevel = API_TRACE_LEVEL_NONE;




// Helpers for easier debugging into "$^*µ" macros
//
DWORD CAPIReflect::SetTraceLevel(DWORD dwTraceLevel)
{
   DWORD dwCurrentLevel = m_dwTraceLevel;
   m_dwTraceLevel = dwTraceLevel;
   return(dwCurrentLevel);
}


HMODULE CAPIReflect::_GetModuleHandle(LPCTSTR szModule)
{
   HMODULE hModule = ::GetModuleHandle(szModule);
   if (API_TRACE_LEVEL_GETMODULEHANDLE & m_dwTraceLevel)
      TRACE("CAPIReflect::_GetModuleHandle(%s) --> 0x%08x\n", szModule, hModule);

   return(hModule);
}


FARPROC CAPIReflect::_GetProcAddress(HMODULE hModule, LPCTSTR szName)
{
   FARPROC pfn = ::GetProcAddress(hModule, szName);
   if (API_TRACE_LEVEL_GETPROCADDRESS & m_dwTraceLevel)
      TRACE("CAPIReflect::_GetProcAddress(0x%08x, %s) --> 0x%08x\n", hModule, szName, pfn);

   return(pfn);
}



// construction/destruction
//
CAPIReflect::CAPIReflect()
{
}

CAPIReflect::~CAPIReflect()
{

}


// default values
//
BOOL CAPIReflect::Init()
{
   if (API_TRACE_LEVEL_INIT & m_dwTraceLevel)
      TRACE("CAPIReflect::Init()\n");

   return(FillStubs(APIR_STATE_INIT, NULL));
}




// reflect API for the given module
// 
BOOL CAPIReflect::DoReflect(HMODULE hModule)
{
   if (API_TRACE_LEVEL_DOREFLECT & m_dwTraceLevel)
      TRACE("CAPIReflect::DoReflect(0x%08x)\n", hModule);

   return(FillStubs(APIR_STATE_ENABLE, hModule));
}


