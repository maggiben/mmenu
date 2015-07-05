/*-------------------------------------------------------------
  timer.c : executing timers
  (C) Kazuto Sato 1997-2003
  For the license, please read readme.txt.
  
  Written by Kazubon, Nanashi-san
---------------------------------------------------------------*/

#include "tctimer.h"

/* Globals */
void TimerStart(PTIMERSTRUCT pTS);
void ClearTimer(void);
BOOL IsTimerRunning(void);
void OnTimerTimer(HWND hDlg);
void OnRequestMenu(HWND hDlg, BOOL bClear);
void OnStopTimer(HWND hDlg, int id);

/* Statics */

static void DoTimer(HWND hwnd, int index);
static void DeleteRunningTimer(HWND hwnd, int index);

static PTIMERSTRUCT m_pTimerRun = NULL;
static int m_numTimerRun = 0;
static int m_idCurrent = 1;

/*-------------------------------------------
  add a timer to m_pTimerRun and start
---------------------------------------------*/
void TimerStart(PTIMERSTRUCT pTS)
{
	PTIMERSTRUCT pNew;
	
	pTS->interval = pTS->minute * 60 + pTS->second;
	pTS->tickonstart = GetTickCount();
	pTS->id = m_idCurrent++;
	
	pNew = AddTimerStruct(m_pTimerRun, m_numTimerRun, pTS);
	m_numTimerRun++;
	if(m_pTimerRun) free(m_pTimerRun);
	m_pTimerRun = pNew;
}

/*-------------------------------------------
  clear all timers
---------------------------------------------*/
void ClearTimer(void)
{
	free(m_pTimerRun);
	m_pTimerRun = NULL;
	m_numTimerRun = 0;
}

/*-------------------------------------------
  timer is running ?
---------------------------------------------*/
BOOL IsTimerRunning(void)
{
	return (m_pTimerRun != NULL);
}

/*-------------------------------------------
  WM_TIMER message
---------------------------------------------*/
void OnTimerTimer(HWND hwnd)
{
	DWORD tick;
	int i, indexDo;
	int nCopyData;
	char s[20];
	wchar_t ws[20];
	
	if(!m_pTimerRun) return;
	
	tick = GetTickCount();
	indexDo = -1;
	for(i = 0; i < m_numTimerRun; i++)
	{
		PTIMERSTRUCT pTS = m_pTimerRun + i;
		
		if(pTS->bDisp)
		{
			int remaining =
				pTS->interval - (tick - pTS->tickonstart)/1000 - 1;
			if(remaining >= 0)
			{
				if(pTS->nDispType == 0)
					nCopyData = COPYDATA_DISP1;
				else if(pTS->nDispType == 2)
					nCopyData = COPYDATA_USTR0 + pTS->nUserStr;
				else
					nCopyData = COPYDATA_CAT1;
				wsprintf(s, "[%02d:%02d]", remaining/60, remaining%60);
				
				MultiByteToWideChar(CP_ACP, 0, s, -1, ws, 19);
				SendStringToOtherW(g_hwndClock, hwnd, ws, nCopyData);
			}
		}
		
		if(indexDo < 0 && tick - pTS->tickonstart > pTS->interval * 1000)
			indexDo = i;
	}
	
	if(indexDo >= 0)
		DoTimer(hwnd, indexDo);
}

