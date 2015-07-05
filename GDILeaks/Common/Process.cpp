#include "stdafx.h"
#include "Process.h"
#include "afxpriv.h"
#include "Wrappers.h"
#include "Helpers.h"
#include <aclapi.h>  // for SetSecurityInfo() and SE_KERNEL_OBJECT


#ifdef _DEBUG
   #undef THIS_FILE
   static char THIS_FILE[]=__FILE__;
   #define new DEBUG_NEW
#endif



//--------------------------------------------------------------------
// Main Window search
//
typedef struct tagENUMINFO
{
// In Parameters
   DWORD PId;

// Out Parameters
   HWND  hWnd;
   HWND  hEmptyWnd;
   HWND  hInvisibleWnd;
   HWND  hEmptyInvisibleWnd;
} ENUMINFO, *PENUMINFO;


BOOL CALLBACK EnumWindowsProc(HWND hWnd, DWORD lParam)
{
   DWORD       pid = 0;
   PENUMINFO   pInfo = (PENUMINFO)lParam;
   TCHAR       szTitle[_MAX_PATH+1];

// sanity checks
   if (pInfo == NULL)
   // stop the enumeration if invalid parameter is given
      return(FALSE);

// get the processid for this window
   if (!::GetWindowThreadProcessId(hWnd, &pid)) 
   // this should never occur :-)
      return(TRUE);

// compare the process ID with the one given as search parameter
   if (pInfo->PId == pid) 
   {
   // look for the visibility first
      if (::IsWindowVisible(hWnd)) 
      {
      // look for the title next
         if (::GetWindowText(hWnd, szTitle, _MAX_PATH) != 0)
         {
            pInfo->hWnd = hWnd;

         // we have found the right window
            return(FALSE);
         }
         else
            pInfo->hEmptyWnd = hWnd;
      }
      else
      {
      // look for the title next
         if (::GetWindowText(hWnd, szTitle, _MAX_PATH) != 0)
         {
            pInfo->hInvisibleWnd = hWnd;
         }
         else
            pInfo->hEmptyInvisibleWnd = hWnd;
      }
   }

// continue the enumeration
   return(TRUE);
}


HWND GetMainWindow(DWORD PId)
{
   ENUMINFO EnumInfo;

// set the search parameters 
   EnumInfo.PId = PId;

// set the return parameters to default values
   EnumInfo.hWnd               = NULL;
   EnumInfo.hEmptyWnd          = NULL;
   EnumInfo.hInvisibleWnd      = NULL;
   EnumInfo.hEmptyInvisibleWnd = NULL;

// do the search among the top level windows
   ::EnumWindows((WNDENUMPROC)EnumWindowsProc, (LPARAM)&EnumInfo);

// return the one found if any
   if (EnumInfo.hWnd != NULL)
      return(EnumInfo.hWnd);
   else if (EnumInfo.hEmptyWnd != NULL)
      return(EnumInfo.hEmptyWnd);
   else if (EnumInfo.hInvisibleWnd != NULL)
      return(EnumInfo.hInvisibleWnd);
   else
      return(EnumInfo.hEmptyInvisibleWnd);
}
//
//--------------------------------------------------------------------



// static member definition
// 
static CNTDLLWrapper g_NTDLL;
static CPSAPIWrapper g_PSAPI;
static CWTSWrapper   g_WTS;


//--------------------------------------------------------------------
// Construction/Destruction
//
IMPLEMENT_DYNAMIC(CProcess, CObject)


CProcess::CProcess()
{
   m_bDelayFillModuleList = TRUE;
   DefaultInit();
}


CProcess::CProcess(BOOL bDelayFillModuleList)
{
   m_bDelayFillModuleList = bDelayFillModuleList;
   DefaultInit();
}


CProcess::~CProcess()
{
}

// set members to default values
BOOL CProcess::DefaultInit()
{
   m_dwPId              = UNKNOWN_PROCESS_ID;
   m_szName             = _T("");
   m_szFilename         = _T("");
   m_szMainWndTitle     = _T("");
   m_hMainWnd           = NULL;
   m_ParentProcessID    = 0;
   m_KERNELHandleCount  = 0;
   m_USERHandleCount    = 0;
   m_GDIHandleCount     = 0;
   m_WorkingSet         = 0;
   m_SessionID          = (DWORD)-1;
   
   return(TRUE);  
}


typedef struct
{
   DWORD Filler[4];
   DWORD InfoBlockAddress;
} __PEB;

typedef struct 
{
   DWORD Filler[17];
   DWORD wszCmdLineAddress;
} __INFOBLOCK;


