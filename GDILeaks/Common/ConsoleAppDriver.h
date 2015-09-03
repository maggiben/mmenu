#if !defined(AFX_CONSOLEAPPDRIVER_H__74BE6FEB_3341_4107_A9FF_389988E98B86__INCLUDED_)
#define AFX_CONSOLEAPPDRIVER_H__74BE6FEB_3341_4107_A9FF_389988E98B86__INCLUDED_

#if _MSC_VER > 1000
   #pragma once
#endif // _MSC_VER > 1000



// string helpers
// 
LPCTSTR SkipBlanks(LPCTSTR pszString);
LPCTSTR SkipNonBlanks(LPCTSTR pszString);



class CConsoleAppDriver  
{
// construction/destruction
//
public:
   CConsoleAppDriver();
   virtual ~CConsoleAppDriver();
   void CleanupState();


// C++ interface
//
public:
   BOOL StartApp(LPCTSTR szCommandLine);


// overridable methods
//
protected:
   // called for each line generated by the console application
   //    TRUE  means "keep on parsing"
   //    FALSE means "stop parsing"
   // --> set m_bParsingOk to FALSE if an error occured
   virtual BOOL OnNewLine(LPCTSTR szLine);
   

// set it during parsing if a problem occurs
protected:
   BOOL m_bParsingOk;


// internal helpers
//
private:
   // call CreateProcess() with the right parameters
   BOOL PrepAndLaunchRedirectedChild(
      HANDLE hChildStdOut,
      HANDLE hChildStdIn,
      HANDLE hChildStdErr
      );

   // Monitors handle for input. Exits when child exits or pipe breaks.
   BOOL ReadAndHandleOutput(HANDLE hPipeRead);


// implementation details
// 
private:
   LPTSTR m_pszCommandLine;

   HANDLE m_hOutputReadTmp;
   HANDLE m_hOutputRead;
   HANDLE m_hOutputWrite;
   HANDLE m_hInputWriteTmp;
   HANDLE m_hInputRead;
   HANDLE m_hInputWrite;
   HANDLE m_hErrorWrite;

   LPTSTR m_pszCurrentLine;
   DWORD  m_FirstFreeChar;
};

#endif // !defined(AFX_CONSOLEAPPDRIVER_H__74BE6FEB_3341_4107_A9FF_389988E98B86__INCLUDED_)