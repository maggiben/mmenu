#include "stdafx.h"
#include "GDITrace.h"
#include "GDIReflect.h"
#include "..\Common\Wrappers.h"


#ifdef _DEBUG
   #undef THIS_FILE
   static char THIS_FILE[]=__FILE__;
   #define new DEBUG_NEW
#endif


/* 
from Plateform SDK WINGDI.H
------------------------------
#define OBJ_PEN            1 
#define OBJ_BRUSH          2 
#define OBJ_DC             3 
#define OBJ_METADC         4 
#define OBJ_PAL            5 
#define OBJ_FONT           6 
#define OBJ_BITMAP         7 
#define OBJ_REGION         8 
#define OBJ_METAFILE       9 
#define OBJ_MEMDC          10 
#define OBJ_EXTPEN         11 
#define OBJ_ENHMETADC      12 
#define OBJ_ENHMETAFILE    13 
#define OBJ_COLORSPACE     14 
*/
#define OBJ_COLORSPACE     14 
#define NB_GDI_TYPE        OBJ_COLORSPACE


LPCTSTR g_TypesGDI[] = 
{
   _T("?"),
   _T("Pen"),
   _T("Brush"),
   _T("DC"),
   _T("MetaDC"),
   _T("Palette"),
   _T("Font"),
   _T("Bitmap"),
   _T("Region"),
   _T("MetaFile"),
   _T("MemDC"),
   _T("ExtPen"),
   _T("EnhMetaDC"),
   _T("EnhMetafile"),
   _T("ColorSpace")
};


LPCTSTR _GetGDIType(DWORD dwType)
{
   if ((dwType >= 0) && (dwType <= NB_GDI_TYPE))
      return(g_TypesGDI[dwType]);
   else
      return(g_TypesGDI[0]);
}



//--------------------------------------------------------------------------
// global variable
//

CStackManager* CGDIReflect::GetStackManager()
{
   return(&s_StackManager);
}


//--------------------------------------------------------------------------
//
// static definitions
//
CTypedPtrMap<CMapPtrToPtr, HANDLE, CHandleInfo*> CGDIReflect::m_HandleMap;
CStackManager CGDIReflect::s_StackManager;


// bitmap
DEFINE_API_REFLECT(CGDIReflect, LoadBitmapA);
DEFINE_API_REFLECT(CGDIReflect, LoadBitmapW);
DEFINE_API_REFLECT(CGDIReflect, CreateBitmap);
DEFINE_API_REFLECT(CGDIReflect, CreateBitmapIndirect);
DEFINE_API_REFLECT(CGDIReflect, CreateCompatibleBitmap);

// brush
DEFINE_API_REFLECT(CGDIReflect, CreateBrushIndirect);
DEFINE_API_REFLECT(CGDIReflect, CreateSolidBrush);
DEFINE_API_REFLECT(CGDIReflect, CreatePatternBrush);
DEFINE_API_REFLECT(CGDIReflect, CreateDIBPatternBrush);
DEFINE_API_REFLECT(CGDIReflect, CreateDIBPatternBrushPt);
DEFINE_API_REFLECT(CGDIReflect, CreateHatchBrush);

// device context
DEFINE_API_REFLECT(CGDIReflect, CreateCompatibleDC);
DEFINE_API_REFLECT(CGDIReflect, CreateDCA);
DEFINE_API_REFLECT(CGDIReflect, CreateDCW);
DEFINE_API_REFLECT(CGDIReflect, CreateICA);
DEFINE_API_REFLECT(CGDIReflect, CreateICW);
DEFINE_API_REFLECT(CGDIReflect, GetDC);
DEFINE_API_REFLECT(CGDIReflect, GetDCEx);
DEFINE_API_REFLECT(CGDIReflect, GetWindowDC);

// font
DEFINE_API_REFLECT(CGDIReflect, CreateFontA);
DEFINE_API_REFLECT(CGDIReflect, CreateFontW);
DEFINE_API_REFLECT(CGDIReflect, CreateFontIndirectA);
DEFINE_API_REFLECT(CGDIReflect, CreateFontIndirectW);