BOOL GetProcessCmdLine(HANDLE hProcess, LPTSTR szCmdLine, DWORD Size)
{
// sanity checks
   ASSERT(hProcess != NULL);
   ASSERT(szCmdLine != NULL);
   ASSERT(Size > 0);
   if ((hProcess == NULL) || (szCmdLine == NULL) || (Size == 0))
      return(FALSE);

// 0. get the Process Environment Block address
   int   iReturn = 1;
   DWORD dwSize;


   PROCESS_BASIC_INFORMATION  pbi;
   pbi.PebBaseAddress = (_PEB*)0x7ffdf000;
// the PEB is always at address 0x7ffdf000 but here is the "right"
// way to get it
//   iReturn = 
//      ::NtQueryInformationProcess(
//         hProcess, 
//         ProcessBasicInformation, 
//         &pbi, 
//         sizeof(pbi), 
//         &dwSize
//         );


// NtQueryInformationProcess returns a negative value if it fails
   if (iReturn >= 0) 
   {
   // 1. find the Process Environment Block 
      __PEB PEB;
      if (!::ReadProcessMemory(hProcess, pbi.PebBaseAddress, &PEB, sizeof(PEB), &dwSize))
      {
      // call GetLastError() to know why
         return(FALSE);
      }

   // 2. from this PEB, get the address of the block containing a pointer to the CmdLine
      __INFOBLOCK Block;
      if (!::ReadProcessMemory(hProcess, (LPVOID)PEB.InfoBlockAddress, &Block, sizeof(Block), &dwSize))
      {
      // call GetLastError() to know why
         return(FALSE);
      }

   // 3. get the CmdLine
      wchar_t wszCmdLine[_MAX_PATH+1];
      if (!::ReadProcessMemory(hProcess, (LPVOID)Block.wszCmdLineAddress, wszCmdLine, _MAX_PATH*sizeof(wchar_t), &dwSize))
      {
      // call GetLastError() to know why
         return(FALSE);
      }

   // 4. skip the application pathname
   //    it can be empty, "c:\...\app.exe" or c:\...\app.exe 
      wchar_t* pPos = wszCmdLine; 
      if (*pPos != L'\0')
      {
         if (*pPos == L'"')
         {
         // find the next " character
            pPos = wcschr(&pPos[1], L'"');
         }
         else
         {
         // find the next SPACE character
            pPos = wcschr(&pPos[1], L' ');
         }

      // skip it
         if (pPos != NULL)
            pPos++;
      }

   // copy it back
      if (pPos != NULL)
      {
         if (*pPos != L'\0')
         {
            USES_CONVERSION;
            _tcsncpy(szCmdLine, W2T(pPos), Size);
         }
         else
            szCmdLine[0] = _T('\0');
      }
      else
         szCmdLine[0] = _T('\0');
   }  
   else
   {
      TRACE("Impossible to get command line for process handle 0x%x...\n", hProcess);
      return(FALSE);
   }

   return(TRUE);
}


BOOL GetProcessCmdLine(DWORD PID, LPTSTR szCmdLine, DWORD Size)
{
// sanity checks
   if ((PID <= 0) || (szCmdLine == NULL))
      return(FALSE);

// check if we can get information for this process 
   HANDLE hProcess = ::OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, PID);
   if (hProcess == NULL)
      return(FALSE);

   BOOL bReturn = GetProcessCmdLine(hProcess, szCmdLine, Size);

// don't forget to release the process handle
   ::CloseHandle(hProcess);

   return(bReturn);
}


void GetProcessOwner(HANDLE hProcess, CString& szOwner)
{
// default value
   szOwner = _T("");

// get process token
   HANDLE hToken = NULL;
   EnablePrivilege(SE_TCB_NAME);
   if (!::OpenProcessToken(hProcess, TOKEN_QUERY, &hToken))
      return;

// Obtain the size of the user information in the token.
   DWORD cbti = 0;
   ::GetTokenInformation(hToken, TokenUser, NULL, 0, &cbti);

// Call should have failed due to zero-length buffer.
   if (::GetLastError() == ERROR_INSUFFICIENT_BUFFER)
   {
   // Allocate buffer for user information in the token.
      PTOKEN_USER ptiUser = (PTOKEN_USER) ::HeapAlloc(GetProcessHeap(), 0, cbti);
      if (ptiUser != NULL)
      {
      // Retrieve the user information from the token.
         if (::GetTokenInformation(hToken, TokenUser, ptiUser, cbti, &cbti))
         {
            SID_NAME_USE   snu;
            TCHAR          szUser[_MAX_PATH];
            DWORD          chUser = _MAX_PATH;
            PDWORD         pcchUser = &chUser; 
            TCHAR          szDomain[_MAX_PATH];
            DWORD          chDomain = _MAX_PATH;
            PDWORD         pcchDomain = &chDomain;

         // Retrieve user name and domain name based on user's SID.
            if (
                  ::LookupAccountSid(
                     NULL, 
                     ptiUser->User.Sid, 
                     szUser, 
                     pcchUser, 
                     szDomain, 
                     pcchDomain, 
                     &snu
                     )
               )
            {
            // build the owner string as \\DomainName\UserName
               szOwner = _T("\\\\");
               szOwner += szDomain;
               szOwner += _T("\\");
               szOwner += szUser;
            }
         }

      // free buffer
         ::HeapFree(::GetProcessHeap(), 0, ptiUser);
      }
   }

// don't forget to free process token
   ::CloseHandle(hToken);
}


