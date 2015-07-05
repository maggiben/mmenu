// From MSDN
//    HOWTO: Spawn Console Processes with Redirected Standard Handles
//    ID: Q190351 
//    
//    Dave McPherson (davemm)   11-March-98
//    Redirect.c
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "ConsoleAppDriver.h"

#ifdef _DEBUG
   #undef THIS_FILE
   static char THIS_FILE[]=__FILE__;
   #define new DEBUG_NEW
#endif


// debug helpers
// 
static void TraceLastError(LPCTSTR szPrefix)
{
   LPVOID lpvMessageBuffer;
   ::FormatMessage(
      FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
      NULL, 
      GetLastError(),
      MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
      (LPTSTR)&lpvMessageBuffer, 
      0, 
      NULL
      );
   OutputDebugString(szPrefix);
   OutputDebugString((LPCTSTR)lpvMessageBuffer);
   OutputDebugString(_T("\n"));

// don't forget to free returned error description
   ::LocalFree(lpvMessageBuffer);
}


// string helpers
// 
LPCTSTR SkipBlanks(LPCTSTR pszString)
{
// sanity checks
   ASSERT(pszString != NULL);
   if (pszString == NULL)
      return(NULL);

   try
   {
      while ((*pszString != _T('\0')) && (*pszString == _T(' ')))
         pszString++;
   }
   catch(...)
   {
      return(NULL);
   }

   return(pszString);
}

LPCTSTR SkipNonBlanks(LPCTSTR pszString)
{
// sanity checks
   ASSERT(pszString != NULL);
   if (pszString == NULL)
      return(NULL);

   try
   {
      while ((*pszString != _T('\0')) && (*pszString != _T(' ')))
         pszString++;
   }
   catch(...)
   {
      return(NULL);
   }

   return(pszString);
}


// construction/destruction
// 

const DWORD MAX_LINE_LENGTH = 1024;

CConsoleAppDriver::CConsoleAppDriver()
{
   m_pszCommandLine = NULL;

   m_pszCurrentLine  = new TCHAR[MAX_LINE_LENGTH];
   m_FirstFreeChar   = 0;

   m_hOutputReadTmp  = NULL;
   m_hOutputRead     = NULL;
   m_hOutputWrite    = NULL;
   m_hInputWriteTmp  = NULL;
   m_hInputRead      = NULL;
   m_hInputWrite     = NULL;
   m_hErrorWrite     = NULL;

   m_bParsingOk = FALSE;
}

CConsoleAppDriver::~CConsoleAppDriver()
{
   CleanupState();

   if (m_pszCurrentLine != NULL)
      delete m_pszCurrentLine;
}



#define CloseHandleAndNullIt(Handle)   \
   if (Handle)                         \
   {                                   \
      VERIFY(CloseHandle(Handle));     \
      Handle = NULL;                   \
   }                                   \


void CConsoleAppDriver::CleanupState()
{
   if (m_pszCommandLine != NULL)
   {
      free(m_pszCommandLine);
      m_pszCommandLine = NULL;
   }

   CloseHandleAndNullIt(m_hOutputReadTmp);
   CloseHandleAndNullIt(m_hOutputRead);
   CloseHandleAndNullIt(m_hOutputWrite);
   CloseHandleAndNullIt(m_hInputWriteTmp);
   CloseHandleAndNullIt(m_hInputRead);
   CloseHandleAndNullIt(m_hInputWrite);
   CloseHandleAndNullIt(m_hErrorWrite);

   m_bParsingOk = FALSE;
}


