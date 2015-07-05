; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CCallStackDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "gditrace.h"
LastPage=0

ClassCount=5
Class1=CBrowseFileEdit
Class2=CGdiResourcesDlg
Class3=CDisplayDlg
Class4=CGDITraceApp

ResourceCount=4
Resource1=IDD_GRAPHIC_DISPLAY (English (U.S.))
Class5=CCallStackDlg
Resource2=IDD_GRAPHIC_DISPLAY
Resource3=IDD_STACK (English (U.S.))
Resource4=IDD_STACK

[CLS:CBrowseFileEdit]
Type=0
BaseClass=CEdit
HeaderFile=\internal\Articles\GDIDebug\SourceCode\Common\BrowseFileEdit.h
ImplementationFile=\internal\Articles\GDIDebug\SourceCode\Common\BrowseFileEdit.cpp
LastObject=CBrowseFileEdit

[CLS:CGdiResourcesDlg]
Type=0
BaseClass=CDialog
HeaderFile=\internal\Articles\GDIDebug\SourceCode\Common\GdiResourcesDlg.h
ImplementationFile=\internal\Articles\GDIDebug\SourceCode\Common\GdiResourcesDlg.cpp
LastObject=CGdiResourcesDlg
Filter=D
VirtualFilter=dWC

[CLS:CDisplayDlg]
Type=0
BaseClass=CDialog
HeaderFile=DisplayDlg.h
ImplementationFile=DisplayDlg.cpp

[CLS:CGDITraceApp]
Type=0
BaseClass=CWinApp
HeaderFile=GDITrace.h
ImplementationFile=GDITrace.cpp

[DLG:IDD_GRAPHIC_DISPLAY]
Type=1
Class=CGdiResourcesDlg
ControlCount=1
Control1=IDC_ITEM_LIST,listbox,1352728963

[DLG:IDD_GRAPHIC_DISPLAY (English (U.S.))]
Type=1
Class=?
ControlCount=1
Control1=IDC_ITEM_LIST,listbox,1352728963

[DLG:IDD_STACK]
Type=1
Class=CCallStackDlg
ControlCount=1
Control1=IDC_EDIT_CALL_STACK,edit,1353912452

[CLS:CCallStackDlg]
Type=0
HeaderFile=..\Common\StackManager.h
ImplementationFile=..\Common\StackManager.cpp
BaseClass=CDialog
Filter=D
LastObject=CCallStackDlg
VirtualFilter=dWC

[DLG:IDD_STACK (English (U.S.))]
Type=1
Class=?
ControlCount=2
Control1=IDC_EDIT_CALL_STACK,edit,1350631552
Control2=IDOK,button,1342242816