BOOL CProcess::AttachProcess(DWORD PID)
{
// check for given parameter
   if (PID <= 0)
   {
   // it should be the "System Idle Process"
      m_szName = _T("System Idle Process");
   }

// check if we can get information for this process 
   HANDLE hProcess = 
      GetProcessHandleWithEnoughRights(
         PID, 
         PROCESS_QUERY_INFORMATION | PROCESS_VM_READ
         );
   if (hProcess != NULL)
   {
      TCHAR szBuffer[_MAX_PATH+1];
   // set the process ID
      m_dwPId = PID;

   // get the module name and filename 
      if (g_PSAPI.GetModuleBaseName(hProcess, NULL, szBuffer, _MAX_PATH) == 0)
      {
         if (PID == 8)
            m_szName = _T("System");
         else
            m_szName = _T("?");
      }
      else
      {
         m_szName = szBuffer;

      // remove the ".exe" from the module name
         m_szName = m_szName.Left(m_szName.Find(_T('.')));
      }
      
      if (g_PSAPI.GetModuleFileNameEx(hProcess, NULL, szBuffer, _MAX_PATH) == 0)
      {
         m_szFilename = _T("");
      }
      else
      {
      // translate strange filenames
         TranslateFilename(szBuffer, m_szFilename.GetBuffer(_MAX_PATH+1));
         m_szFilename.ReleaseBuffer(-1);
      }

   // get the command line
      GetProcessCmdLine(hProcess, m_szCmdLine.GetBuffer(_MAX_PATH+1), _MAX_PATH);
      m_szCmdLine.ReleaseBuffer(-1);

   // get the owner
      GetProcessOwner(hProcess, m_szOwner);

   // get the corresponding session ID
      if (g_WTS.IsValid())
      {
         if (!g_WTS.ProcessIdToSessionId(PID, &m_SessionID))
         {
            m_SessionID = (DWORD)-1;
         }
      }
      else
         m_SessionID = (DWORD)-1;

   // search for the main window
      m_hMainWnd = GetMainWindow(m_dwPId);
      if (::IsWindow(m_hMainWnd))
      {
         int iLength = ::GetWindowText(m_hMainWnd, m_szMainWndTitle.GetBuffer(_MAX_PATH), _MAX_PATH);
         m_szMainWndTitle.ReleaseBuffer(-1);
         if (iLength == 0)
            m_szMainWndTitle.Format(_T("....no title for 0x%x"), m_hMainWnd);
      }
      else
         m_szMainWndTitle = _T("");

   // get the memory info
      PROCESS_MEMORY_COUNTERS pmc;
      pmc.cb = sizeof(PROCESS_MEMORY_COUNTERS);
      if (g_PSAPI.GetProcessMemoryInfo(hProcess, &pmc, sizeof(PROCESS_MEMORY_COUNTERS)))
         m_WorkingSet = pmc.WorkingSetSize;
      else
      {
         TRACE("Impossible to get process(%u) working set...\n", PID);
      }

   // get its parent process ID
      PROCESS_BASIC_INFORMATION  pbi;
      BOOL                       bReturn;
      pbi.InheritedFromUniqueProcessId = UNKNOWN_PROCESS_ID;
      bReturn = 
         g_NTDLL.QueryInformationProcess(
            hProcess, 
            ProcessBasicInformation, 
            &pbi, 
            sizeof(pbi)
            );

      if (bReturn) 
         m_ParentProcessID = pbi.InheritedFromUniqueProcessId;
      else
      {
         m_ParentProcessID = 0;
         TRACE("Impossible to get process(%u) parent process ID...\n", PID);
      }

   // query for handle count
      DWORD dwHandleCount;
      bReturn = 
         g_NTDLL.QueryInformationProcess(
            hProcess, 
            ProcessHandleCount, 
            &dwHandleCount, 
            sizeof(dwHandleCount)
            );
      if (bReturn >= 0)
         m_KERNELHandleCount = dwHandleCount;
      else
      {
         m_KERNELHandleCount = 0;
         TRACE("Impossible to get process(%u) handles count...\n", PID);
      }

      m_USERHandleCount = GetUSERObjectsCount(hProcess);
      m_GDIHandleCount = GetGDIObjectsCount(hProcess);


   // if you want to get exact snapshot, you
   // should call m_ModuleList.Refresh(PID, hProcess);  
   // here. 
   // --> set m_bDelayFillModuleList to FALSE
   //
   // The drawback is the long time it takes...
   // Instead, call it when needed in GetModulelist()
      if (!m_bDelayFillModuleList)
         m_ModuleList.Refresh(PID, hProcess, m_szFilename);

   // don't forget to release the process handle
      ::CloseHandle(hProcess);

      return(TRUE);
   }
   else
      return(FALSE);
}
//
//--------------------------------------------------------------------


