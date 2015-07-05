#include "stdafx.h"
#include "GdiResources.h"



#ifdef _DEBUG
    #undef THIS_FILE
    static char THIS_FILE[]=__FILE__;
    #define new DEBUG_NEW
#endif


//------------------------------------------------------------------
// CGdiObj class implementation
//

// static members
//
LPCTSTR CGdiObj::s_TypeStrings[] = 
{
   "?",
   "Pen",
   "Brush",
   "DC",
   "MetaDC",
   "Palette",
   "Font",
   "Bitmap",
   "Region",
   "MetaFile",
   "MemDC",
   "ExtPen",
   "EnhMetaDC",
   "EnhMetafile",
   "ColorSpace",
};


CGdiObj::CGdiObj()
{
// invalid type by default
   m_Type = 0;
}



DWORD CGdiObj::GetHandle()
{
   return(m_Handle);
}

DWORD CGdiObj::GetType()
{
   return(m_Type);
}


void CGdiObj::GetTypeString(CString& szText)
{
   if ((m_Type >= 1) && (m_Type <= NB_GDI_TYPE))
      szText = s_TypeStrings[m_Type];
   else
      szText = s_TypeStrings[0];   // "?" string
}


void CGdiObj::SetHandle(DWORD Handle)
{
   m_Handle = Handle;
}


void CGdiObj::SetType(DWORD Type)
{
   if ((Type >= 1) && (Type <= NB_GDI_TYPE))
      m_Type = Type;
   else 
   {
   // invalid GDI type is set
      m_Type = 0;
   }
}
//
//------------------------------------------------------------------






//------------------------------------------------------------------
// CGdiResource class implementation
// 
void CGdiResources::Cleanup()
{
// get each element of the map and delete it
   POSITION pos;
   DWORD    key;
   CGdiObj* pObj;
   for (pos = m_HandleMap.GetStartPosition(); (pos != NULL); )
   {
      m_HandleMap.GetNextAssoc(pos, key, pObj);
      delete pObj;
   }

// empty the map itself
   m_HandleMap.RemoveAll();
}


void CGdiResources::DefaultInit()
{
   ::ZeroMemory(m_CountByType, sizeof(m_CountByType[0]) * (NB_GDI_TYPE+1));

   Cleanup();
}

CGdiResources::CGdiResources()
{
   DefaultInit();
}


CGdiResources::~CGdiResources()
{
   Cleanup();
}


void CGdiResources::AddObject(CGdiObj* pObject)
{
// sanity checks
   ASSERT(pObject != NULL);
   if (pObject == NULL)
      return;

// store it
   m_HandleMap[(LPVOID)pObject->GetHandle()] = pObject;

// update type count
   switch(pObject->GetType())
   {
      case OBJ_BITMAP:
      case OBJ_BRUSH:
      case OBJ_PAL:
      case OBJ_PEN:
      case OBJ_DC:
      case OBJ_MEMDC:
      case OBJ_EXTPEN:
      case OBJ_METAFILE:
      case OBJ_METADC:
      case OBJ_ENHMETAFILE:
      case OBJ_ENHMETADC:
      case OBJ_FONT:
      case OBJ_REGION:
      case OBJ_COLORSPACE:
         m_CountByType[pObject->GetType()-1]++;
      break;

      default:
      // invalid GDI object type
         m_CountByType[NB_GDI_TYPE]++;
      break;
   }
}



//------------------------------------------------------------------
// internal GDI handle table structure and helpers
// 

// structure of a entry in the table managed by GDI
// 
typedef struct
{
   DWORD pKernelInfo;

// these fields are swapped in Windows NT
   WORD  ProcessID; 
   WORD  _nCount;

   WORD  nUpper;
   WORD  nType;
   DWORD pUserInfo;
} GDITableEntry;


//
const int MAX_GDI_HANDLE = 0x4000;


static GDITableEntry* GetGDIHandleTable()
{
// using the PEB description from WinDbg
   return((GDITableEntry*) (*(LPDWORD)(0x7ffdf000+0x094)));
}


static DWORD GetProcessIDFromIndex(WORD Index, BOOL b2000_XP)
{
// sanity checks
   if (Index >= MAX_GDI_HANDLE)
      return(0);

   DWORD PID = 0;

// get PID from the corresponding field
   GDITableEntry* pHandleTable = GetGDIHandleTable();
   if (pHandleTable != NULL)
   {
      if (b2000_XP)
      {
      // XP - 2000
         PID = pHandleTable[Index].ProcessID;
      }
      else
      {
      // NT 4.0
         PID = pHandleTable[Index]._nCount;
      }
   }

   return(PID);
}


static BOOL ObjectAtIndexIsDeleted(WORD Index)
{
   BOOL bIsDeleted = TRUE;
   GDITableEntry* pHandleTable = GetGDIHandleTable();
   if (pHandleTable != NULL)
   {
      bIsDeleted = (pHandleTable[Index].pKernelInfo < 0x80000000);
   }

   return(bIsDeleted);
}


