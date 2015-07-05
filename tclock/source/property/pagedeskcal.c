/*-------------------------------------------------------------
  pagedeskcal.c : "Desktop Calendar" page of properties
  (C) Kazuto Sato 1997-2003
  For the license, please read readme.txt.
  
  Written by Kazubon, Nanashi-san
---------------------------------------------------------------*/

#include "tcprop.h"

/* Globals */

BOOL CALLBACK PageDeskCalProc(HWND hDlg, UINT message,
	WPARAM wParam, LPARAM lParam);

/* Statics */

static void SendPSChanged(HWND hDlg);
static void OnInit(HWND hDlg);
static void OnApply(HWND hDlg);
static void OnDeskcal(HWND hDlg);
static void OnSanshoDeskcal(HWND hDlg, WORD id);

static BOOL m_bInit = FALSE;
static BOOL m_bChanged = FALSE;

/*------------------------------------------------
  dialog procedure
--------------------------------------------------*/
BOOL CALLBACK PageDeskCalProc(HWND hDlg, UINT message,
	WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		case WM_INITDIALOG:
			OnInit(hDlg);
			return TRUE;
		case WM_COMMAND:
		{
			WORD id, code;
			id = LOWORD(wParam); code = HIWORD(wParam);
			switch (id)
			{
				case IDC_DESKCAL:
					OnDeskcal(hDlg);
					SendPSChanged(hDlg);
					break;
				case IDC_COMDESKCAL:
					if(code == EN_CHANGE) SendPSChanged(hDlg);
					break;
				case IDC_SANSHODESKCAL:
					OnSanshoDeskcal(hDlg, id);
					break;
				case IDC_MCIWAVE:
				case IDC_ONLYDATECHANGED:
				case IDC_RESUMESUSPEND:
				case IDC_TONIKAKU:
				case IDC_WATCHWALL:
					SendPSChanged(hDlg);
					break;
			}
			return TRUE;
		}
		case WM_NOTIFY:
			switch(((NMHDR *)lParam)->code)
			{
				case PSN_APPLY: OnApply(hDlg); break;
				case PSN_HELP: MyHelp(GetParent(hDlg), "DeskCal"); break;
			}
			return TRUE;
	}
	return FALSE;
}

/*------------------------------------------------
  notify parent window to enable "Apply" button
--------------------------------------------------*/
void SendPSChanged(HWND hDlg)
{
	if(m_bInit)
	{
		g_bApplyMain = TRUE;
		m_bChanged = TRUE;
		SendMessage(GetParent(hDlg), PSM_CHANGED, (WPARAM)(hDlg), 0);
	}
}

/*------------------------------------------------
  initialize
--------------------------------------------------*/
void OnInit(HWND hDlg)
{
	char s[MAX_PATH];
	
	m_bInit = FALSE;
	
	// common/tclang.c
	SetDialogLanguage(hDlg, "DeskCal", g_hfontDialog);
	
	CheckDlgButton(hDlg, IDC_DESKCAL,
		GetMyRegLong(NULL, "Deskcal", FALSE));
	
	GetRegStr(HKEY_CURRENT_USER, "Software\\Shinonon\\Deskcal",
		"ExeFileName", s, MAX_PATH, "");
	if(s[0] == 0) GetMyRegStr(NULL, "DeskcalCommand", s, MAX_PATH, "");
	SetDlgItemText(hDlg, IDC_COMDESKCAL, s);
	
	CheckDlgButton(hDlg, IDC_ONLYDATECHANGED,
		GetMyRegLong(NULL, "DeskcalOnlyDate", FALSE));
	CheckDlgButton(hDlg, IDC_RESUMESUSPEND,
		GetMyRegLong(NULL, "DeskcalResumeSuspend", FALSE));
	CheckDlgButton(hDlg, IDC_TONIKAKU,
		GetMyRegLong(NULL, "DeskcalTonikaku", FALSE));
	CheckDlgButton(hDlg, IDC_WATCHWALL,
		GetMyRegLong(NULL, "WatchWallpaper", TRUE));
	
	OnDeskcal(hDlg);
	
	m_bInit = TRUE;
}

/*------------------------------------------------
  apply
--------------------------------------------------*/
void OnApply(HWND hDlg)
{
	char s[MAX_PATH];
	
	if(!m_bChanged) return;
	m_bChanged = FALSE;
	
	SetMyRegLong(NULL, "Deskcal", IsDlgButtonChecked(hDlg, IDC_DESKCAL));
	
	GetDlgItemText(hDlg, IDC_COMDESKCAL, s, MAX_PATH);
	SetMyRegStr(NULL, "DeskcalCommand", s);
	
	SetMyRegLong(NULL, "DeskcalOnlyDate",
		IsDlgButtonChecked(hDlg, IDC_ONLYDATECHANGED));
	
	SetMyRegLong(NULL, "DeskcalResumeSuspend",
		IsDlgButtonChecked(hDlg, IDC_RESUMESUSPEND));
	
	SetMyRegLong(NULL, "DeskcalTonikaku",
		IsDlgButtonChecked(hDlg, IDC_TONIKAKU));
	SetMyRegLong(NULL, "WatchWallpaper",
		IsDlgButtonChecked(hDlg, IDC_WATCHWALL));
}

/*------------------------------------------------
  checked "Update Desktop Calendar Automatically"
--------------------------------------------------*/
void OnDeskcal(HWND hDlg)
{
	BOOL b;
	HWND hwnd;
	
	b = IsDlgButtonChecked(hDlg, IDC_DESKCAL);
	
	hwnd = GetDlgItem(hDlg, IDC_DESKCAL);
	hwnd = GetWindow(hwnd, GW_HWNDNEXT);
	while(hwnd)
	{
		EnableWindow(hwnd, b);
		hwnd = GetWindow(hwnd, GW_HWNDNEXT);
	}
}

/*------------------------------------------------
  clicked "..." button
--------------------------------------------------*/
void OnSanshoDeskcal(HWND hDlg, WORD id)
{
	char filter[] = "Deskcal.exe\0Deskcal.exe\0";
	char deffile[MAX_PATH+10], fname[MAX_PATH+10];
	
	GetDlgItemText(hDlg, id - 1, deffile, MAX_PATH+10);
	if(strlen(deffile) > 10 && 
		strcmp(deffile + (strlen(deffile) - 10), " -OnlyDraw") == 0)
		deffile[strlen(deffile) - 10] = 0;
	
	if(!SelectMyFile(g_hInst, hDlg, filter, 0, deffile, fname))
		return;
	
	strcat(fname, " -OnlyDraw");
	SetDlgItemText(hDlg, id - 1, fname);
	PostMessage(hDlg, WM_NEXTDLGCTL, 1, FALSE);
	SendPSChanged(hDlg);
}