// metafile
DEFINE_API_REFLECT(CGDIReflect, CreateMetaFileA);
DEFINE_API_REFLECT(CGDIReflect, CreateMetaFileW);
DEFINE_API_REFLECT(CGDIReflect, CreateEnhMetaFileA);
DEFINE_API_REFLECT(CGDIReflect, CreateEnhMetaFileW);
DEFINE_API_REFLECT(CGDIReflect, GetEnhMetaFileA);
DEFINE_API_REFLECT(CGDIReflect, GetEnhMetaFileW);
DEFINE_API_REFLECT(CGDIReflect, GetMetaFileA);
DEFINE_API_REFLECT(CGDIReflect, GetMetaFileW);

// pen
DEFINE_API_REFLECT(CGDIReflect, CreatePen);
DEFINE_API_REFLECT(CGDIReflect, CreatePenIndirect);
DEFINE_API_REFLECT(CGDIReflect, ExtCreatePen);

// region
DEFINE_API_REFLECT(CGDIReflect, PathToRegion);
DEFINE_API_REFLECT(CGDIReflect, CreateEllipticRgn);
DEFINE_API_REFLECT(CGDIReflect, CreateEllipticRgnIndirect);
DEFINE_API_REFLECT(CGDIReflect, CreatePolygonRgn);
DEFINE_API_REFLECT(CGDIReflect, CreatePolyPolygonRgn);
DEFINE_API_REFLECT(CGDIReflect, CreateRectRgn);
DEFINE_API_REFLECT(CGDIReflect, CreateRectRgnIndirect);
DEFINE_API_REFLECT(CGDIReflect, CreateRoundRectRgn);
DEFINE_API_REFLECT(CGDIReflect, ExtCreateRegion);

// palette
DEFINE_API_REFLECT(CGDIReflect, CreateHalftonePalette);
DEFINE_API_REFLECT(CGDIReflect, CreatePalette);

// Object Deletion
DEFINE_API_REFLECT(CGDIReflect, DeleteObject);
DEFINE_API_REFLECT(CGDIReflect, DeleteDC);
DEFINE_API_REFLECT(CGDIReflect, DeleteMetaFile);
DEFINE_API_REFLECT(CGDIReflect, DeleteEnhMetaFile);
DEFINE_API_REFLECT(CGDIReflect, CloseMetaFile);
DEFINE_API_REFLECT(CGDIReflect, CloseEnhMetaFile);



