/*-------------------------------------------------------------
  wndproc.c : window procedure of tclock.exe
  (C) 1997-2003 Kazuto Sato
  Please read readme.txt about the license.
  
  Written by Kazubon, Nanashi-san
---------------------------------------------------------------*/

#include "tclock.h"

/* Globals */

LRESULT CALLBACK WndProc(HWND hwnd, UINT message,
	WPARAM wParam, LPARAM lParam);
HWND g_hwndClock = NULL; // clock window

/* Statics */

static void OnCreate(HWND hwnd);
static void OnDestroy(HWND hwnd);
static void ClearTClockMain(HWND hwnd);
static void OnTimerStart(HWND hwnd);
static void OnTimerMain(HWND hwnd);
static void OnTCMHwndClock(HWND hwnd, LPARAM lParam);
static void OnTCMClockError(HWND hwnd, LPARAM lParam);
static void OnTCMExit(HWND hwnd);
static void OnTCMReloadSetting(HWND hwnd);
static void OnTaskbarRestart(HWND hwnd);
static void OnCopyData(HWND hwnd, HWND hwndFrom, COPYDATASTRUCT* pcds);
static void InitError(int n);

static void OnWinIniChange(HWND hwnd, const char *pParam);
static void OnResumeSuspend(HWND hwnd);
static void OnTCMDeskCal(HWND hwnd, BOOL bSysChanged);

static BOOL m_bHook = FALSE;
static BOOL m_bStartTimer = FALSE;


/*-------------------------------------------
   the window procedure
---------------------------------------------*/
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
	{
		case WM_CREATE:
			OnCreate(hwnd);
			return 0;
		case WM_DESTROY:
			OnDestroy(hwnd);
			return 0;
		case WM_ENDSESSION:
			if(wParam) ClearTClockMain(hwnd);
			break;
		
		case WM_TIMER:
			switch (wParam)
			{
				case IDTIMER_START:
					OnTimerStart(hwnd); break;
				case IDTIMER_MAIN:
					OnTimerMain(hwnd);  break;
				case IDTIMER_MOUSE:
					// mouse.c
					OnTimerMouse(hwnd); break;
				case IDTIMER_MONOFF:
					KillTimer(hwnd, wParam);
					SendMessage(GetDesktopWindow(), WM_SYSCOMMAND,
						SC_MONITORPOWER, 2);
					break;
			}
			return 0;
		
		// menu and commands
		case WM_COMMAND:
			// command.c
			OnTClockCommand(hwnd, LOWORD(wParam), HIWORD(wParam));
			return 0;
		case WM_CONTEXTMENU:
			// menu.c
			OnContextMenu(hwnd, (HWND)wParam, LOWORD(lParam), HIWORD(lParam));
			return 0;
		case WM_EXITMENULOOP:
			// menu.c
			OnExitMenuLoop(hwnd);
			return 0;
		
		// messages sent/posted from other programs
		
		case TCM_HWNDCLOCK:
			OnTCMHwndClock(hwnd, lParam);
			return 0;
		case TCM_CLOCKERROR:
			OnTCMClockError(hwnd, lParam);
			return 0;
		case TCM_EXIT:
			OnTCMExit(hwnd);
			return 0;
		case TCM_RELOADSETTING:
			OnTCMReloadSetting(hwnd);
			return 0;
		// case TCM_REQUESTSNTPLOG:
		//	// sntp.c
		//	OnTCMRequestSNTPLog(hwnd, (HWND)wParam);
		//	return 0;
		
		/* Update Desktop Calendar Automatically */
		case WM_WININICHANGE:
			OnWinIniChange(hwnd, (const char*)lParam);
			return 0;
		case WM_SYSCOLORCHANGE:
			PostMessage(hwnd, TCM_DESKCAL, 1, 0);
			return 0;
		case WM_POWERBROADCAST:  // return from power saving
			if(wParam == 7) //PBT_APMRESUMESUSPEND
				OnResumeSuspend(hwnd);
			break;
		case TCM_DESKCAL:
			OnTCMDeskCal(hwnd, wParam);
			return 0;
		
		case WM_COPYDATA:
			OnCopyData(hwnd, (HWND)wParam, (COPYDATASTRUCT*)lParam);
			return 0;
		
		case MM_MCINOTIFY:
			// common/playfile.c
			OnMCINotify(hwnd, wParam, (LONG)lParam);
			return 0;
		case MM_WOM_DONE: // stop playing wave
		case TCM_STOPSOUND:
			StopFile();
			return 0;
		
		/* -------- mouse ---------- */
		
		case WM_DROPFILES:
			OnDropFiles(hwnd, (HDROP)wParam); // mouse2.c
			return 0;
		
		case WM_MOUSEWHEEL:
			OnMouseWheel(hwnd, wParam, lParam); // mouse2.c
			return 0;
		
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_XBUTTONDOWN:
			StopFile();
			OnMouseDown(hwnd, message, wParam, lParam); // mouse.c
			return 0;
		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
		case WM_MBUTTONUP:
		case WM_XBUTTONUP:
			OnMouseUp(hwnd, message, wParam, lParam); // mouse.c
			return 0;
	}
	
	if(message == g_uTaskbarRestart)
		OnTaskbarRestart(hwnd);
	
	return DefWindowProc(hwnd, message, wParam, lParam);
}

