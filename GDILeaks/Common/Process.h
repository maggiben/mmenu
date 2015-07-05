#if !defined(AFX_PROCESS_H__855FFCAC_AB6C_11D1_BE91_000000000000__INCLUDED_)
#define AFX_PROCESS_H__855FFCAC_AB6C_11D1_BE91_000000000000__INCLUDED_

#if _MSC_VER >= 1000
   #pragma once
#endif // _MSC_VER >= 1000


#include <AfxTempl.h>
#include "Module.h"


const int UNKNOWN_PROCESS_ID = ((DWORD)-1);
const int IDLE_PROCESS_ID    = ((DWORD)-2);
const int MAX_PROCESS        = 1024;

BOOL GetProcessCmdLine(DWORD PID, LPTSTR szCmdLine, DWORD Size);


class CProcess : public CObject  
{
   DECLARE_DYNAMIC(CProcess)

// Construction/Destruction
//
public:
   CProcess(); // by default, the module list is built when requested
   CProcess(BOOL bDelayFillModuleList);
   virtual ~CProcess();


// C++ Interface
//
public:
   DWORD    GetPID();
   CString& GetName();
   CString& GetFilename();
   CString& GetMainWindowTitle();
   HWND     GetMainWindowHandle();
   DWORD    GetParentProcessID();
   DWORD    GetKERNELHandleCount();
   DWORD    GetUSERHandleCount();
   DWORD    GetGDIHandleCount();
   DWORD    GetWorkingSet();
   CString& GetCmdLine();
   CString& GetOwner();
   DWORD    GetSessionID();

// give access to modules loaded by process through a CModuleList object
   CModuleList& GetModuleList();

// provide methods to enumerate spawned processes 
   int GetChildrenCount();


// Internal helpers
//
private:
   BOOL AttachProcess(DWORD PID);
   BOOL DefaultInit();
   void AddChild(CProcess* pProcess);   // used by CProcessList


// Implementation details
//
private:
   BOOL m_bDelayFillModuleList;


// Process descripton
//
private:
// process ID
   DWORD m_dwPId;

// name
   CString  m_szName;

// filename
   CString  m_szFilename;

// Window title handle
   HWND  m_hMainWnd;

// Window title title
   CString  m_szMainWndTitle;

// parent process ID
   DWORD m_ParentProcessID;

// handles count
   DWORD m_KERNELHandleCount;
   DWORD m_USERHandleCount;
   DWORD m_GDIHandleCount;

// working set (used physical RAM)
   DWORD m_WorkingSet;

// module list
   CModuleList m_ModuleList;

// child process list
   CObArray m_ChildProcesses;

// Command line
   CString m_szCmdLine;

// owner 
   CString m_szOwner;

// session ID
   DWORD m_SessionID;


// debug helpers
//
public:
#ifdef _DEBUG
   virtual void AssertValid() const;
   virtual void Dump(CDumpContext& dc) const;
#endif


   friend class CProcessList;
};



class CProcessList : public CObject  
{
   DECLARE_DYNAMIC(CProcessList)

// Construction/Destruction
//
public:
   CProcessList();
   virtual ~CProcessList();


// C++ Interface
//
public:
   void Refresh();
   CProcess* GetProcess(DWORD PID);
   int GetCount();
   CProcess* GetFirst(POSITION& pos);
   CProcess* GetNext(POSITION& pos);


// Debug helpers
//
public:
#ifdef _DEBUG
   virtual void AssertValid() const;
   virtual void Dump(CDumpContext& dc) const;
#endif


// Internal helpers
//
private:
   void DefaultReset();
   void SetChildrenList();


// Implementation details
//
protected:
// store each process description (Ptr = 32 bits --> ID) 
// indexed by its process ID (Ptr = 32 bits --> CProcess*) 
   CTypedPtrMap<CMapPtrToPtr, LPVOID, CProcess*> m_ProcessMap;
};



#endif // !defined(AFX_PROCESS_H__855FFCAC_AB6C_11D1_BE91_000000000000__INCLUDED_)
