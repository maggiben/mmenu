#include "stdafx.h"
#include <string.h>
#include "hookapi.h"


// Macro for adding pointers/DWORDs together without C arithmetic interfering
#define MakePtr( cast, ptr, addValue ) (cast)( (DWORD)(ptr)+(DWORD)(addValue))




//------------------------------------------------------------------------
// From John Robbins's book "Debugging Windows"
// 

//////////////////////////////////////////////////////////////////////
// Indicates that the version information is valid.
static BOOL g_bHasVersion = FALSE ;
// Indicates NT or 95/98.
static BOOL g_bIsNT = TRUE ;

BOOL __stdcall IsNT ( void )
{
    if ( TRUE == g_bHasVersion )
    {
        return ( TRUE == g_bIsNT ) ;
    }

    OSVERSIONINFO stOSVI ;

    memset ( &stOSVI , NULL , sizeof ( OSVERSIONINFO ) ) ;
    stOSVI.dwOSVersionInfoSize = sizeof ( OSVERSIONINFO ) ;

    BOOL bRet = GetVersionEx ( &stOSVI ) ;
    ASSERT ( TRUE == bRet ) ;
    if ( FALSE == bRet )
    {
        TRACE0 ( "GetVersionEx failed!\n" ) ;
        return ( FALSE ) ;
    }

    // Check the version and call the appropriate thing.
    if ( VER_PLATFORM_WIN32_NT == stOSVI.dwPlatformId )
    {
        g_bIsNT = TRUE ;
    }
    else
    {
        g_bIsNT = FALSE ;
    }
    g_bHasVersion = TRUE ;
    return ( TRUE == g_bIsNT ) ;
}



typedef struct tag_HOOKFUNCDESCA
{
    // The name of the function to hook.
    LPCSTR szFunc   ;
    // The procedure to blast in.
    PROC   pProc    ;
} HOOKFUNCDESCA , * LPHOOKFUNCDESCA ;

typedef struct tag_HOOKFUNCDESCW
{
    // The name of the function to hook.
    LPCWSTR szFunc   ;
    // The procedure to blast in.
    PROC    pProc    ;
} HOOKFUNCDESCW , * LPHOOKFUNCDESCW ;

#ifdef UNICODE
#define HOOKFUNCDESC   HOOKFUNCDESCW
#define LPHOOKFUNCDESC LPHOOKFUNCDESCW
#else
#define HOOKFUNCDESC   HOOKFUNCDESCA
#define LPHOOKFUNCDESC LPHOOKFUNCDESCA
#endif  // UNICODE


