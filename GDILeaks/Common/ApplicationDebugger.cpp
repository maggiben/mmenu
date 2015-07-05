#include "stdafx.h"
#include "ApplicationDebugger.h"

#ifdef _DEBUG
   #undef THIS_FILE
   static char THIS_FILE[]=__FILE__;
   #define new DEBUG_NEW
#endif


// definition of static members
//
long CApplicationDebugger::s_iQuit = 0;



CApplicationDebugger::CApplicationDebugger(LPCTSTR szCmdLine, DWORD dwLatency)
{
// sanity checks
   ASSERT(dwLatency != INFINITE);
   ASSERT((szCmdLine != NULL) && (_tcslen(szCmdLine) > 0));

   m_szCmdLine = szCmdLine;
   m_dwLatency = dwLatency;

// set members default value
   m_bFirstBreakpoint = TRUE;
}

CApplicationDebugger::~CApplicationDebugger()
{

}


// the next four functions are "stolen" from 
//    LOADPROF32 - Matt Pietrek 1995
//
// MakePtr is a macro that allows you to easily add to values (including
// pointers) together without dealing with C's pointer arithmetic.  It
// essentially treats the last two parameters as DWORDs.  The first
// parameter is used to typecast the result to the appropriate pointer type.
#define MakePtr( cast, ptr, addValue ) (cast)( (DWORD)(ptr) + (DWORD)(addValue))


// Fills in an IMAGE_NT_HEADERS structure for the specified HMODULE
//
BOOL CApplicationDebugger::GetModuleHeader(LPCVOID hModule, PIMAGE_NT_HEADERS pNTHdr)
{
   IMAGE_DOS_HEADER dosHdr;

   if (!ReadProcessMemoryEx(hModule, &dosHdr, sizeof(dosHdr)))
      return(FALSE);

   if (!ReadProcessMemoryEx(MakePtr( PVOID, hModule, dosHdr.e_lfanew ), pNTHdr, sizeof(*pNTHdr)))
      return(FALSE);

    return TRUE;
}


// Gets the name of the specified DLL in the debuggee process
// 
BOOL CApplicationDebugger::GetModuleNameInProcess(HMODULE hModule, LPTSTR szModName, DWORD cbModName )
{
   IMAGE_NT_HEADERS        ntHdr;
   IMAGE_EXPORT_DIRECTORY  exportDir;
   DWORD                   exportsRVA;

   _tcscpy(szModName, _T(""));  // In case something goes wrong

   if (!GetModuleHeader(hModule, &ntHdr))
      return(FALSE);

   exportsRVA = ntHdr.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;

   if (!exportsRVA)
      return(FALSE);

   if (!ReadProcessMemoryEx(MakePtr(PVOID, hModule, exportsRVA), &exportDir, sizeof(exportDir)))
      return(FALSE);

   if (!ReadProcessMemoryEx(MakePtr(PVOID, hModule, exportDir.Name), szModName, cbModName))
      return(FALSE);

   return(TRUE);
}


// get the prefered load address of the given hModule in the debuggee address space
// 
PVOID CApplicationDebugger::GetPreferredLoadAddress(LPCVOID hModule)
{
   IMAGE_NT_HEADERS ntHdr;

   if (!GetModuleHeader(hModule, &ntHdr))
      return FALSE;

   return(((LPVOID)ntHdr.OptionalHeader.ImageBase));
}


// read memory in the debuggee address space
// 
BOOL CApplicationDebugger::ReadProcessMemoryEx(LPCVOID lpBaseAddress, LPVOID lpBuffer, DWORD cbRead)
{
// sanity checks
   ASSERT(m_ProcessInformation.hProcess != NULL);
   if (m_ProcessInformation.hProcess == NULL)
      return(FALSE);

   DWORD nBytesRead;
   if (!ReadProcessMemory(m_ProcessInformation.hProcess, lpBaseAddress, lpBuffer, cbRead, &nBytesRead))
      return(FALSE);
   
   return(nBytesRead == cbRead);
}


