#include "stdafx.h"
#include "Module.h"

#include "psapi.h"
#include "PEEXE.H"
#include "ModulVer.h"
#include "Wrappers.h"
#include "Helpers.h"
#include "DEPENDENCYLIST.h"



#ifdef _DEBUG
   #undef THIS_FILE
   static char THIS_FILE[]=__FILE__;
   #define new DEBUG_NEW
#endif




// from WINNT.H
//
void GetSubsystemString(WORD SubSystem, CString& szSubSystem)
{
   switch(SubSystem)
   {
      case IMAGE_SUBSYSTEM_NATIVE:
         szSubSystem = _T("Native");
      break;

      case IMAGE_SUBSYSTEM_WINDOWS_GUI:
         szSubSystem = _T("GUI");
      break;

      case IMAGE_SUBSYSTEM_WINDOWS_CUI:
         szSubSystem = _T("Console");
      break;

      case IMAGE_SUBSYSTEM_OS2_CUI:
         szSubSystem = _T("OS/2");
      break;

      case IMAGE_SUBSYSTEM_POSIX_CUI:
         szSubSystem = _T("Posix");
      break;

      case IMAGE_SUBSYSTEM_NATIVE_WINDOWS:
         szSubSystem = _T("Win9x driver");
      break;

      case IMAGE_SUBSYSTEM_WINDOWS_CE_GUI:
         szSubSystem = _T("CE");
      break;

      case IMAGE_SUBSYSTEM_UNKNOWN:
      default:
         szSubSystem = _T("Unknown");
      break;
   }
}


IMPLEMENT_DYNAMIC(CModule, CObject)


//--------------------------------------------------------------------
// construction/destruction
//
void CModule::Refresh()
{
// from Matt Pietrek's DEPENDS (MSJ February 1997)
//
// get file date/time
   HFILE FileHandle = _lopen(m_szFullPathName, OF_READ);
   if (HFILE_ERROR != FileHandle)
   {
      if (::GetFileTime((HANDLE)FileHandle, 0, 0, &m_FileTime))
      {
         TCHAR szFileDate[32] = { 0 };
         TCHAR szFileTime[32] = { 0 };

         GetFileDateAsString(&m_FileTime, szFileDate, sizeof(szFileDate));
         GetFileTimeAsString(&m_FileTime, szFileTime, sizeof(szFileTime), TRUE);
         m_szFileTime.Format(_T("%s %s"), szFileDate, szFileTime);
      }

      _lclose(FileHandle);
   }

// from Matt Pietrek's DEPENDS (MSJ February 1997)
//
// get PE information
   TCHAR szFileDate[32] = { 0 };
   TCHAR szFileTime[32] = { 0 };

   PE_EXE exe(m_szFullPathName);
   if (!exe.IsValid())
   {
      m_FileSize = 0;
      TRACE("Failure for %s\n", (LPCTSTR)m_szFullPathName);
      return;
   }

// get link time and version...
   TimeDateStampToFileTime(exe.GetTimeDateStamp(), &m_LinkTime);
   GetFileDateAsString(&m_LinkTime, szFileDate, sizeof(szFileDate));
   GetFileTimeAsString(&m_LinkTime, szFileTime, sizeof(szFileTime), TRUE);
   m_szLinkTime.Format("%s %s", szFileDate, szFileTime);
   m_LinkVersion = exe.GetLinkerVersion();

// ...and other interesting PE header fields
   GetSubsystemString(exe.GetSubsystem(), m_szSubSystem);
   m_BaseAddress = exe.GetImageBase();

// from Paul DiLasca  (MSJ April 1998)
//
   CModuleVersion ver;

   if (ver.GetFileVersionInfo(m_szFullPathName)) 
   {
      m_VersionInfo = ver;

      m_szCompanyName      = ver.GetValue(_T("CompanyName"));
      m_szFileDescription  = ver.GetValue(_T("FileDescription"));
      m_szFileVersion      = ver.GetValue(_T("FileVersion"));
      m_szInternalName     = ver.GetValue(_T("InternalName"));
      m_szLegalCopyright   = ver.GetValue(_T("LegalCopyright"));
      m_szOriginalFilename = ver.GetValue(_T("OriginalFilename"));
      m_szProductName      = ver.GetValue(_T("ProductName"));
      m_szProductVersion   = ver.GetValue(_T("ProductVersion"));
      m_szComments         = ver.GetValue(_T("Comments"));
      m_szLegalTrademarks  = ver.GetValue(_T("LegalTrademarks"));
      m_szPrivateBuild     = ver.GetValue(_T("PrivateBuild"));
      m_szSpecialBuild     = ver.GetValue(_T("SpecialBuild"));
   }
   else
      ::ZeroMemory(&m_VersionInfo, sizeof(m_VersionInfo));

// and finally, the file size
   m_FileSize = exe.GetFileSize();
}


