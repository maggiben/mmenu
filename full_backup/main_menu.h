#ifndef __main_menu_h__
#define __main_menu_h__

#ifndef __LCC__
#define __LCC__
#endif

//==========================================================
/*
	include file para __main_menu_h__
*/
//==========================================================



BOOL create_Controls(HINSTANCE main_inst, HWND hwnd_parent, int skin_tpy);

LRESULT CALLBACK ButtonProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
RECT MapDialogControlRect(HWND, int, BOOL);

//BOOL LoadSong(HWND LB_Parent_Hwnd);

#endif