//
// different system initializations
//
BOOL CGDIReflect::FillStubs(DWORD dwState, HMODULE hModule)
{
   BEGIN_IMPLEMENT_API_REFLECT()

   // bitmap
      IMPLEMENT_API_REFLECT(hModule, "USER32.DLL", LoadBitmapA);
      IMPLEMENT_API_REFLECT(hModule, "USER32.DLL", LoadBitmapW);
      IMPLEMENT_API_REFLECT(hModule, "GDI32.DLL", CreateBitmap);
      IMPLEMENT_API_REFLECT(hModule, "GDI32.DLL", CreateBitmapIndirect);
      IMPLEMENT_API_REFLECT(hModule, "GDI32.DLL", CreateCompatibleBitmap);

   // brush
      IMPLEMENT_API_REFLECT(hModule, "GDI32.DLL", CreateBrushIndirect);
      IMPLEMENT_API_REFLECT(hModule, "GDI32.DLL", CreateSolidBrush);
      IMPLEMENT_API_REFLECT(hModule, "GDI32.DLL", CreatePatternBrush);
      IMPLEMENT_API_REFLECT(hModule, "GDI32.DLL", CreateDIBPatternBrush);
      IMPLEMENT_API_REFLECT(hModule, "GDI32.DLL", CreateDIBPatternBrushPt);
      IMPLEMENT_API_REFLECT(hModule, "GDI32.DLL", CreateHatchBrush);

   // device context
      IMPLEMENT_API_REFLECT(hModule, "GDI32.DLL", CreateCompatibleDC);
      IMPLEMENT_API_REFLECT(hModule, "GDI32.DLL", CreateDCA);
      IMPLEMENT_API_REFLECT(hModule, "GDI32.DLL", CreateDCW);
      IMPLEMENT_API_REFLECT(hModule, "GDI32.DLL", CreateICA);
      IMPLEMENT_API_REFLECT(hModule, "GDI32.DLL", CreateICW);
      IMPLEMENT_API_REFLECT(hModule, "USER32.DLL", GetDC);
      IMPLEMENT_API_REFLECT(hModule, "USER32.DLL", GetDCEx);
      IMPLEMENT_API_REFLECT(hModule, "USER32.DLL", GetWindowDC);

   // font
      IMPLEMENT_API_REFLECT(hModule, "GDI32.DLL", CreateFontA);
      IMPLEMENT_API_REFLECT(hModule, "GDI32.DLL", CreateFontW);
      IMPLEMENT_API_REFLECT(hModule, "GDI32.DLL", CreateFontIndirectA);
      IMPLEMENT_API_REFLECT(hModule, "GDI32.DLL", CreateFontIndirectW);

   // metafile
      IMPLEMENT_API_REFLECT(hModule, "GDI32.DLL", CreateMetaFileA);
      IMPLEMENT_API_REFLECT(hModule, "GDI32.DLL", CreateMetaFileW);
      IMPLEMENT_API_REFLECT(hModule, "GDI32.DLL", CreateEnhMetaFileA);
      IMPLEMENT_API_REFLECT(hModule, "GDI32.DLL", CreateEnhMetaFileW);
      IMPLEMENT_API_REFLECT(hModule, "GDI32.DLL", GetEnhMetaFileA);
      IMPLEMENT_API_REFLECT(hModule, "GDI32.DLL", GetEnhMetaFileW);
      IMPLEMENT_API_REFLECT(hModule, "GDI32.DLL", GetMetaFileA);
      IMPLEMENT_API_REFLECT(hModule, "GDI32.DLL", GetMetaFileW);

   // pen
      IMPLEMENT_API_REFLECT(hModule, "GDI32.DLL", CreatePen);
      IMPLEMENT_API_REFLECT(hModule, "GDI32.DLL", CreatePenIndirect);
      IMPLEMENT_API_REFLECT(hModule, "GDI32.DLL", ExtCreatePen);

   // region
      IMPLEMENT_API_REFLECT(hModule, "GDI32.DLL", PathToRegion);
      IMPLEMENT_API_REFLECT(hModule, "GDI32.DLL", CreateEllipticRgn);
      IMPLEMENT_API_REFLECT(hModule, "GDI32.DLL", CreateEllipticRgnIndirect);
      IMPLEMENT_API_REFLECT(hModule, "GDI32.DLL", CreatePolygonRgn);
      IMPLEMENT_API_REFLECT(hModule, "GDI32.DLL", CreatePolyPolygonRgn);
      IMPLEMENT_API_REFLECT(hModule, "GDI32.DLL", CreateRectRgn);
      IMPLEMENT_API_REFLECT(hModule, "GDI32.DLL", CreateRectRgnIndirect);
      IMPLEMENT_API_REFLECT(hModule, "GDI32.DLL", CreateRoundRectRgn);
      IMPLEMENT_API_REFLECT(hModule, "GDI32.DLL", ExtCreateRegion);

   // palette
      IMPLEMENT_API_REFLECT(hModule, "GDI32.DLL", CreateHalftonePalette);
      IMPLEMENT_API_REFLECT(hModule, "GDI32.DLL", CreatePalette);

   
   // object deletion
      IMPLEMENT_API_REFLECT(hModule, "GDI32.DLL", DeleteObject);
      IMPLEMENT_API_REFLECT(hModule, "GDI32.DLL", DeleteDC);
      IMPLEMENT_API_REFLECT(hModule, "GDI32.DLL", DeleteMetaFile);
      IMPLEMENT_API_REFLECT(hModule, "GDI32.DLL", DeleteEnhMetaFile);
      IMPLEMENT_API_REFLECT(hModule, "GDI32.DLL", CloseMetaFile);
      IMPLEMENT_API_REFLECT(hModule, "GDI32.DLL", CloseEnhMetaFile);
      
   END_IMPLEMENT_API_REFLECT()
}