CModule::CModule(HMODULE hModule, LPCTSTR szFilename, BOOL bDynamic)
{
   m_hModule = hModule;
   m_bDynamic = bDynamic;
   m_szFullPathName = szFilename; 

// store module name and its path
   CString szExtension;
   _tsplitpath(m_szFullPathName, NULL, NULL, m_szModuleName.GetBuffer(_MAX_PATH), szExtension.GetBuffer(_MAX_PATH));
   szExtension.ReleaseBuffer(-1);
   m_szModuleName.ReleaseBuffer(-1);
   m_szModuleName += szExtension;
   m_szPathName = m_szFullPathName.Left(m_szFullPathName.ReverseFind(_T('\\')));

// these fields are expensive in time to compute
// wait until they are really needed
   m_BaseAddress  = 0;
   m_FileSize     = 0;
   m_LinkVersion  = 0;
   ::ZeroMemory(&m_FileTime, sizeof(m_FileTime));
   ::ZeroMemory(&m_LinkTime, sizeof(m_LinkTime));
   ::ZeroMemory(&m_VersionInfo, sizeof(m_VersionInfo));
}


CModule::~CModule()
{

}
//
//--------------------------------------------------------------------


//--------------------------------------------------------------------
// CModule accessors
//
// the first are computed in the constructor
   HMODULE CModule::GetModuleHandle()
   { 
      return(m_hModule);
   }
   
   CString& CModule::GetModuleName()
   { 
      return(m_szModuleName);
   }

   CString& CModule::GetPathName()
   { 
      return(m_szPathName);
   }

   CString& CModule::GetFullPathName()
   { 
      return(m_szFullPathName);
   }

   BOOL CModule::IsDynamic()
   {
      return(m_bDynamic);
   }



// the rest are computed when needed
   DWORD CModule::GetBaseAddress()
   { 
      if (m_FileSize == 0)
         Refresh();

      return(m_BaseAddress);
   }

   CString& CModule::GetFileDescription()
   { 
      if (m_FileSize == 0)
         Refresh();

      return(m_szFileDescription);
   } 

   CString& CModule::GetFileTime()
   { 
      if (m_FileSize == 0)
         Refresh();

      return(m_szFileTime);
   }
   
   void CModule::GetFileTime(FILETIME& ft)
   { 
      if (m_FileSize == 0)
         Refresh();

      ft = m_FileTime;
   }

   DWORD CModule::GetFileSize()
   { 
      if (m_FileSize == 0)
         Refresh();

      return(m_FileSize);
   }

   CString& CModule::GetSubSystem()
   { 
      if (m_FileSize == 0)
         Refresh();

      return(m_szSubSystem);
   }

   CString& CModule::GetLinkTime()
   { 
      if (m_FileSize == 0)
         Refresh();

      return(m_szLinkTime);
   } 

   void CModule::GetLinkTime(FILETIME& ft)
   { 
      if (m_FileSize == 0)
         Refresh();

      ft = m_LinkTime;
   }

   WORD CModule::GetLinkVersion()
   { 
      if (m_FileSize == 0)
         Refresh();

      return(m_LinkVersion);
   }

   // version details
   VS_FIXEDFILEINFO CModule::GetFileVersion()
   { 
      if (m_FileSize == 0)
         Refresh();

      return(m_VersionInfo);
   }

   CString& CModule::GetCompanyName()
   { 
      if (m_FileSize == 0)
         Refresh();

      return(m_szCompanyName);
   }

   CString& CModule::GetVersion()
   { 
      if (m_FileSize == 0)
         Refresh();

      return(m_szFileVersion);
   }

   CString& CModule::GetInternalName()
   { 
      if (m_FileSize == 0)
         Refresh();

      return(m_szInternalName);
   }

   CString& CModule::GetCopyright()
   { 
      if (m_FileSize == 0)
         Refresh();

      return(m_szLegalCopyright);
   }
   
   CString& CModule::GetOriginalFilename()
   { 
      if (m_FileSize == 0)
         Refresh();

      return(m_szOriginalFilename);
   }
   
   CString& CModule::GetProductName()
   { 
      if (m_FileSize == 0)
         Refresh();

      return(m_szProductName);
   }

   CString& CModule::GetProductVersion()
   { 
      if (m_FileSize == 0)
         Refresh();

      return(m_szProductVersion);
   }

   CString& CModule::GetComments()
   { 
      if (m_FileSize == 0)
         Refresh();

      return(m_szComments);
   }

   CString& CModule::GetTrademarks()
   { 
      if (m_FileSize == 0)
         Refresh();

      return(m_szLegalTrademarks);
   }
   
   CString& CModule::GetPrivateBuild()
   { 
      if (m_FileSize == 0)
         Refresh();

      return(m_szPrivateBuild);
   }

   CString& CModule::GetSpecialBuild()
   { 
      if (m_FileSize == 0)
         Refresh();

      return(m_szSpecialBuild);
   }
