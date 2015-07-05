/*-------------------------------------------------------------
  pagesize.c : "Size and Position" page of properties
  (C) Kazuto Sato 1997-2003
  For the license, please read readme.txt.
  
  Written by Kazubon, Nanashi-san
---------------------------------------------------------------*/

#include "tcprop.h"

/* Globals */

BOOL CALLBACK PageSizeProc(HWND hDlg, UINT message,
	WPARAM wParam, LPARAM lParam);

/* Statics */

static void SendPSChanged(HWND hDlg);
static void OnInit(HWND hDlg);
static void OnApply(HWND hDlg);
static void OnFitClock(HWND hDlg);
static BOOL  m_bInit = FALSE;
static BOOL  m_bChanged = FALSE;

/*------------------------------------------------
  Dialog procedure
--------------------------------------------------*/
BOOL CALLBACK PageSizeProc(HWND hDlg, UINT message,
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
			switch(id)
			{
				case IDC_ALIGNLEFT:
				case IDC_ALIGNCENTER:
				case IDC_ALIGNRIGHT:
					SendPSChanged(hDlg);
					break;
				case IDC_FITCLOCK:
					OnFitClock(hDlg);
					SendPSChanged(hDlg);
					break;
				case IDC_CLOCKHEIGHT:
				case IDC_CLOCKWIDTH:
				case IDC_VERTPOS:
				case IDC_LINEHEIGHT:
					if(code == EN_CHANGE) SendPSChanged(hDlg);
					break;
			}
			return TRUE;
		}
		case WM_NOTIFY:
			switch(((NMHDR *)lParam)->code)
			{
				case PSN_APPLY: OnApply(hDlg); break;
				case PSN_HELP: MyHelp(GetParent(hDlg), "Size"); break;
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
		g_bApplyClock = TRUE;
		m_bChanged = TRUE;
		SendMessage(GetParent(hDlg), PSM_CHANGED, (WPARAM)(hDlg), 0);
	}
}

/*------------------------------------------------
  Initialize
--------------------------------------------------*/
void OnInit(HWND hDlg)
{
	int nTextPos, id;
	
	m_bInit = FALSE;
	
	// common/tclang.c
	SetDialogLanguage(hDlg, "Size", g_hfontDialog);
	
	SendDlgItemMessage(hDlg, IDC_SPINVPOS, UDM_SETBUDDY,
		(WPARAM)GetDlgItem(hDlg, IDC_VERTPOS), 0);
	SendDlgItemMessage(hDlg, IDC_SPINLHEIGHT, UDM_SETBUDDY,
		(WPARAM)GetDlgItem(hDlg, IDC_LINEHEIGHT), 0);
	SendDlgItemMessage(hDlg, IDC_SPINCWIDTH, UDM_SETBUDDY,
		(WPARAM)GetDlgItem(hDlg, IDC_CLOCKWIDTH), 0);
	SendDlgItemMessage(hDlg, IDC_SPINCHEIGHT, UDM_SETBUDDY,
		(WPARAM)GetDlgItem(hDlg, IDC_CLOCKHEIGHT), 0);
	
	nTextPos = GetMyRegLong(NULL, "TextPos", 0);
	if(nTextPos == 1) id = IDC_ALIGNLEFT;
	else if(nTextPos == 2) id = IDC_ALIGNRIGHT;
	else id = IDC_ALIGNCENTER;
	CheckRadioButton(hDlg, IDC_ALIGNLEFT, IDC_ALIGNRIGHT, id);
	
	SendDlgItemMessage(hDlg, IDC_SPINVPOS, UDM_SETRANGE, 0,
		(32 << 16) | (WORD)(-32));
	SendDlgItemMessage(hDlg, IDC_SPINVPOS, UDM_SETPOS, 0,
		GetMyRegLong(NULL, "VertPos", 0));
	SendDlgItemMessage(hDlg, IDC_SPINLHEIGHT, UDM_SETRANGE, 0,
		(32 << 16) | (WORD)(-32));
	SendDlgItemMessage(hDlg, IDC_SPINLHEIGHT, UDM_SETPOS, 0,
		GetMyRegLong(NULL, "LineHeight", 0));
	
	if(g_winver&WINXP)
		CheckDlgButton(hDlg, IDC_FITCLOCK,
			GetMyRegLong(NULL, "FitClock", TRUE));
	else
		EnableDlgItem(hDlg, IDC_FITCLOCK, FALSE);
	
	SendDlgItemMessage(hDlg, IDC_SPINCWIDTH, UDM_SETRANGE, 0,
		(32 << 16) | (WORD)(-32));
	SendDlgItemMessage(hDlg, IDC_SPINCWIDTH, UDM_SETPOS, 0,
		GetMyRegLong(NULL, "ClockWidth", 0));
	SendDlgItemMessage(hDlg, IDC_SPINCHEIGHT, UDM_SETRANGE, 0,
		(32 << 16) | (WORD)(-32));
	SendDlgItemMessage(hDlg, IDC_SPINCHEIGHT, UDM_SETPOS, 0,
		GetMyRegLong(NULL, "ClockHeight", 0));
	
	OnFitClock(hDlg);
	
	m_bInit = TRUE;
}