// try to load PSAPI.DLL and call dynamically GetModuleFileNameEx()
// 
// DWORD GetModuleFileNameEx(
//   HANDLE    hProcess,   // handle to process
//   HMODULE   hModule,    // handle to module
//   LPTSTR    lpFilename, // path buffer
//   DWORD     nSize       // maximum characters to retrieve
//   );
//
typedef DWORD (WINAPI *PFNGETMODULEFILENAMEEXW)(HANDLE, HMODULE, LPWSTR, DWORD);
//
// since we are under Windows 2000, let's work in UNICODE

BOOL CApplicationDebugger::GetFullModuleName(HMODULE hModule, CString& szFullPathname)
{
// nothing if it fails
   szFullPathname = _T("");

   HMODULE hPSAPI = ::LoadLibrary(_T("PSAPI.DLL"));
   if (hPSAPI != NULL)
   {
      PFNGETMODULEFILENAMEEXW pfnGetModuleFileNameExW = 
         (PFNGETMODULEFILENAMEEXW)::GetProcAddress(hPSAPI, _T("GetModuleFileNameExW"));
      if (pfnGetModuleFileNameExW != NULL)
      {
         wchar_t  wszFullPathname[_MAX_PATH];
         DWORD    dwLength;
         dwLength = 
            pfnGetModuleFileNameExW(
               GetDebuggeeHandle(),
               hModule,
               wszFullPathname, 
               _MAX_PATH
               );
         if (dwLength > 0)
            szFullPathname = wszFullPathname;
      }

   // don't forget to release PSAPI.DLL
      ::FreeLibrary(hPSAPI);
   }

   return(!szFullPathname.IsEmpty());
}


