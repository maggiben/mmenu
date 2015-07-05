#include "stdafx.h"
#include "WndSaveRestoreHelper.h"

#ifdef _DEBUG
   #undef THIS_FILE
   static char THIS_FILE[]=__FILE__;
   #define new DEBUG_NEW
#endif


//--------------------------------------------------------------------
// Construction/Destruction
//

CWndSaveRestoreHelper::CWndSaveRestoreHelper()
{
   m_szSection = _T("");
   m_szEntry = _T("");
   m_hAttachedWindow = NULL;
}

CWndSaveRestoreHelper::CWndSaveRestoreHelper(HWND hWnd, LPCTSTR szSection, LPCTSTR szEntry)
{
   ASSERT((szSection != NULL) && (_tcslen(szSection) > 0));
   m_szSection = szSection;

   ASSERT((szEntry != NULL) && (_tcslen(szEntry) > 0));
   m_szEntry = szEntry;

   ASSERT(::IsWindow(hWnd));
   m_hAttachedWindow = hWnd;
}


CWndSaveRestoreHelper::~CWndSaveRestoreHelper()
{
// nothing to delete
}


// store the given window handle
BOOL CWndSaveRestoreHelper::AttachToWindow(HWND hWnd)
{
   if (::IsWindow(hWnd))
   {
      m_hAttachedWindow = hWnd;
      return(TRUE);
   }
   else
   {
      return(FALSE);
   }
}
//
//--------------------------------------------------------------------


//--------------------------------------------------------------------
// Save/Restore public interface
//

// Load the window position from the INI file/Registry
BOOL CWndSaveRestoreHelper::Load()
{
// SetSectionName() must be called before this method is used
// SetEntryName() must be called before this method is used
   ASSERT(m_szSection.GetLength() > 0);
   ASSERT(m_szEntry.GetLength() > 0);

// AttachToWindow() must be called before this method is used
   if (::IsWindow(m_hAttachedWindow))
   {
   // restore window placement
      WINDOWPLACEMENT wp;
      wp.length = sizeof(WINDOWPLACEMENT);

      CString strBuffer = AfxGetApp()->GetProfileString(m_szSection, m_szEntry);

   // nothing already saved 
      if (strBuffer.IsEmpty())
         return(FALSE);

      int cRead = 
         _stscanf (
            strBuffer, 
            "%u:%u:%d:%d:%d:%d:%d:%d:%d:%d",
            &wp.flags, 
            &wp.showCmd,
            &wp.ptMinPosition.x, 
            &wp.ptMinPosition.y,
            &wp.ptMaxPosition.x, 
            &wp.ptMaxPosition.y,
            &wp.rcNormalPosition.left, 
            &wp.rcNormalPosition.top,
            &wp.rcNormalPosition.right, 
            &wp.rcNormalPosition.bottom
            );

      if (cRead != 10)
      {
         TRACE("LoadWindowPlacement(): Invalid Registry/INI file entry\n");
         return(FALSE);
      }
      
   // take into account the saved position
      ::SetWindowPlacement(m_hAttachedWindow, &wp);
      ::ShowWindow(m_hAttachedWindow, wp.showCmd);

      return(TRUE);
   }
   else
   {
      TRACE("LoadWindowPlacement(): AttachToWindow() must be called before\n");

      return(FALSE);
   }
}


// Save the window position into the INI file/Registry
BOOL CWndSaveRestoreHelper::Save()
{
// save the current window placement
   WINDOWPLACEMENT wp;

// AttachToWindow() must be called before this methods is used
   if (IsWindow(m_hAttachedWindow))
   {
      wp.length = sizeof(WINDOWPLACEMENT);
      if (::GetWindowPlacement(m_hAttachedWindow, &wp))
      {
         wp.flags = 0;
         if (::IsZoomed(m_hAttachedWindow))
            wp.flags |= WPF_RESTORETOMAXIMIZED;

      // save the window placement
         CString strBuffer;
         strBuffer.Format(
            "%u:%u:%d:%d:%d:%d:%d:%d:%d:%d",
            wp.flags, wp.showCmd,
            wp.ptMinPosition.x, 
            wp.ptMinPosition.y,
            wp.ptMaxPosition.x, 
            wp.ptMaxPosition.y,
            wp.rcNormalPosition.left, 
            wp.rcNormalPosition.top,
            wp.rcNormalPosition.right, 
            wp.rcNormalPosition.bottom
            );

         AfxGetApp()->WriteProfileString (m_szSection, m_szEntry, strBuffer);
         return(TRUE);
      }
      else
      // should never occur...
         TRACE("SaveWindowPlacement(): GetWindowPlacement() has failed\n");
   }
   else
   {
      TRACE("SaveWindowPlacement(): AttachToWindow() must be called before\n");
   }
   
   return(FALSE);
}


// This method is called to set the name of the section where the position is 
// saved in the INI file/Registry
//
// Note: 
//    this method MUST be called before the Load/Save methods are used
void CWndSaveRestoreHelper::GetSectionName(LPCTSTR szSection, LPCTSTR szEntry)
{
// defensive programming
   ASSERT((szSection != NULL) && (_tcslen(szSection) > 0));
   ASSERT((szEntry != NULL) && (_tcslen(szEntry) > 0));

   m_szSection = szSection;
   m_szEntry = szEntry;
}
//--------------------------------------------------------------------

