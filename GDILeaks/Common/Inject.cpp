// from Jeffrey Richter  
//    "Programming Applications For Windows Fourth Edition"
//    Module:  InjLib.cpp


#include "stdafx.h"
#include "Inject.h"


#include <tchar.h>
#include <malloc.h>        // For alloca
#include "Wrappers.h"

#include "Helpers.h"


#ifdef UNICODE
   #define InjectLib InjectLibW
   #define EjectLib  EjectLibW
#else
   #define InjectLib InjectLibA
   #define EjectLib  EjectLibA
#endif   // !UNICODE


//static CTOOLHELPWrapper g_TOOLHELP32;
static CPSAPIWrapper g_PSAPI;

BOOL WINAPI InjectLibW(DWORD dwProcessId, PCWSTR pszLibFile) 
{
// check if PSAPI is available, otherwise, the injection 
// will work but it will be impossible to unload the DLL!
   if (!g_PSAPI.IsValid())
      return(FALSE);

   BOOL fOk = FALSE; // Assume that the function fails
   HANDLE hProcess = NULL, hThread = NULL;
   PWSTR pszLibFileRemote = NULL;

// Get a handle for the target process.
   hProcess = 
      GetProcessHandleWithEnoughRights(
         dwProcessId,
         PROCESS_QUERY_INFORMATION |   // Required by Alpha
         PROCESS_CREATE_THREAD     |   // For CreateRemoteThread
         PROCESS_VM_OPERATION      |   // For VirtualAllocEx/VirtualFreeEx
         PROCESS_VM_WRITE              // For WriteProcessMemory
         );
   if (hProcess == NULL)
      return(FALSE);

// Calculate the number of bytes needed for the DLL's pathname
   int cch = 1 + lstrlenW(pszLibFile);
   int cb  = cch * sizeof(WCHAR);

// Allocate space in the remote process for the pathname
   pszLibFileRemote = 
      (PWSTR) VirtualAllocEx(hProcess, NULL, cb, MEM_COMMIT, PAGE_READWRITE);
   
   if (pszLibFileRemote != NULL)
   {
   // Copy the DLL's pathname to the remote process's address space
      if (WriteProcessMemory(hProcess, pszLibFileRemote, 
         (PVOID) pszLibFile, cb, NULL))
      {
      // Get the real address of LoadLibraryW in Kernel32.dll
         PTHREAD_START_ROUTINE pfnThreadRtn = (PTHREAD_START_ROUTINE)
            GetProcAddress(GetModuleHandle(TEXT("Kernel32")), "LoadLibraryW");
         if (pfnThreadRtn != NULL)
         {
         // Create a remote thread that calls LoadLibraryW(DLLPathname)
            hThread = CreateRemoteThread(hProcess, NULL, 0, 
               pfnThreadRtn, pszLibFileRemote, 0, NULL);
            if (hThread != NULL)
            {
            // Wait for the remote thread to terminate
               WaitForSingleObject(hThread, INFINITE);

               fOk = TRUE; // Everything executed successfully

               CloseHandle(hThread);
            }
         }
      }
   // Free the remote memory that contained the DLL's pathname
      VirtualFreeEx(hProcess, pszLibFileRemote, 0, MEM_RELEASE);
   }
   
   CloseHandle(hProcess);

   return(fOk);
}


BOOL WINAPI InjectLibA(DWORD dwProcessId, PCSTR pszLibFile) {

   // Allocate a (stack) buffer for the Unicode version of the pathname
   PWSTR pszLibFileW = (PWSTR) 
      _alloca((lstrlenA(pszLibFile) + 1) * sizeof(WCHAR));

   // Convert the ANSI pathname to its Unicode equivalent
   wsprintfW(pszLibFileW, L"%S", pszLibFile);

   // Call the Unicode version of the function to actually do the work.
   return(InjectLibW(dwProcessId, pszLibFileW));
}


