#if !defined(AFX_FONTHELPER_H__CF1319A3_C0AB_11D1_B080_0020AF7CB378__INCLUDED_)
#define AFX_FONTHELPER_H__CF1319A3_C0AB_11D1_B080_0020AF7CB378__INCLUDED_

#if _MSC_VER >= 1000
   #pragma once
#endif // _MSC_VER >= 1000



class CFontHelper : public CObject  
{
// Construction/Destruction
//
public:
   CFontHelper();
   CFontHelper(LPCTSTR szSection, LPCTSTR szEntry);
   virtual ~CFontHelper();


// C++ interface
//
public:
// data access 
   HFONT CreateFont();
   void  GetLogFont(LOGFONT& LogFont);
   HFONT GetFontHandle();
   
// save/restore
   void Save();
   void Load();
   void SetSectionName(LPCTSTR szSection);
   void SetEntryName(LPCTSTR szEntry);

// Windows common font picker dialog
   BOOL ChooseFont();


// Internal helpers
//
private:
   // default font 
   void DefaultFontInit();
   void DefaultInit();


// Implementation details
//
private:
   HFONT    m_hFont;
   LOGFONT  m_LogFont;
   CString  m_szSection;
   CString  m_szEntry;
};


#endif // !defined(AFX_FONTHELPER_H__CF1319A3_C0AB_11D1_B080_0020AF7CB378__INCLUDED_)