BOOL CApplicationDebugger::GetExceptionDescription(DWORD ExceptionCode, CString& szDescription)
{
// Not a breakpoint exception.  Display information about it.
   CString  szInfo;
   BOOL     bFound = TRUE;
   switch (ExceptionCode)
   { 
      case EXCEPTION_ACCESS_VIOLATION: 
      // First chance: Pass this on to the kernel. 
      // Last chance: Display an appropriate error.  
         szInfo = _T("Access Violation");
      break;

      case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
      // The thread tried to access an array element that is out of bounds and 
      // the underlying hardware supports bounds checking.
         szInfo = _T("Array bounds exceeded");
      break;

      case EXCEPTION_BREAKPOINT: 
      // First chance: Display the current 
      // instruction and register values.  
         szInfo = _T("Breakpoint");
      break;

      case EXCEPTION_DATATYPE_MISALIGNMENT: 
      // First chance: Pass this on to the kernel. 
      // Last chance: Display an appropriate error.  
         szInfo = _T("Datatype misalignment");
      break;

      case EXCEPTION_FLT_DENORMAL_OPERAND:
      // One of the operands in a floating-point operation is denormal. 
      // A denormal value is one that is too small to represent as a standard floating-point value. 
         szInfo = _T("Floating-Point enormal operand");
      break;

      case EXCEPTION_FLT_DIVIDE_BY_ZERO:
      // The thread tried to divide a floating-point value by a floating-point divisor of zero. 
         szInfo = _T("Floating-Point divide by zero");
      break;

      case EXCEPTION_FLT_INEXACT_RESULT:
      // The result of a floating-point operation cannot be represented exactly as a decimal fraction. 
         szInfo = _T("Floating-Point inexact result");
      break;

      case EXCEPTION_FLT_INVALID_OPERATION:
      // This exception represents any floating-point exception not included in this list. 
         szInfo = _T("Floating-Point invalid operation");
      break;

      case EXCEPTION_FLT_OVERFLOW:
      // The exponent of a floating-point operation is greater than the magnitude allowed 
      // by the corresponding type. 
         szInfo = _T("Floating-Point overflow");
      break;

      case EXCEPTION_FLT_STACK_CHECK:
      // The stack overflowed or underflowed as the result of a floating-point operation 
         szInfo = _T("Floating-Point stack check");
      break;

      case EXCEPTION_FLT_UNDERFLOW:
      // The exponent of a floating-point operation is less than the magnitude allowed 
      // by the corresponding type. 
         szInfo = _T("Floating-Point underflow");
      break;

      case EXCEPTION_ILLEGAL_INSTRUCTION:
      // The thread tried to execute an invalid instruction. 
         szInfo = _T("Illegal instruction");
      break;

      case EXCEPTION_IN_PAGE_ERROR:
      // The thread tried to access a page that was not present, and the system 
      // was unable to load the page. For example, this exception might occur 
      // if a network connection is lost while running a program over the network. 
         szInfo = _T("Integer page error");
      break;

      case EXCEPTION_INT_DIVIDE_BY_ZERO:
      // The thread tried to divide an integer value by an integer divisor of zero. 
         szInfo = _T("Integer divide by zero");
      break;

      case EXCEPTION_INT_OVERFLOW:
      // The result of an integer operation caused a carry out of the most significant 
      // bit of the result. 
         szInfo = _T("Integer overflow");
      break;

      case EXCEPTION_INVALID_DISPOSITION:
      // An exception handler returned an invalid disposition to the exception dispatcher. 
      // Programmers using a high-level language such as C should never encounter this exception. 
         szInfo = _T("Invalid disposition");
      break;

      case EXCEPTION_NONCONTINUABLE_EXCEPTION:
      // The thread tried to continue execution after a noncontinuable exception occurred. 
         szInfo = _T("non continuable exception");
      break;

      case EXCEPTION_PRIV_INSTRUCTION:
      // The thread tried to execute an instruction whose operation is not allowed 
      // in the current machine mode. 
         szInfo = _T("Priviledge instruction");
      break;

      case EXCEPTION_SINGLE_STEP: 
      // First chance: Update the display of the 
      // current instruction and register values.  
         szInfo = _T("Single step");
      break;

      case EXCEPTION_STACK_OVERFLOW:
      // The thread used up its stack:
         szInfo = _T("Stack overflow");
      break;

      case DBG_CONTROL_C: 
      // First chance: Pass this on to the kernel. 
      // Last chance: Display an appropriate error.  
      // Handle other exceptions.
         szInfo = _T("Ctrl+C");
      break;


   // some without any documented explanation
      case EXCEPTION_GUARD_PAGE:
         szInfo = _T("Guard Page Hit");
      break;

      case EXCEPTION_INVALID_HANDLE:
         szInfo = _T("Invalid Handle");
      break;

      case CONTROL_C_EXIT:
         szInfo = _T("Control C Exit");
      break;

      case STATUS_FLOAT_MULTIPLE_FAULTS:
         szInfo = _T("Float - Multiple faults");
      break;

      case STATUS_FLOAT_MULTIPLE_TRAPS:
         szInfo = _T("Float - Multiple traps");
      break;

/*
case STATUS_ILLEGAL_VLM_REFERENCE:
         szInfo = _T("Illegal VLM Reference");
      break;
*/


   // from VC++ exceptions listbox
      case 0xc0000135:
         szInfo = _T("DLL Not Found");
      break;

      case 0xc0000142:
         szInfo = _T("DLL Initialization Failed");
      break;

      case 0xc06d007e:
         szInfo = _T("Module Not Found");
      break;

      case 0xc06d007f:
         szInfo = _T("Procedure Not Found");
      break;

      case 0xe06d7363:
         szInfo = _T("Microsoft C++ Exception");
      break;


// Get the exceptions from the MSDEV listbox + WINNT.H + WINBASE.H
// --> see also FormatMessage(NTDLL.DLL) in CrashDump...

/*
//
WINNT.H
-------------------------------------------------------------------------
#define STATUS_WAIT_0                    ((DWORD   )0x00000000L)
#define STATUS_ABANDONED_WAIT_0          ((DWORD   )0x00000080L)
#define STATUS_USER_APC                  ((DWORD   )0x000000C0L)
#define STATUS_TIMEOUT                   ((DWORD   )0x00000102L)
#define STATUS_PENDING                   ((DWORD   )0x00000103L)
#define STATUS_SEGMENT_NOTIFICATION      ((DWORD   )0x40000005L)
#define STATUS_GUARD_PAGE_VIOLATION      ((DWORD   )0x80000001L)
#define STATUS_DATATYPE_MISALIGNMENT     ((DWORD   )0x80000002L)
#define STATUS_BREAKPOINT                ((DWORD   )0x80000003L)
#define STATUS_SINGLE_STEP               ((DWORD   )0x80000004L)
#define STATUS_ACCESS_VIOLATION          ((DWORD   )0xC0000005L)
#define STATUS_IN_PAGE_ERROR             ((DWORD   )0xC0000006L)
#define STATUS_INVALID_HANDLE            ((DWORD   )0xC0000008L)
#define STATUS_NO_MEMORY                 ((DWORD   )0xC0000017L)
#define STATUS_ILLEGAL_INSTRUCTION       ((DWORD   )0xC000001DL)
#define STATUS_NONCONTINUABLE_EXCEPTION  ((DWORD   )0xC0000025L)
#define STATUS_INVALID_DISPOSITION       ((DWORD   )0xC0000026L)
#define STATUS_ARRAY_BOUNDS_EXCEEDED     ((DWORD   )0xC000008CL)
#define STATUS_FLOAT_DENORMAL_OPERAND    ((DWORD   )0xC000008DL)
#define STATUS_FLOAT_DIVIDE_BY_ZERO      ((DWORD   )0xC000008EL)
#define STATUS_FLOAT_INEXACT_RESULT      ((DWORD   )0xC000008FL)
#define STATUS_FLOAT_INVALID_OPERATION   ((DWORD   )0xC0000090L)
#define STATUS_FLOAT_OVERFLOW            ((DWORD   )0xC0000091L)
#define STATUS_FLOAT_STACK_CHECK         ((DWORD   )0xC0000092L)
#define STATUS_FLOAT_UNDERFLOW           ((DWORD   )0xC0000093L)
#define STATUS_INTEGER_DIVIDE_BY_ZERO    ((DWORD   )0xC0000094L)
#define STATUS_INTEGER_OVERFLOW          ((DWORD   )0xC0000095L)
#define STATUS_PRIVILEGED_INSTRUCTION    ((DWORD   )0xC0000096L)
#define STATUS_STACK_OVERFLOW            ((DWORD   )0xC00000FDL)
#define STATUS_CONTROL_C_EXIT            ((DWORD   )0xC000013AL)
#define STATUS_FLOAT_MULTIPLE_FAULTS     ((DWORD   )0xC00002B4L)
#define STATUS_FLOAT_MULTIPLE_TRAPS      ((DWORD   )0xC00002B5L)
#define STATUS_ILLEGAL_VLM_REFERENCE     ((DWORD   )0xC00002C0L)


#define DBG_CONTINUE                    ((DWORD   )0x00010002L)
#define DBG_TERMINATE_THREAD            ((DWORD   )0x40010003L)
#define DBG_TERMINATE_PROCESS           ((DWORD   )0x40010004L)
#define DBG_CONTROL_C                   ((DWORD   )0x40010005L)
#define DBG_CONTROL_BREAK               ((DWORD   )0x40010008L)
#define DBG_EXCEPTION_NOT_HANDLED       ((DWORD   )0x80010001L)


WINBASE.H
-------------------------------------------------------------------------
#define EXCEPTION_ACCESS_VIOLATION          STATUS_ACCESS_VIOLATION
#define EXCEPTION_DATATYPE_MISALIGNMENT     STATUS_DATATYPE_MISALIGNMENT
#define EXCEPTION_BREAKPOINT                STATUS_BREAKPOINT
#define EXCEPTION_SINGLE_STEP               STATUS_SINGLE_STEP
#define EXCEPTION_ARRAY_BOUNDS_EXCEEDED     STATUS_ARRAY_BOUNDS_EXCEEDED
#define EXCEPTION_FLT_DENORMAL_OPERAND      STATUS_FLOAT_DENORMAL_OPERAND
#define EXCEPTION_FLT_DIVIDE_BY_ZERO        STATUS_FLOAT_DIVIDE_BY_ZERO
#define EXCEPTION_FLT_INEXACT_RESULT        STATUS_FLOAT_INEXACT_RESULT
#define EXCEPTION_FLT_INVALID_OPERATION     STATUS_FLOAT_INVALID_OPERATION
#define EXCEPTION_FLT_OVERFLOW              STATUS_FLOAT_OVERFLOW
#define EXCEPTION_FLT_STACK_CHECK           STATUS_FLOAT_STACK_CHECK
#define EXCEPTION_FLT_UNDERFLOW             STATUS_FLOAT_UNDERFLOW
#define EXCEPTION_INT_DIVIDE_BY_ZERO        STATUS_INTEGER_DIVIDE_BY_ZERO
#define EXCEPTION_INT_OVERFLOW              STATUS_INTEGER_OVERFLOW
#define EXCEPTION_PRIV_INSTRUCTION          STATUS_PRIVILEGED_INSTRUCTION
#define EXCEPTION_IN_PAGE_ERROR             STATUS_IN_PAGE_ERROR
#define EXCEPTION_ILLEGAL_INSTRUCTION       STATUS_ILLEGAL_INSTRUCTION
#define EXCEPTION_NONCONTINUABLE_EXCEPTION  STATUS_NONCONTINUABLE_EXCEPTION
#define EXCEPTION_STACK_OVERFLOW            STATUS_STACK_OVERFLOW
#define EXCEPTION_INVALID_DISPOSITION       STATUS_INVALID_DISPOSITION
#define EXCEPTION_GUARD_PAGE                STATUS_GUARD_PAGE_VIOLATION
#define EXCEPTION_INVALID_HANDLE            STATUS_INVALID_HANDLE
#define CONTROL_C_EXIT                      STATUS_CONTROL_C_EXIT


   40010005 Control-C                  DBG_CONTROL_C
   40010008 Control-Break              DBG_CONTROL_BREAK
   80000002 Datatype Misalignment      STATUS_DATATYPE_MISALIGNMENT
   c0000005 Access Violation           STATUS_ACCESS_VIOLATION
   c0000006 In Page Error              STATUS_IN_PAGE_ERROR
   c000001d Illegal Instruction        STATUS_ILLEGAL_INSTRUCTION
   c000008c Array Bounds Exceeded      STATUS_ARRAY_BOUNDS_EXCEEDED
   c000008d Float Denormal Operand     STATUS_FLOAT_DENORMAL_OPERAND
   c000008e Float Divide by Zero       STATUS_FLOAT_DIVIDE_BY_ZERO
   c000008f Float Inexact Result       STATUS_FLOAT_INEXACT_RESULT
   c0000090 Float Invalid Operation    STATUS_FLOAT_INVALID_OPERATION
   c0000091 Float Overflow             STATUS_FLOAT_OVERFLOW
   c0000092 Float Stack Check          STATUS_FLOAT_STACK_CHECK
   c0000093 Float Underflow            STATUS_FLOAT_UNDERFLOW
   c0000017 No Memory                  STATUS_NO_MEMORY
   c0000025 Noncontinuable Exception   STATUS_NONCONTINUABLE_EXCEPTION
   c0000026 Invalid Disposition        STATUS_INVALID_DISPOSITION
   c0000094 Integer Divide by Zero     STATUS_INTEGER_DIVIDE_BY_ZERO
   c0000095 Integer Overflow           STATUS_INTEGER_OVERFLOW
   c0000096 Privileged Instruction     STATUS_PRIVILEGED_INSTRUCTION
   c00000fd Stack Overflow             STATUS_STACK_OVERFLOW
   c0000135 DLL Not Found              ?
   c0000142 DLL Initialization Failed  ?
   c06d007e Module Not Found           ?
   c06d007f Procedure Not Found        ?
   c0000008 Invalid Handle             STATUS_INVALID_HANDLE
   e06d7363 Microsoft C++ Exception    ?
*/
      default:
         szInfo = _T("unknown... See WINNT.H/WINBASE.H for exception codes (STATUS_xxx/DBG_xxx/EXCEPTION_xxx)");
         bFound = FALSE;
      break;
   } 

   szDescription.Format(
      _T("Exception: %08X -> %s"),
      ExceptionCode,
      (LPCTSTR)szInfo
      );
   
   return(bFound);
}