/*-------------------------------------------
  add item to tcmenu*.txt
---------------------------------------------*/
void OnRequestMenu(HWND hwnd, BOOL bClear)
{
	char fname[MAX_PATH], tcmenutxt[MAX_PATH];
	WIN32_FIND_DATA fd;
	HANDLE hfind;
	HFILE hf;
	int size;
	char *buf;
	const char *p, *np;
	static BOOL bWrite = FALSE;
	
	if(!bClear && m_pTimerRun == NULL) return;
	
	if(bClear && !bWrite) return;
	
	// ../common/tclang.c
	FindFileWithLangCode(tcmenutxt, GetUserDefaultLangID(), TCMENUTXT);
	
	hfind = FindFirstFile(tcmenutxt, &fd);
	if(hfind == INVALID_HANDLE_VALUE) return;
	
	FindClose(hfind);
	size = (int)fd.nFileSizeLow;
	buf = malloc(size+1);
	if(!buf) return;
	
	hf = _lopen(tcmenutxt, OF_READWRITE);
	if(hf == HFILE_ERROR) { free(buf); return; }
	_lread(hf, buf, size);
	*(buf + size) = 0;
	
	_llseek(hf, 0, 0);
	
	p = buf;
	while(*p)
	{
		if(strncmp(p, "#Timer Begin", 12) == 0)
		{
			int i;
			DWORD tick;
			char s[160];
			
			np = nextline(p);
			_lwrite(hf, p, np - p);
			p = np;
			
			tick = GetTickCount();
			
			for(i = 0; i < m_numTimerRun && m_pTimerRun; i++)
			{
				PTIMERSTRUCT pTS = m_pTimerRun + i;
				int remaining =
					pTS->interval - (tick - pTS->tickonstart)/1000 - 1;
				
				wsprintf(s, "\"%s %s %02d:%02d\" post %s %d 0 %d\r\n",
					MyString(IDS_STOP, "Stop"),
					pTS->name, remaining/60, remaining%60,
					CLASS_TCLOCKTIMER, TIMERM_STOP, pTS->id);
				_lwrite(hf, s, strlen(s));
			}
			
			while(*p)
			{
				if(strncmp(p, "#Timer End", 10) == 0)
					break;
				p = nextline(p);
			}
		}
		else
		{
			np = nextline(p);
			_lwrite(hf, p, np - p);
			p = np;
		}
	}
	
	_lwrite(hf, NULL, 0); // truncate
	
	_lclose(hf);
	free(buf);
	
	bWrite = TRUE;
}

/*-------------------------------------------
  WM_TIMER message
---------------------------------------------*/
void OnStopTimer(HWND hwnd, int id)
{
	int i;
	
	if(!m_pTimerRun) return;
	
	for(i = 0; i < m_numTimerRun; i++)
	{
		if((m_pTimerRun + i)->id == id)
		{
			DeleteRunningTimer(hwnd, i);
			break;
		}
	}
}

/*-------------------------------------------
  execute timer
---------------------------------------------*/
void DoTimer(HWND hwnd, int index)
{
	PTIMERSTRUCT pTS;
	char command[MAX_PATH + 10];
	HWND hwndMain;
	
	if(!m_pTimerRun ||
		!(0 <= index && index < m_numTimerRun)) return;
	
	pTS = m_pTimerRun + index;
	
	command[0] = 0;
	if(pTS->bRepeat) strcpy(command, "-1 ");
	strcat(command, pTS->fname);
	hwndMain = GetTClockMainWindow();
	if(hwndMain)
		SendStringToOther(hwndMain, hwnd, command, COPYDATA_SOUND);
	
	if(pTS->bBlink)
		PostMessage(g_hwndClock, CLOCKM_BLINK, 0, 0);
	
	DeleteRunningTimer(hwnd, index);
}

/*-------------------------------------------
  delete an executed timer from array
---------------------------------------------*/
void DeleteRunningTimer(HWND hwnd, int index)
{
	PTIMERSTRUCT pTS;
	
	if(!m_pTimerRun) return;
	
	pTS = m_pTimerRun + index;
	
	if(pTS->bDisp)
	{
		int nCopyData;
		if(pTS->nDispType == 0)
			nCopyData = COPYDATA_DISP1;
		else if(pTS->nDispType == 2)
			nCopyData = COPYDATA_USTR0 + pTS->nUserStr;
		else
			nCopyData = COPYDATA_CAT1;
		SendStringToOtherW(g_hwndClock, hwnd, L"", nCopyData);
	}
	
	if(m_numTimerRun > 1)
	{
		PTIMERSTRUCT pNew;
		pNew = DelTimerStruct(m_pTimerRun, m_numTimerRun, index);
		m_numTimerRun--;
		free(m_pTimerRun);
		m_pTimerRun = pNew;
	}
	else
	{
		m_numTimerRun = 0;
		free(m_pTimerRun);
		m_pTimerRun = NULL;
	}
	
	OnRequestMenu(hwnd, TRUE);
	
	if(m_numTimerRun == 0 && !(g_hDlg && IsWindow(g_hDlg)))
		PostMessage(hwnd, WM_CLOSE, 0, 0);
}
