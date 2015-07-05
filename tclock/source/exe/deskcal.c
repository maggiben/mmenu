/*-------------------------------------------
  deskcal.c: Update Desktop Calendar automatically
  (C) 1997-2004 Kazuto Sato
  Please read readme.txt about the license.
---------------------------------------------*/

#include "tclock.h"

BOOL ScreenSaverExist(void);

static HANDLE m_hProcessDeskcal = NULL;
static char *m_wallpapername = NULL;
static int m_tilewallpaper, m_wallpaperstyle;
static WIN32_FIND_DATA m_fd_wallpaper;
static DWORD m_colorbackground;

/*------------------------------------------------
  Execute Deskcal.exe
--------------------------------------------------*/
BOOL ExecDeskcal(HWND hwnd)
{
	char s[MAX_PATH];
	char fname[MAX_PATH], option[MAX_PATH];
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	
	if(GetMyRegLong(NULL, "Deskcal", FALSE) == FALSE)
		return TRUE;
	
	if(m_hProcessDeskcal)
	{
		DWORD dwExitCode;
		if(GetExitCodeProcess(m_hProcessDeskcal, &dwExitCode) &&
			dwExitCode == STILL_ACTIVE)
		{
			return TRUE;
		}
	}
	
	if(FindWindow("TDeskcalForm", NULL))
		return TRUE;
	
	if(ScreenSaverExist()) return FALSE;
	
	GetRegStr(HKEY_CURRENT_USER, "Software\\Shinonon\\Deskcal",
		"ExeFileName", s, MAX_PATH, "");
	if(s[0] == 0)
	{
		GetMyRegStr(NULL, "DeskcalCommand", s, MAX_PATH, "");
		if(s[0] == 0) return TRUE;
	}
	
	if(s[0] != '\"')
	{
		GetFileAndOption(s, fname, option);
		s[0] = '\"'; strcpy(s + 1, fname);
		strcat(s, "\" "); strcat(s, option);
	}
	
	memset(&si, 0, sizeof(si));
	si.cb = sizeof(si);
	
	m_hProcessDeskcal = NULL;
	if(!CreateProcess(NULL, s, NULL, NULL, FALSE, 0,
		NULL, NULL, &si, &pi))
	{
		return TRUE;
	}
	m_hProcessDeskcal = pi.hProcess;
	
	InitWatchWallpaper();
	
	return TRUE;
}

/*------------------------------------------------
  If screensaver is running
--------------------------------------------------*/
BOOL ScreenSaverExist(void)
{
	HWND hwnd;
	RECT rc;
	char s[80];
	
	hwnd = GetForegroundWindow();
	if(!hwnd) return FALSE;

	GetWindowRect(hwnd, &rc);
	if(rc.left <= 0 && rc.top <= 0 &&
		rc.right >= GetSystemMetrics(SM_CXSCREEN) &&
		rc.bottom >= GetSystemMetrics(SM_CYSCREEN))
	{
		GetClassName(hwnd, s, 80);
		if(strcmp(s, "Progman") != 0 &&
			strcmp(s, "DeskSaysNoPeekingItsASurprise") != 0)
			return TRUE;
	}
	return FALSE;
}

/*----------------------------------------
   Watch the wallpaper file
----------------------------------------*/
void InitWatchWallpaper(void)
{
	char s[30];
	char section[] = "Control Panel\\desktop";
	HANDLE hfind;
	
	EndWatchWallpaper();
	
	if(GetMyRegLong(NULL, "WatchWallpaper", FALSE) == FALSE)
		return;
	
	m_wallpapername = (char*)malloc(MAX_PATH);
	GetRegStr(HKEY_CURRENT_USER, section, "Wallpaper",
		m_wallpapername, MAX_PATH, "");
	
	GetRegStr(HKEY_CURRENT_USER, section, "TileWallpaper", s, 30, "0");
	m_tilewallpaper = atoi(s);
	GetRegStr(HKEY_CURRENT_USER, section, "WallpaperStyle", s, 30, "0");
	m_wallpaperstyle = atoi(s);
	
	m_colorbackground = GetSysColor(COLOR_BACKGROUND);
	
	if(m_wallpapername[0])
	{
		hfind = FindFirstFile(m_wallpapername, &m_fd_wallpaper);
		if(hfind != INVALID_HANDLE_VALUE)
			FindClose(hfind);
	}
}

void EndWatchWallpaper(void)
{
	if(m_wallpapername) free(m_wallpapername);
	m_wallpapername = NULL;
}

void OnTimerWatchWallpaper(HWND hwnd, SYSTEMTIME* pt)
{
	char section[] = "Control Panel\\desktop";
	char s[30], fname[MAX_PATH];
	WIN32_FIND_DATA fd;
	HANDLE hfind;
	BOOL b;
	
	if(!m_wallpapername) return;
	
	if(GetMyRegLong(NULL, "WatchWallpaper", FALSE) == FALSE)
		return;
	
	b = FALSE;
	GetRegStr(HKEY_CURRENT_USER, section, "Wallpaper", fname, MAX_PATH, "");
	if(strcmp(fname, m_wallpapername) != 0) b = TRUE;
	GetRegStr(HKEY_CURRENT_USER, section, "TileWallpaper", s, 30, "0");
	if(m_tilewallpaper != atoi(s)) b = TRUE;
	GetRegStr(HKEY_CURRENT_USER, section, "WallpaperStyle", s, 30, "0");
	if(m_wallpaperstyle != atoi(s)) b = TRUE;
	
	if(m_colorbackground != GetSysColor(COLOR_BACKGROUND))
		b = TRUE;
	
	if(!b && fname[0])
	{
		hfind = FindFirstFile(m_wallpapername, &fd);
		if(hfind != INVALID_HANDLE_VALUE)
		{
			FindClose(hfind);
			if(fd.nFileSizeLow != m_fd_wallpaper.nFileSizeLow ||
				*(DWORDLONG*)&(fd.ftCreationTime) != 
					*(DWORDLONG*)&(m_fd_wallpaper.ftCreationTime) ||
				*(DWORDLONG*)&(fd.ftLastWriteTime) != 
					*(DWORDLONG*)&(m_fd_wallpaper.ftLastWriteTime) )
			{
				b = TRUE;
			}
		}
	}
	
	if(b)
	{
		if(FindWindow("TDeskcalForm", NULL)) return;
		ExecDeskcal(hwnd);
	}
}