void CApplicationDebugger::OnThreadMessage(LPMSG pMsg)
{
// nothing special...
}


void CApplicationDebugger::OnWindowMessage(LPMSG pMsg)
{
// nothing special...
}



DWORD CApplicationDebugger::PreLoadingProcess()
{
// nothing special
   return(DP_OK);
}


void CApplicationDebugger::OnDebugEvent(DEBUG_EVENT* pEvent)
{
// nothing special...
}


void CApplicationDebugger::OnIdle()
{
// nothing special...
}


DWORD CApplicationDebugger::PostMortem()
{
// nothing special...
   return(DP_OK);
}



BOOL CApplicationDebugger::LoadTheProcess()
{
   STARTUPINFO startupInfo;
   LPTSTR      pszCmdLine = m_szCmdLine.GetBuffer(m_szCmdLine.GetLength()+1);
   
   memset(&startupInfo, 0, sizeof(startupInfo));
   startupInfo.cb = sizeof(startupInfo);

   BOOL bReturn = 
      ::CreateProcess(
         NULL,                      // lpszImageName
         pszCmdLine,                // lpszCommandLine
         NULL, NULL,                // lpsaProcess and lpsaThread
         FALSE,                     // fInheritHandles
         DEBUG_ONLY_THIS_PROCESS,   // fdwCreate
         // use DEBUG_PROCESS to get notifications for processes launched by the debuggee
         NULL, NULL,                // lpvEnvironment and lpszCurDir
         &startupInfo,              // lpsiStartupInfo
         &m_ProcessInformation      // lppiProcInfo
         );

   m_szCmdLine.ReleaseBuffer(-1);

   return(bReturn);
}



