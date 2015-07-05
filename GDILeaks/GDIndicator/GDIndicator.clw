; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CGDIndicatorView
LastTemplate=CFrameWnd
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "GDIndicator.h"
LastPage=0

ClassCount=7
Class1=CGDIndicatorApp
Class2=CGDIndicatorDoc
Class3=CGDIndicatorView
Class4=CMainFrame

ResourceCount=4
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Class5=CAboutDlg
Resource3=IDR_MAINFRAME (English (U.S.))
Class6=CBaseApp
Class7=CBaseFrame
Resource4=IDD_ABOUTBOX (English (U.S.))

[CLS:CGDIndicatorApp]
Type=0
HeaderFile=GDIndicator.h
ImplementationFile=GDIndicator.cpp
Filter=N

[CLS:CGDIndicatorDoc]
Type=0
HeaderFile=GDIndicatorDoc.h
ImplementationFile=GDIndicatorDoc.cpp
Filter=N
LastObject=CGDIndicatorDoc

[CLS:CGDIndicatorView]
Type=0
HeaderFile=GDIndicatorView.h
ImplementationFile=GDIndicatorView.cpp
Filter=C
LastObject=CGDIndicatorView
BaseClass=CSortListView
VirtualFilter=VWC


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T




[CLS:CAboutDlg]
Type=0
HeaderFile=GDIndicator.cpp
ImplementationFile=GDIndicator.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Class=CAboutDlg

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command3=ID_FILE_SAVE
Command4=ID_FILE_SAVE_AS
Command5=ID_FILE_MRU_FILE1
Command6=ID_APP_EXIT
Command10=ID_EDIT_PASTE
Command11=ID_VIEW_TOOLBAR
Command12=ID_VIEW_STATUS_BAR
Command13=ID_APP_ABOUT
CommandCount=13
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command7=ID_EDIT_UNDO
Command8=ID_EDIT_CUT
Command9=ID_EDIT_COPY

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command5=ID_EDIT_CUT
Command6=ID_EDIT_COPY
Command7=ID_EDIT_PASTE
Command8=ID_EDIT_UNDO
Command9=ID_EDIT_CUT
Command10=ID_EDIT_COPY
Command11=ID_EDIT_PASTE
Command12=ID_NEXT_PANE
CommandCount=13
Command4=ID_EDIT_UNDO
Command13=ID_PREV_PANE


[TB:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_EDIT_COPY
Command3=ID_APP_ABOUT
CommandCount=3

[MNU:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_APP_EXIT
Command3=ID_EDIT_COPY
Command4=ID_VIEW_TOOLBAR
Command5=ID_VIEW_STATUS_BAR
Command6=ID_APP_ABOUT
CommandCount=6

[ACL:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=ID_EDIT_COPY
Command2=ID_FILE_NEW
Command3=ID_NEXT_PANE
Command4=ID_PREV_PANE
Command5=ID_EDIT_COPY
CommandCount=5

[DLG:IDD_ABOUTBOX (English (U.S.))]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342177298

[CLS:CBaseApp]
Type=0
HeaderFile=..\common\BaseClass.h
ImplementationFile=..\common\BaseClass.cpp
BaseClass=CWinThread
Filter=N

[CLS:CBaseFrame]
Type=0
HeaderFile=..\Common\BaseClass.h
ImplementationFile=..\Common\BaseClass.cpp
BaseClass=CFrameWnd
Filter=T
LastObject=CBaseFrame