/*----------------------------------------------------------------------
FUNCTION        :   GetNamedImportDescriptor
DISCUSSION      :
    Gets the import descriptor for the requested module.  If the module
is not imported in hModule, NULL is returned.
    This is a potential useful function in the future.
PARAMETERS      :
    hModule      - The module to hook in.
    szImportMod  - The module name to get the import descriptor for.
RETURNS         :
    NULL  - The module was not imported or hModule is invalid.
    !NULL - The import descriptor.
----------------------------------------------------------------------*/
PIMAGE_IMPORT_DESCRIPTOR
                     GetNamedImportDescriptor ( HMODULE hModule     ,
                                                LPCSTR  szImportMod  )
{
    // Always check parameters.
    ASSERT ( NULL != szImportMod ) ;
    ASSERT ( NULL != hModule     ) ;
    if ( ( NULL == szImportMod ) || ( NULL == hModule ) )
    {
        SetLastErrorEx ( ERROR_INVALID_PARAMETER , SLE_ERROR ) ;
        return ( NULL ) ;
    }

    PIMAGE_DOS_HEADER pDOSHeader = (PIMAGE_DOS_HEADER)hModule ;

    // Is this the MZ header?
    if ( ( TRUE == IsBadReadPtr ( pDOSHeader                  ,
                                 sizeof ( IMAGE_DOS_HEADER )  ) ) ||
         ( IMAGE_DOS_SIGNATURE != pDOSHeader->e_magic           )   )
    {
        ASSERT ( !"Unable to read MZ header!" ) ;
        SetLastErrorEx ( ERROR_INVALID_PARAMETER , SLE_ERROR ) ;
        return ( NULL ) ;
    }

    // Get the PE header.
    PIMAGE_NT_HEADERS pNTHeader = MakePtr ( PIMAGE_NT_HEADERS       ,
                                            pDOSHeader              ,
                                            pDOSHeader->e_lfanew     ) ;

    // Is this a real PE image?
    if ( ( TRUE == IsBadReadPtr ( pNTHeader ,
                                  sizeof ( IMAGE_NT_HEADERS ) ) ) ||
         ( IMAGE_NT_SIGNATURE != pNTHeader->Signature           )   )
    {
        ASSERT ( !"Not able to read PE header" ) ;
        SetLastErrorEx ( ERROR_INVALID_PARAMETER , SLE_ERROR ) ;
        return ( NULL ) ;
    }

    // If there is no imports section, leave now.
    if ( 0 == pNTHeader->OptionalHeader.
                         DataDirectory[ IMAGE_DIRECTORY_ENTRY_IMPORT ].
                                                      VirtualAddress   )
    {
        return ( NULL ) ;
    }

    // Get the pointer to the imports section.
    PIMAGE_IMPORT_DESCRIPTOR pImportDesc
     = MakePtr ( PIMAGE_IMPORT_DESCRIPTOR ,
                 pDOSHeader               ,
                 pNTHeader->OptionalHeader.
                         DataDirectory[ IMAGE_DIRECTORY_ENTRY_IMPORT ].
                                                      VirtualAddress ) ;

    // Loop through the import module descriptors looking for the
    // module whose name matches szImportMod.
    while ( NULL != pImportDesc->Name )
    {
        PSTR szCurrMod = MakePtr ( PSTR              ,
                                   pDOSHeader        ,
                                   pImportDesc->Name  ) ;
        if ( 0 == stricmp ( szCurrMod , szImportMod ) )
        {
            // Found it.
            break ;
        }
        // Look at the next one.
        pImportDesc++ ;
    }

    // If the name is NULL, then the module is not imported.
    if ( NULL == pImportDesc->Name )
    {
        return ( NULL ) ;
    }

    // All OK, Jumpmaster!
    return ( pImportDesc ) ;

}



/*----------------------------------------------------------------------
FUNCTION        :   HookImportedFunctionsByName
DISCUSSION      :
    Hooks the specified functions imported into hModule by the module
indicated by szImportMod.  This function can be used to hook from one
to 'n' of the functions imported.
    The techniques used in the function are slightly different than
that shown by Matt Pietrek in his book, "Windows 95 System Programming
Secrets."  He uses the address of the function to hook as returned by
GetProcAddress.  Unfortunately, while this works in almost all cases, it
does not work when the program being hooked is running under a debugger
on Windows95 (and presumably, Windows98).  The problem is that
GetProcAddress under a debugger returns a "debug thunk," not the address
that is stored in the Import Address Table (IAT).
    This function gets around that by using the real thunk list in the
PE file, the one not bashed by the loader when the module is loaded and
fixed up, to find where the named import is located.  Once the named
import is found, then the original table is blasted to make the hook.
As the name implies, this function will only hook functions imported by
name.
PARAMETERS      :
    hModule     - The module where the imports will be hooked.
    szImportMod - The name of the module whose functions will be
                  imported.
    uiCount     - The number of functions to hook.  This is the size of
                  the paHookArray and paOrigFuncs arrays.
    paHookArray - The array of function descriptors that list which
                  functions to hook.  At this point, the array does not
                  have to be in szFunc name order.  Also, if a
                  particular pProc is NULL, then that item will just be
                  skipped.  This makes it much easier for debugging.
    paOrigFuncs - The array of original addresses that were hooked.  If
                  a function was not hooked, then that item will be
                  NULL.  This parameter can be NULL if the returned
                  information is not needed.
    pdwHooked   - Returns the number of functions hooked out of
                  paHookArray.  This parameter can be NULL if the
                  returned information is not needed.
RETURNS         :
    FALSE - There was a problem, check GetLastError.
    TRUE  - The function succeeded.  See the parameter discussion for
            the output parameters.
----------------------------------------------------------------------*/