// C++ interface
//
BOOL CConsoleAppDriver::StartApp(LPCTSTR szCommandLine)
{
// cleanup previous execution state
   CleanupState();


// sanity checks
   ASSERT(szCommandLine != NULL);
   if (szCommandLine == NULL)
      return(FALSE);

   m_pszCommandLine = _tcsdup(szCommandLine);


// Set up the security attributes struct.
   SECURITY_ATTRIBUTES sa;
   sa.nLength              = sizeof(SECURITY_ATTRIBUTES);
   sa.lpSecurityDescriptor = NULL;
   sa.bInheritHandle       = TRUE;  // make it inheritable

 
// Create the child output pipe.
   BOOL bReturn;
   bReturn = CreatePipe(&m_hOutputReadTmp, &m_hOutputWrite, &sa, 0);
   if (!bReturn)
   {
      CleanupState();
      TraceLastError(_T("<Output CreatePipe failed> "));
      return(FALSE);
   }

// Create a duplicate of the output write handle for the std error
// write handle. This is necessary in case the child application
// closes one of its std output handles.
   bReturn = 
      DuplicateHandle(
         GetCurrentProcess(),
         m_hOutputWrite,
         GetCurrentProcess(),
         &m_hErrorWrite,
         0,
         TRUE, // make it inheritable
         DUPLICATE_SAME_ACCESS
         );
   if (!bReturn)
   {
   // don't forget to free already created pipes ends
      CleanupState();

      TraceLastError(_T("<Error pipe duplication failed> "));
      return(FALSE);
   }


// Create the child input pipe.
   bReturn = CreatePipe(&m_hInputRead, &m_hInputWriteTmp, &sa, 0);
   if (!bReturn)
   {
   // don't forget to free already created kernel objects
      CleanupState();

      TraceLastError(_T("<Input CreatePipe failed> "));
      return(FALSE);
   }


// Create new output read handle and the input write handles. Set
// the Properties to FALSE. Otherwise, the child inherits the
// properties and, as a result, non-closeable handles to the pipes
// are created.
   bReturn = 
      DuplicateHandle(
         GetCurrentProcess(),
         m_hOutputReadTmp,
         GetCurrentProcess(),
         &m_hOutputRead, 
         0,
         FALSE, // Make it uninheritable.
         DUPLICATE_SAME_ACCESS);

   if (!bReturn)
   {
   // don't forget to free already created kernel objects
      CleanupState();

      TraceLastError(_T("<Output read pipe duplication failed> "));
      return(FALSE);
   }


   bReturn = 
      DuplicateHandle(
         GetCurrentProcess(),
         m_hInputWriteTmp,
         GetCurrentProcess(),
         &m_hInputWrite, 
         0,
         FALSE, // Make it uninheritable.
         DUPLICATE_SAME_ACCESS
         );
   if (!bReturn)
   {
   // don't forget to free already created kernel objects
      CleanupState();

      TraceLastError(_T("<Input write pipe duplication failed> "));
      return(FALSE);
   }


// Close inheritable copies of the handles you do not want to be
// inherited.
   CloseHandleAndNullIt(m_hOutputReadTmp);
   CloseHandleAndNullIt(m_hInputWriteTmp);


// call CreateProcess() with the right parameters
   PrepAndLaunchRedirectedChild(m_hOutputWrite, m_hInputRead, m_hErrorWrite);


// Close pipe handles (do not continue to modify the parent).
// You need to make sure that no handles to the write end of the
// output pipe are maintained in this process or else the pipe will
// not close when the child process exits and the ReadFile will hang.
   CloseHandleAndNullIt(m_hOutputWrite);
   CloseHandleAndNullIt(m_hInputRead);
   CloseHandleAndNullIt(m_hErrorWrite);


// Read the child's output.
   if (!ReadAndHandleOutput(m_hOutputRead))
   {
      CleanupState();

      TraceLastError(_T("<Problem during child process output reading> "));
      return(FALSE);
   }

// handle cleanup
   CloseHandleAndNullIt(m_hOutputRead);
   CloseHandleAndNullIt(m_hInputWrite);

   return(TRUE);
}



// overridable methods
//
BOOL CConsoleAppDriver::OnNewLine(LPCTSTR szLine)
{
   OutputDebugString(szLine);
   Sleep(20);

   return(TRUE);
}