/* ------------------------------- Statics -------------------------------- */

/*-------------------------------------------------------
  WM_CREATE message
---------------------------------------------------------*/
void OnCreate(HWND hwnd)
{
	int nDelay;
	
	InitAlarm();  // alarm.c
	InitMouseFunction(hwnd); // mouse.c
	// InitSNTP(hwnd); // sntp.c
	InitWatchWallpaper(); // deskcal.c
	
	// delay starting
	nDelay = GetMyRegLong(NULL, "DelayStart", 0);
	if(nDelay > 0)
	{
		SetTimer(hwnd, IDTIMER_START, nDelay * 1000, NULL);
		m_bStartTimer = TRUE;
	}
	else OnTimerStart(hwnd);
	
	SetTimer(hwnd, IDTIMER_MAIN, 1000, NULL);
}

/*-------------------------------------------------------
  WM_DESTROY message
---------------------------------------------------------*/
void OnDestroy(HWND hwnd)
{
	ClearTClockMain(hwnd);
	
	PostQuitMessage(0);
}

/*-------------------------------------------------------
  clear up
---------------------------------------------------------*/
void ClearTClockMain(HWND hwnd)
{
	static BOOL bCleared = FALSE;
	
	if(bCleared) return;
	bCleared = TRUE;
	
	if(g_hDlgAbout && IsWindow(g_hDlgAbout))
		DestroyWindow(g_hDlgAbout);
	
	// EndSNTP(hwnd);
	StopFile();
	EndMouseFunction(hwnd);
	EndAlarm();
	EndMenu();
	EndWatchWallpaper(); // deskcal.c
	
	KillTimer(hwnd, IDTIMER_MAIN);
	
	if(m_bStartTimer) KillTimer(hwnd, IDTIMER_START);
	m_bStartTimer = FALSE;
	
	if(m_bHook) HookEnd();  // dll/main.c - uninstall the hook
	m_bHook = FALSE;
}

/*-------------------------------------------------------
  WM_TIMER message, wParam = IDTIMER_START
  start customizing the tray clock
---------------------------------------------------------*/
void OnTimerStart(HWND hwnd)
{
	if(m_bStartTimer) KillTimer(hwnd, IDTIMER_START);
	m_bStartTimer = FALSE;
	if(!m_bHook)
		m_bHook = HookStart(hwnd); // dll/main.c - install a hook
	
	OnTimerAlarm(hwnd, NULL, 1); // alarm.c
}

/*-------------------------------------------------------
  WM_TIMER message, wParam = IDTIMER_MAIN
---------------------------------------------------------*/
void OnTimerMain(HWND hwnd)
{
	static BOOL bTimerAdjusting = FALSE;
	static int hourLast = -1, minuteLast = -1;
	static int daySaved = -1;
	SYSTEMTIME st;
	BOOL bMinuteChanged;
	
	GetLocalTime(&st);
	
	bMinuteChanged = TRUE;
	if(hourLast == (int)st.wHour &&
		minuteLast == (int)st.wMinute) bMinuteChanged = FALSE;
	hourLast = st.wHour;
	minuteLast = st.wMinute;
	
	// adjusting milliseconds gap
	if((st.wMilliseconds > 200 ||
		((g_winver | WINNT) && st.wMilliseconds > 50)))
	{
		KillTimer(hwnd, IDTIMER_MAIN);
		SetTimer(hwnd, IDTIMER_MAIN, 1001 - st.wMilliseconds, NULL);
		bTimerAdjusting = TRUE;
	}
	else if(bTimerAdjusting)
	{
		KillTimer(hwnd, IDTIMER_MAIN);
		bTimerAdjusting = FALSE;
		SetTimer(hwnd, IDTIMER_MAIN, 1000, NULL);
	}
	
	OnTimerAlarm(hwnd, &st, 0); // alarm.c
	
	if(daySaved >= 0 && st.wDay != daySaved)
	{
		if(ExecDeskcal(hwnd)) // deskcal.c
			daySaved = st.wDay;
	}
	else daySaved = st.wDay;
	
	if(bMinuteChanged)
		OnTimerWatchWallpaper(hwnd, &st); // deskcal.c
	
	// OnTimerSNTP(hwnd);  // sntp.c
}

