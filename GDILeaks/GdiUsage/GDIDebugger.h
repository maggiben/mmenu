#if !defined(AFX_GDIDEBUGGER_H__C02E4865_81DE_11D2_B108_000000000000__INCLUDED_)
#define AFX_GDIDEBUGGER_H__C02E4865_81DE_11D2_B108_000000000000__INCLUDED_

#if _MSC_VER > 1000
   #pragma once
#endif // _MSC_VER > 1000


#include <AfxTempl.h>
#include "..\Common\ApplicationDebugger.h"
#include "..\Common\GdiResources.h"
class CLibraryInfo;



// Class responsible for GDI spying & debuggee event reflection
//
class CGDIDebugger : public CApplicationDebugger  
{
// constructor/destructor
//
public:
   CGDIDebugger(LPCTSTR szCmdLine, HWND hDialog, HWND hStringListBox);
   virtual ~CGDIDebugger();

// C++ interface
// 
public:
   void AddDebugString(LPCTSTR format, ...);
   void ShowList(CGdiResources* pList, HWND hWnd);
   void GetGDIList(DWORD Notification);


// overridables methods
// 
protected:
   // framework overridables
   virtual DWORD PreLoadingProcess();

   // debug events handlers
   virtual DWORD OnCreateProcessDebugEvent(CREATE_PROCESS_DEBUG_INFO* pInfo);
   virtual DWORD OnExitProcessDebugEvent(DWORD dwExitCode);

   virtual DWORD OnLoadDLLDebugEvent(LOAD_DLL_DEBUG_INFO* pInfo);
   virtual DWORD OnUnloadDLLDebugEvent(LPVOID lpBaseOfDll);

   virtual DWORD OnOutputDebugStringDebugEvent(OUTPUT_DEBUG_STRING_INFO* pInfo);

   virtual void OnThreadMessage(LPMSG pMsg);


// implementation details
// 
private:
   // interface with the rest of the world
   HWND m_hDialog;
   HWND m_hStringListBox;

   // internal behaviour process/libraries/threads...
   CString  m_szReason;

   // members used to manage debug events
   BOOL m_bTrace;
   CTypedPtrMap<CMapPtrToPtr, DWORD, CLibraryInfo*> m_Libraries;
   int m_TotalLibraries;

   BOOL m_bGDISpyIsStarted;


// internal helpers
// 
protected:
   void DefaultReset();
   void AddLibrary(CLibraryInfo* pLibrary);
   void BuildLibraryName(CLibraryInfo* pInfo, CString& szName);
};

#endif // !defined(AFX_GDIDEBUGGER_H__C02E4865_81DE_11D2_B108_000000000000__INCLUDED_)
