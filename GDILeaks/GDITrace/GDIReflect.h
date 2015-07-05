#if !defined(AFX_GDIREFLECT_H__E7B39634_8705_11D2_B112_000000000000__INCLUDED_)
#define AFX_GDIREFLECT_H__E7B39634_8705_11D2_B112_000000000000__INCLUDED_

#if _MSC_VER > 1000
   #pragma once
#endif // _MSC_VER > 1000


#include "..\Common\APIReflect.h"
#include <AfxTempl.h>
#include "..\Common\StackManager.h"


// force functions export since we are in the DLL
//
#define _GDITRACE_EXPORT
#include "_GDITrace.h"


// store information related to a GDI object
// 
class CHandleInfo
{
public:
   CHandleInfo()
   {
      m_hObject   = NULL;  // handle
      m_Type      = 0;     // type (bitmap, brush,...)
      m_pStack    = NULL;  // call stack
      m_Depth     = 0;     // call stack depth
   }

   virtual ~CHandleInfo()
   {
      if (m_pStack != NULL)
         delete [] m_pStack;
   }

public:
   HANDLE   m_hObject;
   DWORD    m_Type;
   DWORD    m_Depth;
   DWORD*   m_pStack;
};



// global API
//
LPCTSTR _GetGDIType(DWORD dwType);




// macros for deletion API reflection
//
// APIR_STATE_ENABLE : reflect the API for the given module
// APIR_STATE_INIT   : get default value via ::GetProcAddress()
//
#define _DELETE_GDI_OBJECT(hObject)                   \
   CHandleInfo* pInfo = NULL;                         \
   if (m_HandleMap.Lookup((HANDLE)hObject, pInfo))    \
   {                                                  \
      delete pInfo;                                   \
      m_HandleMap.RemoveKey((HANDLE)hObject);         \
   }                                                  \