//--------------------------------------------------------------------
// CProcess debug helpers
//
#ifdef _DEBUG
void CProcess::AssertValid() const
{
   CObject::AssertValid();
}

void CProcess::Dump(CDumpContext& dc) const
{
   CObject::Dump(dc);

   dc << "\nm_szName             = " << (LPCTSTR)m_szName;
   dc << "\nm_dwPId              = " << m_dwPId;
   dc << "\nm_szFilename         = " << (LPCTSTR)m_szFilename;
   dc << "\nm_szCmdLine          = " << (LPCTSTR)m_szCmdLine;
   dc << "\nm_szOwner            = " << (LPCTSTR)m_szOwner;
   dc << "\nm_hMainWnd           = " << m_hMainWnd;
   dc << "\nm_szMainWndTitle     = " << (LPCTSTR)m_szMainWndTitle;
   dc << "\nm_ParentProcessID    = " << m_ParentProcessID;
   dc << "\nm_KERNELHandleCount  = " << m_KERNELHandleCount;
   dc << "\nm_USERHandleCount    = " << m_USERHandleCount;
   dc << "\nm_GDIHandleCount     = " << m_GDIHandleCount;
   dc << "\nm_WorkingSet         = " << m_WorkingSet;
   dc << "\nm_SessionID          = " << m_SessionID;
   dc << "\n";
}
#endif //_DEBUG
//
//--------------------------------------------------------------------


//--------------------------------------------------------------------
// child proces access methods
//
void CProcess::AddChild(CProcess* pProcess)
{
// impossible to add a NULL process
   ASSERT(pProcess != NULL);

// add it into the children array
   if (pProcess != NULL)
      m_ChildProcesses.Add(pProcess);
}

int CProcess::GetChildrenCount()
{
   return(m_ChildProcesses.GetSize());
}
//
//--------------------------------------------------------------------


//--------------------------------------------------------------------
// CProcess access methods
//
DWORD CProcess::GetPID()
{ 
   return(m_dwPId);
}

CString& CProcess::GetName()
{ 
   return(m_szName);
}

CString& CProcess::GetFilename()
{
   return(m_szFilename);
}

HWND CProcess::GetMainWindowHandle()
{ 
   return(m_hMainWnd); 
}

CString& CProcess::GetMainWindowTitle()
{ 
   return(m_szMainWndTitle); 
}

DWORD CProcess::GetParentProcessID()
{ 
   return(m_ParentProcessID);
}

DWORD CProcess::GetKERNELHandleCount()
{ 
   return(m_KERNELHandleCount);
}

DWORD CProcess::GetUSERHandleCount()
{ 
   return(m_USERHandleCount);
}

DWORD CProcess::GetGDIHandleCount()
{ 
   return(m_GDIHandleCount);
}

DWORD CProcess::GetWorkingSet()
{ 
   return(m_WorkingSet);
}

CModuleList& CProcess::GetModuleList()
{
   if (m_bDelayFillModuleList)
   {
   // if you want to get exact snapshot, you
   // should call m_ModuleList.Refresh(PID, hProcess);
   // in AttachProcess().
   // --> set m_bDelayFillModuleList to FALSE
   //
   // The drawback is the long time it takes...
      if (m_ModuleList.GetCount() == 0)
      {
      // check if we can get information for this process 
         HANDLE hProcess = ::OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, m_dwPId);

         if (hProcess != NULL)
         {
            m_ModuleList.Refresh(m_dwPId, hProcess, m_szFilename);

         // don't forget to release the process handle
            ::CloseHandle(hProcess);
         }
      }
   }
   return(m_ModuleList);
}

