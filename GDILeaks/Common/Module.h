#if !defined(AFX_MODULE_H__855FFCAA_AB6C_11D1_BE91_000000000000__INCLUDED_)
#define AFX_MODULE_H__855FFCAA_AB6C_11D1_BE91_000000000000__INCLUDED_

#if _MSC_VER >= 1000
   #pragma once
#endif // _MSC_VER >= 1000



class CModule : public CObject  
{
   DECLARE_DYNAMIC(CModule)

// Construction/Destruction
//
public:
   CModule(HMODULE hModule, LPCTSTR szFilename, BOOL bDynamic);
   virtual ~CModule();
private:
   CModule();


// C++ Interface
//
public:
   HMODULE  GetModuleHandle();
   DWORD    GetBaseAddress();
   CString& GetModuleName();
   CString& GetPathName();
   CString& GetFullPathName();
   CString& GetFileTime();
   void     GetFileTime(FILETIME& ft);
   DWORD    GetFileSize();
   CString& GetSubSystem();
   CString& GetLinkTime();
   void     GetLinkTime(FILETIME& ft);
   WORD     GetLinkVersion();
   BOOL     IsDynamic();
   
   // version details
   VS_FIXEDFILEINFO GetFileVersion();
   CString& GetCompanyName();
   CString& GetFileDescription();
   CString& GetVersion();
   CString& GetInternalName();
   CString& GetCopyright();
   CString& GetOriginalFilename();
   CString& GetProductName();
   CString& GetProductVersion();
   CString& GetComments();
   CString& GetTrademarks();
   CString& GetPrivateBuild();
   CString& GetSpecialBuild();

// Implementation helpers
// 
private:
   void Refresh();

// Implementation details
//
private:
   BOOL     m_bDynamic;

   HMODULE  m_hModule;
   DWORD    m_BaseAddress;
   CString  m_szModuleName;
   CString  m_szPathName;
   CString  m_szFullPathName;
   FILETIME m_FileTime;
   CString  m_szFileTime;
   DWORD    m_FileSize;
   CString  m_szSubSystem;
   FILETIME m_LinkTime;
   CString  m_szLinkTime;
   WORD     m_LinkVersion;

   // version details
   VS_FIXEDFILEINFO m_VersionInfo;
   CString m_szCompanyName;
   CString m_szFileDescription;
   CString m_szFileVersion;
   CString m_szInternalName;
   CString m_szLegalCopyright;
   CString m_szOriginalFilename;
   CString m_szProductName;
   CString m_szProductVersion;
   CString m_szComments;
   CString m_szLegalTrademarks;
   CString m_szPrivateBuild;
   CString m_szSpecialBuild;


// Debug helpers
//
public:
#ifdef _DEBUG
   virtual void AssertValid() const;
   virtual void Dump(CDumpContext& dc) const;
#endif
};



class CModuleList : public CObject  
{
// Construction/Destruction
//
public:
   CModuleList();
   virtual ~CModuleList();


// C++ interface
//
public:
   void Refresh(DWORD PID, HANDLE hProcess, LPCTSTR szFilename);
   int GetCount();
   CModule* GetFirst(int& pos);
   CModule* GetNext(int& pos);


// Internal helpers
//
private:
   void FreeModuleArray();
   BOOL RefreshTOOLHELP32(DWORD PID, LPCTSTR szFilename);


// Implementation details
//
private:
   CObArray m_ModuleArray;
};



#endif // !defined(AFX_MODULE_H__855FFCAA_AB6C_11D1_BE91_000000000000__INCLUDED_)