//
// default values
//
BOOL CGDIReflect::Init()
{
// base class processing
   BOOL bReturn = CAPIReflect::Init();
   if (!bReturn)
      return(FALSE);

// internal members
   DefaultReset();

// init stack manager
   bReturn = s_StackManager.Init();

   return(bReturn);
}


//
// reflect API for the given module
// 
BOOL CGDIReflect::DoReflect(HMODULE hModule)
{
// base class processing
   return(CAPIReflect::DoReflect(hModule));
}
//
//--------------------------------------------------------------------------



//--------------------------------------------------------------------------
//
// construction/destruction
//
CGDIReflect::CGDIReflect()
{
   m_HandleMap.InitHashTable(2039, TRUE); // 2039 is the prime number closest to 2048

// TO DO: For debug only

//    API_TRACE_LEVEL_HOOK_IMPORTED_FUNCTION 
//    API_TRACE_LEVEL_REFLECT_CALL
//    API_TRACE_LEVEL_DOREFLECT
//    API_TRACE_LEVEL_INIT
   SetTraceLevel(
      API_TRACE_LEVEL_INIT
//      | API_TRACE_LEVEL_REFLECT_CALL  
      );
}

CGDIReflect::~CGDIReflect()
{
   DefaultReset();
}


void CGDIReflect::DefaultReset()
{
// delete each element if needed
   POSITION       pos;
   CHandleInfo*   pInfo = NULL;
   HANDLE         hObject;

   for (pos = m_HandleMap.GetStartPosition(); (pos != NULL); )
   {
      m_HandleMap.GetNextAssoc(pos, hObject, pInfo);
      delete pInfo;
   }

// reset map content and structure
   m_HandleMap.RemoveAll();
}
//
//--------------------------------------------------------------------------



//--------------------------------------------------------------------------
//
// validation interface
//
BOOL CGDIReflect::IsValidGDIObject(HANDLE hObject, DWORD& dwType)
{
   dwType = ::GetObjectType((HGDIOBJ)hObject);
   if (dwType != 0)
   {
      switch(dwType)
      {
         case OBJ_BITMAP :
         {
            HBITMAP hBitmap = (HBITMAP)hObject;
            BITMAP bmInfo;
            return(::GetObject(hBitmap, sizeof(BITMAP), &bmInfo) == sizeof(BITMAP));
         }


         case OBJ_FONT :
         {
            LOGFONT logFont;
            HFONT hFont = (HFONT)hObject;
            return(::GetObject(hFont, sizeof(LOGFONT), &logFont) == sizeof(LOGFONT));
         }


         case OBJ_PEN :
         {
            HPEN hPen = (HPEN)hObject;
            LOGPEN penInfo;
            return(::GetObject(hPen, sizeof(LOGPEN), &penInfo) == sizeof(LOGPEN));
         }


         case OBJ_BRUSH :
         {
            HBRUSH hBrush = (HBRUSH)hObject;
            LOGBRUSH BrushInfo;
            return(::GetObject(hBrush, sizeof(LOGBRUSH), &BrushInfo) == sizeof(LOGBRUSH));
         }


         case OBJ_PAL :
         {
            HPALETTE hPalette = (HPALETTE)hObject;
            WORD wCount;
            return(::GetObject(hPalette, sizeof(WORD), &wCount) == sizeof(WORD));
         }


         case OBJ_EXTPEN :
         {
            HPEN hPen = (HPEN)hObject;
            EXTLOGPEN penInfo;
            BOOL bReturn = ::GetObject(hPen, sizeof(EXTLOGPEN), &penInfo) == sizeof(EXTLOGPEN);
            TRACE("Invalid EXT PEN '0x%x'\n", hObject);

            return(bReturn);
         }


         case OBJ_REGION :
         {
            HRGN hRegion = (HRGN)hObject;
            RECT  rgnRect;
            int iResult = ::GetRgnBox(hRegion, &rgnRect);
            return(iResult != 0);
         }


      // find a way to do the check for these types too
         case OBJ_DC :
         case OBJ_MEMDC :
         case OBJ_METAFILE :
         case OBJ_METADC :
         case OBJ_ENHMETAFILE :
         case OBJ_ENHMETADC :
            return(TRUE);


         default:
            return(FALSE);
      }

      return(TRUE);
   }
   else
   {
      return(FALSE);
   }
}
//
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
//
// enumeration interface
//
POSITION CGDIReflect::GetFirstItem(HANDLE& hObject, CHandleInfo*& pInfo)
{
// start again
   POSITION iCurrent = m_HandleMap.GetStartPosition();
   return(GetNextItem(iCurrent, hObject, pInfo));
}