void CApplicationDebugger::AskToQuit()
{
   ::InterlockedIncrement(&s_iQuit);
}


BOOL CApplicationDebugger::NeedToQuit()
{
   return(s_iQuit > 0);
}


HANDLE CApplicationDebugger::GetDebuggeeHandle()
{
   return(m_ProcessInformation.hProcess);
}


DWORD CApplicationDebugger::DebugProcess()
{
   DEBUG_EVENT event;
   DWORD       continueStatus;
   DWORD       dwReturn = 0;

// allow initialization
   dwReturn = PreLoadingProcess();
   if (dwReturn != DP_OK)
      return(dwReturn);

// wait for the first breakpoint to come
   m_bFirstBreakpoint = TRUE;


// start the process
   if (!LoadTheProcess())
   {
      TRACE("Unable to start program\n");
      return(DP_CREATEPROCESS_FAILED);
   }

// The main debug event loop. Loop until we see an EXIT_PROCESS_DEBUG_EVENT
   while (TRUE)
   {
      if (::WaitForDebugEvent(&event, m_dwLatency))
      {
      // first, allow a treatment just after a debug event
         OnDebugEvent(&event);
         
      // next, do what is needed for the debug event
         continueStatus = HandleDebugEvent(&event);

         if (event.dwDebugEventCode == EXIT_PROCESS_DEBUG_EVENT)
            break;
         else 
            ::ContinueDebugEvent(event.dwProcessId, event.dwThreadId, continueStatus);
      }
   // no debug event
      else
      {
      // first, allow a treatment just after a debug event
         OnDebugEvent(NULL);
         
      // next, empty our message queue
         MSG msg;
         while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
         {
            if (msg.hwnd == 0)
               OnThreadMessage(&msg);
            else
               OnWindowMessage(&msg);
         }

      // do idle processing
         OnIdle();

      // check if we need to stop the thread
         if (NeedToQuit())
            break;
      }
   }

// don't forget to close the handles returned by ::CreateProcess()
   ::CloseHandle(m_ProcessInformation.hProcess);
   ::CloseHandle(m_ProcessInformation.hThread);


// Post mortem 
   dwReturn = PostMortem();

   return(dwReturn);
}