//
//--------------------------------------------------------------------


//--------------------------------------------------------------------
// CModule debug helpers
//
#ifdef _DEBUG
void CModule::AssertValid() const
{
   CObject::AssertValid();

   ASSERT((m_szModuleName != _T("")) && (m_szPathName != _T("")) && (m_hModule != 0));
}

void CModule::Dump(CDumpContext& dc) const
{
   CObject::Dump(dc);

   dc << _T("\nm_szModulename = ")        << (LPCTSTR)m_szModuleName;
   dc << _T("\nm_szPathname = ")          << (LPCTSTR)m_szPathName;
   dc << _T("\nm_hModule    = ")          << m_hModule;

   dc << _T("\nm_BaseAddress = ")         << m_BaseAddress;
   dc << _T("\nm_szFullPathName = ")      << (LPCTSTR)m_szFullPathName;
   dc << _T("\nm_szFileTime = ")          << (LPCTSTR)m_szFileTime;
   dc << _T("\nm_FileSize = ")            << m_FileSize;
   dc << _T("\nm_szSubSystem = ")         << (LPCTSTR)m_szSubSystem;
   dc << _T("\nm_szLinkTime = ")          << (LPCTSTR)m_szLinkTime;
   dc << _T("\nm_LinkVersion = ")         << m_LinkVersion;
   dc << _T("\nm_szCompanyName = ")       << (LPCTSTR)m_szCompanyName;
   dc << _T("\nm_szFileDescription = ")   << (LPCTSTR)m_szFileDescription;
   dc << _T("\nm_szFileVersion = ")       << (LPCTSTR)m_szFileVersion;
   dc << _T("\nm_szInternalName = ")      << (LPCTSTR)m_szInternalName;
   dc << _T("\nm_szLegalCopyright = ")    << (LPCTSTR)m_szLegalCopyright;
   dc << _T("\nm_szOriginalFilename = ")  << (LPCTSTR)m_szOriginalFilename;
   dc << _T("\nm_szProductName = ")       << (LPCTSTR)m_szProductName;
   dc << _T("\nm_szProductVersion = ")    << (LPCTSTR)m_szProductVersion;
   dc << _T("\nm_szComments = ")          << (LPCTSTR)m_szComments;
   dc << _T("\nm_szLegalTrademarks = ")   << (LPCTSTR)m_szLegalTrademarks;
   dc << _T("\nm_szPrivateBuild = ")      << (LPCTSTR)m_szPrivateBuild;
   dc << _T("\nm_szSpecialBuild = ")      << (LPCTSTR)m_szSpecialBuild;

   dc << _T("\n");
}
#endif //_DEBUG
//
//--------------------------------------------------------------------





//--------------------------------------------------------------------
// Construction/Destruction
//


// static member definition
static CPSAPIWrapper    g_PSAPI;
static CTOOLHELPWrapper g_TOOLHELP32;


CModuleList::CModuleList()
{
}

CModuleList::~CModuleList()
{
// don't forget to delete resources
   FreeModuleArray();
}


// free each object stored into the array and empty the array itself
void CModuleList::FreeModuleArray()
{
   if (m_ModuleArray.GetSize() > 0)
   {
   // free each element
      for (int iModule = 0; iModule <= m_ModuleArray.GetUpperBound(); iModule++)
         delete (CModule*) m_ModuleArray[iModule];

   // delete the array itself
      m_ModuleArray.RemoveAll();
   }
}


// from Matt Pietrek -- Under the Hood -- September 1998
//  
BOOL CModuleList::RefreshTOOLHELP32(DWORD PID, LPCTSTR szFilename)
{
// check if the TOOLHELP32 API is available
   if (!g_TOOLHELP32.IsValid())
      return(FALSE);

   EnablePrivilege(SE_DEBUG_NAME);

// Enumerate the module list for this process.  Start by taking
// another ToolHelp32 snapshot, this time of the process's module list
   HANDLE hSnapshotModule;
   hSnapshotModule = g_TOOLHELP32.CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, PID);
   if (hSnapshotModule == (HANDLE)-1)
   {
      TCHAR szInfo[2*_MAX_PATH];
      GetLastErrorMessage(szInfo);
      TRACE("CreateToolhelp32Snapshot() failed:\n   %s\n", szInfo);

      return(FALSE);
   }