BOOL HookImportedFunctionsByName(
   HMODULE           hModule,
   LPCSTR            szImportMod,
   UINT              uiCount,
   LPHOOKFUNCDESCA   paHookArray,
   PROC *            paOrigFuncs,
   LPDWORD           pdwHooked
   )
{
// Assert the parameters.
   ASSERT(FALSE == IsBadReadPtr(hModule, sizeof(IMAGE_DOS_HEADER)));
   ASSERT(FALSE == IsBadStringPtr(szImportMod, MAX_PATH));
   ASSERT(0 != uiCount);
   ASSERT(NULL != paHookArray) ;
   ASSERT(FALSE == IsBadReadPtr(paHookArray, sizeof(HOOKFUNCDESC) * uiCount));

// In debug builds, perform deep validation of paHookArray.
#ifdef _DEBUG
    if ( NULL != paOrigFuncs )
    {
        ASSERT ( FALSE == IsBadWritePtr ( paOrigFuncs ,
                                          sizeof ( PROC ) * uiCount ) );

    }
    if ( NULL != pdwHooked )
    {
        ASSERT ( FALSE == IsBadWritePtr ( pdwHooked , sizeof ( UINT )));
    }

    // Check each item in the hook array.
    {
        for ( UINT i = 0 ; i < uiCount ; i++ )
        {
            ASSERT ( NULL != paHookArray[ i ].szFunc  ) ;
            ASSERT ( '\0' != *paHookArray[ i ].szFunc ) ;
            // If the function address isn't NULL, it is validated.
            if ( NULL != paHookArray[ i ].pProc )
            {
                ASSERT ( FALSE == IsBadCodePtr ( paHookArray[i].pProc));
            }
        }
    }
#endif

// Perform the error checking for the parameters.
   if (
         (0    == uiCount) ||
         (NULL == szImportMod) ||
         (TRUE == IsBadReadPtr(paHookArray, sizeof(HOOKFUNCDESC) * uiCount))
      )
   {
      SetLastErrorEx ( ERROR_INVALID_PARAMETER , SLE_ERROR ) ;
      return(FALSE);
    }
    if ( ( NULL != paOrigFuncs )                                &&
         ( TRUE == IsBadWritePtr ( paOrigFuncs ,
                                   sizeof ( PROC ) * uiCount ) )  )
    {
        SetLastErrorEx ( ERROR_INVALID_PARAMETER , SLE_ERROR ) ;
        return ( FALSE ) ;
    }
    if ( ( NULL != pdwHooked )                                    &&
         ( TRUE == IsBadWritePtr ( pdwHooked , sizeof ( UINT ) ) )  )
    {
        SetLastErrorEx ( ERROR_INVALID_PARAMETER , SLE_ERROR ) ;
        return ( FALSE ) ;
    }

    // Is this a system DLL above the 2-GB line, which Windows 98 won't
    // let you patch?
    if ( ( FALSE == IsNT ( ) ) && ( (DWORD)hModule >= 0x80000000 ) )
    {
        SetLastErrorEx ( ERROR_INVALID_HANDLE , SLE_ERROR ) ;
        return ( FALSE ) ;
    }


    // TODO TODO
    // Should each item in the hook array be checked in release builds?

    if ( NULL != paOrigFuncs )
    {
        // Set all the values in paOrigFuncs to NULL.
        memset ( paOrigFuncs , NULL , sizeof ( PROC ) * uiCount ) ;
    }
    if ( NULL != pdwHooked )
    {
        // Set the number of functions hooked to 0.
        *pdwHooked = 0 ;
    }

    // Get the specific import descriptor.
    PIMAGE_IMPORT_DESCRIPTOR pImportDesc =
                     GetNamedImportDescriptor ( hModule , szImportMod );
    if ( NULL == pImportDesc )
    {
        // The requested module wasn't imported. Don't return an error.
        return ( TRUE ) ;
    }

    // Get the original thunk information for this DLL. I can't use
    // the thunk information stored in pImportDesc->FirstThunk
    // because the loader has already changed that array to fix up
    // all the imports. The original thunk gives me access to the
    // function names.
    PIMAGE_THUNK_DATA pOrigThunk =
                        MakePtr ( PIMAGE_THUNK_DATA       ,
                                  hModule                 ,
                                  pImportDesc->OriginalFirstThunk  ) ;
    // Get the array the pImportDesc->FirstThunk points to because
    // I'll do the actual bashing and hooking there.
    PIMAGE_THUNK_DATA pRealThunk = MakePtr ( PIMAGE_THUNK_DATA       ,
                                             hModule                 ,
                                             pImportDesc->FirstThunk  );

    // Loop through and find the functions to hook.
    while ( NULL != pOrigThunk->u1.Function )
    {
        // Look only at functions that are imported by name, not those
        // that are imported by ordinal value.
        if (  IMAGE_ORDINAL_FLAG !=
                        ( pOrigThunk->u1.Ordinal & IMAGE_ORDINAL_FLAG ))
        {
            // Look at the name of this imported function.
            PIMAGE_IMPORT_BY_NAME pByName ;

            pByName = MakePtr ( PIMAGE_IMPORT_BY_NAME    ,
                                hModule                  ,
                                pOrigThunk->u1.AddressOfData  ) ;

            // If the name starts with NULL, skip it.
            if ( '\0' == pByName->Name[ 0 ] )
            {
                continue ;
            }

            // Determines whether I hook the function
            BOOL bDoHook = FALSE ;

            // TODO TODO
            // Might want to consider bsearch here.

            // See whether the imported function name is in the hook
            // array. Consider requiring paHookArray to be sorted by
            // function name so that bsearch can be used, which
            // will make the lookup faster. However, the size of
            // uiCount coming into this function should be rather
            // small, so it's OK to search the entire paHookArray for
            // each function imported by szImportMod.
            for ( UINT i = 0 ; i < uiCount ; i++ )
            {
                if ( ( paHookArray[i].szFunc[0] ==
                                                pByName->Name[0] ) &&
                     ( 0 == strcmpi ( paHookArray[i].szFunc ,
                                      (char*)pByName->Name   )   )    )
                {
                    // If the function address is NULL, exit now;
                    // otherwise, go ahead and hook the function.
                    if ( NULL != paHookArray[ i ].pProc )
                    {
                        bDoHook = TRUE ;
                    }
                    break ;
                }
            }

            if ( TRUE == bDoHook )
            {
                // I found a function to hook. Now I need to change
                // the memory protection to writable before I overwrite
                // the function pointer. Note that I'm now writing into
                // the real thunk area!

                MEMORY_BASIC_INFORMATION mbi_thunk ;

                VirtualQuery ( pRealThunk                          ,
                               &mbi_thunk                          ,
                               sizeof ( MEMORY_BASIC_INFORMATION ) ) ;

                if ( FALSE == VirtualProtect ( mbi_thunk.BaseAddress ,
                                               mbi_thunk.RegionSize  ,
                                               PAGE_READWRITE        ,
                                               &mbi_thunk.Protect     ))
                {
                    ASSERT ( !"VirtualProtect failed!" ) ;
                    SetLastErrorEx ( ERROR_INVALID_HANDLE , SLE_ERROR );
                    return ( FALSE ) ;
                }

                // Save the original address if requested.
                if ( NULL != paOrigFuncs )
                {
                    paOrigFuncs[i] = (PROC)pRealThunk->u1.Function ;
                }

                // Microsoft has two different definitions of the
                // PIMAGE_THUNK_DATA fields as they are moving to
                // support Win64. The W2K RC2 Platform SDK is the
                // latest header, so I'll use that one and force the
                // Visual C++ 6 Service Pack 3 headers to deal with it.

                // Hook the function.
                DWORD * pTemp = (DWORD*)&pRealThunk->u1.Function ;
                *pTemp = (DWORD)(paHookArray[i].pProc);

                DWORD dwOldProtect ;

                // Change the protection back to what it was before I
                // overwrote the function pointer.
                VERIFY ( VirtualProtect ( mbi_thunk.BaseAddress ,
                                          mbi_thunk.RegionSize  ,
                                          mbi_thunk.Protect     ,
                                          &dwOldProtect          ) ) ;

                if ( NULL != pdwHooked )
                {
                    // Increment the total number of functions hooked.
                    *pdwHooked += 1 ;
                }
            }
        }
        // Increment both tables.
        pOrigThunk++ ;
        pRealThunk++ ;
    }

    // All OK, JumpMaster!
    SetLastError ( ERROR_SUCCESS ) ;
    return ( TRUE ) ;
}