static HANDLE RebuildGDIHandle(WORD Index)
{
   HANDLE hObject = NULL;
   GDITableEntry* pHandleTable = GetGDIHandleTable();
   if (pHandleTable != NULL)
   {
      hObject = (HANDLE)MAKELONG(Index, pHandleTable[Index].nUpper);
   }
   
   return(hObject);
}


static WORD GetOffsetFromHandle(DWORD hObject)
{
   return((WORD)hObject & 0x3FFF);
}


static BYTE GetInternalType(WORD Index)
{
   BYTE Type = 0;
   GDITableEntry* pHandleTable = GetGDIHandleTable();
   if (pHandleTable != NULL)
   {
      Type = LOBYTE(pHandleTable[Index].nUpper) & 0x7F;
   }
   
   return(Type);
}


static DWORD GetWin32GDIType(BYTE InternalType)
{
   switch(InternalType)
   {
      case 1:
         return(OBJ_DC);
      break;

      case 4:
         return(OBJ_REGION);
      break;

      case 5:
         return(OBJ_BITMAP);
      break;

      case 8:
         return(OBJ_PAL);
      break;

      case 10:
         return(OBJ_FONT);
      break;

      case 16:
         return(OBJ_BRUSH);
      break;

      case 33:
         return(OBJ_ENHMETAFILE);
      break;

      case 48:
         return(OBJ_PEN);
      break;

      case 80:
         return(OBJ_EXTPEN);
      break;


      default:
         return(0);
      break;
   }
}
//
//
//------------------------------------------------------------------


// Exported function
// 
LPCTSTR GetGDIType(DWORD dwType)
{
   if ((dwType >= 0) && (dwType <= NB_GDI_TYPE))
      return(CGdiObj::s_TypeStrings[dwType]);
   else
      return(CGdiObj::s_TypeStrings[0]);
}


BOOL IsStockObject(DWORD hObject)
{
   return((hObject & 0x800000) == 0x800000);
}


BOOL ObjectIsDeleted(DWORD hObject)
{
   return(!ObjectAtIndexIsDeleted(GetOffsetFromHandle(hObject)));
}




DWORD CGdiResources::GetGDIList(DWORD ProcessID)
{
   GDITableEntry* pHandleTable = GetGDIHandleTable();
   if (pHandleTable == NULL)
      return(0);

   DWORD    Count = 0;
   HANDLE   hObject;

// maybe the memory is not there...
   try
   {
   // get Windows version
      BOOL b2000_XP = TRUE;
      OSVERSIONINFO VersionInfo;
      ::ZeroMemory(&VersionInfo, sizeof(VersionInfo));
      VersionInfo.dwOSVersionInfoSize = sizeof(VersionInfo);
      if (
            (!::GetVersionEx(&VersionInfo)) ||
            (VersionInfo.dwPlatformId != VER_PLATFORM_WIN32_NT) ||
            (VersionInfo.dwMajorVersion < 4)
         )
         return(0);
      
      b2000_XP = VersionInfo.dwMajorVersion > 4;

      TCHAR szInfo[_MAX_PATH];
      CGdiObj* pObject;
      
      for (WORD Index = 0; Index < MAX_GDI_HANDLE; Index++)
      {
      // check if it is owned by the right process
         if (GetProcessIDFromIndex(Index, b2000_XP) != ProcessID)
            continue;

      // check if object is deleted
         if (ObjectAtIndexIsDeleted(Index))
            continue;

      // get the corresponding handle
         hObject = RebuildGDIHandle(Index);
         if (hObject == NULL)
            continue;

      // create a new object to wrap this GDI handle
         pObject = new CGdiObj;
         if (pObject == NULL)
            break;

         pObject->SetHandle((DWORD)hObject);
         BYTE GDIType = GetInternalType(Index);
         DWORD Win32Type = GetWin32GDIType(GDIType);         
         pObject->SetType(Win32Type);

         if (Win32Type == 0)
         {
            wsprintf(
               szInfo,
               "Invalid GDI type in process %u:\t0x%08X\t%u\n", 
               ProcessID,
               hObject,
               GetInternalType(Index)
               );
            OutputDebugString(szInfo);
         }

      // and store it
         AddObject(pObject);

      // update counters
         Count++;
      }
   }
   catch(...)
   {
   // Oops...
   }

   return(Count);
}


// reset the state based on the given list
// 
DWORD CGdiResources::CreateFromList(GDI_LIST* pList)
{
// sanity checks
   ASSERT(pList != NULL);
   if (pList == NULL)
      return(0);
   
// reset state
   DefaultInit();

// get the objects count
   DWORD Count = pList->dwCount;
   
// insert each object 
   CGdiObj* pObject;
   for (DWORD CurrentObject = 0; CurrentObject < Count; CurrentObject++)
   {
   // create a new object to wrap this GDI handle
      pObject = new CGdiObj;
      if (pObject == NULL)
         break;

      pObject->SetHandle((DWORD)pList->Items[CurrentObject].hObject);
      pObject->SetType(pList->Items[CurrentObject].dwType);

   // and store it
      AddObject(pObject);
   }

   return(Count);
}