BOOL WINAPI EjectLibW(DWORD dwProcessId, PCWSTR pszLibFile) 
{
/*
// check if the TOOLHELP32 API is available
   if (!g_TOOLHELP32.IsValid())
      return(FALSE);

   BOOL fOk = FALSE; // Assume that the function fails
   HANDLE hthSnapshot = NULL;
   HANDLE hProcess = NULL, hThread = NULL;

// Grab a new snapshot of the process
   hthSnapshot = g_TOOLHELP32.CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwProcessId);
   if (hthSnapshot == NULL)
      return(FALSE);

// Get the HMODULE of the desired library
   MODULEENTRY32W me = { sizeof(me) };
   BOOL fFound = FALSE;
   BOOL fMoreMods = g_TOOLHELP32.Module32FirstW(hthSnapshot, &me);
   for (; fMoreMods; fMoreMods = g_TOOLHELP32.Module32NextW(hthSnapshot, &me)) 
   {
      fFound = (lstrcmpiW(me.szModule,  pszLibFile) == 0) || 
               (lstrcmpiW(me.szExePath, pszLibFile) == 0);
      if (fFound) break;
   }

   if (fFound)
   {
   // Get a handle for the target process.
      hProcess = GetProcessHandleWithEnoughRights(
         dwProcessId,
         PROCESS_QUERY_INFORMATION |   // Required by Alpha
         PROCESS_CREATE_THREAD     | 
         PROCESS_VM_OPERATION          // For CreateRemoteThread
         );
      if (hProcess != NULL)
      {
      // Get the real address of LoadLibraryW in Kernel32.dll
         PTHREAD_START_ROUTINE pfnThreadRtn = (PTHREAD_START_ROUTINE)
            GetProcAddress(GetModuleHandle(TEXT("Kernel32")), "FreeLibrary");
         if (pfnThreadRtn != NULL)
         {
         // Create a remote thread that calls LoadLibraryW(DLLPathname)
            hThread = CreateRemoteThread(hProcess, NULL, 0, 
               pfnThreadRtn, me.modBaseAddr, 0, NULL);
            if (hThread != NULL)
            {
            // Wait for the remote thread to terminate
               WaitForSingleObject(hThread, INFINITE);

               fOk = TRUE; // Everything executed successfully

               CloseHandle(hThread);
            }
         }

         CloseHandle(hProcess);
      }
   }

   CloseHandle(hthSnapshot);
*/
// check if PSAPI is available
   if (!g_PSAPI.IsValid())
      return(FALSE);

   BOOL     fOk      = FALSE; // Assume that the function fails
   HANDLE   hProcess = NULL;
   HANDLE   hThread  = NULL;
   BOOL     fFound   = FALSE;

// Get a handle for the target process.
   hProcess = GetProcessHandleWithEnoughRights(
      dwProcessId,
      PROCESS_VM_READ            |  // for EnumProcessModules
      PROCESS_QUERY_INFORMATION  |  // Required by Alpha
      PROCESS_CREATE_THREAD      | 
      PROCESS_VM_OPERATION          // For CreateRemoteThread
      );
   if (hProcess != NULL)
   {
      HMODULE  hMods[0x800];
      DWORD    cbNeeded;
      DWORD    CurrentModule = 0;
      if (g_PSAPI.EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded))
      {
         if (cbNeeded > 0)
         {
            TCHAR szModName[MAX_PATH];
            for (CurrentModule = 0; CurrentModule < (cbNeeded / sizeof(HMODULE)); CurrentModule++)
            {
            // Get the full path to the module's file
               if (g_PSAPI.GetModuleFileNameEx(hProcess, hMods[CurrentModule], szModName, sizeof(szModName)))
               {
#ifdef _UNICODE
                  fFound = (wcscmp(szModName, pszLibFile) == 0);
#else
                  wchar_t wszFilename[MAX_PATH];
                  ::MultiByteToWideChar(CP_ACP, 0, szModName, -1, wszFilename, MAX_PATH);
                  fFound = (wcscmp(wszFilename, pszLibFile) == 0);
#endif
                  if (fFound)
                     break;
               }
            }
         }
      }   

      if (fFound)
      {
      // Get the real address of LoadLibraryW in Kernel32.dll
         PTHREAD_START_ROUTINE pfnThreadRtn = (PTHREAD_START_ROUTINE)
            GetProcAddress(GetModuleHandle(TEXT("Kernel32")), "FreeLibrary");
         if (pfnThreadRtn != NULL)
         {
         // Create a remote thread that calls LoadLibraryW(DLLPathname)
            hThread = CreateRemoteThread(hProcess, NULL, 0, 
               pfnThreadRtn, hMods[CurrentModule], 0, NULL);
            if (hThread != NULL)
            {
            // Wait for the remote thread to terminate
               WaitForSingleObject(hThread, INFINITE);

               fOk = TRUE; // Everything executed successfully

               CloseHandle(hThread);
            }
         }
      }

      CloseHandle(hProcess);
   }

   return(fOk);
}


BOOL WINAPI EjectLibA(DWORD dwProcessId, PCSTR pszLibFile) {

   // Allocate a (stack) buffer for the Unicode version of the pathname
   PWSTR pszLibFileW = (PWSTR) 
      _alloca((lstrlenA(pszLibFile) + 1) * sizeof(WCHAR));

   // Convert the ANSI pathname to its Unicode equivalent
   wsprintfW(pszLibFileW, L"%S", pszLibFile);

   // Call the Unicode version of the function to actually do the work.
   return(EjectLibW(dwProcessId, pszLibFileW));
}