DWORD CApplicationDebugger::OnCreateProcessDebugEvent(CREATE_PROCESS_DEBUG_INFO* pInfo)
{
   return(DBG_CONTINUE);
}

DWORD CApplicationDebugger::OnExitProcessDebugEvent(DWORD dwExitCode)
{
   return(DBG_CONTINUE);
}

void CApplicationDebugger::OnProcessRunning()
{
}

DWORD CApplicationDebugger::OnExceptionDebugEvent(EXCEPTION_RECORD* pInfo, DWORD dwFirstChance)
{
   return(DBG_EXCEPTION_NOT_HANDLED);
}

DWORD CApplicationDebugger::OnOutputDebugStringDebugEvent(OUTPUT_DEBUG_STRING_INFO* pInfo)
{
   return(DBG_CONTINUE);
}

DWORD CApplicationDebugger::OnCreateThreadDebugEvent(CREATE_THREAD_DEBUG_INFO* pInfo)
{
   return(DBG_CONTINUE);
}

DWORD CApplicationDebugger::OnExitThreadDebugEvent(DWORD dwExitCode)
{
   return(DBG_CONTINUE);
}

DWORD CApplicationDebugger::OnLoadDLLDebugEvent(LOAD_DLL_DEBUG_INFO* pInfo)
{
   return(DBG_CONTINUE);
}