// internal helpers
// 
BOOL CConsoleAppDriver::PrepAndLaunchRedirectedChild(
   HANDLE hChildStdOut,
   HANDLE hChildStdIn,
   HANDLE hChildStdErr
   )
{
// Set up the start up info struct.
   STARTUPINFO si;
   ZeroMemory(&si, sizeof(STARTUPINFO));
   si.cb          = sizeof(STARTUPINFO);
   si.dwFlags     = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
   si.hStdOutput  = hChildStdOut;
   si.hStdInput   = hChildStdIn;
   si.hStdError   = hChildStdErr;
   si.wShowWindow = SW_HIDE;  // we don't want to see the console

// call CreateProcess
   PROCESS_INFORMATION pi;
   BOOL bReturn = 
      CreateProcess(
         NULL,
         m_pszCommandLine,
         NULL,
         NULL,
         TRUE,
         CREATE_NEW_CONSOLE,
         NULL,
         NULL,
         &si,
         &pi
         );
   if (!bReturn)
   {
      TraceLastError(_T("<CreateProcess failed> "));

      return(FALSE);
   }


// don't forget to close unused handles
   CloseHandle(pi.hProcess);
   CloseHandle(pi.hThread);

   return(TRUE);
}


BOOL CConsoleAppDriver::ReadAndHandleOutput(HANDLE hPipeRead)
{
   TCHAR szBuffer[MAX_LINE_LENGTH];
   DWORD nBytesRead;
   BOOL  bSuccess = TRUE;

// reset current line state
   m_bParsingOk = TRUE;
   m_FirstFreeChar = 0;

   while (bSuccess)
   {
   // read from the pipe: get the child process output
      bSuccess = 
         ReadFile(hPipeRead, szBuffer, sizeof(szBuffer)-1, &nBytesRead, NULL) &&
         (nBytesRead != 0);

   // check for possible error
      if (!bSuccess)
      {
         if (GetLastError() == ERROR_BROKEN_PIPE)
         {
            bSuccess = TRUE;
            break; // pipe done - normal exit path.
         }
         else
         {
         // something bad happened.
            return(FALSE);
         }
      }

   // make it a '0' ending string
      szBuffer[nBytesRead] = 0;

   // split it into lines 
      LPTSTR pPos    = szBuffer;
      LPTSTR pNext   = pPos;
      LPTSTR pBefore = NULL;
      bSuccess       = TRUE;
      
      try 
      {
         while (bSuccess && (pPos != NULL))
         {
         // look for '\n'
            pNext = _tcschr(pPos, _T('\n'));
            if (pNext != NULL)
            {
            // check if it is the first char
            // --> Current line is finished
               if (pNext == pPos)
               {
                  bSuccess = OnNewLine(m_pszCurrentLine);
                  m_FirstFreeChar = 0;
                  pNext++; // skip \n
               }
               else
               {
               // check for \r\n
                  pBefore = pNext - 1;
                  if (*pBefore == _T('\r'))
                  {
                  // check for \r\n alone
                     if (pPos != pBefore)
                     {
                        CopyMemory(
                           &m_pszCurrentLine[m_FirstFreeChar],
                           pPos, 
                           pBefore-pPos
                           );
                        m_FirstFreeChar += (pBefore-pPos)/sizeof(TCHAR);
                     }

                     m_pszCurrentLine[m_FirstFreeChar] = _T('\0');
                     bSuccess = OnNewLine(m_pszCurrentLine);
                     m_FirstFreeChar = 0;
                     pNext++; // skip \r\n
                  }
                  else
                  {
                  // just /n
                     if (pPos != pBefore)
                     {
                        CopyMemory(
                           &m_pszCurrentLine[m_FirstFreeChar],
                           pPos, 
                           pNext-pPos
                           );
                        m_FirstFreeChar += (pNext-pPos)/sizeof(TCHAR);
                     }

                     m_pszCurrentLine[m_FirstFreeChar] = _T('\0');
                     bSuccess = OnNewLine(m_pszCurrentLine);
                     m_FirstFreeChar = 0;
                     pNext++; // skip \n
                  }
               }
            }
            else
            {
            // simply append buffer to the current line
               CopyMemory(
                  &m_pszCurrentLine[m_FirstFreeChar], 
                  pPos, 
                  _tcslen(pPos)
                  );

               m_FirstFreeChar += _tcslen(pPos);
            }

         // next line, please
            pPos = pNext;
         };
      }
      catch(...)
      {
      // oops...
      // some kind of buffer over(read/write) or memory allocation error
         return(FALSE);
      }
   }

   return(m_bParsingOk);
}
