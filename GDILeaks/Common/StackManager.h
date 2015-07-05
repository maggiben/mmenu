#if !defined(AFX_STACKMANAGER_H__FADFF864_49D9_11D4_927B_00104B4AF156__INCLUDED_)
#define AFX_STACKMANAGER_H__FADFF864_49D9_11D4_927B_00104B4AF156__INCLUDED_

#if _MSC_VER > 1000
   #pragma once
#endif // _MSC_VER > 1000


//--------------------------------------------------------------------------
//
class CStackManager  
{
// construction/destruction
//
public:
   CStackManager();
   virtual ~CStackManager();


// C++ interface
// 
public:
   BOOL Init();
   BOOL Cleanup();
   DWORD* DoStackAddressDump(DWORD& Depth, DWORD StartFrom);
   LPCTSTR ConvertStackAddressIntoFunctionName(LPVOID pAddress);
   void DumpStackAllocation(DWORD Depth, DWORD* pStack, LPCTSTR szLineSeparator, CString& szInfo);
   

// debugging helpers
// 
public:
   int GetMapCount();


// implementation details
// 
private:
   CMapPtrToPtr m_AddressToName;
};
//
//--------------------------------------------------------------------------



//--------------------------------------------------------------------------
// check that the needed declarations and resources are included into your project
// 
#ifndef IDC_ITEM_LIST
   #pragma message("You have to add IDC_EDIT_CALL_STACK and IDD_STACK into your project resource.h")
//
// definition of needed resource identifiers to add in the resource.h
// 
   #define IDC_EDIT_CALL_STACK             1000
   #define IDD_STACK                       1001
#endif


#ifndef IDD_GRAPHIC_DISPLAY
   #pragma message("You have to add the IDD_STACK dialog description into your resources")
//
// you should add the following dialog box description into your .RC file
//
/*
IDD_STACK DIALOG DISCARDABLE  0, 0, 346, 207
STYLE DS_MODALFRAME | WS_POPUP | WS_CAPTION | WS_SYSMENU
CAPTION "Show Calls Stack"
FONT 8, "MS Sans Serif"
BEGIN
    EDITTEXT        IDC_EDIT_CALL_STACK,3,4,339,176,ES_AUTOHSCROLL
    PUSHBUTTON      "&Exit",IDOK,292,187,50,14
END
*/
#endif
//
//--------------------------------------------------------------------------


class CCallStackDlg : public CDialog
{
// Construction
public:
   CCallStackDlg(CWnd* pParent = NULL);   // standard constructor


// C++ Interface
// 
public:
   void SetStackManager(CStackManager* pStackManager);
   void SetCallStack(DWORD Depth, DWORD* pCallStack);


// Implementation details
// 
private:
   CStackManager* m_pStackManager;
   CString        m_szCallStack;

// MFC stuff
// 
public:
// Dialog Data
//{{AFX_DATA(CCallStackDlg)
   enum { IDD = IDD_STACK };
   // NOTE: the ClassWizard will add data members here
//}}AFX_DATA


// Overrides
// ClassWizard generated virtual function overrides
//{{AFX_VIRTUAL(CCallStackDlg)
protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
//}}AFX_VIRTUAL

// Implementation
protected:

// Generated message map functions
//{{AFX_MSG(CCallStackDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG

   DECLARE_MESSAGE_MAP()
};


#endif // !defined(AFX_STACKMANAGER_H__FADFF864_49D9_11D4_927B_00104B4AF156__INCLUDED_)