/*------------------------------------------------
  Apply
--------------------------------------------------*/
void OnApply(HWND hDlg)
{
	int nTextPos;
	
	if(!m_bChanged) return;
	m_bChanged = FALSE;
	
	if(IsDlgButtonChecked(hDlg, IDC_ALIGNLEFT)) nTextPos = 1;
	else if(IsDlgButtonChecked(hDlg, IDC_ALIGNRIGHT)) nTextPos = 2;
	else nTextPos = 0;
	SetMyRegLong(NULL, "TextPos", nTextPos);
	
	SetMyRegLong(NULL, "VertPos",
		SendDlgItemMessage(hDlg, IDC_SPINVPOS, UDM_GETPOS, 0, 0));
	SetMyRegLong(NULL, "LineHeight",
		SendDlgItemMessage(hDlg, IDC_SPINLHEIGHT, UDM_GETPOS, 0, 0));
	
	SetMyRegLong(NULL, "ClockWidth",
		SendDlgItemMessage(hDlg, IDC_SPINCWIDTH, UDM_GETPOS, 0, 0));
	SetMyRegLong(NULL, "ClockHeight",
		SendDlgItemMessage(hDlg, IDC_SPINCHEIGHT, UDM_GETPOS, 0, 0));
	
	SetMyRegLong(NULL, "FitClock",
		IsDlgButtonChecked(hDlg, IDC_FITCLOCK));
}

/*------------------------------------------------
  "Fit clock to tray"
--------------------------------------------------*/
void OnFitClock(HWND hDlg)
{
	HWND hwndBar, hwnd;
	RECT rc;
	BOOL b1, b2;
	
	if(!(g_winver&WINXP)) return;
	
	if(IsDlgButtonChecked(hDlg, IDC_FITCLOCK))
	{
		b1 = TRUE; b2 = FALSE;
		hwndBar = GetTaskbarWindow();
		if(hwndBar != NULL)
		{
			GetClientRect(hwndBar, &rc);
			// vertical task bar
			if(rc.right < rc.bottom)
			{
				b1 = FALSE; b2 = TRUE;
			}
		}
	}
	else { b1 = b2 = TRUE; }
	
	hwnd = GetDlgItem(hDlg, IDC_CLOCKWIDTH);
	EnableWindow(hwnd, b1);
	EnableWindow(GetWindow(hwnd, GW_HWNDNEXT), b1);
	EnableWindow(GetWindow(hwnd, GW_HWNDPREV), b1);
	hwnd = GetDlgItem(hDlg, IDC_CLOCKHEIGHT);
	EnableWindow(hwnd, b2);
	EnableWindow(GetWindow(hwnd, GW_HWNDNEXT), b2);
	EnableWindow(GetWindow(hwnd, GW_HWNDPREV), b2);
}
