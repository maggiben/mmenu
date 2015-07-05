#if !defined(AFX_LIBRARYINFO_H__26A75474_8247_11D2_B10B_000000000000__INCLUDED_)
#define AFX_LIBRARYINFO_H__26A75474_8247_11D2_B10B_000000000000__INCLUDED_

#if _MSC_VER > 1000
   #pragma once
#endif // _MSC_VER > 1000




class CLibraryInfo
{
public:
   CLibraryInfo(LPCTSTR szDllName, DWORD dwLoadAddress, DWORD preferredLoadAddr, LPCTSTR szReason);
   virtual ~CLibraryInfo();

public:
   CString  m_szDllName;
   DWORD    m_dwLoadAddress;
   DWORD    m_dwPreferedLoadAddress;
   BOOL     m_bLoaded;
   int      m_iLoaded;
   int      m_iRemoved;
   CString  m_szReason;
};



#endif // !defined(AFX_LIBRARYINFO_H__26A75474_8247_11D2_B10B_000000000000__INCLUDED_)