CString& CProcess::GetCmdLine()
{
   return(m_szCmdLine);
}

CString& CProcess::GetOwner()
{
   return(m_szOwner);
}

DWORD CProcess::GetSessionID()
{
   return(m_SessionID);
}
//
//--------------------------------------------------------------------





//--------------------------------------------------------------------
// Construction/Destruction
//
IMPLEMENT_DYNAMIC(CProcessList, CObject)

CProcessList::CProcessList()
{
}

CProcessList::~CProcessList()
{
// empty the map and free each of its element
   DefaultReset();
}

void CProcessList::DefaultReset()
{
// do nothing if the map is empty
   if (m_ProcessMap.IsEmpty())
      return;

// get each element of the map and delete it
   POSITION pos;
   DWORD    key;

// delete one process after the other
   CProcess*   pProcess;

   for (pos = m_ProcessMap.GetStartPosition(); pos != NULL; )
   {
      m_ProcessMap.GetNextAssoc(pos, (LPVOID&)key, pProcess);
      delete pProcess;
   }

// delete the map itself
   m_ProcessMap.RemoveAll();
}
//
//--------------------------------------------------------------------


//--------------------------------------------------------------------
// Debug helpers
//
#ifdef _DEBUG
void CProcessList::AssertValid() const
{
   CObject::AssertValid();
}

void CProcessList::Dump(CDumpContext& dc) const
{
   CObject::Dump(dc);

   dc << "\n# of processes= " << m_ProcessMap.GetCount();
   dc << "\n";
}
#endif //_DEBUG
//
//--------------------------------------------------------------------



//--------------------------------------------------------------------
// C++ Interface 
//

// return number of processes in the list
int CProcessList::GetCount()
{
   return(m_ProcessMap.GetCount());
}


// return the first process in the list
CProcess* CProcessList::GetFirst(POSITION& pos)
{
   pos = m_ProcessMap.GetStartPosition();
   return(GetNext(pos));
}


// next iteration step
CProcess* CProcessList::GetNext(POSITION& pos)
{
   if (pos != NULL)
   {
      DWORD PID = 0;
      CProcess* pProcess = NULL;
      m_ProcessMap.GetNextAssoc(pos, (LPVOID&)PID, pProcess);
      return(pProcess);
   }
   else
      return(NULL);
}


// return the process info associated with the given process ID
CProcess* CProcessList::GetProcess(DWORD PID)
{
   CProcess* pProcess = NULL;
   if (m_ProcessMap.Lookup((LPVOID)PID, pProcess))
      return(pProcess);
   else
      return(NULL);
}


void CProcessList::SetChildrenList()
{
   CProcess* pParent  = NULL;
   CProcess* pProcess = NULL;

   POSITION Pos = 0;
   DWORD    PID = 0;
   for (
         pProcess = GetFirst(Pos); 
         (pProcess != NULL); 
         pProcess = GetNext(Pos)
       )
   {
   // get a process pointer to its parent and 
   // add itself into this parent process child list
      if (pProcess != NULL)
      {
         PID = pProcess->GetParentProcessID();
         if (m_ProcessMap.Lookup((LPVOID)PID, pParent))
            pParent->AddChild(pProcess);
      }
   }
}


// retrieve and fill the processes list 
void CProcessList::Refresh()
{
// don't forget to reset and free the current list
   DefaultReset();

// store the current process list
   DWORD aProcesses[MAX_PROCESS];
   DWORD cbNeeded = 0;

// get a snapshot of the processes
   if (!g_PSAPI.EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
      return;
    
// Calculate how many process IDs were returned
   DWORD cProcesses = cbNeeded / sizeof(DWORD);

// attach a CProcess object to each process ID
   DWORD     dwProcessID;
   CProcess* pProcess;
   for (
         DWORD dwCurrentProcess = 0; 
         dwCurrentProcess < cProcesses; 
         dwCurrentProcess++
       )
   {
      dwProcessID = aProcesses[dwCurrentProcess];

   // add the process definition into the map
      pProcess = new CProcess(TRUE);
      if (pProcess != NULL)
      {
      // fill in the process information for the current process ID
         if (!pProcess->AttachProcess(aProcesses[dwCurrentProcess]))
            delete pProcess;
         else
         // store into the map
            m_ProcessMap[(LPVOID)dwProcessID] = pProcess;
      }
   }

// a second iteration is needed to know the process children 
   SetChildrenList();
}
//
//--------------------------------------------------------------------

