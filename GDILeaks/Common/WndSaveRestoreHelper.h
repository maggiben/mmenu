#if !defined(AFX_WNDSAVERESTOREHELPER_H__EB2534C3_D68F_1241_A85E_444553540000__INCLUDED_)
#define AFX_WNDSAVERESTOREHELPER_H__EB2534C3_D68F_1241_A85E_444553540000__INCLUDED_


#if _MSC_VER >= 1000
   #pragma once
#endif // _MSC_VER >= 1000



class CWndSaveRestoreHelper : public CObject  
{
// Construction/Destruction
//
public:
   CWndSaveRestoreHelper(HWND hWnd, LPCTSTR szSectionName, LPCTSTR szEntry);
   CWndSaveRestoreHelper();
   virtual ~CWndSaveRestoreHelper();


// C++ Interface
//
public:
   BOOL AttachToWindow(HWND hWnd);
   BOOL Load();
   BOOL Save();
   void GetSectionName(LPCTSTR szSection, LPCTSTR szEntry);


// Implementation details
//
private:
   CString  m_szSection;
   CString  m_szEntry;
   HWND     m_hAttachedWindow;
};

#endif // !defined(AFX_WNDSAVERESTOREHELPER_H__EB2534C3_D68F_1241_A85E_444553540000__INCLUDED_)