/*-------------------------------------------------------
  TCM_HWNDCLOCK message
---------------------------------------------------------*/
void OnTCMHwndClock(HWND hwnd, LPARAM lParam)
{
	g_hwndClock = (HWND)lParam;
}

/*-------------------------------------------------------
  TCM_CLOCKERROR message
---------------------------------------------------------*/
void OnTCMClockError(HWND hwnd, LPARAM lParam)
{
	char s[160];
	
	wsprintf(s, "%s: %d", "failed to start TClock.", lParam);
	MessageBox(NULL, s, "Error", MB_OK|MB_ICONEXCLAMATION);
	
	PostMessage(hwnd, WM_CLOSE, 0, 0);
}

/*-------------------------------------------------------
  TCM_EXIT message
---------------------------------------------------------*/
void OnTCMExit(HWND hwnd)
{
	if(g_hwndClock)
		PostMessage(g_hwndClock, CLOCKM_EXIT, 0, 0);
}

/*-------------------------------------------------------
  TCM_RELOADSETTING message
---------------------------------------------------------*/
void OnTCMReloadSetting(HWND hwnd)
{
	InitAlarm(); // alarm.c
	InitMouseFunction(hwnd); // mouse.c
	
	InitWatchWallpaper(); // deskcal.c
}

/*-------------------------------------------------------
  When Explorer is hung up,
  and the taskbar is recreated.
---------------------------------------------------------*/
void OnTaskbarRestart(HWND hwnd)
{
	if(m_bHook) HookEnd();
	m_bHook = FALSE; g_hwndClock = NULL;
	
	if(GetMyRegLong(NULL, "TaskbarRestart", FALSE))
	{
		SetTimer(hwnd, IDTIMER_START, 1000, NULL);
		m_bStartTimer = TRUE;
	}
	else
		PostMessage(hwnd, WM_CLOSE, 0, 0);
}

/*-------------------------------------------------------
  WM_COPYDATA message
---------------------------------------------------------*/
void OnCopyData(HWND hwnd, HWND hwndFrom, COPYDATASTRUCT* pcds)
{
	const char *p = (char *)pcds->lpData;
	
	switch(pcds->dwData)
	{
		case COPYDATA_SOUND:
			PlayFileCmdLine(hwnd, p);  // common/playfile.c
			break;
		case COPYDATA_EXEC:
			ExecCommandString(hwnd, p);  // command.c
			break;
	}
}

/* Update Desktop Calendar Automatically */

/*-------------------------------------------------------
  WM_WININICHANGE message
---------------------------------------------------------*/
void OnWinIniChange(HWND hwnd, const char *pParam)
{
	BOOL b;
	
	b = GetMyRegLong(NULL, "DeskcalTonikaku", FALSE);
	
	if(pParam &&
		(strcmp(pParam, "Desktop") == 0 || 
			strcmp(pParam, "RefreshDesktop") == 0))
		PostMessage(hwnd, TCM_DESKCAL, 1, 0);
	else if(b && (pParam == 0 || *pParam == 0))
		PostMessage(hwnd, TCM_DESKCAL, 1, 0);
}

/*-------------------------------------------------------
  WM_POWERBROADCAST message, wParam = PBT_APMRESUMESUSPEND
---------------------------------------------------------*/
void OnResumeSuspend(HWND hwnd)
{
	if(GetMyRegLong(NULL, "DeskcalResumeSuspend", FALSE))
		PostMessage(hwnd, TCM_DESKCAL, 0,0);
}

/*-------------------------------------------------------
  TCM_DESKCAL message
---------------------------------------------------------*/
void OnTCMDeskCal(HWND hwnd, BOOL bSysChanged)
{
	if(bSysChanged && GetMyRegLong(NULL, "DeskcalOnlyDate", FALSE))
		return;
	
	ExecDeskcal(hwnd);
}