PROC WINAPI HookImportedFunction(
      HMODULE hFromModule,        // Module to intercept calls from
      PSTR    pszFunctionModule,  // Module to intercept calls to
      PSTR    pszFunctionName,    // Function to intercept calls to
      PROC    pfnNewProc          // New function (replaces old function)
      )
{
   PROC           pfnOriginalProc;
   DWORD          uiCount;
   HOOKFUNCDESC   ToHook;

   ToHook.szFunc = pszFunctionName;
   ToHook.pProc  = pfnNewProc;

   BOOL bRet = 
      HookImportedFunctionsByName(
         hFromModule, 
         pszFunctionModule, 
         1, 
         &ToHook, 
         (PROC*)&pfnOriginalProc,
         &uiCount
         );
   if (bRet)
      return(pfnOriginalProc);
   else
      return(NULL);
}
//
//------------------------------------------------------------------------



//------------------------------------------------------------------------
/*
//==================================
// SIMONSEZ - Matt Pietrek 1995
// FILE: HOOKAPI.C/INTRCPT.C
//==================================

typedef DWORD (__stdcall *XPROC)(DWORD);

// Converts an HMODULE under Win32s to a base address in memory
DWORD GetModuleBaseFromWin32sHMod(HMODULE hMod)
{
   XPROC ImteFromHModule, BaseAddrFromImte;
   HMODULE hModule;
   DWORD imte;
    
   hModule = GetModuleHandle(_T("W32SKRNL.DLL"));
   if( !hModule )
      return 0;
    
   ImteFromHModule = (XPROC)GetProcAddress(hModule, _T("_ImteFromHModule@4"));
   if ( !ImteFromHModule )
      return 0;
    
   BaseAddrFromImte = (XPROC)GetProcAddress(hModule, _T("_BaseAddrFromImte@4"));
   if ( !BaseAddrFromImte )
      return 0;

   imte = ImteFromHModule( (DWORD)hMod);
   if ( !imte )
      return 0;
    
   return BaseAddrFromImte(imte);
}


PROC WINAPI HookImportedFunction(
      HMODULE hFromModule,        // Module to intercept calls from
      PSTR    pszFunctionModule,  // Module to intercept calls to
      PSTR    pszFunctionName,    // Function to intercept calls to
      PROC    pfnNewProc          // New function (replaces old function)
      )
{
// Original Pietrek's code 
   PROC pfnOriginalProc;
   PIMAGE_DOS_HEADER pDosHeader;
   PIMAGE_NT_HEADERS pNTHeader;
   PIMAGE_IMPORT_DESCRIPTOR pImportDesc;
   PIMAGE_THUNK_DATA pThunk;

   if ( IsBadCodePtr(pfnNewProc) ) // Verify that a valid pfn was passed
      return 0;
    
// First, verify the the module and function names passed to use are valid
   pfnOriginalProc = GetProcAddress( GetModuleHandle(pszFunctionModule),
                                      pszFunctionName );
   if ( !pfnOriginalProc )
      return 0;
    
   if ( (GetVersion() & 0xC0000000) == 0x80000000 )
      pDosHeader =                                            // Win32s
         (PIMAGE_DOS_HEADER)GetModuleBaseFromWin32sHMod(hFromModule);
   else
      pDosHeader = (PIMAGE_DOS_HEADER)hFromModule;            // other

// Tests to make sure we're looking at a module image (the 'MZ' header)
   if ( IsBadReadPtr(pDosHeader, sizeof(IMAGE_DOS_HEADER)) )
      return 0;
   if ( pDosHeader->e_magic != IMAGE_DOS_SIGNATURE )
      return 0;

// The MZ header has a pointer to the PE header
   pNTHeader = MakePtr(PIMAGE_NT_HEADERS, pDosHeader, pDosHeader->e_lfanew);

// More tests to make sure we're looking at a "PE" image
   if ( IsBadReadPtr(pNTHeader, sizeof(IMAGE_NT_HEADERS)) )
      return 0;
   if ( pNTHeader->Signature != IMAGE_NT_SIGNATURE )
      return 0;

// We know have a valid pointer to the module's PE header.  Now go
// get a pointer to its imports section
   pImportDesc = 
      MakePtr(
         PIMAGE_IMPORT_DESCRIPTOR, 
         pDosHeader, 
         pNTHeader->OptionalHeader.
         DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress
         );
                        
// Bail out if the RVA of the imports section is 0 (it doesn't exist)
   if ( pImportDesc == (PIMAGE_IMPORT_DESCRIPTOR)pNTHeader )
      return 0;

// Iterate through the array of imported module descriptors, looking
// for the module whose name matches the pszFunctionModule parameter
   while ( pImportDesc->Name )
   {
      PSTR pszModName = MakePtr(PSTR, pDosHeader, pImportDesc->Name);
      if ( stricmp(pszModName, pszFunctionModule) == 0 )
         break;

      pImportDesc++;  // Advance to next imported module descriptor
   }
    
// Bail out if we didn't find the import module descriptor for the
// specified module.  pImportDesc->Name will be non-zero if we found it.
   if ( pImportDesc->Name == 0 )
      return 0;

// Get a pointer to the found module's import address table (IAT)
   pThunk = MakePtr(PIMAGE_THUNK_DATA, pDosHeader, pImportDesc->FirstThunk);


// Blast through the table of import addresses, looking for the one
// that matches the address we got back from GetProcAddress above.
   while ( pThunk->u1.Function )
   {
   // We found it!  Overwrite the original address with the
   // address of the interception function.  
      if ( pThunk->u1.Function == (PDWORD)pfnOriginalProc )
      {
      // first, ensure the memory is writable
         MEMORY_BASIC_INFORMATION MemInfo;
         ::VirtualQuery(pThunk, &MemInfo, sizeof(MemInfo));
         if (MemInfo.Protect != PAGE_READWRITE)
         {
            DWORD OldProtect;
            ::VirtualProtect(MemInfo.BaseAddress, MemInfo.RegionSize, PAGE_READWRITE, &OldProtect);
         }

      // and to be really sure :^)
         try
         {
            pThunk->u1.Function = (PDWORD)pfnNewProc;
         }
         catch(...)
         {
         // Function not found
            pfnOriginalProc = 0;
         }

      // Return the original address to the caller so that they 
      // can chain on to it.
         return pfnOriginalProc;
      }
      else
      {
      // If it's Chicago, and the app is being debugged (as this app is)
      // the loader doesn't fix up the calls to point directly at the
      // DLL's entry point.  Instead, the address in the .idata section
      // points to a PUSH xxxxxxxx / JMP yyyyyyyy stub.  The address in
      // xxxxxxxx points to another stub: PUSH aaaaaaaa / JMP bbbbbbbb.
      // The address in aaaaaaaa is the real address of the function in the
      // DLL.  This ugly code verifies we're looking at this stub setup,
      // and if so, grabs the real DLL entry point, and scans through
      // the InterceptedAPIArray list of addresses again.
      // ***WARNING*** ***WARNING*** ***WARNING*** ***WARNING*** 
      // This code is subject to change, and is current only as of 9/94.
         if (TRUE)
         {
            PVOID    lunacy;
            PVOID    stubAddress;
            LPVOID   address = pThunk->u1.Function;

            if ( address < (PVOID)0x80000000 )  // Only shared, system DLLs
               return 0;                        // have stubs

            if ( 
                  IsBadReadPtr(address, 9)   || 
                  (*(PBYTE)address != 0x68)  || 
                  (*((PBYTE)address+5) != 0xE9) 
               )
               return 0;

            stubAddress = (PVOID) *(PDWORD)((PBYTE)address+1);
            
            if ( pfnOriginalProc == stubAddress )
               return pfnOriginalProc;
            
            lunacy = pfnOriginalProc;
            
            if ( !IsBadReadPtr(lunacy, 9) && (*(PBYTE)lunacy == 0x68) && (*((PBYTE)lunacy+5) == 0xE9) )
            {
               lunacy = (PVOID)*(PDWORD)((PBYTE)lunacy+1);
               if ( lunacy == stubAddress )
                  return pfnOriginalProc;
            }
         }
      }

      pThunk++;   // Advance to next imported function address
   }

   return 0;   // Function not found
}
*/
//------------------------------------------------------------------------
