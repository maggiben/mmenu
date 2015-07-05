/*-------------------------------------------------------------
  main2.c : TClockExeMain, and initializing functions
  (C) 1997-2003 Kazuto Sato
  Please read readme.txt about the license.
  
  Written by Kazubon, Nanashi-san
---------------------------------------------------------------*/

#include "tclock.h"

/* Globals */

int TClockExeMain(void);

char g_mydir[MAX_PATH];     // path to tclock.exe
BOOL g_bIniSetting = FALSE; // save setting to ini file?
char g_inifile[MAX_PATH];   // ini file name
int  g_winver;              // windows version
UINT g_uTaskbarRestart;     // taskbar recreating message

/* Statics */

static void InitTClockMain(void);
static BOOL CheckTCDLL(void);
static void InitTextColor(void);
static void InitFormat(void);

/*-------------------------------------------
   main routine
---------------------------------------------*/
int TClockExeMain(void)
{
	MSG msg;
	WNDCLASS wndclass;
	HWND hwnd;
	
	// not to execute the program twice
	hwnd = GetTClockMainWindow();
	if(hwnd != NULL)
	{
		CheckCommandLine(hwnd, TRUE);
		return 1;
	}
	
	if(!CheckTCDLL()) { return 1; }
	
	if(FindWindow("ObjectBar Toolbar", NULL))
	{
		MessageBox(NULL, "ObjectBar is running",
			"Error", MB_OK|MB_ICONEXCLAMATION);
		return 1;
	}
	
	InitTClockMain();
	
	// register a window class
	wndclass.style         = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wndclass.lpfnWndProc   = WndProc;
	wndclass.cbClsExtra    = 0;
	wndclass.cbWndExtra    = 0;
	wndclass.hInstance     = g_hInst;
	wndclass.hIcon         = LoadIcon(g_hInst, "MYICON");
	wndclass.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wndclass.lpszMenuName  = NULL;
	wndclass.lpszClassName = CLASS_TCLOCKMAIN;
	RegisterClass(&wndclass);
	
	// create a hidden window
	hwnd = CreateWindowEx(0,
		CLASS_TCLOCKMAIN, TITLE_TCLOCKMAIN, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,
		NULL, NULL, g_hInst, NULL);
	// ShowWindow(hwnd, SW_SHOW);
	// UpdateWindow(hwnd);
	
	CheckCommandLine(hwnd, FALSE);
	
	while(GetMessage(&msg, NULL, 0, 0))
	{
		if(g_hDlgAbout && IsDialogMessage(g_hDlgAbout, &msg)) ;
		else
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	
	UnregisterClass(CLASS_TCLOCKMAIN, g_hInst);
	
	return msg.wParam;
}

/*-------------------------------------------
  initialize variables, format, etc.
---------------------------------------------*/
void InitTClockMain(void)
{
	// get the path where .exe is positioned
	GetModuleFileName(g_hInst, g_mydir, MAX_PATH);
	del_title(g_mydir);
	
	g_bIniSetting = FALSE;
	SetMyRegStr(NULL, "ExePath", g_mydir);
	
	// ini file name
	strcpy(g_inifile, g_mydir);
	add_title(g_inifile, "tclock.ini");
	g_bIniSetting = TRUE;
/*  g_bIniSetting = IsFile(g_inifile); */
	
	g_winver = CheckWinVersion();
	
	// Message of the taskbar recreating
	// Special thanks to Mr.Inuya
	g_uTaskbarRestart = RegisterWindowMessage("TaskbarCreated");
	
	DelMyRegKey("OnContextMenu"); // temporarily
	
	InitTextColor();
	
	InitFormat();
}

/*-------------------------------------------
  Check version of dll
---------------------------------------------*/
BOOL CheckTCDLL(void)
{
	char str[80];
	GetTClockVersion(str);
	if(strcmp(str, TCLOCKVERSION) != 0)
	{
		MessageBox(NULL, "Invalid file version: tcdll.tclock",
			"Error", MB_OK|MB_ICONEXCLAMATION);
		return FALSE;
	}
	return TRUE;
}

/*------------------------------------------------
  for Luna theme of Windows XP
--------------------------------------------------*/
void InitTextColor(void)
{
	char s[80];
	char themekey[] =
		"Software\\Microsoft\\Windows\\CurrentVersion\\ThemeManager";
	
	if(GetMyRegStr(NULL, "ForeColor", s, 20, "") > 0) return;
	
	GetRegStr(HKEY_CURRENT_USER, themekey,
		"DllName", s, 80, "");
	if(strstr(s, "\\luna.msstyles") != NULL)
	{
		GetRegStr(HKEY_CURRENT_USER, themekey,
			"ColorName", s, 80, "");
		if(strcmp(s, "NormalColor") == 0)
			SetMyRegLong(NULL, "ForeColor", RGB(255,255,255));
	}
}

/*------------------------------------------------
  initialize format string
--------------------------------------------------*/
void InitFormat(void)
{
	char s[BUFSIZE_FORMAT];
	BOOL parts[NUM_FORMATPART];
	RECT rc;
	HWND hwnd;
	BOOL bbreak;
	int ilang;
	int i;
	
	if(GetMyRegStr("", "Format", s, BUFSIZE_FORMAT, "") > 0)
		return;
	
	ilang = GetMyRegLong("", "Locale", (int)GetUserDefaultLangID());
	if(GetMyRegStr("", "Locale", s, 20, "") == 0)
		SetMyRegLong("", "Locale", ilang);
	
	InitAutoFormat(ilang); // common/autoformat.c
	
	for(i = 0; i < NUM_FORMATPART; i++)
		parts[i] = FALSE;
	
	parts[PART_YEAR] = TRUE;
	parts[PART_MONTH] = TRUE;
	parts[PART_DAY] = TRUE;
	parts[PART_WEEKDAY] = TRUE;
	parts[PART_HOUR] = TRUE;
	parts[PART_MINUTE] = TRUE;
	parts[PART_SECOND] = TRUE;
	
	bbreak = FALSE;
	hwnd = GetTaskbarWindow();
	if(hwnd != NULL)
	{
		GetClientRect(hwnd, &rc);
		// vertical task bar
		if(rc.right < rc.bottom) bbreak = TRUE;
		
		if(g_winver&WINXP)
		{
			hwnd = FindWindowEx(hwnd, NULL, "TrayNotifyWnd", NULL);
			if(hwnd)
			{
				RECT rc;
				GetClientRect(hwnd, &rc);
				if(rc.bottom - rc.top > 32) bbreak = TRUE;
			}
		}
	}
	
	parts[PART_BREAK] = bbreak;
	
	AutoFormat(s, parts);  // common/autoformat.c
	
	SetMyRegStr("", "Format", s);
	
	SetMyRegLong("", "Kaigyo", parts[PART_BREAK]);
}