// return the total objects count
//
DWORD CGdiResources::GetTotalCount()
{
   return(m_HandleMap.GetCount());
}


CGdiObj* CGdiResources::GetFirst(POSITION& pos)
{
   pos = m_HandleMap.GetStartPosition();
   return(GetNext(pos));
}


// next iteration step
//
CGdiObj* CGdiResources::GetNext(POSITION& pos)
{
   if (pos != NULL)
   {
      DWORD Handle = 0;
      CGdiObj* pObj = NULL;
      m_HandleMap.GetNextAssoc(pos, Handle, pObj);

/*
   // check if object is deleted
      GDITableEntry* pHandleTable = GetGDIHandleTable();
      if (pHandleTable != NULL)
      {
         if (!ObjectAtIndexIsDeleted(GetOffsetFromHandle(Handle)))
            return(GetNext(pos));
      }
*/

      return(pObj);
   }
   else
      return(NULL);
}


// return the GDI onject info associated with the given handle
//
CGdiObj* CGdiResources::GetObjectFromMap(DWORD Handle)
{
   CGdiObj* pObj = NULL;
   if (m_HandleMap.Lookup((LPVOID)Handle, pObj))
      return(pObj);
   else
      return(NULL);
}
//
//------------------------------------------------------------------




//------------------------------------------------------------------
// CResourcesCompare class implementation
// 
CResourcesCompare::CResourcesCompare()
{
   Reset();
}


void CResourcesCompare::Reset()
{
   ::ZeroMemory(m_CountByType, sizeof(m_CountByType[0]) * (NB_GDI_TYPE+1));
   ::ZeroMemory(m_OutCountByType, sizeof(m_OutCountByType[0]) * (NB_GDI_TYPE+1));
   ::ZeroMemory(m_NewCountByType, sizeof(m_NewCountByType[0]) * (NB_GDI_TYPE+1));
}


DWORD CResourcesCompare::GetSameFromType(int iType)
{
   if ((iType >= 1) && (iType <= NB_GDI_TYPE))
      return(m_CountByType[iType-1]);
   else
      return(0);
}

DWORD CResourcesCompare::GetOutFromType(int iType)
{
   if ((iType >= 1) && (iType <= NB_GDI_TYPE))
      return(m_OutCountByType[iType-1]);
   else
      return(0);
}

DWORD CResourcesCompare::GetNewFromType(int iType)
{
   if ((iType >= 1) && (iType <= NB_GDI_TYPE))
      return(m_NewCountByType[iType-1]);
   else
      return(0);
}



void CResourcesCompare::ComputeCompare(CGdiResources& Current, CGdiResources& Reference)
{
// set 0 to all counters
   DefaultInit();
   Reset();

   POSITION Pos;
   CGdiObj* pGdiObject = NULL;
   CGdiObj* pObj = NULL;
   DWORD    hGDIObject;
   DWORD    dwObjectType;

// a 'Same' is when a handle is in Current and WAS also in Reference
// a 'New' is when a handle is in Current but WAS NOT in Reference
// --> only New are saved in the map
   for (
         pGdiObject = Current.GetFirst(Pos); 
         (pGdiObject != NULL); 
         pGdiObject = Current.GetNext(Pos)
       )
   {
      if ((pGdiObject != NULL))
      {
         hGDIObject     = pGdiObject->GetHandle();
         dwObjectType   = pGdiObject->GetType();
         if (Reference.GetObjectFromMap(hGDIObject) != NULL)
            m_CountByType[dwObjectType-1]++;
         else 
         {
         // count it
            m_NewCountByType[dwObjectType-1]++;

         // add it into the map
            pObj = new CGdiObj;
            if (pObj != NULL)
            {
               pObj->SetHandle(hGDIObject);
               pObj->SetType(dwObjectType);

               m_HandleMap[(LPVOID)hGDIObject] = pObj;
            }
            else
               break;
         }
      }
   }

// an 'Out' is when a handle WAS in Reference but no more in Current 
   for (
         pGdiObject = Reference.GetFirst(Pos); 
         (pGdiObject != NULL); 
         pGdiObject = Reference.GetNext(Pos)
       )
   {
      if ((pGdiObject != NULL))
      {
         hGDIObject     = pGdiObject->GetHandle();
         dwObjectType   = pGdiObject->GetType();
         
         if (Current.GetObjectFromMap(hGDIObject) == NULL)
            m_OutCountByType[dwObjectType-1]++;
      }
   }
}
//
//------------------------------------------------------------------
