// ApplicationDebugger.h: interface for the CApplicationDebugger class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_APPLICATIONDEBUGGER_H__C02E4864_81DE_11D2_B108_000000000000__INCLUDED_)
#define AFX_APPLICATIONDEBUGGER_H__C02E4864_81DE_11D2_B108_000000000000__INCLUDED_

#if _MSC_VER > 1000
   #pragma once
#endif // _MSC_VER > 1000



// return values for debugging overloaded methods
// 
const int DP_OK                     = 0;
const int DP_CREATEPROCESS_FAILED   = 1;



// debugger wrapper object
//
class CApplicationDebugger
{
// constructor/destructor
//
public:
   CApplicationDebugger(LPCTSTR szCmdLine, DWORD dwLatency = 100); // 1/10 second
   virtual ~CApplicationDebugger() = 0;   // make it pure virtual 
private:
   CApplicationDebugger();    // impossible to call it


// public interface
//
public:
   DWORD DebugProcess();
   BOOL ReadProcessMemoryEx(LPCVOID lpBaseAddress, LPVOID lpBuffer, DWORD cbRead);
   BOOL GetModuleHeader(LPCVOID hModule, PIMAGE_NT_HEADERS pNTHdr);
   BOOL GetModuleNameInProcess(HMODULE hModule, LPTSTR szModName, DWORD cbModName);
   PVOID GetPreferredLoadAddress(LPCVOID hModule);
   HANDLE GetDebuggeeHandle();
   virtual BOOL NeedToQuit();

   // say bye-bye
   static void AskToQuit();

   // general purpose helper
   BOOL GetExceptionDescription(DWORD ExceptionCode, CString& szDescription);
   BOOL GetFullModuleName(HMODULE hModule, CString& szFullPathname);

// debug events handlers
//
protected:
   virtual DWORD OnCreateProcessDebugEvent(CREATE_PROCESS_DEBUG_INFO* pInfo);
   virtual DWORD OnExitProcessDebugEvent(DWORD dwExitCode);

   virtual DWORD OnCreateThreadDebugEvent(CREATE_THREAD_DEBUG_INFO* pInfo);
   virtual DWORD OnExitThreadDebugEvent(DWORD dwExitCode);

   virtual DWORD OnLoadDLLDebugEvent(LOAD_DLL_DEBUG_INFO* pInfo);
   virtual DWORD OnUnloadDLLDebugEvent(LPVOID lpBaseOfDll);

   virtual DWORD OnExceptionDebugEvent(EXCEPTION_RECORD* pInfo, DWORD dwFirstChance);
   virtual DWORD OnOutputDebugStringDebugEvent(OUTPUT_DEBUG_STRING_INFO* pInfo);


// framework overridables
//
protected:
   virtual DWORD PreLoadingProcess();
   virtual DWORD PostMortem();
   virtual void OnProcessRunning();
   virtual void OnThreadMessage(LPMSG pMsg);
   virtual void OnWindowMessage(LPMSG pMsg);
   virtual void OnIdle();
   
   // this method is called either if a debug event if received
   // either the timeout between two debug event has elapsed (pEvent == NULL)
   virtual void OnDebugEvent(DEBUG_EVENT* pEvent);


// members local to a debug event
//
protected:
   DWORD m_ProcessID;
   DWORD m_ThreadID;


// internal details
//
protected:
// global members
   CString              m_szCmdLine;
   PROCESS_INFORMATION  m_ProcessInformation;
   DWORD                m_dwLatency;
   BOOL                 m_bFirstBreakpoint;

// static member
   static long s_iQuit;


// internal helpers
//
private:
   BOOL LoadTheProcess();
   DWORD HandleDebugEvent(DEBUG_EVENT* event);
};

#endif // !defined(AFX_APPLICATIONDEBUGGER_H__C02E4864_81DE_11D2_B108_000000000000__INCLUDED_)