DWORD CApplicationDebugger::OnUnloadDLLDebugEvent(LPVOID lpBaseOfDll)
{
   return(DBG_CONTINUE);
}


DWORD CApplicationDebugger::HandleDebugEvent(DEBUG_EVENT* event)
{
// store event context
   m_ProcessID = event->dwProcessId;
   m_ThreadID = event->dwThreadId;
   
// dispatch according to event code
   switch (event->dwDebugEventCode)
   {
      case OUTPUT_DEBUG_STRING_EVENT:
         return(OnOutputDebugStringDebugEvent(&(event->u.DebugString)));


      case LOAD_DLL_DEBUG_EVENT:
      {
         DWORD dwReturn = OnLoadDLLDebugEvent(&(event->u.LoadDll));
      
      // don't forget to close DLL file handle
         if (event->u.LoadDll.hFile)
            CloseHandle(event->u.LoadDll.hFile);
      }
      break;
      

      case UNLOAD_DLL_DEBUG_EVENT:
      {
         DWORD dwReturn = OnUnloadDLLDebugEvent(event->u.UnloadDll.lpBaseOfDll);
         return(dwReturn);
      }
      break;


      case CREATE_THREAD_DEBUG_EVENT:
      {
         DWORD dwReturn = OnCreateThreadDebugEvent(&(event->u.CreateThread));
      
      // don't forget to close the thread handle
         if (event->u.CreateThread.hThread)
            CloseHandle(event->u.CreateThread.hThread);
      
         return(dwReturn);
      }
      break;

      case EXIT_THREAD_DEBUG_EVENT:
      {
         DWORD dwReturn = OnExitThreadDebugEvent(event->u.ExitThread.dwExitCode);
         return(dwReturn);
      }
      break;


      case CREATE_PROCESS_DEBUG_EVENT:
      {
         DWORD dwReturn = OnCreateProcessDebugEvent(&(event->u.CreateProcessInfo));

      // don't forget to close the file handle
         if (event->u.CreateProcessInfo.hFile)
            CloseHandle(event->u.CreateProcessInfo.hFile);

      // don't forget to close the process handle
         if (event->u.CreateProcessInfo.hProcess)
            CloseHandle(event->u.CreateProcessInfo.hProcess);
         
      // don't forget to close the thread handle
         if (event->u.CreateProcessInfo.hThread)
            CloseHandle(event->u.CreateProcessInfo.hThread);
            
         return(dwReturn);
      }


      case EXIT_PROCESS_DEBUG_EVENT:
      {
         DWORD dwReturn = OnExitProcessDebugEvent(event->u.ExitProcess.dwExitCode);
         return(dwReturn);
      }
      break;


      case EXCEPTION_DEBUG_EVENT:
      {
         if (STATUS_BREAKPOINT == event->u.Exception.ExceptionRecord.ExceptionCode)
         {
            if (m_bFirstBreakpoint)
            {
            // First breakpoint comes automatically
            // from a loader call to DebugBreak
               m_bFirstBreakpoint = FALSE;

            // overridable notification
               OnProcessRunning();
            }

         // handle ASSERT() sepcial case
            return(DBG_CONTINUE);
         }

         return(OnExceptionDebugEvent(&(event->u.Exception.ExceptionRecord), event->u.Exception.dwFirstChance));
      }

      default:
      // should never occur
         TRACE("Unknown debug event %u\n", event->dwDebugEventCode);
         ASSERT(FALSE);

      break;
    }

    return(DBG_CONTINUE);
}