// get the static links
   MODULE_DEPENDENCY_LIST* pStaticLinks = new MODULE_DEPENDENCY_LIST(szFilename);
   if (pStaticLinks != NULL)
      if (!pStaticLinks->IsValid())
      {
         TRACE(
            "Impossible to get static links: %s %s\n", 
            szFilename, 
            pStaticLinks->GetErrorString()
            );
         delete pStaticLinks;
         pStaticLinks = NULL;
      }

// Iterate through each module in the snapshot
   MODULEENTRY32 modEntry = { sizeof(MODULEENTRY32) };
   BOOL fModWalkContinue;
   DWORD CurrentModule = 0;
   for (
         fModWalkContinue = g_TOOLHELP32.Module32First(hSnapshotModule, &modEntry);
         fModWalkContinue;
         fModWalkContinue = g_TOOLHELP32.Module32Next(hSnapshotModule, &modEntry) 
       )
   {
   // check if it is a dynamic module
      BOOL bDynamic = FALSE;
      if (pStaticLinks != NULL)
         bDynamic = (pStaticLinks->LookupModule(modEntry.szModule, FALSE) == NULL);

   // create a CModule object to store the module info
      CModule* pModule = new CModule(modEntry.hModule, modEntry.szExePath, bDynamic);
   
   // insert it into the internal module array
      if (pModule != NULL)
      {
         m_ModuleArray.InsertAt(CurrentModule, pModule);
         ++CurrentModule;
      }
   }
        
   ::CloseHandle(hSnapshotModule); // Done with module list snapshot
    
// don't forget to release static links list
   delete pStaticLinks;

   return(TRUE);
}
//--------------------------------------------------------------------


//--------------------------------------------------------------------
// public interface


#define MAX_MODULES  1024

// fill the list with the given process
void CModuleList::Refresh(DWORD PID, HANDLE hProcess, LPCTSTR szFilename)
{
// try first with TOOLHELP32 
//   if (RefreshTOOLHELP32(PID, szFilename))
//      return;

// otherwise, use PSAPI
   HMODULE  hMods[MAX_MODULES];
   DWORD    cbNeeded;

   if (g_PSAPI.EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded))
   {
      if (cbNeeded > 0)
      {
      // free the module array current content 
         FreeModuleArray();

      // set its new size
         m_ModuleArray.SetSize((cbNeeded / sizeof(HMODULE)) + 1);

      // get the static links
         MODULE_DEPENDENCY_LIST* pStaticLinks = new MODULE_DEPENDENCY_LIST(szFilename);
         if (pStaticLinks != NULL)
            if (!pStaticLinks->IsValid())
            {
               TRACE(
                  "Impossible to get static links: %s %s\n", 
                  szFilename, 
                  pStaticLinks->GetErrorString()
                  );
               delete pStaticLinks;
               pStaticLinks = NULL;
            }

         TCHAR szModName[MAX_PATH];
         for (DWORD CurrentModule = 0; CurrentModule < (cbNeeded / sizeof(HMODULE)); CurrentModule++)
         {
         // Get the full path to the module's file
            if (g_PSAPI.GetModuleFileNameEx(hProcess, hMods[CurrentModule], szModName, sizeof(szModName)))
            {
            // translate strange filenames if needed
               TCHAR szFullPathName[_MAX_PATH];
               TranslateFilename(szModName, szFullPathName);

            // check if it is a dynamic module
               BOOL bDynamic = FALSE;
               if (pStaticLinks != NULL)
                  bDynamic = (pStaticLinks->LookupModule(szFullPathName, TRUE) == NULL);

            // create a CModule object to store the module info
               CModule* pModule = new CModule(hMods[CurrentModule], szFullPathName, bDynamic);
            
            // insert it into the internal module array
               if (pModule != NULL)
                  m_ModuleArray.InsertAt(CurrentModule, pModule);
            }
         }

      // don't forget to release static links list
         delete pStaticLinks;
      }
   }   
}

// return number of modules in the list
int CModuleList::GetCount()
{
   return(m_ModuleArray.GetSize());
}


// get the first module to start an enumeration
CModule* CModuleList::GetFirst(int& pos)
{
   pos = 0;
   return(GetNext(pos));
}


// get the next module of an enumeration
CModule* CModuleList::GetNext(int& pos)
{
   CModule* pModule = NULL;
   if (pos <= m_ModuleArray.GetUpperBound())
      pModule = (CModule*) m_ModuleArray[pos++];

   return(pModule);
}
//
//--------------------------------------------------------------------
