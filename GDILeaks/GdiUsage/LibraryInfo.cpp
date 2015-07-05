#include "stdafx.h"
#include "LibraryInfo.h"

#ifdef _DEBUG
   #undef THIS_FILE
   static char THIS_FILE[]=__FILE__;
   #define new DEBUG_NEW
#endif

CLibraryInfo::CLibraryInfo(LPCTSTR szDllName, DWORD dwLoadAddress, DWORD preferredLoadAddr, LPCTSTR szReason)
{
   m_szDllName             = szDllName;
   m_dwLoadAddress         = dwLoadAddress;
   m_dwPreferedLoadAddress = preferredLoadAddr;
   m_bLoaded               = TRUE;
   m_szReason              = szReason;

   m_iLoaded               = 0;
   m_iRemoved              = 0;
}

CLibraryInfo::~CLibraryInfo()
{
}