#define DECLARE_REFLECT_DELETE(Name, vType,  x)       \
   typedef BOOL (__stdcall *Name##Proc)(vType x);     \
   static Name##Proc __##Name;                        \
   static BOOL WINAPI _##Name(vType x)                \
   {                                                  \
      BOOL bReturn = __##Name(x);                     \
                                                      \
      TraceReflectCall(#Name, (DWORD)x);              \
                                                      \
      _DELETE_GDI_OBJECT(x)                           \
                                                      \
      return(bReturn);                                \
   }




// macros for GDI reflection
//

#define _STORE_GDI_OBJECT(hObject)                    \
   CHandleInfo*   pInfo = NULL;                       \
   HANDLE         Handle = NULL;                      \
   if (m_HandleMap.Lookup((HANDLE)hObject, pInfo))    \
   {                                                  \
      delete pInfo;                                   \
   }                                                  \
   pInfo = new CHandleInfo;                           \
   if (pInfo != NULL)                                 \
   {                                                  \
      pInfo->m_hObject = hObject;                     \
      if (!IsValidGDIObject(hObject, pInfo->m_Type))  \
      {                                               \
         TRACE("...Invalid object to store");         \
         pInfo->m_Type = 0;                           \
      }                                               \
      pInfo->m_pStack  =                              \
         CGDIReflect::GetStackManager()->DoStackAddressDump(\
            pInfo->m_Depth,                           \
            1                                         \
            );                                        \
      if (pInfo->m_pStack == NULL)                    \
         pInfo->m_Depth = 0;                          \
      m_HandleMap[(HANDLE)hObject] = pInfo;           \
   }                                                  \


   #define DECLARE_REFLECT_API1(Name, ResultType, vType,  x)      \
      typedef ResultType (__stdcall *Name##Proc)(vType x);        \
      static Name##Proc __##Name;                                 \
      static ResultType WINAPI _##Name(vType x)                   \
      {                                                           \
         ResultType hVar##ResultType = __##Name(x);               \
                                                                  \
         TraceReflectCall(#Name, (DWORD)hVar##ResultType);        \
                                                                  \
         _STORE_GDI_OBJECT(hVar##ResultType)                      \
                                                                  \
         return(hVar##ResultType);                                \
      }


   #define DECLARE_REFLECT_API2(Name, ResultType, vType1, x1, vType2, x2) \
      typedef ResultType (__stdcall *Name##Proc)(vType1 x1, vType2 x2); \
      static Name##Proc __##Name; \
      static ResultType WINAPI _##Name(vType1 x1, vType2 x2)      \
      {                                                           \
         ResultType hVar##ResultType = __##Name(x1, x2);          \
                                                                  \
         TraceReflectCall(#Name, (DWORD)hVar##ResultType);        \
                                                                  \
         _STORE_GDI_OBJECT(hVar##ResultType)                      \
                                                                  \
         return(hVar##ResultType);                                \
      }

   #define DECLARE_REFLECT_API3(Name, ResultType, vType1, x1, vType2, x2, vType3, x3) \
      typedef ResultType (__stdcall *Name##Proc)(vType1 x1, vType2 x2, vType3 x3); \
      static Name##Proc __##Name; \
      static ResultType WINAPI _##Name(vType1 x1, vType2 x2, vType3 x3)  \
      {                                                           \
         ResultType hVar##ResultType = __##Name(x1, x2, x3);      \
                                                                  \
         TraceReflectCall(#Name, (DWORD)hVar##ResultType);        \
                                                                  \
         _STORE_GDI_OBJECT(hVar##ResultType)                      \
                                                                  \
         return(hVar##ResultType);                                \
      }

   #define DECLARE_REFLECT_API4(Name, ResultType, vType1, x1, vType2, x2, vType3, x3, vType4, x4) \
      typedef ResultType (__stdcall *Name##Proc)(vType1 x1, vType2 x2, vType3 x3, vType4 x4); \
      static Name##Proc __##Name; \
      static ResultType WINAPI _##Name(vType1 x1, vType2 x2, vType3 x3, vType4 x4)  \
      {                                                           \
         ResultType hVar##ResultType = __##Name(x1, x2, x3, x4);  \
                                                                  \
         TraceReflectCall(#Name, (DWORD)hVar##ResultType);        \
                                                                  \
         _STORE_GDI_OBJECT(hVar##ResultType)                      \
                                                                  \
         return(hVar##ResultType);                                \
      }

   #define DECLARE_REFLECT_API5(Name, ResultType, vType1, x1, vType2, x2, vType3, x3, vType4, x4, vType5, x5) \
      typedef ResultType (__stdcall *Name##Proc)(vType1 x1, vType2 x2, vType3 x3, vType4 x4, vType5 x5); \
      static Name##Proc __##Name; \
      static ResultType WINAPI _##Name(vType1 x1, vType2 x2, vType3 x3, vType4 x4, vType5 x5)  \
      {                                                           \
         ResultType hVar##ResultType = __##Name(x1, x2, x3, x4, x5); \
                                                                  \
         TraceReflectCall(#Name, (DWORD)hVar##ResultType);        \
                                                                  \
         _STORE_GDI_OBJECT(hVar##ResultType)                      \
                                                                  \
         return(hVar##ResultType);                                \
      }

   #define DECLARE_REFLECT_API6(Name, ResultType, vType1, x1, vType2, x2, vType3, x3, vType4, x4, vType5, x5, vType6, x6) \
      typedef ResultType (__stdcall *Name##Proc)(vType1 x1, vType2 x2, vType3 x3, vType4 x4, vType5 x5, vType6 x6); \
      static Name##Proc __##Name; \
      static ResultType WINAPI _##Name(vType1 x1, vType2 x2, vType3 x3, vType4 x4, vType5 x5, vType6 x6)  \
      {                                                           \
         ResultType hVar##ResultType = __##Name(x1, x2, x3, x4, x5, x6); \
                                                                  \
         TraceReflectCall(#Name, (DWORD)hVar##ResultType);        \
                                                                  \
         _STORE_GDI_OBJECT(hVar##ResultType)                      \
                                                                  \
         return(hVar##ResultType);                                \
      }

   #define DECLARE_REFLECT_API14(Name, ResultType, vType1, x1, vType2, x2, vType3, x3, vType4, x4, vType5, x5, vType6, x6, \
                                                vType7, x7, vType8, x8, vType9, x9, vType10, x10, vType11, x11, vType12, x12, \
                                                vType13, x13, vType14, x14) \
      typedef ResultType (__stdcall *Name##Proc)( \
         vType1 x1, vType2 x2, vType3 x3, vType4 x4, vType5 x5, vType6 x6, \
         vType7 x7, vType8 x8, vType9 x9, vType10 x10, vType11 x11, vType12 x12, \
         vType13 x13, vType14 x14 \
         ); \
      static Name##Proc __##Name; \
      static ResultType WINAPI _##Name( \
         vType1 x1, vType2 x2, vType3 x3, vType4 x4, vType5 x5, vType6 x6, \
         vType7 x7, vType8 x8, vType9 x9, vType10 x10, vType11 x11, vType12 x12, \
         vType13 x13, vType14 x14 \
         )  \
      {                                                           \
         ResultType hVar##ResultType = __##Name(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14); \
                                                                  \
         TraceReflectCall(#Name, (DWORD)hVar##ResultType);        \
                                                                  \
         _STORE_GDI_OBJECT(hVar##ResultType)                      \
                                                                  \
         return(hVar##ResultType);                                \
      }
#define END_REFLECT_LIST()



class CGDIReflect : public CAPIReflect
{
// construction/destruction
//
public:
   CGDIReflect();
   virtual ~CGDIReflect();


// C++ interface
//
public:
   POSITION GetFirstItem(HANDLE& hObject, CHandleInfo*& pInfo);
   POSITION GetNextItem(POSITION iCurrent, HANDLE& hObject, CHandleInfo*& pInfo);
   CHandleInfo* GetHandleInfo(HANDLE hObject);
   DWORD DumpLeaks();

public:
   static BOOL IsValidGDIObject(HANDLE hObject, DWORD& dwType);
   static CStackManager* GetStackManager();


// overridables 
// 
public:
   virtual BOOL Init();
   virtual BOOL DoReflect(HMODULE hModule);
   virtual BOOL FillStubs(DWORD dwState, HMODULE hModule);


// reflectors definition
//
// not spyed:
//    GetStockObject()
//    BeginPaint()
//
// one good idea would have been to reflect SelectObject()
// 
public:
   BEGIN_REFLECT_LIST()
   // bitmap API
      DECLARE_REFLECT_API2(LoadBitmapA, HBITMAP, HINSTANCE, hInstance, LPSTR, lpBitmapName)
      DECLARE_REFLECT_API2(LoadBitmapW, HBITMAP, HINSTANCE, hInstance, LPWSTR, lpBitmapName)
      DECLARE_REFLECT_API5(CreateBitmap, HBITMAP, int, nWidth, int, nHeight, UINT, cPlanes, UINT, cBitsPerPel, CONST VOID*, lpvBits)
      DECLARE_REFLECT_API1(CreateBitmapIndirect, HBITMAP, CONST BITMAP*, lpbm)
      DECLARE_REFLECT_API3(CreateCompatibleBitmap, HBITMAP, HDC, hdc, int, nWidth, int, nHeight)

   // brush API
      DECLARE_REFLECT_API1(CreateBrushIndirect, HBRUSH, CONST LOGBRUSH*, lplb)
      DECLARE_REFLECT_API1(CreateSolidBrush, HBRUSH, COLORREF, crColor)
      DECLARE_REFLECT_API1(CreatePatternBrush, HBRUSH, HBITMAP, hbmp)
      DECLARE_REFLECT_API2(CreateDIBPatternBrush, HBRUSH, HGLOBAL, hglbDIBPacked, UINT, fuColorSpec)
      DECLARE_REFLECT_API2(CreateDIBPatternBrushPt, HBRUSH, CONST VOID*, lpPackedDIB, UINT, iUsage)
      DECLARE_REFLECT_API2(CreateHatchBrush, HBRUSH, int, fnStyle, COLORREF, clrref)

   // device context
      DECLARE_REFLECT_API1(CreateCompatibleDC, HDC, HDC, hdc)
      DECLARE_REFLECT_API4(CreateDCA, HDC, LPCSTR, lpszDriver, LPCSTR, lpszDevice, LPCSTR, lpszOutput, CONST DEVMODE*, lpInitData)
      DECLARE_REFLECT_API4(CreateDCW, HDC, LPCWSTR, lpszDriver, LPCWSTR, lpszDevice, LPCWSTR, lpszOutput, CONST DEVMODE*, lpInitData)
      DECLARE_REFLECT_API4(CreateICA, HDC, LPCSTR, lpszDriver, LPCSTR, lpszDevice, LPCSTR, lpszOutput, CONST DEVMODE*, lpdvmInit)
      DECLARE_REFLECT_API4(CreateICW, HDC, LPCWSTR, lpszDriver, LPCWSTR, lpszDevice, LPCWSTR, lpszOutput, CONST DEVMODE*, lpdvmInit)
      DECLARE_REFLECT_API1(GetDC, HDC, HWND, hWnd)
      DECLARE_REFLECT_API3(GetDCEx, HDC, HWND, hWnd, HRGN, hrgnClip, DWORD, flag)
      DECLARE_REFLECT_API1(GetWindowDC, HDC, HWND, hWnd)

   // font
      DECLARE_REFLECT_API14(CreateFontA, HFONT, 
         int, nHeight, int, nWidth, int, nEscapement, int, nOrientation, int, fnWeight, DWORD, fdwItalic, DWORD, fdwUnderline,
         DWORD, fdwStrikeOut, DWORD, fdwCharSet, DWORD, fdwOutputPrecision, DWORD, fdwClipPrecision, DWORD, fdwQuality, 
         DWORD, fdwPitchAndFamily, LPCSTR, lpszFace)
      DECLARE_REFLECT_API14(CreateFontW, HFONT, 
         int, nHeight, int, nWidth, int, nEscapement, int, nOrientation, int, fnWeight, DWORD, fdwItalic, DWORD, fdwUnderline,
         DWORD, fdwStrikeOut, DWORD, fdwCharSet, DWORD, fdwOutputPrecision, DWORD, fdwClipPrecision, DWORD, fdwQuality, 
         DWORD, fdwPitchAndFamily, LPCWSTR, lpszFace)
      DECLARE_REFLECT_API1(CreateFontIndirectA, HFONT, CONST LOGFONTA*, lplf)
      DECLARE_REFLECT_API1(CreateFontIndirectW, HFONT, CONST LOGFONTW*, lplf)

   // metafile
      DECLARE_REFLECT_API1(CreateMetaFileA, HDC, LPCSTR, lpszFile)
      DECLARE_REFLECT_API1(CreateMetaFileW, HDC, LPCWSTR, lpszFile)
      DECLARE_REFLECT_API4(CreateEnhMetaFileA, HDC, HDC, hdcRef, LPCSTR, lpFilename, CONST RECT*, lpRect, LPCSTR, lpDescription)
      DECLARE_REFLECT_API4(CreateEnhMetaFileW, HDC, HDC, hdcRef, LPCWSTR, lpFilename, CONST RECT*, lpRect, LPCWSTR, lpDescription)
      DECLARE_REFLECT_API1(GetEnhMetaFileA, HENHMETAFILE, LPCSTR, lpszMetaFile)
      DECLARE_REFLECT_API1(GetEnhMetaFileW, HENHMETAFILE, LPCWSTR, lpszMetaFile)
      DECLARE_REFLECT_API1(GetMetaFileA, HMETAFILE, LPCSTR, lpszMetaFile)
      DECLARE_REFLECT_API1(GetMetaFileW, HMETAFILE, LPCWSTR, lpszMetaFile)

   // pen
      DECLARE_REFLECT_API3(CreatePen, HPEN, int, fnPenStyle, int, nWidth, COLORREF, crColor)
      DECLARE_REFLECT_API1(CreatePenIndirect, HPEN, CONST LOGPEN*, lplgpn)
      DECLARE_REFLECT_API5(ExtCreatePen, HPEN, DWORD, dwPenStyle, DWORD, dwWidth, CONST LOGBRUSH*, lplb, DWORD, dwStyleCount, CONST DWORD*, lpStyle)

   // region
      DECLARE_REFLECT_API1(PathToRegion, HRGN, HDC, hdc)
      DECLARE_REFLECT_API4(CreateEllipticRgn, HRGN, int, nLeftRect, int, nTopRect, int, nRightRect, int, nBottomRect)
      DECLARE_REFLECT_API1(CreateEllipticRgnIndirect, HRGN, CONST RECT*, lprc)
      DECLARE_REFLECT_API3(CreatePolygonRgn, HRGN, CONST POINT*, lppt, int, cPoints, int, fnPolyFillMode)
      DECLARE_REFLECT_API4(CreatePolyPolygonRgn, HRGN, CONST POINT*, lppt, CONST INT*, lpPolyCounts, int, nCount, int, fnPolyFillMode)
      DECLARE_REFLECT_API4(CreateRectRgn, HRGN, int, nLeftRect, int, nTopRect, int, nRightRect, int, nBottomRect)
      DECLARE_REFLECT_API1(CreateRectRgnIndirect, HRGN, CONST RECT*, lprc)
      DECLARE_REFLECT_API6(CreateRoundRectRgn, HRGN, int, nLeftRect, int, nTopRect, int, nRightRect, int, nBottomRect, int, nWidthEllipse, int, nHeightEllipse)
      DECLARE_REFLECT_API3(ExtCreateRegion, HRGN, CONST XFORM*, lpXform, DWORD, nCount, CONST RGNDATA*, lpRgnData)
   
   // palette
      DECLARE_REFLECT_API1(CreateHalftonePalette, HPALETTE, HDC, hdc)
      DECLARE_REFLECT_API1(CreatePalette, HPALETTE, CONST LOGPALETTE*, lplgpl)

   // object deletion
      DECLARE_REFLECT_DELETE(DeleteObject, HGDIOBJ, hObject)
      DECLARE_REFLECT_DELETE(DeleteDC, HDC, hDC)
      DECLARE_REFLECT_DELETE(DeleteMetaFile, HMETAFILE, hmf)
      DECLARE_REFLECT_DELETE(DeleteEnhMetaFile, HENHMETAFILE, hEmf)
      DECLARE_REFLECT_DELETE(CloseMetaFile, HMETAFILE, hmf)
      DECLARE_REFLECT_DELETE(CloseEnhMetaFile, HENHMETAFILE, hEmf)

   END_REFLECT_LIST()



// internal helpers
// 
private:
   void DefaultReset();   


// implementation details
// 
private:
   static CTypedPtrMap<CMapPtrToPtr, HANDLE, CHandleInfo*> m_HandleMap;
   static CStackManager s_StackManager;
};



#endif // !defined(AFX_GDIREFLECT_H__E7B39634_8705_11D2_B112_000000000000__INCLUDED_)