POSITION CGDIReflect::GetNextItem(POSITION iCurrent, HANDLE& hObject, CHandleInfo*& pInfo)
{
   if (iCurrent != NULL)
   {
      m_HandleMap.GetNextAssoc(iCurrent, hObject, pInfo);

   // check handle type 
      DWORD dwType;
      if (
            (pInfo != NULL) && 
            (IsValidGDIObject(hObject, dwType))
         )
      {
         return(iCurrent);
      }
      else
      {
      // search for the next one
         return(GetNextItem(iCurrent, hObject, pInfo));
      }
   }
   else
   {
      pInfo    = NULL;
      hObject  = NULL;
      return(NULL);
   }
}


CHandleInfo* CGDIReflect::GetHandleInfo(HANDLE hObject)
{
   CHandleInfo* pInfo;
   if (!m_HandleMap.Lookup(hObject, pInfo))
   {
      pInfo = NULL;
   }

   return(pInfo);
}
//
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
//
// Tracing
//
void TRACEEX(LPCTSTR lpszFormat, ...)
{
	va_list args;
	va_start(args, lpszFormat);

	int nBuf;
	TCHAR szBuffer[0x2001];   // instead of 512 with MFC :^(

	nBuf = _vsntprintf(szBuffer, 0x2000, lpszFormat, args);

	// was there an error? was the expanded string too long?
	ASSERT(nBuf >= 0);

   OutputDebugString(szBuffer);

	va_end(args);
}


DWORD CGDIReflect::DumpLeaks()
{
   TRACEEX("Dumping valid GDI objects\n");
   TRACEEX("---------------------------------------------\n");

   DWORD          dwMax = 0;
   DWORD          dwType;
   HANDLE         hObject;
   CHandleInfo*   pInfo;
   CString        szCallStack = _T("");
   CStackManager* pStackManager = GetStackManager();
   ASSERT(pStackManager != NULL);
   for (POSITION pos = GetFirstItem(hObject, pInfo); (pos != NULL); pos = GetNextItem(pos, hObject, pInfo))
   {
      if (
            (pInfo != NULL) && 
            (!ObjectIsDeleted((DWORD)hObject)) &&
            (IsValidGDIObject(hObject, dwType))
         )
      {
         TRACEEX("  %04u %10s --- 0x%08x\n", ++dwMax, _GetGDIType(dwType), hObject);

      // dump the corresponding calls stack
         if (pStackManager != NULL)
         {
            szCallStack = _T("");
            pStackManager->DumpStackAllocation(pInfo->m_Depth, pInfo->m_pStack, _T("\n"), szCallStack);
            TRACEEX("%s\n", (LPCTSTR)szCallStack);
         }
      }
   }
   TRACEEX("---------------------------------------------\n");

   return(dwMax);
}
//
//--------------------------------------------------------------------------


