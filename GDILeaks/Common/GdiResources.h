#if !defined(AFX_GDIRESOURCES_H__E54EE064_35A4_11D2_837A_00A0C9A528B7__INCLUDED_)
#define AFX_GDIRESOURCES_H__E54EE064_35A4_11D2_837A_00A0C9A528B7__INCLUDED_

#if _MSC_VER >= 1000
   #pragma once
#endif // _MSC_VER >= 1000


#include <AfxTempl.h>



// Helpers
// 
LPCTSTR GetGDIType(DWORD dwType);
BOOL IsStockObject(DWORD hObject);
BOOL ObjectIsDeleted(DWORD hObject);


// types for inter process marshalling of a GDI objects list
// 
const DWORD MAX_GDI_ITEMS = 0x4000; // GDI objects max count

typedef struct 
{
   DWORD    dwType;
   HGDIOBJ  hObject; 
} GDI_ITEM;


// warning C4200: nonstandard extension used : zero-sized array in struct/union
#pragma warning( disable : 4200 )
typedef struct 
{
   DWORD    dwCount; // count of meaningful GDI_ITEM slots in Items
   GDI_ITEM Items[]; 
} GDI_LIST;
#pragma warning( default : 4200 )



class CGdiObj 
{
public:
// construction/destruction
//
   CGdiObj();


// C++ interface
// 
public:
   DWORD GetHandle();
   DWORD GetType();
   void GetTypeString(CString& szText);


// implementation helpers
// 
private:
   void SetHandle(DWORD Handle);  
   void SetType(DWORD Type); 


// implementation details
// 
private:
   DWORD m_Handle;
   DWORD m_Type;

public:
static LPCTSTR s_TypeStrings[];

friend class CGdiResources;
friend class CResourcesCompare;
};


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


class CGdiResources 
{
// construction/destruction
// 
public:
   CGdiResources();
   virtual ~CGdiResources();


// C++ interface
// 
public:
   DWORD CreateFromList(GDI_LIST* pList);
   DWORD GetGDIList(DWORD ProcessID);
   DWORD NbBitmap()     { return m_CountByType[OBJ_BITMAP-1];        }
   DWORD NbBrush()      { return m_CountByType[OBJ_BRUSH-1];         }
   DWORD NbPen()        { return m_CountByType[OBJ_PEN-1];           }
   DWORD NbFont()       { return m_CountByType[OBJ_FONT-1];          }
   DWORD NbRegion()     { return m_CountByType[OBJ_REGION-1];        }
   DWORD NbDC()         { return m_CountByType[OBJ_DC-1];            }
   DWORD NbMemDC()      { return m_CountByType[OBJ_MEMDC-1];         }
   DWORD NbPalette()    { return m_CountByType[OBJ_PAL-1];           }
   DWORD NbMetaFile()   { return m_CountByType[OBJ_METAFILE-1];      }
   DWORD NbExtPen()     { return m_CountByType[OBJ_EXTPEN-1];        }
   DWORD NbMetaDC()     { return m_CountByType[OBJ_METADC-1];        }
   DWORD NbEnhMetaDC()  { return m_CountByType[OBJ_ENHMETADC-1];     }
   DWORD NbEnhMetafile(){ return m_CountByType[OBJ_ENHMETAFILE-1];   }
   DWORD NbColorSpace() { return m_CountByType[OBJ_COLORSPACE-1];    }

   DWORD NbOther()      { return m_CountByType[NB_GDI_TYPE];         }

   CGdiObj* GetFirst(POSITION& pos);
   CGdiObj* GetNext(POSITION& pos);
   CGdiObj* GetObjectFromMap(DWORD Handle);
   DWORD GetTotalCount();


// implementation helpers
// 
protected:
   void DefaultInit();

private:
   void Cleanup();
   void AddObject(CGdiObj* pObject);


// implementation details
// 
protected:
// store each GDI object description 
// indexed by its handle (Ptr = 32 bits --> CGdiObj*) 
   CTypedPtrMap<CMapPtrToPtr, DWORD, CGdiObj*> m_HandleMap;

// count by GDI object type        V-- plus the unknown's
   DWORD m_CountByType[NB_GDI_TYPE+1];   
};



class CResourcesCompare : public CGdiResources
{
// construction/destruction
// 
public:
   CResourcesCompare();


// C++ interface
// 
public:
   void ComputeCompare(CGdiResources& Current, CGdiResources& Reference);
   DWORD GetSameFromType(int iType);
   DWORD GetOutFromType(int iType);
   DWORD GetNewFromType(int iType);


// implementation helpers
// 
private:
   void Reset();


// implementation details
// 
private:
   DWORD m_OutCountByType[NB_GDI_TYPE+1];
   DWORD m_NewCountByType[NB_GDI_TYPE+1];
};





#endif // !defined(AFX_GDIRESOURCES_H__E54EE064_35A4_11D2_837A_00A0C9A528B7__INCLUDED_)
