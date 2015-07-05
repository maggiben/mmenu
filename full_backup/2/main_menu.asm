	.file	"c:\lcc\include\stddef.h"
_$M0:
	.file	"g:\cosas\c\deox\main_menu.c"
	.text
	.file	"c:\lcc\include\stdlib.h"
_$M1:
	.file	"c:\lcc\include\safelib.h"
_$M2:
	.file	"c:\lcc\include\stdlib.h"
_$M3:
	.file	"c:\lcc\include\string.h"
_$M4:
	.file	"c:\lcc\include\stdarg.h"
_$M5:
	.file	"c:\lcc\include\ctype.h"
_$M6:
	.file	"c:\lcc\include\win.h"
_$M7:
	.file	"c:\lcc\include\basetsd.h"
_$M8:
	.file	"c:\lcc\include\win.h"
_$M9:
	.file	"c:\lcc\include\stdio.h"
_$M10:
	.file	"c:\lcc\include\pshpack1.h"
_$M11:
	.file	"c:\lcc\include\shellapi.h"
_$M12:
	.file	"c:\lcc\include\poppack.h"
_$M13:
	.file	"c:\lcc\include\tchar.h"
_$M14:
	.file	"c:\lcc\include\wchar.h"
_$M15:
	.file	"c:\lcc\include\rpc.h"
_$M16:
	.file	"c:\lcc\include\rpcdce.h"
_$M17:
	.file	"c:\lcc\include\rpcdcep.h"
_$M18:
	.file	"c:\lcc\include\rpcnsi.h"
_$M19:
	.file	"c:\lcc\include\rpcasync.h"
_$M20:
	.file	"c:\lcc\include\rpcnsip.h"
_$M21:
	.file	"c:\lcc\include\rpcndr.h"
_$M22:
	.file	"c:\lcc\include\objbase.h"
_$M23:
	.file	"c:\lcc\include\unknwn.h"
_$M24:
	.file	"c:\lcc\include\ole2.h"
_$M25:
	.file	"c:\lcc\include\oleauto.h"
_$M26:
	.file	"c:\lcc\include\oaidl.h"
_$M27:
	.file	"c:\lcc\include\objidl.h"
_$M28:
	.file	"c:\lcc\include\oaidl.h"
_$M29:
	.file	"c:\lcc\include\oleauto.h"
_$M30:
	.file	"c:\lcc\include\oleidl.h"
_$M31:
	.file	"c:\lcc\include\ole2.h"
_$M32:
	.file	"c:\lcc\include\cguid.h"
_$M33:
	.file	"c:\lcc\include\objbase.h"
_$M34:
	.file	"c:\lcc\include\urlmon.h"
_$M35:
	.file	"c:\lcc\include\servprov.h"
_$M36:
	.file	"c:\lcc\include\msxml.h"
_$M37:
	.file	"c:\lcc\include\urlmon.h"
_$M38:
	.file	"c:\lcc\include\propidl.h"
_$M39:
	.file	"c:\lcc\include\objbase.h"
_$M40:
	.file	"c:\lcc\include\time.h"
_$M41:
	.file	"g:\cosas\c\deox\disphelper.h"
_$M42:
	.file	"g:\cosas\c\deox\main_menu.h"
_$M43:
	.file	"g:\cosas\c\deox\main_menu.c"
_$M44:
	.data
	.globl	_MAX_CLASS_NAME
	.align	2
	.type	_MAX_CLASS_NAME,object
_MAX_CLASS_NAME:
	.long	256
	.globl	__WndPropName_OldProc
	.type	__WndPropName_OldProc,object
__WndPropName_OldProc:
; "XPWndProp_OldProc\x0"
	.byte	88,80,87,110,100,80,114,111,112,95,79,108,100,80,114,111
	.byte	99,0
	.globl	__WndPropName_MenuXP
	.type	__WndPropName_MenuXP,object
__WndPropName_MenuXP:
; "XPWndProp_MenuXP\x0"
	.byte	88,80,87,110,100,80,114,111,112,95,77,101,110,117,88,80
	.byte	0
	.text
;    1 /////////////////////////////////////////////////////////////////////////////
;    2 // Título:      main_menu                                                  //
;    3 // Autor:       Benjamin Maggi                                             //
;    4 // Descripcion: start like menu program                                    //
;    5 // fecha:       17/01/2005                                                 //
;    6 // ver:         0.2                                                        //
;    7 // compiler:    LCC-Win32 (uses ansi-C standar only)                       //
;    8 // mail:        benjaminmaggi@yahoo.com.ar                                 //
;    9 //	                       								                   //
;   10 // Detais & compilating information
;   11 // ver 0.1
;   12 // I{ve compiled and linked this using lcc but should work in other c/c++
;   13 // compiler.
;   14 // be carful debugging the hooked function it may cause a GPF
;   15 // you can turn hooks off 									                                       //
;   16 ////////////////////////////////////////////////////////////////////////////
;   17 
;   18 
;   19 
;   20 //	Includes
;   21 #include <windows.h>
;   22 #include <windowsx.h>
;   23 #include <commctrl.h>
;   24 #include <string.h>
;   25 #include <stdio.h>
;   26 #include <shellapi.h>
;   27 #include <tchar.h>
;   28 #include <wchar.h>
;   29 
;   30 #include "disphelper.h"
;   31 #include "main_menures.h"
;   32 #include "main_menu.h"
;   33 
;   34 //#include <msxml.h>
;   35 
;   36 //#defines aqui
;   37 #define HR_TRY(func) if (FAILED(func)) { goto cleanup; }
;   38 /*
;   39 {
;   40 	//char line_num[255];
;   41 	//wsprintf(line_num,"%d",__LINE__);
;   42 	//message(0,"Error fatal Linea:", "22",MB_OK);
;   43 	goto cleanup;
;   44 }
;   45 */
;   46 //MessageBox(hwnd,"No HDC !!!","Error de tipo:",MB_OK);
;   47 typedef struct _MENU_DATA
;   48 {
;   49 		//magnament exclusive
;   50 		void *    nextItem;		//point to next MENU_DATA
;   51 		void * 	  prevItem;		//point to prev MENU_DATA
;   52 		HMENU     menu_handle;
;   53 		UINT	  wID;
;   54 		//drawing
;   55 		LPSTR     menu_capti;
;   56 		LPSTR     menu_exec;
;   57 		HBITMAP   menu_image;
;   58 		BOOL      menu_imask;
;   59 		HFONT     menu_font;
;   60 		HFONT     menu_fsize;
;   61 		COLORREF  menu_color;
;   62 		COLORREF  menu_selor;
;   63 		short     menu_width;
;   64 		short     menu_height;
;   65 		int       menu_popup;
;   66 }MENU_DATA, *LPMENU_DATA;
;   67 
;   68 LPMENU_DATA	primary_menu_data;
;   69 LPMENU_DATA	  first_menu_data;
;   70 //MENU_ITEM Menu_item;
;   71 
;   72 struct menu_item
;   73 	{
;   74 		struct    menu_item * nextItem;
;   75 		struct    menu_item * prevItem;
;   76 		HMENU     menu_handle;
;   77 		LPSTR     menu_capti;
;   78 		HBITMAP   menu_image;
;   79 		HFONT     menu_font;
;   80 		COLORREF  menu_color;
;   81 		COLORREF  menu_selor;
;   82 		int       menu_popup;
;   83 		int       menu_Itype;
;   84 	} Menu_item;
;   85 
;   86 //	Global
;   87 HINSTANCE hInst;		// Instance handle
;   88 HWND      hwndMain;		// Main window handle
;   89 HWND	  main_window;
;   90 HWND      menu_button;
;   91 int 	  menu_objects;
;   92 UINT      menu_msg;
;   93 //int       MMU_IDS;
;   94 HWND      opti_button;
;   95 HWND      plus_button;
;   96 HMENU     main_menu;
;   97 WNDPROC	  buttons_proc[10];
;   98 
;   99 
;  100 //
;  101 //	BMP de los botones
;  102 //  1° Normal 2° Abajo 3° HotTracking
;  103 //	---------------------------------
;  104 //	Para main
;  105 HBITMAP 	g_hBmpUp;
;  106 HBITMAP 	g_hBmpDown;
;  107 HBITMAP 	g_hBmpHot;
;  108 //	---------------------------------
;  109 //	Para plus
;  110 
;  111 HBITMAP 	plus_up;
;  112 HBITMAP 	plus_dw;
;  113 HBITMAP 	plus_ht;
;  114 
;  115 FILE* outfile;
;  116 //	-------------------------------
;  117 //	Para opciones
;  118 
;  119 HBITMAP 	opti_up;
;  120 HBITMAP 	opti_dw;
;  121 HBITMAP 	opti_ht;
;  122 
;  123 //Bitmaps de uso global y sistemas de redibujo
;  124 
;  125 LPBITMAPINFO         	lpBitmap;
;  126 LPTSTR               	lpBits;
;  127 HBITMAP 				MainBitmap;
;  128 
;  129 #define RT_BITMAP	MAKEINTRESOURCE(2)
;  130 #define MAIN_BUT    	33033;
;  131 int  MAX_CLASS_NAME =	256;
;  132 
;  133 
;  134 int			g_BmpWidth;
;  135 int			g_BmpHeight;
;  136 
;  137 
;  138 //
;  139 //	Menu Globals & funcs
;  140 //
;  141 static MENU_DATA get_menu_formWID (HMENU , UINT);
;  142 static LPMENU_DATA redim_items      (LPMENU_DATA , int );
;  143 HMENU menu_builder                  (HWND);
;  144 int draw_buttons                    (HWND , UINT , WPARAM , LPARAM );
;  145 int draw_menu                       (HWND , UINT , WPARAM , LPARAM );
;  146 
;  147 static HMENU FindMenuFromID         (HMENU , UINT );
;  148 int menuType_fromID                 (HMENU , UINT );
;  149 UINT ISmenu_root                    (UINT);
;  150 
;  151 void DrawTransparentBitmap          (HDC , HBITMAP , short , short , COLORREF );
;  152 void drawIconToBitmap               (HBITMAP , char * , int  );
;  153 HBITMAP CreateBitmapMask            (HBITMAP , COLORREF );
;  154 static HFONT CreateAngledFont       (int , int, LPCTSTR);
;  155 //
;  156 //	XML Loader & Parser Uses COM MSXML
;  157 //
;  158 int LoadXML(char * , int );
;  159 int parse_XML(IDispatch*, HMENU );
;  160 //	Function defines
;  161 
;  162 //	----------------------------------------
;  163 //	Core Hooking functions proto & variables
;  164 //	----------------------------------------
;  165 HHOOK	hoo_handle;
;  166 WNDPROC old_menu_proc;
;  167 HANDLE  mnu_hwnd;
;  168 HWND    damm_handle;
;  169 const TCHAR _WndPropName_OldProc[] = "XPWndProp_OldProc";
;  170 const TCHAR _WndPropName_MenuXP[] =  "XPWndProp_MenuXP";
;  171 
;  172 
;  173 //	----------------------------------------
;  174 void initHook();
;  175 void unitHook();
;  176 LRESULT CALLBACK install_Hook        (int code, WPARAM wParam, LPARAM lParam);
;  177 LRESULT CALLBACK Sub_Class_MenuProc (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
;  178 LRESULT CALLBACK menudraw_sclass    (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
;  179 LRESULT CALLBACK menuproc_sclass    (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
;  180 
;  181 //
;  182 //	core window
;  183 //
;  184 LRESULT CALLBACK MainWndProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam);
;  185 
;  186 //	------------
;  187 //	Program code
;  188 //	------------
;  189 
;  190 
;  191 
;  192 //	Registro de la clase para la ventana
;  193 static BOOL InitApplication(void)
	.type	_InitApplication,function
_InitApplication:
	pushl	%ebp
	movl	%esp,%ebp
	subl	$64,%esp
;  194 {
	.line	194
;  195 	WNDCLASS wc;
;  196 
;  197 	memset(&wc,0,sizeof(WNDCLASS));
	.line	197
	pushl	$40
	pushl	$0
	leal	-40(%ebp),%eax
	pushl	%eax
	call	_memset
	addl	$12,%esp
;  198 
;  199 	wc.style         = CS_HREDRAW|CS_VREDRAW |CS_DBLCLKS ;
	.line	199
	movl	$11,-40(%ebp)
;  200 	wc.lpfnWndProc   = (WNDPROC)MainWndProc;
	.line	200
	leal	_MainWndProc@16,%eax
	movl	%eax,-36(%ebp)
;  201 	wc.hInstance     = hInst;
	.line	201
	movl	_hInst,%eax
	movl	%eax,-24(%ebp)
;  202 	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);
	.line	202
	movl	$0x5,-12(%ebp)
;  203 	wc.lpszClassName = "main_menu_WndClass";
	.line	203
	leal	_$185,%eax
	movl	%eax,-4(%ebp)
;  204 	wc.lpszMenuName  = NULL; //MAKEINTRESOURCE (IDMAINMENU);
	.line	204
	andl	$0,-8(%ebp)
;  205 	wc.hCursor       = LoadCursor      (NULL,IDC_ARROW);
	.line	205
	pushl	$0x7f00
	pushl	$0
	call	_LoadCursorA@8
	movl	%eax,-16(%ebp)
;  206 	wc.hIcon         = 0; //LoadIcon        (NULL,IDI_APPLICATION);
	.line	206
	andl	$0,-20(%ebp)
;  207 
;  208 	if (!RegisterClass(&wc))
	.line	208
	leal	-40(%ebp),%eax
	pushl	%eax
	call	_RegisterClassA@4
	orw	%eax,%eax
	jne	_$189
;  209 		return 0;
	.line	209
	xor	%eax,%eax
	jmp	_$180
_$189:
;  210 
;  211 	//	Load resurces
;  212 	//	can be edited externaly using a resource hacker like tool
;  213 
;  214 	g_hBmpUp    = LoadImage(hInst, MAKEINTRESOURCE(ID_BMPUP), IMAGE_BITMAP,
	.line	214
	pushl	$4096
	pushl	$0
	pushl	$0
	pushl	$0
	pushl	$0x398
	pushl	_hInst
	call	_LoadImageA@24
	movl	%eax,_g_hBmpUp
;  215 												0, 0, LR_LOADMAP3DCOLORS);
;  216 
;  217 	g_hBmpDown  = LoadImage(hInst, MAKEINTRESOURCE(ID_BMPDOWN), IMAGE_BITMAP,
	.line	217
	pushl	$4096
	pushl	$0
	pushl	$0
	pushl	$0
	pushl	$0x399
	pushl	_hInst
	call	_LoadImageA@24
	movl	%eax,_g_hBmpDown
;  218 												0, 0, LR_LOADMAP3DCOLORS);
;  219 
;  220 	g_hBmpHot   = LoadImage(hInst, MAKEINTRESOURCE(ID_BMPHOT), IMAGE_BITMAP,
	.line	220
	pushl	$4096
	pushl	$0
	pushl	$0
	pushl	$0
	pushl	$0x39a
	pushl	_hInst
	call	_LoadImageA@24
	movl	%eax,_g_hBmpHot
;  221 												0, 0, LR_LOADMAP3DCOLORS);
;  222 
;  223 	MainBitmap  = LoadImage(hInst, MAKEINTRESOURCE(12346), IMAGE_BITMAP,
	.line	223
	pushl	$4096
	pushl	$0
	pushl	$0
	pushl	$0
	pushl	$0x303a
	pushl	_hInst
	call	_LoadImageA@24
	movl	%eax,_MainBitmap
;  224 												0, 0, LR_LOADMAP3DCOLORS);
;  225 
;  226 	//
;  227 	//	Boton de Plus
;  228 	//
;  229 	plus_up 	= LoadImage(hInst, MAKEINTRESOURCE(ID_PLUS1), IMAGE_BITMAP,
	.line	229
	pushl	$4096
	pushl	$0
	pushl	$0
	pushl	$0
	pushl	$0x3a2
	pushl	_hInst
	call	_LoadImageA@24
	movl	%eax,_plus_up
;  230 												0, 0, LR_LOADMAP3DCOLORS);
;  231 	plus_ht 	= LoadImage(hInst, MAKEINTRESOURCE(ID_PLUS2), IMAGE_BITMAP,
	.line	231
	pushl	$4096
	pushl	$0
	pushl	$0
	pushl	$0
	pushl	$0x3a3
	pushl	_hInst
	call	_LoadImageA@24
	movl	%eax,_plus_ht
;  232 												0, 0, LR_LOADMAP3DCOLORS);
;  233 	plus_dw 	= LoadImage(hInst, MAKEINTRESOURCE(ID_PLUS3), IMAGE_BITMAP,
	.line	233
	pushl	$4096
	pushl	$0
	pushl	$0
	pushl	$0
	pushl	$0x3a4
	pushl	_hInst
	call	_LoadImageA@24
	movl	%eax,_plus_dw
;  234 												0, 0, LR_LOADMAP3DCOLORS);
;  235 
;  236 	opti_up		= LoadImage(hInst, MAKEINTRESOURCE(ID_OPTI1), IMAGE_BITMAP,
	.line	236
	pushl	$4096
	pushl	$0
	pushl	$0
	pushl	$0
	pushl	$0x3ac
	pushl	_hInst
	call	_LoadImageA@24
	movl	%eax,_opti_up
;  237 												0, 0, LR_LOADMAP3DCOLORS);
;  238 	opti_ht		= LoadImage(hInst, MAKEINTRESOURCE(ID_OPTI2), IMAGE_BITMAP,
	.line	238
	pushl	$4096
	pushl	$0
	pushl	$0
	pushl	$0
	pushl	$0x3ad
	pushl	_hInst
	call	_LoadImageA@24
	movl	%eax,_opti_ht
;  239 												0, 0, LR_LOADMAP3DCOLORS);
;  240 	opti_dw		= LoadImage(hInst, MAKEINTRESOURCE(ID_OPTI3), IMAGE_BITMAP,
	.line	240
	pushl	$4096
	pushl	$0
	pushl	$0
	pushl	$0
	pushl	$0x3ae
	pushl	_hInst
	call	_LoadImageA@24
	movl	%eax,_opti_dw
;  241 												0, 0, LR_LOADMAP3DCOLORS);
;  242 
;  243 
;  244 	BITMAP bm;
;  245 
;  246 	// Get width & height
;  247 	GetObject(g_hBmpUp, sizeof(BITMAP), &bm);
	.line	247
	leal	-64(%ebp),%eax
	pushl	%eax
	pushl	$24
	pushl	_g_hBmpUp
	call	_GetObjectA@12
;  248 
;  249 	g_BmpWidth  = bm.bmWidth;
	.line	249
	movl	-60(%ebp),%eax
	movl	%eax,_g_BmpWidth
;  250 	g_BmpHeight = bm.bmHeight;
	.line	250
	movl	-56(%ebp),%eax
	movl	%eax,_g_BmpHeight
;  251 
;  252 	return 1;
	.line	252
	xor	%eax,%eax
	inc	%eax
_$180:
;  253 }
	.line	253
	leave
	ret
_$205:
	.size	_InitApplication,_$205-_InitApplication
;  254 
;  255 //	---------------
;  256 //	Crea la ventana
;  257 //	---------------
;  258 
;  259 HWND Createmain_menu_WndClassWnd(void)
	.type	_Createmain_menu_WndClassWnd,function
_Createmain_menu_WndClassWnd:
	pushl	%ebp
	movl	%esp,%ebp
;  260 {
	.line	260
;  261 		return CreateWindowEx
	.line	261
	pushl	$0
	pushl	_hInst
	pushl	$0
	pushl	$0
	pushl	$40
	pushl	$115
	pushl	$100
	pushl	$100
	pushl	$0x80000000
	pushl	$_$207
	pushl	$_$185
	pushl	$0
	call	_CreateWindowExA@48
;  262 		(
;  263 		WS_EX_LEFT,
;  264 		"main_menu_WndClass",
;  265 		"Título",
;  266 		WS_POPUP,	//	style
;  267 		100,		//	pos x of win
;  268 		100 ,		//	pos y of win,
;  269 		115,
;  270 		40,
;  271         	NULL,
;  272 		NULL,
;  273 		hInst,
;  274 		NULL
;  275 		);
;  276 }
	.line	276
	leave
	ret
_$210:
	.size	_Createmain_menu_WndClassWnd,_$210-_Createmain_menu_WndClassWnd
	.globl	_Createmain_menu_WndClassWnd
;  277 
;  278 //	--------------------------
;  279 //	Handle command subroutine
;  280 //	--------------------------
;  281 void MainWndProc_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
	.type	_MainWndProc_OnCommand,function
_MainWndProc_OnCommand:
	pushl	%ebp
	movl	%esp,%ebp
	subl	$100,%esp
	pushl	%ebx
	pushl	%esi
	pushl	%edi
;  282 {
	.line	282
	movl	12(%ebp),%ebx
;  283 	HMENU	hMenu_ret;
;  284 
;  285 	BOOL    ret_u;
;  286 	POINT   point;
;  287 	RECT    rc_1;
;  288 
;  289 	MENU_DATA    menu_data;
;  290 
;  291 	char * texto = malloc(256);
	.line	291
	pushl	$256
	call	_malloc
	popl	%ecx
	movl	%eax,-60(%ebp)
;  292 	int ret;
;  293 
;  294 	switch(id) {
	.line	294
	movl	%ebx,%eax
	cmpl	$300,%eax
	je	_$215
	jl	_$213
	cmpl	$330,%eax
	je	_$216
	cmpl	$331,%eax
	je	_$218
	cmpl	$332,%eax
	je	_$213
	jmp	_$213
_$215:
;  295 		case IDM_EXIT:
;  296 			PostMessage(hwnd,WM_CLOSE,0,0);
	.line	296
	pushl	$0
	pushl	$0
	pushl	$16
	pushl	8(%ebp)
	call	_PostMessageA@16
;  297 		break;
	.line	297
	jmp	_$213
_$216:
;  298 
;  299 		case 330:
;  300 			hMenu_ret = menu_builder(hwnd);
	.line	300
	pushl	8(%ebp)
	call	_menu_builder
	popl	%ecx
	movl	%eax,-88(%ebp)
;  301 			ClientToScreen(menu_button, &point);
	.line	301
	leal	-68(%ebp),%eax
	pushl	%eax
	pushl	_menu_button
	call	_ClientToScreen@8
;  302 			GetWindowRect(hwnd,&rc_1);
	.line	302
	leal	-84(%ebp),%eax
	pushl	%eax
	pushl	8(%ebp)
	call	_GetWindowRect@8
;  303 			point = point;
	.line	303
	leal	-68(%ebp),%edi
	leal	-68(%ebp),%esi
	movsl
	movsl
;  304 			//HMENU menu = LoadMenu(hInst,MAKEINTRESOURCE(IDMAINMENU));
;  305 			ret_u = TrackPopupMenu(hMenu_ret,TPM_LEFTALIGN | TPM_RETURNCMD | TPM_NONOTIFY, rc_1.left, (rc_1.top + 42), 0, main_window, 0);
	.line	305
	pushl	$0
	pushl	_main_window
	pushl	$0
	movl	-80(%ebp),%eax
	addl	$42,%eax
	pushl	%eax
	pushl	-84(%ebp)
	pushl	$384
	pushl	-88(%ebp)
	call	_TrackPopupMenu@28
	movl	%eax,-92(%ebp)
;  306 			SendMessage(main_window, WM_COMMAND, ret_u, (LPARAM)0);
	.line	306
	pushl	$0
	pushl	%eax
	pushl	$273
	pushl	_main_window
	call	_SendMessageA@16
;  307 
;  308 		break;
	.line	308
	jmp	_$213
_$218:
;  309 		case 331:
;  310 		exit(1);
	.line	310
	pushl	$1
	call	_exit
	popl	%ecx
;  311 		break;
;  312 		case 332:
;  313 		//fclose(outfile);
;  314 		break;
	.line	314
;  315 
;  316 
;  317 		default:
;  318 		{
;  319 
;  320 
;  321 		break;
	.line	321
_$213:
;  322 		}
;  323 	}
;  324 //	------------------------
;  325 //	Handle the dynamic menu
;  326 //	------------------------
;  327 
;  328 		if(id >= 1900)
	.line	328
	cmpl	$1900,%ebx
	jl	_$221
;  329 		{
;  330 		menu_data = get_menu_formWID(main_menu,id);
	.line	330
	pushl	%ebx
	pushl	_main_menu
	leal	-56(%ebp),%eax
	pushl	%eax
	call	_get_menu_formWID
	addl	$12,%esp
;  331 		//MessageBox(hwnd,menu_data.menu_capti,"Error de tipo:",MB_OK);
;  332 		ShellExecute(0, "open", menu_data.menu_exec, "", "", SW_SHOW);
	.line	332
	pushl	$5
	pushl	$_$226
	pushl	$_$226
	pushl	-36(%ebp)
	pushl	$_$224
	pushl	$0
	call	_ShellExecuteA@24
_$221:
;  333 		}
;  334 }
	.line	334
	popl	%edi
	popl	%esi
	popl	%ebx
	leave
	ret
_$230:
	.size	_MainWndProc_OnCommand,_$230-_MainWndProc_OnCommand
	.globl	_MainWndProc_OnCommand
	.bss
	.align	2
	.type	_$S1,object
	.lcomm	_$S1,4
	.data
	.align	2
	.type	_$S2,object
_$S2:
	.long	0
	.text
;  335 
;  336 LRESULT CALLBACK MainWndProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam)
	.type	_MainWndProc@16,function
_MainWndProc@16:
	pushl	%ebp
	movl	%esp,%ebp
	subl	$164,%esp
	pushl	%ebx
	pushl	%esi
	pushl	%edi
;  337 {
	.line	337
;  338 	static HANDLE 	hRes;
;  339 	HDC 			hdc;
;  340 	HDC     		mDC;
;  341 	RECT 			rd;
;  342 	int     		x;
;  343 	int                     idx;
;  344 	static BOOL			open_file = FALSE;
;  345 	BOOL   def_result = 0;
	.line	345
	andl	$0,-8(%ebp)
;  346 	/*
;  347 	if(!open_file)
;  348 	{
;  349 	outfile = fopen("msgout.txt","wb");
;  350     	if(!outfile)
;  351     	{
;  352         	//printf("Error writing to file %s\n",outname);
;  353         	//exit(1);
;  354     	}
;  355 	open_file = TRUE;
;  356 	}
;  357 	*/
;  358 	//fprintf(outfile,"MSG:\t%d\t wParam:\t%d\t lParam\t%d\n",msg,idx);
;  359 
;  360 
;  361 	switch (msg) {
	.line	361
	movl	12(%ebp),%eax
	cmpl	$123,%eax
	je	_$235
	jg	_$260
	cmpl	$15,%eax
	je	_$240
	jg	_$262
	cmpl	$1,%eax
	je	_$237
	cmpl	$2,%eax
	je	_$258
	jmp	_$232
_$262:
	cmpl	$43,%eax
	je	_$252
	cmpl	$44,%eax
	je	_$244
	jmp	_$232
_$260:
	cmpl	$513,%eax
	je	_$239
	jg	_$264
	cmpl	$273,%eax
	je	_$236
	jmp	_$232
_$264:
	cmpl	$534,%eax
	je	_$242
	jmp	_$232
_$235:
;  362 	/*
;  363 	case 293: //WM_MENUSELECT:
;  364 		{
;  365 		fprintf(outfile,"-----------------------------------------------------------------------\n");
;  366 		fprintf(outfile,"Esoteric:\t%d\twParam:\t%d\n",msg,wParam);
;  367 		fprintf(outfile,"-----------------------------------------------------------------------\n\n");
;  368 		UINT mem_buff[1024];
;  369 		CopyMemory(&mem_buff,&lParam,256);
;  370 		int lines;
;  371 		for(int j = 0;j<256;j++)
;  372 		{
;  373 		fprintf(outfile,"[%u]",mem_buff[j]);
;  374 		lines++;
;  375 		if(lines > 10){fprintf(outfile,"\n"); lines = 0; }
;  376 		}
;  377 		fprintf(outfile,"\n-----------------------------------------------------------------------\n\n");
;  378 		return 0;
;  379 		}
;  380 	*/
;  381 	case WM_CONTEXTMENU :
;  382 	{
	.line	382
;  383 	//HMENU hmenu = GetMenu(main_menu);
;  384 	TrackPopupMenuEx(main_menu, TPM_RIGHTBUTTON, LOWORD(lParam), HIWORD(lParam), hwnd, NULL);
	.line	384
	pushl	$0
	pushl	8(%ebp)
	movl	20(%ebp),%esi
	movl	%esi,%ebx
	shrl	$16,%ebx
	andl	$0xffff,%ebx
	movzwl	%bx,%ebx
	pushl	%ebx
	movzwl	%si,%esi
	pushl	%esi
	pushl	$2
	pushl	_main_menu
	call	_TrackPopupMenuEx@24
;  385 	break;
	.line	385
	jmp	_$233
_$236:
;  386 	}
;  387 
;  388 	case WM_COMMAND:
;  389 
;  390 		HANDLE_WM_COMMAND(hwnd,wParam,lParam,MainWndProc_OnCommand);
	.line	390
	movl	16(%ebp),%esi
	movl	%esi,%ebx
	shrl	$16,%ebx
	andl	$0xffff,%ebx
	movzwl	%bx,%ebx
	pushl	%ebx
	pushl	20(%ebp)
	movzwl	%si,%esi
	pushl	%esi
	pushl	8(%ebp)
	call	_MainWndProc_OnCommand
	addl	$16,%esp
;  391 		break;
	.line	391
	jmp	_$233
_$237:
;  392 
;  393 	case WM_CREATE:
;  394 		//
;  395 		//	Cargar los recursos
;  396 		//
;  397 		hRes = LoadResource(hInst,FindResource(hInst, "#12346", RT_BITMAP));
	.line	397
	pushl	$0x2
	pushl	$_$238
	pushl	_hInst
	call	_FindResourceA@12
	pushl	%eax
	pushl	_hInst
	call	_LoadResource@8
	movl	%eax,_$S1
;  398 
;  399 		lpBitmap = (LPBITMAPINFO) LockResource(hRes);
	.line	399
	pushl	%eax
	call	_LockResource@4
	movl	%eax,_lpBitmap
;  400  		lpBits   = (LPTSTR) lpBitmap;
	.line	400
	movl	%eax,_lpBits
;  401 	    	lpBits   += lpBitmap->bmiHeader.biSize + (256 * sizeof(RGBQUAD));
	.line	401
	movl	_lpBitmap,%ecx
	movl	(,%ecx),%eax
	addl	$1024,%eax
	addl	_lpBits,%eax
	movl	%eax,_lpBits
;  402 
;  403 		//
;  404 		//	Creo los botones de control
;  405 		//
;  406 		def_result = create_Controls(hInst, hwnd, 1);
	.line	406
	pushl	$1
	pushl	8(%ebp)
	pushl	_hInst
	call	_create_Controls
	addl	$12,%esp
	movl	%eax,-8(%ebp)
;  407 		break;
	.line	407
	jmp	_$233
_$239:
;  408 
;  409 	case WM_LBUTTONDOWN:
;  410 		//
;  411 		//	Mover la ventana
;  412 		//
;  413 		ReleaseCapture();
	.line	413
	call	_ReleaseCapture@0
;  414         	SendMessage(hwnd, WM_NCLBUTTONDOWN, HTCAPTION, 0);
	.line	414
	pushl	$0
	pushl	$2
	pushl	$161
	pushl	8(%ebp)
	call	_SendMessageA@16
;  415         return 0;
	.line	415
	xor	%eax,%eax
	jmp	_$231
_$240:
;  416 
;  417 	case WM_PAINT:
;  418 		{
	.line	418
;  419 
;  420 		PAINTSTRUCT   pPaint;
;  421     		COLORREF      crColor;
;  422     		crColor   =   RGB(0xff,0,0);
	.line	422
	movl	$255,-104(%ebp)
;  423 
;  424 		HBRUSH   hbrush;
;  425 
;  426 		hbrush  = CreateSolidBrush (crColor);
	.line	426
	pushl	-104(%ebp)
	call	_CreateSolidBrush@4
	movl	%eax,-108(%ebp)
;  427     		hdc 	= BeginPaint(hwnd, &pPaint);
	.line	427
	leal	-100(%ebp),%eax
	pushl	%eax
	pushl	8(%ebp)
	call	_BeginPaint@8
	movl	%eax,-4(%ebp)
;  428 
;  429 		SetStretchBltMode(hdc, 4);
	.line	429
	pushl	$4
	pushl	%eax
	call	_SetStretchBltMode@8
;  430 		rd = MapDialogControlRect(hwnd, 0, FALSE);
	.line	430
	pushl	$0
	pushl	$0
	pushl	8(%ebp)
	leal	-28(%ebp),%eax
	pushl	%eax
	call	_MapDialogControlRect
	addl	$16,%esp
;  431 
;  432 			mDC =  CreateCompatibleDC(hdc);
	.line	432
	pushl	-4(%ebp)
	call	_CreateCompatibleDC@4
	movl	%eax,-12(%ebp)
;  433 			SelectObject(mDC, MainBitmap);
	.line	433
	pushl	_MainBitmap
	pushl	%eax
	call	_SelectObject@8
;  434 
;  435 			BitBlt(hdc, 0, 0, lpBitmap->bmiHeader.biWidth, lpBitmap->bmiHeader.biHeight, mDC, 0, 0, SRCCOPY);
	.line	435
	pushl	$0xcc0020
	pushl	$0
	pushl	$0
	pushl	-12(%ebp)
	movl	_lpBitmap,%eax
	pushl	8(%eax)
	pushl	4(%eax)
	pushl	$0
	pushl	$0
	pushl	-4(%ebp)
	call	_BitBlt@36
;  436 
;  437 			DeleteDC(mDC);
	.line	437
	pushl	-12(%ebp)
	call	_DeleteDC@4
;  438 
;  439 		SetBkMode(hdc, TRANSPARENT);
	.line	439
	pushl	$1
	pushl	-4(%ebp)
	call	_SetBkMode@8
;  440 		ReleaseDC(hwnd, hdc);
	.line	440
	pushl	-4(%ebp)
	pushl	8(%ebp)
	call	_ReleaseDC@8
;  441 		//	Ojo endPaint SIEMPRE AQUI !!!
;  442 		//	be shure to clean this up
;  443     		EndPaint(hwnd, &pPaint);
	.line	443
	leal	-100(%ebp),%eax
	pushl	%eax
	pushl	8(%ebp)
	call	_EndPaint@8
;  444 		break;
	.line	444
	jmp	_$233
_$242:
;  445 		}
;  446 
;  447 
;  448 	case WM_MOVING:
;  449 
;  450 			hdc = GetDC(hwnd);
	.line	450
	pushl	8(%ebp)
	call	_GetDC@4
	movl	%eax,-4(%ebp)
;  451 			SetStretchBltMode(hdc, 4);
	.line	451
	pushl	$4
	pushl	%eax
	call	_SetStretchBltMode@8
;  452 			rd = MapDialogControlRect(hwnd, 0, FALSE);
	.line	452
	pushl	$0
	pushl	$0
	pushl	8(%ebp)
	leal	-28(%ebp),%eax
	pushl	%eax
	call	_MapDialogControlRect
	addl	$16,%esp
;  453 
;  454 			mDC =  CreateCompatibleDC(hdc);
	.line	454
	pushl	-4(%ebp)
	call	_CreateCompatibleDC@4
	movl	%eax,-12(%ebp)
;  455 			SelectObject(mDC, MainBitmap);
	.line	455
	pushl	_MainBitmap
	pushl	%eax
	call	_SelectObject@8
;  456 
;  457 				BitBlt(hdc, 0, 0, lpBitmap->bmiHeader.biWidth, lpBitmap->bmiHeader.biHeight, mDC, 0, 0, SRCCOPY);
	.line	457
	pushl	$0xcc0020
	pushl	$0
	pushl	$0
	pushl	-12(%ebp)
	movl	_lpBitmap,%eax
	pushl	8(%eax)
	pushl	4(%eax)
	pushl	$0
	pushl	$0
	pushl	-4(%ebp)
	call	_BitBlt@36
;  458 
;  459 			DeleteDC(mDC);
	.line	459
	pushl	-12(%ebp)
	call	_DeleteDC@4
;  460 
;  461 			SetBkMode(hdc, TRANSPARENT);
	.line	461
	pushl	$1
	pushl	-4(%ebp)
	call	_SetBkMode@8
;  462 			ReleaseDC(hwnd, hdc);
	.line	462
	pushl	-4(%ebp)
	pushl	8(%ebp)
	call	_ReleaseDC@8
;  463 
;  464 		return 0;
	.line	464
	xor	%eax,%eax
	jmp	_$231
_$244:
;  465 
;  466 		case WM_MEASUREITEM:
;  467 
;  468 		{
	.line	468
;  469 		TEXTMETRIC          tm;
;  470 		LPMEASUREITEMSTRUCT pmis;
;  471 		HDC                 hdc;
;  472 		pmis = (LPMEASUREITEMSTRUCT)lParam;
	.line	472
	movl	20(%ebp),%eax
	movl	%eax,-96(%ebp)
;  473 
;  474 		MENU_DATA menu_data = get_menu_formWID(main_menu,pmis->itemID);
	.line	474
	pushl	8(%eax)
	pushl	_main_menu
	leal	-92(%ebp),%eax
	pushl	%eax
	call	_get_menu_formWID
	addl	$12,%esp
;  475 		
;  476 		
;  477 		//  Seteo medidas w y h del elemento del menu (puede ser independietnte)
;  478 		//  chequear tamano en pixels de la fuente
;  479 		if (pmis->CtlType == ODT_MENU)
	.line	479
	movl	-96(%ebp),%eax
	cmpl	$1,(,%eax)
	jne	_$246
;  480 			{
;  481 			hdc               = GetDC(hwnd);
	.line	481
	pushl	8(%ebp)
	call	_GetDC@4
	movl	%eax,-100(%ebp)
;  482 			GetTextMetrics(hdc, &tm);
	.line	482
	leal	-157(%ebp),%eax
	pushl	%eax
	pushl	-100(%ebp)
	call	_GetTextMetricsA@8
;  483 		UINT is_root = ISmenu_root(pmis->itemID);
	.line	483
	movl	-96(%ebp),%eax
	pushl	8(%eax)
	call	_ISmenu_root
	popl	%ecx
	movl	%eax,-164(%ebp)
;  484 			if(is_root == 1)
	.line	484
	cmpl	$1,%eax
	jne	_$248
;  485 			{
;  486 			pmis->itemHeight  = 40;
	.line	486
	movl	-96(%ebp),%eax
	movl	$40,16(%eax)
;  487 			pmis->itemWidth   = 210;
	.line	487
	movl	$210,12(%eax)
	jmp	_$249
_$248:
;  488 			}
;  489 			else
;  490 			{
	.line	490
;  491 			pmis->itemHeight  = 26;
	.line	491
	movl	-96(%ebp),%eax
	movl	$26,16(%eax)
;  492 			pmis->itemWidth   = 210;
	.line	492
	movl	$210,12(%eax)
_$249:
;  493 			//pmis->itemWidth   = menu_data.menu_width;//210;
;  494 			
;  495 			}
;  496 
;  497 			//Set MF_SEPARATOR height
;  498 			if(pmis->itemData <= 0)
	.line	498
	movl	-96(%ebp),%eax
	cmpl	$0,20(%eax)
	jne	_$250
;  499 				{
;  500 				pmis->itemHeight  = 8;
	.line	500
	movl	$8,16(%eax)
_$250:
;  501 				}
;  502 			ReleaseDC(hwnd, hdc);
	.line	502
	pushl	-100(%ebp)
	pushl	8(%ebp)
	call	_ReleaseDC@8
_$246:
;  503 			}
;  504 
;  505 
;  506 		return 0;
	.line	506
	xor	%eax,%eax
	jmp	_$231
_$252:
;  507 		}
;  508 	case WM_DRAWITEM:
;  509 	{
	.line	509
;  510 	// to avoid messing up this pool we better do this
;  511 	LPDRAWITEMSTRUCT lpDs = (LPDRAWITEMSTRUCT)lParam;
	.line	511
	movl	20(%ebp),%eax
	movl	%eax,-40(%ebp)
;  512 	switch(lpDs->CtlType)
	.line	512
	movl	(,%eax),%eax
	cmpl	$1,%eax
	je	_$257
	cmpl	$4,%eax
	jne	_$254
_$256:
;  513 	{
;  514 	case ODT_BUTTON:
;  515 		draw_buttons(hwnd, msg, wParam, lParam);
	.line	515
	pushl	20(%ebp)
	pushl	16(%ebp)
	pushl	12(%ebp)
	pushl	8(%ebp)
	call	_draw_buttons
	addl	$16,%esp
;  516 		break;
	.line	516
	jmp	_$254
_$257:
;  517 	case ODT_MENU:
;  518 		draw_menu(hwnd, msg, wParam, lParam);
	.line	518
	pushl	20(%ebp)
	pushl	16(%ebp)
	pushl	12(%ebp)
	pushl	8(%ebp)
	call	_draw_menu
	addl	$16,%esp
;  519 		break;
	.line	519
_$254:
;  520 	}
;  521 	return 0;
	.line	521
	xor	%eax,%eax
	jmp	_$231
_$258:
;  522 
;  523 	}
;  524 	case WM_DESTROY:
;  525 		PostQuitMessage(0);
	.line	525
	pushl	$0
	call	_PostQuitMessage@4
;  526 		//windows can handle this any way (i think)
;  527 		unitHook();
	.line	527
	call	_unitHook
;  528 		break;
	.line	528
	jmp	_$233
_$232:
;  529 	default:
;  530 		return DefWindowProc(hwnd,msg,wParam,lParam);
	.line	530
	pushl	20(%ebp)
	pushl	16(%ebp)
	pushl	12(%ebp)
	pushl	8(%ebp)
	call	_DefWindowProcA@16
	jmp	_$231
_$233:
;  531 	}
;  532 	return 0;
	.line	532
	xor	%eax,%eax
_$231:
;  533 }
	.line	533
	popl	%edi
	popl	%esi
	popl	%ebx
	leave
	ret	$16
_$282:
	.size	_MainWndProc@16,_$282-_MainWndProc@16
	.globl	_MainWndProc@16
;  534 
;  535 //	------------------------------
;  536 //	Process windows message queque
;  537 //	------------------------------
;  538 int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, INT nCmdShow)
	.type	_WinMain@16,function
_WinMain@16:
	pushl	%ebp
	movl	%esp,%ebp
	subl	$28,%esp
	pushl	%esi
	pushl	%edi
;  539 {
	.line	539
;  540 	MSG msg;
;  541 	HANDLE hAccelTable;
;  542 
;  543 	hInst = hInstance;
	.line	543
	movl	8(%ebp),%eax
	movl	%eax,_hInst
;  544 	menu_objects = 0;
	.line	544
	andl	$0,_menu_objects
;  545 
;  546 	//Turn it off in debugging instances
;  547 	//other wise you will get GPF's a lot of times
;  548 	//specialy LCC whos debbugger is a bit messy
;  549 	initHook();
	.line	549
	call	_initHook
;  550 
;  551 	if (!InitApplication())
	.line	551
	call	_InitApplication
	or	%eax,%eax
	je	_$283
;  552 		{
;  553 		return 0;
	.line	553
_$284:
;  554 		}
;  555 
;  556 	hAccelTable = LoadAccelerators(hInst,MAKEINTRESOURCE(IDACCEL));
	.line	556
	pushl	$0x64
	pushl	_hInst
	call	_LoadAcceleratorsA@8
	movl	%eax,%edi
;  557 
;  558 	if ((hwndMain = Createmain_menu_WndClassWnd()) == (HWND)0)
	.line	558
	call	_Createmain_menu_WndClassWnd
	movl	%eax,_hwndMain
	or	%eax,%eax
	je	_$283
;  559 		{
;  560 		return 0;
	.line	560
_$286:
;  561 		}
;  562 
;  563 	ShowWindow(hwndMain,SW_SHOW);
	.line	563
	pushl	$5
	pushl	_hwndMain
	call	_ShowWindow@8
;  564 	//
;  565 	//	copia doble del puntero
;  566 	//
;  567 	main_window = hwndMain;
	.line	567
	movl	_hwndMain,%eax
	movl	%eax,_main_window
	jmp	_$289
_$288:
;  568 
;  569 
;  570 	while (GetMessage(&msg,NULL,0,0)) {
;  571 	/*
;  572 	fprintf(outfile,"\n_________________________\n");
;  573 	fprintf(outfile,"Dispach msg:\t%d\t",msg);
;  574 	fprintf(outfile,"\n¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯\n");
;  575 	*/
;  576 		if (!TranslateAccelerator(msg.hwnd,hAccelTable,&msg))
	.line	576
	leal	-28(%ebp),%eax
	pushl	%eax
	pushl	%edi
	pushl	-28(%ebp)
	call	_TranslateAcceleratorA@12
	or	%eax,%eax
	jne	_$289
;  577 		{
;  578 			TranslateMessage(&msg);
	.line	578
	leal	-28(%ebp),%eax
	pushl	%eax
	call	_TranslateMessage@4
;  579 			DispatchMessage(&msg);
	.line	579
	leal	-28(%ebp),%eax
	pushl	%eax
	call	_DispatchMessageA@4
;  580 		}
;  581 	}
	.line	581
_$289:
	.line	570
	pushl	$0
	pushl	$0
	pushl	$0
	leal	-28(%ebp),%eax
	pushl	%eax
	call	_GetMessageA@16
	or	%eax,%eax
	jne	_$288
;  582 	return msg.wParam;
	.line	582
	movl	-20(%ebp),%eax
_$283:
;  583 }
	.line	583
	popl	%edi
	popl	%esi
	leave
	ret	$16
_$300:
	.size	_WinMain@16,_$300-_WinMain@16
	.globl	_WinMain@16
;  584 
;  585 
;  586 //	-----------------------------------------------
;  587 //	Crear los botones de control para la aplicacion
;  588 //	-----------------------------------------------
;  589 
;  590 BOOL create_Controls(HINSTANCE main_inst, HWND hwnd_parent, int skin_tpy)
	.type	_create_Controls,function
_create_Controls:
	pushl	%ebp
	movl	%esp,%ebp
	subl	$24,%esp
	pushl	%ebx
	pushl	%esi
	pushl	%edi
;  591 {
	.line	591
	movl	8(%ebp),%ebx
	movl	12(%ebp),%esi
;  592 
;  593 	 int 		i = 0;
	.line	593
	xor	%edi,%edi
;  594 	 POINT     	menu_bt;
;  595 	 POINT      clos_bt;
;  596 	 POINT      opti_bt;
;  597 
;  598 	//
;  599 	//	330 simbolo inicial comando boton
;  600 	//
;  601 	menu_button = CreateWindow("button", NULL, WS_CHILD | WS_VISIBLE | BS_PUSHLIKE | BS_OWNERDRAW ,
	.line	601
	pushl	$0
	pushl	%ebx
	pushl	$0x14a
	pushl	%esi
	pushl	$30
	pushl	$75
	pushl	$5
	pushl	$5
	pushl	$0x5000100b
	pushl	$0
	pushl	$_$302
	pushl	$0
	call	_CreateWindowExA@48
	movl	%eax,_menu_button
;  602 					5,
;  603 					5,
;  604 				   75,
;  605 				   30,
;  606 					hwnd_parent, (HMENU)(330 + i), main_inst, NULL);
;  607 
;  608 	i++;
	.line	608
	incl	%edi
;  609 	opti_button = CreateWindow("button", NULL, WS_CHILD | WS_VISIBLE | BS_PUSHLIKE | BS_OWNERDRAW,
	.line	609
	pushl	$0
	pushl	%ebx
	movl	%edi,%eax
	addl	$330,%eax
	pushl	%eax
	pushl	%esi
	pushl	$9
	pushl	$9
	pushl	$2
	pushl	$104
	pushl	$0x5000100b
	pushl	$0
	pushl	$_$302
	pushl	$0
	call	_CreateWindowExA@48
	movl	%eax,_opti_button
;  610 				  104,
;  611 					2,
;  612 					9,
;  613 					9,
;  614 					hwnd_parent, (HMENU)(330 + i), main_inst, NULL);
;  615 	i++;
	.line	615
	incl	%edi
;  616     plus_button = CreateWindow("button", NULL, WS_CHILD | WS_VISIBLE | BS_PUSHLIKE | BS_OWNERDRAW,
	.line	616
	pushl	$0
	pushl	%ebx
	movl	%edi,%eax
	addl	$330,%eax
	pushl	%eax
	pushl	%esi
	pushl	$9
	pushl	$9
	pushl	$12
	pushl	$104
	pushl	$0x5000100b
	pushl	$0
	pushl	$_$302
	pushl	$0
	call	_CreateWindowExA@48
	movl	%eax,_plus_button
;  617 				  104,
;  618 				   12,
;  619 					9,
;  620 					9,
;  621 					hwnd_parent, (HMENU)(330 + i), main_inst, NULL);
;  622 
;  623 	//
;  624 	//	Subclasing elements (keep orden)
;  625 	//
;  626 	buttons_proc[1] = (WNDPROC)SetWindowLong(menu_button,GWL_WNDPROC, (DWORD)ButtonProc) ;
	.line	626
	pushl	$_ButtonProc@16
	pushl	$-4
	pushl	_menu_button
	call	_SetWindowLongA@12
	movl	%eax,_buttons_proc+4
;  627 
;  628 	 return 1;
	.line	628
	xor	%eax,%eax
	inc	%eax
;  629 
;  630 
;  631 }
	.line	631
	popl	%edi
	popl	%esi
	popl	%ebx
	leave
	ret
_$308:
	.size	_create_Controls,_$308-_create_Controls
	.globl	_create_Controls
;  632 
;  633 //
;  634 //	Buttons subclased goes here
;  635 //
;  636 LRESULT CALLBACK ButtonProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	.type	_ButtonProc@16,function
_ButtonProc@16:
	pushl	%ebp
	movl	%esp,%ebp
	subl	$388,%esp
	pushl	%esi
	pushl	%edi
;  637 {
	.line	637
	movl	12(%ebp),%edi
;  638 	char    msg[255];
;  639 	int		i;
;  640 	int		j;
;  641 	int		k;
;  642 	int 	x;
;  643 	// para los botones dibujados
;  644 	int		wich_btt;
;  645 
;  646 	BOOL    is_onedraw;
;  647 
;  648 //	static  BOOL        m_is_down;
;  649 //	static 	HBITMAP 	numBMP;
;  650 	BITMAP  bmp;
;  651 
;  652 	RECT			rect;
;  653 	RECT            rcSep;
;  654 	HBRUSH          hBrush;
;  655 
;  656 
;  657 	// WM_MLEAVE message dispacher api need this struct to know what
;  658 	// window to track !!!
;  659 	// bmaggi 2003 for lcc-win32
;  660 
;  661 
;  662 	HDC                         hDC;
;  663 	HDC                         hDC1;
;  664 	HDC                         hDC2;
;  665 	HDC                			hdc;
;  666 	LPMEASUREITEMSTRUCT         pmis;
;  667 
;  668 
;  669 
;  670 	switch (message)
	.line	670
	movl	%edi,%eax
	cmpl	$32,%eax
	je	_$311
	jg	_$337
	cmpl	$1,%eax
	je	_$311
	jl	_$310
	cmpl	$15,%eax
	je	_$326
	jmp	_$310
_$337:
	cmpl	$512,%eax
	je	_$318
	jg	_$340
	cmpl	$43,%eax
	je	_$330
	cmpl	$44,%eax
	je	_$315
	jmp	_$310
_$340:
	cmpl	$675,%eax
	je	_$325
	jmp	_$310
;  671 	{
;  672 
;  673 		case WM_CREATE:
;  674 
;  675 			break;
;  676 		case WM_SETCURSOR:
;  677 			break;
	.line	677
_$315:
;  678 		case WM_MEASUREITEM:
;  679 				MessageBox(hWnd,"Dibujando el Componente","Error de tipo:",MB_OK);
	.line	679
	pushl	$0
	pushl	$_$317
	pushl	$_$316
	pushl	8(%ebp)
	call	_MessageBoxA@16
;  680 		return 0;
	.line	680
	xor	%eax,%eax
	jmp	_$309
_$318:
;  681 
;  682 		case WM_MOUSEMOVE:
;  683 			{
	.line	683
;  684 			hDC = GetDC(menu_button);	// create a memory DC
	.line	684
	pushl	_menu_button
	call	_GetDC@4
	movl	%eax,-4(%ebp)
;  685 
;  686 			if(hDC)
	.line	686
	test	%eax,%eax
	jne	_$320
_$319:
;  687 			{
;  688 				//	Beguin casting members
;  689 				//	selected by their hwnd
;  690 				//SelectObject(hDC, g_hBmpHot);
;  691 			}
;  692 			else
;  693 			{
	.line	693
;  694 				MessageBox(hWnd,"No HDC !!!","Error de tipo:",MB_OK);
	.line	694
	pushl	$0
	pushl	$_$317
	pushl	$_$321
	pushl	8(%ebp)
	call	_MessageBoxA@16
_$320:
;  695 			}
;  696 			//BitBlt(hDC, 0, 0, g_BmpWidth, g_BmpHeight, hDC, 0, 0, SRCCOPY);
;  697 			HDC		mDC;
;  698 			mDC =  CreateCompatibleDC(hDC);
	.line	698
	pushl	-4(%ebp)
	call	_CreateCompatibleDC@4
	movl	%eax,-380(%ebp)
;  699 			SelectObject(mDC, g_hBmpHot);
	.line	699
	pushl	_g_hBmpHot
	pushl	%eax
	call	_SelectObject@8
;  700 
;  701 			BitBlt(hDC, 0, 0, g_BmpWidth, g_BmpHeight, mDC, 0, 0, SRCCOPY);
	.line	701
	pushl	$0xcc0020
	pushl	$0
	pushl	$0
	pushl	-380(%ebp)
	pushl	_g_BmpHeight
	pushl	_g_BmpWidth
	pushl	$0
	pushl	$0
	pushl	-4(%ebp)
	call	_BitBlt@36
;  702 			DeleteDC(mDC);
	.line	702
	pushl	-380(%ebp)
	call	_DeleteDC@4
;  703 			ReleaseDC(menu_button, hDC);
	.line	703
	pushl	-4(%ebp)
	pushl	_menu_button
	call	_ReleaseDC@8
;  704 			//
;  705 			//	Hot Traking activado
;  706 			//
;  707 			TRACKMOUSEEVENT     stTRACKMOUSEEVENT;
;  708 
;  709 			stTRACKMOUSEEVENT.cbSize 		=   sizeof(stTRACKMOUSEEVENT);
	.line	709
	movl	$16,-376(%ebp)
;  710 			stTRACKMOUSEEVENT.hwndTrack		=	hWnd;
	.line	710
	movl	8(%ebp),%eax
	movl	%eax,-368(%ebp)
;  711 			stTRACKMOUSEEVENT.dwFlags		=	TME_LEAVE;
	.line	711
	movl	$2,-372(%ebp)
;  712 			stTRACKMOUSEEVENT.dwHoverTime 	=   HOVER_DEFAULT;
	.line	712
	orl	$-1,-364(%ebp)
;  713 
;  714 			TrackMouseEvent(&stTRACKMOUSEEVENT);
	.line	714
	leal	-376(%ebp),%eax
	pushl	%eax
	call	_TrackMouseEvent@4
;  715 			}
;  716 			break;
	.line	716
	jmp	_$311
_$325:
;  717 		case WM_MOUSELEAVE:
;  718 			{
	.line	718
;  719 				HDC		mDC;
;  720 
;  721 				hDC = GetDC(menu_button);
	.line	721
	pushl	_menu_button
	call	_GetDC@4
	movl	%eax,-4(%ebp)
;  722 				mDC = CreateCompatibleDC(hDC);
	.line	722
	pushl	%eax
	call	_CreateCompatibleDC@4
	movl	%eax,-364(%ebp)
;  723 
;  724 				SelectObject(mDC, g_hBmpUp);
	.line	724
	pushl	_g_hBmpUp
	pushl	%eax
	call	_SelectObject@8
;  725 
;  726 				BitBlt(hDC, 0, 0, g_BmpWidth, g_BmpHeight, mDC, 0, 0, SRCCOPY);
	.line	726
	pushl	$0xcc0020
	pushl	$0
	pushl	$0
	pushl	-364(%ebp)
	pushl	_g_BmpHeight
	pushl	_g_BmpWidth
	pushl	$0
	pushl	$0
	pushl	-4(%ebp)
	call	_BitBlt@36
;  727 
;  728 				DeleteDC(mDC);
	.line	728
	pushl	-364(%ebp)
	call	_DeleteDC@4
;  729 				ReleaseDC(menu_button, hDC);
	.line	729
	pushl	-4(%ebp)
	pushl	_menu_button
	call	_ReleaseDC@8
;  730 
;  731 
;  732 			break;
	.line	732
	jmp	_$311
_$326:
;  733 			}
;  734 
;  735 
;  736 		case WM_PAINT:
;  737 
;  738 				rcSep.top 		= 1;
	.line	738
	movl	$1,-16(%ebp)
;  739 				rcSep.left		= 1;
	.line	739
	movl	$1,-20(%ebp)
;  740 				rcSep.bottom		= 15;
	.line	740
	movl	$15,-8(%ebp)
;  741 				rcSep.right		= 15;
	.line	741
	movl	$15,-12(%ebp)
;  742 
;  743 				hDC = GetDC(hWnd);
	.line	743
	pushl	8(%ebp)
	call	_GetDC@4
	movl	%eax,-4(%ebp)
;  744 				hBrush = CreateSolidBrush(RGB(255, 0, 0));
	.line	744
	pushl	$255
	call	_CreateSolidBrush@4
	movl	%eax,-24(%ebp)
;  745 
;  746 				//FillRect(hDC, &rcSep, hBrush);
;  747 
;  748 				DeleteObject(hBrush);
	.line	748
	pushl	%eax
	call	_DeleteObject@4
;  749 				ReleaseDC(hWnd, hDC);
	.line	749
	pushl	-4(%ebp)
	pushl	8(%ebp)
	call	_ReleaseDC@8
;  750 
;  751 				break;
	.line	751
	jmp	_$311
_$330:
;  752 
;  753 		//	Cant cast drawitem here ???
;  754 		//	el subclass parece no funcionar y el mensaje lo recibe la veentana (parent)
;  755 		//
;  756 		case WM_DRAWITEM:
;  757 
;  758 		{ //  Handles the ownerdraw button
	.line	758
;  759 
;  760 		LPDRAWITEMSTRUCT lpDs = (LPDRAWITEMSTRUCT)lParam;
	.line	760
	movl	20(%ebp),%eax
	movl	%eax,-364(%ebp)
;  761 		HDC 	hDC;
;  762 		RECT 	rcSep;
;  763 		HBRUSH  hBrush;
;  764 
;  765 		hDC = CreateCompatibleDC(lpDs->hDC);	// create a memory DC
	.line	765
	pushl	24(%eax)
	call	_CreateCompatibleDC@4
	movl	%eax,-368(%ebp)
;  766 
;  767 		if(hDC){
	.line	767
	test	%eax,%eax
	je	_$331
;  768 			if(lpDs->itemState & ODS_SELECTED) //  if button is down
	.line	768
	movl	-364(%ebp),%eax
	testb	$1,16(%eax)
	je	_$333
;  769 			{
;  770 				SelectObject(hDC, g_hBmpUp);
	.line	770
	pushl	_g_hBmpUp
	pushl	-368(%ebp)
	call	_SelectObject@8
	jmp	_$334
_$333:
;  771 			}
;  772 			else
;  773 			{
	.line	773
;  774 				SelectObject(hDC, g_hBmpDown);
	.line	774
	pushl	_g_hBmpDown
	pushl	-368(%ebp)
	call	_SelectObject@8
_$334:
;  775 			}
;  776 
;  777 				BitBlt(lpDs->hDC, 0, 0, g_BmpWidth, g_BmpHeight, hDC, 0, 0, SRCCOPY);
	.line	777
	pushl	$0xcc0020
	pushl	$0
	pushl	$0
	pushl	-368(%ebp)
	pushl	_g_BmpHeight
	pushl	_g_BmpWidth
	pushl	$0
	pushl	$0
	movl	-364(%ebp),%eax
	pushl	24(%eax)
	call	_BitBlt@36
;  778 				DeleteDC(hDC);
	.line	778
	pushl	-368(%ebp)
	call	_DeleteDC@4
_$331:
;  779 			}
;  780 		    return 1;
	.line	780
	xor	%eax,%eax
	inc	%eax
	jmp	_$309
_$310:
;  781 		}
;  782 
;  783 		default:
;  784 			return CallWindowProc(buttons_proc[1], hWnd, message, wParam, lParam) ;
	.line	784
	pushl	20(%ebp)
	pushl	16(%ebp)
	pushl	%edi
	pushl	8(%ebp)
	pushl	_buttons_proc+4
	call	_CallWindowProcA@20
	jmp	_$309
_$311:
;  785 	}
;  786 
;  787 			return CallWindowProc(buttons_proc[1], hWnd, message, wParam, lParam) ;
	.line	787
	pushl	20(%ebp)
	pushl	16(%ebp)
	pushl	%edi
	pushl	8(%ebp)
	pushl	_buttons_proc+4
	call	_CallWindowProcA@20
_$309:
;  788 
;  789 }
	.line	789
	popl	%edi
	popl	%esi
	leave
	ret	$16
_$349:
	.size	_ButtonProc@16,_$349-_ButtonProc@16
	.globl	_ButtonProc@16
;  790 
;  791 //
;  792 //	Helper func provided (?)
;  793 //
;  794 RECT MapDialogControlRect(HWND hWnd, int id, BOOL flag)
	.type	_MapDialogControlRect,function
_MapDialogControlRect:
	pushl	%ebp
	movl	%esp,%ebp
	subl	$60,%esp
	pushl	%ebx
	pushl	%esi
	pushl	%edi
;  795 {
	.line	795
	movl	12(%ebp),%ebx
;  796 	int w, h;
;  797 	RECT rw, rc, rect;
;  798 
;  799 	if (GetWindowLong(hWnd, GWL_STYLE) & WS_SYSMENU)
	.line	799
	pushl	$-16
	pushl	%ebx
	call	_GetWindowLongA@8
	testl	$0x80000,%eax
	je	_$352
;  800 	{
;  801 		w = GetSystemMetrics(SM_CXDLGFRAME);
	.line	801
	pushl	$7
	call	_GetSystemMetrics@4
	movl	%eax,-52(%ebp)
;  802 		h = GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYDLGFRAME);
	.line	802
	pushl	$4
	call	_GetSystemMetrics@4
	movl	%eax,-60(%ebp)
	pushl	$8
	call	_GetSystemMetrics@4
	movl	-60(%ebp),%edi
	addl	%eax,%edi
	movl	%edi,-56(%ebp)
	jmp	_$353
_$352:
;  803 	}
;  804 	else if (GetWindowLong(hWnd, GWL_STYLE) & WS_DLGFRAME)
	.line	804
	pushl	$-16
	pushl	%ebx
	call	_GetWindowLongA@8
	testl	$0x400000,%eax
	je	_$354
;  805 	{
;  806 		w = GetSystemMetrics(SM_CXDLGFRAME);
	.line	806
	pushl	$7
	call	_GetSystemMetrics@4
	movl	%eax,-52(%ebp)
;  807 		h = GetSystemMetrics(SM_CYDLGFRAME);
	.line	807
	pushl	$8
	call	_GetSystemMetrics@4
	movl	%eax,-56(%ebp)
	jmp	_$353
_$354:
;  808 	}
;  809 	else  if (GetWindowLong(hWnd, GWL_STYLE) & WS_BORDER)
	.line	809
	pushl	$-16
	pushl	%ebx
	call	_GetWindowLongA@8
	testl	$0x800000,%eax
	je	_$353
;  810 	{
;  811 		w = GetSystemMetrics(SM_CXBORDER);
	.line	811
	pushl	$5
	call	_GetSystemMetrics@4
	movl	%eax,-52(%ebp)
;  812 		h = GetSystemMetrics(SM_CYBORDER);
	.line	812
	pushl	$6
	call	_GetSystemMetrics@4
	movl	%eax,-56(%ebp)
;  813 	}
;  814 
;  815 	GetWindowRect(hWnd, &rw);
	.line	815
_$353:
	leal	-32(%ebp),%eax
	pushl	%eax
	pushl	%ebx
	call	_GetWindowRect@8
;  816 	GetWindowRect(GetDlgItem(hWnd, id), &rc);
	.line	816
	pushl	16(%ebp)
	pushl	%ebx
	call	_GetDlgItem@8
	leal	-48(%ebp),%edi
	pushl	%edi
	pushl	%eax
	call	_GetWindowRect@8
;  817 
;  818 	if (id == 0)
	.line	818
	cmpl	$0,16(%ebp)
	jne	_$358
;  819 	{
;  820 		GetClientRect(hWnd, &rect);
	.line	820
	leal	-16(%ebp),%eax
	pushl	%eax
	pushl	%ebx
	call	_GetClientRect@8
;  821 		rect.left = 0;
	.line	821
	andl	$0,-16(%ebp)
;  822 	    rect.top  = 0;
	.line	822
	andl	$0,-12(%ebp)
	jmp	_$359
_$358:
;  823 	}
;  824 	else
;  825 	{
	.line	825
;  826 		GetClientRect(GetDlgItem(hWnd, id), &rect);
	.line	826
	pushl	16(%ebp)
	pushl	%ebx
	call	_GetDlgItem@8
	leal	-16(%ebp),%edi
	pushl	%edi
	pushl	%eax
	call	_GetClientRect@8
;  827 		rect.left = rc.left - rw.left - w;
	.line	827
	movl	-48(%ebp),%eax
	subl	-32(%ebp),%eax
	subl	-52(%ebp),%eax
	movl	%eax,-16(%ebp)
;  828 		rect.top  = rc.top - rw.top - h;
	.line	828
	movl	-44(%ebp),%ecx
	subl	-28(%ebp),%ecx
	subl	-56(%ebp),%ecx
	movl	%ecx,-12(%ebp)
;  829 		if (flag)
	.line	829
	cmpl	$0,20(%ebp)
	je	_$359
;  830 		{
;  831 			rect.right  = rect.left + rect.right;
	.line	831
	addl	-8(%ebp),%eax
	movl	%eax,-8(%ebp)
;  832 			rect.bottom = rect.top + rect.bottom;
	.line	832
	movl	%ecx,%eax
	addl	-4(%ebp),%eax
	movl	%eax,-4(%ebp)
;  833 		}
;  834 	}
	.line	834
_$359:
;  835 
;  836 	return (rect);
	.line	836
	movl	8(%ebp),%edi
	leal	-16(%ebp),%esi
	movl	$4,%ecx
	rep
	movsl
;  837 }
	.line	837
	popl	%edi
	popl	%esi
	popl	%ebx
	leave
	ret
_$384:
	.size	_MapDialogControlRect,_$384-_MapDialogControlRect
	.globl	_MapDialogControlRect
	.data
	.align	2
	.type	_$S3,object
_$S3:
	.long	0
	.text
;  838 
;  839 //
;  840 //	Constructor del menu
;  841 //
;  842 HMENU menu_builder(HWND parent)
	.type	_menu_builder,function
_menu_builder:
	pushl	%ebp
	movl	%esp,%ebp
	subl	$12,%esp
;  843 {
	.line	843
;  844 	HMENU	hmenu_1;
;  845 	int     h_ref;
;  846 	BOOL    ret_u;
;  847 
;  848 	static BOOL menu_is_done = FALSE;
;  849 	//load once
;  850 	if(!menu_is_done)
	.line	850
	cmpl	$0,_$S3
	jne	_$386
;  851 	{
;  852 		//main_menu = CreatePopupMenu();
;  853 		//SetParent(main_menu,main_window);
;  854 		//main_menu = LoadMenu(hInst,MAKEINTRESOURCE(IDMAINMENU));
;  855 		main_menu = CreatePopupMenu();
	.line	855
	call	_CreatePopupMenu@0
	movl	%eax,_main_menu
;  856 		//main_menu = CreateMenu();
;  857 		//hmenu_1   = CreatePopupMenu();
;  858 		dhInitialize(TRUE);
	.line	858
	pushl	$0
	pushl	$1
	call	_dhInitializeImp
;  859 		dhToggleExceptions(TRUE);
	.line	859
	pushl	$1
	call	_dhToggleExceptions
;  860 		/*
;  861 		fprintf(outfile,"---------------------------------------------\n");
;  862 		fprintf(outfile,"Inciando MAIN MENU\n");
;  863 		fprintf(outfile,"---------------------------------------------\n");
;  864 		fprintf(outfile,"main_menu:\t%d\n\n",main_menu);
;  865 		*/
;  866 		h_ref = LoadXML("menu.xml", 256);
	.line	866
	pushl	$256
	pushl	$_$388
	call	_LoadXML
	movl	%eax,-4(%ebp)
;  867 
;  868 		dhUninitialize(TRUE);
	.line	868
	pushl	$1
	call	_dhUninitialize
	addl	$24,%esp
;  869 
;  870 		//AppendMenu(hmenu_1,MF_MENUBREAK | MF_OWNERDRAW ,1005, "hola");
;  871 		//AppendMenu(main_menu,MF_POPUP| MF_OWNERDRAW ,(unsigned int)hmenu_1, "pop up");
;  872 		//FindMenuFromID(main_menu,1);
;  873 		menu_is_done = TRUE;
	.line	873
	movl	$1,_$S3
_$386:
;  874 	}
;  875 	return main_menu;
	.line	875
	movl	_main_menu,%eax
;  876 
;  877 }
	.line	877
	leave
	ret
_$391:
	.size	_menu_builder,_$391-_menu_builder
	.globl	_menu_builder
;  878 
;  879 //	------------------
;  880 //	Load the XML file
;  881 //	------------------
;  882 int LoadXML(char * fname, int flags)
	.type	_LoadXML,function
_LoadXML:
	pushl	%ebp
	movl	%esp,%ebp
	pushl	%ecx
	pushl	%eax
	pushl	%ebx
	pushl	%edi
;  883 {
	.line	883
;  884 	DISPATCH_OBJ(xmlDoc);
	.line	884
	andl	$0,-4(%ebp)
;  885 	DISPATCH_OBJ(NodeList);
	.line	885
	andl	$0,-8(%ebp)
;  886 	HR_TRY( dhCreateObject(L"MSXML.DOMDocument", NULL, &xmlDoc) );
	.line	886
	leal	-4(%ebp),%eax
	pushl	%eax
	pushl	$0
	pushl	$_$395
	call	_dhCreateObject
	addl	$12,%esp
	xorl	%edi,%edi
	cmpl	%edi,%eax
	xchgl	%edi,%eax
	setl	%al
	xchgl	%eax,%edi
	or	%edi,%edi
	jne	_$399
_$393:
;  887 	HR_TRY( dhPutValue(xmlDoc, L".Async = %b", FALSE) );
	.line	887
	pushl	$0
	pushl	$_$402
	pushl	-4(%ebp)
	call	_dhPutValue
	addl	$12,%esp
	xorl	%edi,%edi
	cmpl	%edi,%eax
	xchgl	%edi,%eax
	setl	%al
	xchgl	%eax,%edi
	or	%edi,%edi
	jne	_$399
_$400:
;  888 	HR_TRY( dhCallMethod(xmlDoc, L".Load(%S)", L"menu.xml") );
	.line	888
	pushl	$_$409
	pushl	$_$408
	pushl	-4(%ebp)
	call	_dhCallMethod
	addl	$12,%esp
	xorl	%edi,%edi
	cmpl	%edi,%eax
	xchgl	%edi,%eax
	setl	%al
	xchgl	%eax,%edi
	or	%edi,%edi
	jne	_$399
_$406:
;  889 
;  890 	dhGetValue(L"%o",&NodeList,xmlDoc, L".documentElement.childNodes");
	.line	890
	pushl	$_$414
	pushl	-4(%ebp)
	leal	-8(%ebp),%eax
	pushl	%eax
	pushl	$_$413
	call	_dhGetValue
;  891 	parse_XML(NodeList, main_menu);
	.line	891
	pushl	_main_menu
	pushl	-8(%ebp)
	call	_parse_XML
	addl	$24,%esp
;  892 	return 1;
	.line	892
	xor	%eax,%eax
	inc	%eax
	jmp	_$392
_$399:
;  893 cleanup:
;  894 	SAFE_RELEASE(xmlDoc);
	.line	894
	cmpl	$0,-4(%ebp)
	je	_$415
	movl	-4(%ebp),%eax
	pushl	%eax
	movl	(,%eax),%ebx
	call	*8(%ebx)
	andl	$0,-4(%ebp)
_$415:
;  895 	SAFE_RELEASE(NodeList);
	.line	895
	cmpl	$0,-8(%ebp)
	je	_$417
	movl	-8(%ebp),%eax
	pushl	%eax
	movl	(,%eax),%ebx
	call	*8(%ebx)
	andl	$0,-8(%ebp)
_$417:
;  896 	return -1;
	.line	896
	orl	$-1,%eax
_$392:
;  897 }
	.line	897
	popl	%edi
	popl	%ebx
	leave
	ret
_$424:
	.size	_LoadXML,_$424-_LoadXML
	.globl	_LoadXML
	.data
	.align	2
	.type	_$S4,object
_$S4:
	.long	0
	.text
;  898 
;  899 static MENU_DATA get_menu_formWID(HMENU hMenu, UINT m_wid)
	.type	_get_menu_formWID,function
_get_menu_formWID:
	pushl	%ebp
	movl	%esp,%ebp
	subl	$180,%esp
	pushl	%ebx
	pushl	%esi
	pushl	%edi
;  900 {
	.line	900
;  901 	MENU_DATA ptr1;
;  902 	MENU_DATA ptr2;
;  903 	MENU_DATA return_val;
;  904 	LPMENU_DATA pointer_holder = &ptr1;
	.line	904
	leal	-124(%ebp),%ebx
;  905 	LPMENU_DATA next_pointer   = &ptr2;
	.line	905
	leal	-180(%ebp),%eax
	movl	%eax,-8(%ebp)
;  906 	int count = 0;
	.line	906
	andl	$0,-4(%ebp)
;  907 	static BOOL inited = FALSE;
;  908 	//pointer_holder = first_menu_data;
;  909 	CopyMemory(next_pointer, first_menu_data, sizeof(MENU_DATA));
	.line	909
	pushl	$56
	pushl	_first_menu_data
	pushl	%eax
	call	_memcpy
;  910 	int menuType = menuType_fromID(hMenu, m_wid);
	.line	910
	pushl	16(%ebp)
	pushl	12(%ebp)
	call	_menuType_fromID
	addl	$20,%esp
	movl	%eax,-12(%ebp)
;  911 
;  912 	if(menuType == MF_STRING)
	.line	912
	test	%eax,%eax
	jne	_$427
	jmp	_$430
_$429:
;  913 	{
;  914 		while(pointer_holder->wID != m_wid )
;  915 		{
;  916 			CopyMemory(pointer_holder, next_pointer->nextItem, sizeof(MENU_DATA));
	.line	916
	pushl	$56
	movl	-8(%ebp),%eax
	pushl	(,%eax)
	pushl	%ebx
	call	_memcpy
	addl	$12,%esp
;  917 			next_pointer = pointer_holder;
	.line	917
	movl	%ebx,-8(%ebp)
;  918 			if(count > menu_objects -1) {break;}
	.line	918
	movl	_menu_objects,%eax
	decl	%eax
	cmpl	%eax,-4(%ebp)
	jg	_$427
_$432:
;  919 			count++;
	.line	919
	incl	-4(%ebp)
_$430:
	.line	914
	movl	16(%ebp),%eax
	cmpl	%eax,12(%ebx)
	jne	_$429
;  920 		}
;  921 	}
	.line	921
_$427:
;  922 	//Handle the case POPUP
;  923 	//Popups have a dynamic wID which causes detection more messy
;  924 	if(menuType == MF_POPUP)
	.line	924
	cmpl	$16,-12(%ebp)
	jne	_$434
	jmp	_$437
_$436:
;  925 	{
;  926 		while((UINT)pointer_holder->menu_handle != m_wid )
;  927 		{
;  928 			CopyMemory(pointer_holder, next_pointer->nextItem, sizeof(MENU_DATA));
	.line	928
	pushl	$56
	movl	-8(%ebp),%eax
	pushl	(,%eax)
	pushl	%ebx
	call	_memcpy
	addl	$12,%esp
;  929 			next_pointer = pointer_holder;
	.line	929
	movl	%ebx,-8(%ebp)
;  930 			if(count > menu_objects -1) {break;}
	.line	930
	movl	_menu_objects,%eax
	decl	%eax
	cmpl	%eax,-4(%ebp)
	jg	_$434
_$439:
;  931 			count++;
	.line	931
	incl	-4(%ebp)
_$437:
	.line	926
	movl	16(%ebp),%eax
	cmpl	%eax,8(%ebx)
	jne	_$436
;  932 		}
;  933 
;  934 	}
	.line	934
_$434:
;  935 
;  936 	//free(pointer_holder);
;  937 	//free(next_pointer);
;  938 	CopyMemory(&return_val, pointer_holder, sizeof(MENU_DATA));
	.line	938
	pushl	$56
	pushl	%ebx
	leal	-68(%ebp),%eax
	pushl	%eax
	call	_memcpy
	addl	$12,%esp
;  939 	return return_val;
	.line	939
	movl	8(%ebp),%edi
	leal	-68(%ebp),%esi
	movl	$14,%ecx
	rep
	movsl
;  940 }
	.line	940
	popl	%edi
	popl	%esi
	popl	%ebx
	leave
	ret
_$442:
	.size	_get_menu_formWID,_$442-_get_menu_formWID
;  941 //	----------------------------------
;  942 //	basic like redim func
;  943 //	reason 1 creates a new element
;  944 //	reason 2 gives current ptr
;  945 //	check cleans & memout
;  946 //	----------------------------------
;  947 static LPMENU_DATA redim_items(LPMENU_DATA data_pointer, int reason)
	.type	_redim_items,function
_redim_items:
	pushl	%ebp
	movl	%esp,%ebp
	pushl	%ecx
	pushl	%ebx
	pushl	%esi
	pushl	%edi
;  948 {
	.line	948
	movl	12(%ebp),%esi
;  949 	LPMENU_DATA pointer_holder;
;  950 	void * memory;
;  951 
;  952 	//return pointer_holder;
;  953 	if(menu_objects == 0)
	.line	953
	cmpl	$0,_menu_objects
	jne	_$444
;  954 	{
;  955 	//init the object loop
;  956 	primary_menu_data = malloc(sizeof(MENU_DATA));
	.line	956
	pushl	$56
	call	_malloc
	popl	%ecx
	movl	%eax,_primary_menu_data
;  957 	ZeroMemory(primary_menu_data, sizeof(MENU_DATA));
	.line	957
	pushl	$56
	pushl	%eax
	call	_RtlZeroMemory@8
;  958 	first_menu_data = primary_menu_data;
	.line	958
	movl	_primary_menu_data,%eax
	movl	%eax,_first_menu_data
;  959 	menu_objects++;
	.line	959
	incl	_menu_objects
;  960 	return 	primary_menu_data;
	.line	960
	movl	_primary_menu_data,%eax
	jmp	_$443
_$444:
;  961 
;  962 	}
;  963 	if(reason == 1)
	.line	963
	cmpl	$1,%esi
	jne	_$446
;  964 	{
;  965 	//redim the structure
;  966 	primary_menu_data->nextItem = malloc(sizeof(MENU_DATA));
	.line	966
	pushl	$56
	call	_malloc
	movl	_primary_menu_data,%ebx
	movl	%eax,(,%ebx)
;  967 	//save this
;  968 	pointer_holder = primary_menu_data->nextItem;
	.line	968
	movl	%ebx,%eax
	movl	(,%eax),%edi
;  969 	//copy this onto main pointer
;  970 	CopyMemory(pointer_holder,primary_menu_data, sizeof(MENU_DATA));
	.line	970
	pushl	$56
	pushl	%eax
	pushl	%edi
	call	_memcpy
	addl	$16,%esp
;  971 	primary_menu_data = pointer_holder;
	.line	971
	movl	%edi,_primary_menu_data
;  972 	menu_objects++;
	.line	972
	incl	_menu_objects
_$446:
;  973 	}
;  974 	if(reason == 2)
	.line	974
	cmpl	$2,%esi
	jne	_$448
;  975 	{
;  976 	//get current pointer
;  977 	pointer_holder = primary_menu_data;
	.line	977
	movl	_primary_menu_data,%edi
_$448:
;  978 	}
;  979 	return 	pointer_holder;
	.line	979
	movl	%edi,%eax
_$443:
;  980 }
	.line	980
	popl	%edi
	popl	%esi
	popl	%ebx
	leave
	ret
_$452:
	.size	_redim_items,_$452-_redim_items
	.data
	.align	2
	.type	_$S5,object
_$S5:
	.long	0
	.align	2
	.type	_$S6,object
_$S6:
	.long	1900
	.text
;  981 //	-----------------------------------------
;  982 //	Recluse in XML file
;  983 //	xmlObj = IXMLDOMNodeList
;  984 //	constructs the menu by reclusing the tree
;  985 //	-----------------------------------------
;  986 int parse_XML(IDispatch *NodeList, HMENU holder)
	.type	_parse_XML,function
_parse_XML:
	pushl	%ebp
	movl	%esp,%ebp
	subl	$120,%esp
	pushl	%ebx
	pushl	%esi
	pushl	%edi
;  987 {
	.line	987
;  988 	DISPATCH_OBJ(TempNode);
	.line	988
	xor	%esi,%esi
	movl	%esi,-12(%ebp)
;  989 	DISPATCH_OBJ(NodeItem);
	.line	989
	movl	%esi,-4(%ebp)
;  990 
;  991 	LPWSTR	    text;
;  992 	LPSTR       caption;
;  993 	LPSTR       image_path;
;  994 	LPSTR       exec_path;
;  995 	short       width,height;
;  996 	HBITMAP     loaded_bmp;
;  997 	COLORREF    menu_fore_color;
;  998 	COLORREF    menu_select_color;
;  999 	BOOL        bolVal;
; 1000 	int         ilter = 0;
	.line	1000
	movl	%esi,-48(%ebp)
; 1001 	static int  reclusines = 0;
; 1002 	SIZE zSize;
; 1003 	HDC         nullDC;
; 1004 	int         cnt   = 0;
	.line	1004
	xor	%esi,%esi
; 1005 	int         depth = 0;
	.line	1005
	movl	%esi,-20(%ebp)
; 1006 	int         icoSize;
; 1007 
; 1008 	if(reclusines == 0){icoSize = 32; }else { icoSize = 16;}
	.line	1008
	xorl	%eax,%eax
	cmpl	$0,_$S5
	setne	%al
	decl	%eax
	andl	$16,%eax
	addl	$16,%eax
	movl	%eax,-36(%ebp)
; 1009 	static int MMU_IDS = 1900;
; 1010 	MENUITEMINFO mii;
; 1011 
; 1012 	LPMENU_DATA menu_data = redim_items(menu_data, 2);
	.line	1012
	pushl	$2
	pushl	%edi
	call	_redim_items
	movl	%eax,%edi
; 1013 
; 1014 
; 1015 	dhGetValue(L"%d", &ilter, NodeList, L".length");
	.line	1015
	pushl	$_$457
	pushl	8(%ebp)
	leal	-48(%ebp),%eax
	pushl	%eax
	pushl	$_$456
	call	_dhGetValue
	addl	$24,%esp
; 1016 
; 1017 	for(cnt = 0;cnt < ilter; cnt++)
	.line	1017
	xor	%esi,%esi
	jmp	_$461
_$458:
; 1018 	{
; 1019 		dhGetValue(L"%o", &NodeItem       ,NodeList, L".item(%d)",cnt);
	.line	1019
	pushl	%esi
	pushl	$_$462
	pushl	8(%ebp)
	leal	-4(%ebp),%eax
	pushl	%eax
	pushl	$_$413
	call	_dhGetValue
; 1020 		dhGetValue(L"%s", &caption        ,NodeItem, L".selectSingleNode(%S).text", L"CAPTION");
	.line	1020
	pushl	$_$465
	pushl	$_$464
	pushl	-4(%ebp)
	leal	-8(%ebp),%eax
	pushl	%eax
	pushl	$_$463
	call	_dhGetValue
; 1021 		dhGetValue(L"%s", &image_path     ,NodeItem, L".selectSingleNode(%S).text", L"IMAGE");
	.line	1021
	pushl	$_$466
	pushl	$_$464
	pushl	-4(%ebp)
	leal	-16(%ebp),%eax
	pushl	%eax
	pushl	$_$463
	call	_dhGetValue
; 1022 		dhGetValue(L"%u", &menu_fore_color,NodeItem, L".selectSingleNode(%S).text", L"COLOR");
	.line	1022
	pushl	$_$468
	pushl	$_$464
	pushl	-4(%ebp)
	leal	-32(%ebp),%eax
	pushl	%eax
	pushl	$_$467
	call	_dhGetValue
; 1023 		dhGetValue(L"%s", &exec_path      ,NodeItem, L".selectSingleNode(%S).text", L"EXECUTE");
	.line	1023
	pushl	$_$469
	pushl	$_$464
	pushl	-4(%ebp)
	leal	-24(%ebp),%eax
	pushl	%eax
	pushl	$_$463
	call	_dhGetValue
; 1024 		//search for any for more objects
; 1025 		dhGetValue(L"%o", &TempNode, NodeItem, L".selectNodes(%S)",L"MENUITEM");
	.line	1025
	pushl	$_$471
	pushl	$_$470
	pushl	-4(%ebp)
	leal	-12(%ebp),%eax
	pushl	%eax
	pushl	$_$413
	call	_dhGetValue
; 1026 
; 1027 		dhGetValue(L"%d", &depth, TempNode, L".length");
	.line	1027
	pushl	$_$457
	pushl	-12(%ebp)
	leal	-20(%ebp),%eax
	pushl	%eax
	pushl	$_$456
	call	_dhGetValue
	addl	$136,%esp
; 1028 	if(depth > 0)
	.line	1028
	cmpl	$0,-20(%ebp)
	jle	_$472
; 1029 		{
; 1030 		//Create popups
; 1031 
; 1032 		if(image_path[0] == '\0' )
	.line	1032
	movl	-16(%ebp),%eax
	cmpb	$0,(,%eax)
	jne	_$474
; 1033 		{
; 1034 		//MessageBox(main_window,"no path","Error de tipo:",MB_OK);
; 1035 		HDC hDCDesktop = GetDC(0);
	.line	1035
	pushl	$0
	call	_GetDC@4
	movl	%eax,-116(%ebp)
; 1036 		loaded_bmp = CreateCompatibleBitmap( hDCDesktop, icoSize,icoSize);
	.line	1036
	movl	-36(%ebp),%eax
	pushl	%eax
	pushl	%eax
	pushl	-116(%ebp)
	call	_CreateCompatibleBitmap@12
	movl	%eax,%ebx
; 1037   		ReleaseDC(NULL, hDCDesktop );
	.line	1037
	pushl	-116(%ebp)
	pushl	$0
	call	_ReleaseDC@8
; 1038 
; 1039   		drawIconToBitmap( loaded_bmp, exec_path, icoSize);
	.line	1039
	pushl	-36(%ebp)
	pushl	-24(%ebp)
	pushl	%ebx
	call	_drawIconToBitmap
	addl	$12,%esp
	jmp	_$475
_$474:
; 1040 		}
; 1041 		else
; 1042 		{
	.line	1042
; 1043 		loaded_bmp = LoadImage(hInst,image_path,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
	.line	1043
	pushl	$16
	pushl	$0
	pushl	$0
	pushl	$0
	pushl	-16(%ebp)
	pushl	_hInst
	call	_LoadImageA@24
	movl	%eax,%ebx
_$475:
; 1044 		}
; 1045 		HMENU newPop = CreatePopupMenu();
	.line	1045
	call	_CreatePopupMenu@0
	movl	%eax,-112(%ebp)
; 1046 		menu_data = redim_items(menu_data, 1);
	.line	1046
	pushl	$1
	pushl	%edi
	call	_redim_items
	movl	%eax,%edi
; 1047 		menu_data->menu_image = loaded_bmp;
	.line	1047
	movl	%ebx,24(%edi)
; 1048 		//menu_data->menu_width = width;
; 1049 		//menu_data->menu_height non staticaly set
; 1050 		menu_data->menu_handle = newPop;
	.line	1050
	movl	-112(%ebp),%eax
	movl	%eax,8(%edi)
; 1051 		menu_data->menu_popup = 1;
	.line	1051
	movl	$1,52(%edi)
; 1052 		menu_data->menu_capti = caption;
	.line	1052
	movl	-8(%ebp),%eax
	movl	%eax,16(%edi)
; 1053 		menu_data->menu_exec  = exec_path;
	.line	1053
	movl	-24(%ebp),%eax
	movl	%eax,20(%edi)
; 1054 		menu_data->menu_color = menu_fore_color;
	.line	1054
	movl	-32(%ebp),%eax
	movl	%eax,40(%edi)
; 1055 		menu_data->wID = (UINT)menu_data->menu_handle;
	.line	1055
	movl	8(%edi),%eax
	movl	%eax,12(%edi)
; 1056 		AppendMenu(holder,MF_POPUP | MF_OWNERDRAW,(unsigned int)newPop, caption);
	.line	1056
	pushl	-8(%ebp)
	pushl	-112(%ebp)
	pushl	$272
	pushl	12(%ebp)
	call	_AppendMenuA@16
; 1057 		reclusines = depth;
	.line	1057
	movl	-20(%ebp),%eax
	movl	%eax,_$S5
; 1058 		parse_XML(TempNode,newPop);
	.line	1058
	pushl	-112(%ebp)
	pushl	-12(%ebp)
	call	_parse_XML
	addl	$16,%esp
	jmp	_$473
_$472:
; 1059 		}
; 1060 	else
; 1061 		{
	.line	1061
; 1062 		//set data for this item
; 1063 		if(image_path[0] == '\0' )
	.line	1063
	movl	-16(%ebp),%eax
	cmpb	$0,(,%eax)
	jne	_$476
; 1064 		{
; 1065 		//MessageBox(main_window,"no path","Error de tipo:",MB_OK);
; 1066 		HDC hDCDesktop = GetDC(0);
	.line	1066
	pushl	$0
	call	_GetDC@4
	movl	%eax,-116(%ebp)
; 1067 		loaded_bmp = CreateCompatibleBitmap( hDCDesktop, icoSize,icoSize);
	.line	1067
	movl	-36(%ebp),%eax
	pushl	%eax
	pushl	%eax
	pushl	-116(%ebp)
	call	_CreateCompatibleBitmap@12
	movl	%eax,%ebx
; 1068   		ReleaseDC(NULL, hDCDesktop );
	.line	1068
	pushl	-116(%ebp)
	pushl	$0
	call	_ReleaseDC@8
; 1069   		drawIconToBitmap( loaded_bmp, exec_path, icoSize);
	.line	1069
	pushl	-36(%ebp)
	pushl	-24(%ebp)
	pushl	%ebx
	call	_drawIconToBitmap
	addl	$12,%esp
	jmp	_$477
_$476:
; 1070 		}
; 1071 		else
; 1072 		{
	.line	1072
; 1073 		loaded_bmp = LoadImage(hInst,image_path,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
	.line	1073
	pushl	$16
	pushl	$0
	pushl	$0
	pushl	$0
	pushl	-16(%ebp)
	pushl	_hInst
	call	_LoadImageA@24
	movl	%eax,%ebx
_$477:
; 1074 		}
; 1075 		if(menu_data->menu_image != NULL)
	.line	1075
	cmpl	$0,24(%edi)
	je	_$478
_$478:
; 1076 		{
; 1077 		//MessageBox(main_window,image_path,"Error de tipo:",MB_OK);
; 1078 		}
; 1079 
; 1080 		//Build an isolated menu
; 1081 		menu_data = redim_items(menu_data, 1);
	.line	1081
	pushl	$1
	pushl	%edi
	call	_redim_items
	addl	$8,%esp
	movl	%eax,%edi
; 1082 		menu_data->menu_image = loaded_bmp;
	.line	1082
	movl	%ebx,24(%edi)
; 1083 		menu_data->menu_handle = holder;
	.line	1083
	movl	12(%ebp),%eax
	movl	%eax,8(%edi)
; 1084 		menu_data->menu_popup = 0;
	.line	1084
	andl	$0,52(%edi)
; 1085 		menu_data->menu_capti = caption;
	.line	1085
	movl	-8(%ebp),%eax
	movl	%eax,16(%edi)
; 1086 		menu_data->menu_exec  = exec_path;
	.line	1086
	movl	-24(%ebp),%eax
	movl	%eax,20(%edi)
; 1087 		menu_data->menu_color = menu_fore_color;
	.line	1087
	movl	-32(%ebp),%eax
	movl	%eax,40(%edi)
; 1088 		menu_data->wID = MMU_IDS;
	.line	1088
	movl	_$S6,%eax
	movl	%eax,12(%edi)
; 1089 		//menu_data->menu_width =
; 1090 		//zSize
; 1091 		//nullDC // get default font
; 1092 		nullDC = GetDC(NULL);
	.line	1092
	pushl	$0
	call	_GetDC@4
	movl	%eax,-28(%ebp)
; 1093 		HANDLE oldFt = SelectObject(nullDC, GetStockObject(DEFAULT_GUI_FONT));
	.line	1093
	pushl	$17
	call	_GetStockObject@4
	pushl	%eax
	pushl	-28(%ebp)
	call	_SelectObject@8
	movl	%eax,-112(%ebp)
; 1094 		GetTextExtentPoint32(nullDC, caption, lstrlen(caption), &zSize);
	.line	1094
	pushl	-8(%ebp)
	call	_lstrlenA@4
	leal	-44(%ebp),%edx
	pushl	%edx
	pushl	%eax
	pushl	-8(%ebp)
	pushl	-28(%ebp)
	call	_GetTextExtentPoint32A@16
; 1095 		menu_data->menu_width = 0;
	.line	1095
	movw	$0,48(%edi)
; 1096 		menu_data->menu_width = max(menu_data->menu_width,zSize.cx);
	.line	1096
	movswl	48(%edi),%eax
	cmpl	-44(%ebp),%eax
	jle	_$481
	movswl	48(%edi),%eax
	movl	%eax,-116(%ebp)
	jmp	_$482
_$481:
	movl	-44(%ebp),%eax
	movl	%eax,-116(%ebp)
_$482:
	movl	-116(%ebp),%eax
	movw	%eax,48(%edi)
; 1097 		SelectObject(nullDC, oldFt);
	.line	1097
	pushl	-112(%ebp)
	pushl	-28(%ebp)
	call	_SelectObject@8
; 1098 		DeleteDC(nullDC);
	.line	1098
	pushl	-28(%ebp)
	call	_DeleteDC@4
; 1099 		
; 1100 		if(strlen(caption) <= 1)
	.line	1100
	movl	-8(%ebp),%eax
	movl	%eax,%ecx
	orl	$-1,%eax
_$strlen0:
	inc	%eax
	cmpb	$0,(%ecx,%eax)
	jnz	_$strlen0
	cmpl	$1,%eax
	ja 	_$483
; 1101 		{
; 1102 			switch(caption[0])
	.line	1102
	movl	-8(%ebp),%eax
	movsbl	(,%eax),%eax
	cmpl	$45,%eax
	je	_$490
	jg	_$494
	cmpl	$0,%eax
	je	_$484
	jmp	_$484
_$494:
	cmpl	$66,%eax
	je	_$488
	jl	_$484
	cmpl	$82,%eax
	je	_$484
	jmp	_$484
_$488:
; 1103 			{
; 1104 			case 'B':
; 1105 				menu_data->menu_capti = "";
	.line	1105
	leal	_$226,%eax
	movl	%eax,16(%edi)
; 1106 				AppendMenu(holder,MF_MENUBARBREAK | MF_OWNERDRAW,MMU_IDS, NULL );
	.line	1106
	pushl	$0
	pushl	_$S6
	pushl	$288
	pushl	12(%ebp)
	call	_AppendMenuA@16
; 1107 				break;
	.line	1107
	jmp	_$484
; 1108 			case 'R':
; 1109 			break;
	.line	1109
_$490:
; 1110 			case '-':
; 1111 				menu_data->menu_capti = "-";
	.line	1111
	leal	_$491,%eax
	movl	%eax,16(%edi)
; 1112 				AppendMenu(holder,MF_SEPARATOR | MF_OWNERDRAW,MMU_IDS, NULL );
	.line	1112
	pushl	$0
	pushl	_$S6
	pushl	$2304
	pushl	12(%ebp)
	call	_AppendMenuA@16
; 1113 			break;
; 1114 			case '\0':
; 1115 			break;
	.line	1115
; 1116 			}
; 1117 		}
	.line	1117
	jmp	_$484
_$483:
; 1118 		else
; 1119 		{
	.line	1119
; 1120 			AppendMenu(holder,MF_STRING | MF_OWNERDRAW,MMU_IDS, caption );
	.line	1120
	pushl	-8(%ebp)
	pushl	_$S6
	pushl	$256
	pushl	12(%ebp)
	call	_AppendMenuA@16
_$484:
; 1121 		
; 1122 		}
; 1123 		
; 1124 		//}//end if
; 1125 	
; 1126 		//AppendMenu(holder,MF_STRING,MMU_IDS, Menu_item.menu_capti );
; 1127 		//menu_data->wID =  GetMenuItemID(holder, cnt);
; 1128 
; 1129 		MMU_IDS++;
	.line	1129
	incl	_$S6
_$473:
; 1130 		}
; 1131 
; 1132 	}//NEXT
	.line	1132
	.line	1017
	incl	%esi
_$461:
	cmpl	-48(%ebp),%esi
	jl	_$458
; 1133 
; 1134 	//return 1;
; 1135 cleanup:
; 1136 	//dhFreeString(caption);
; 1137 	//dhFreeString(exec_path);
; 1138 	reclusines--;
	.line	1138
	decl	_$S5
; 1139 	dhFreeString(image_path);
	.line	1139
	pushl	-16(%ebp)
	call	_SysFreeString@4
; 1140 	SAFE_RELEASE(TempNode);
	.line	1140
	cmpl	$0,-12(%ebp)
	je	_$497
	movl	-12(%ebp),%eax
	pushl	%eax
	movl	(,%eax),%ebx
	call	*8(%ebx)
	andl	$0,-12(%ebp)
_$497:
; 1141 	SAFE_RELEASE(NodeItem);
	.line	1141
	cmpl	$0,-4(%ebp)
	je	_$499
	movl	-4(%ebp),%eax
	pushl	%eax
	movl	(,%eax),%ebx
	call	*8(%ebx)
	andl	$0,-4(%ebp)
_$499:
; 1142 	return 1;
	.line	1142
	xor	%eax,%eax
	inc	%eax
; 1143 
; 1144 }
	.line	1144
	popl	%edi
	popl	%esi
	popl	%ebx
	leave
	ret
_$520:
	.size	_parse_XML,_$520-_parse_XML
	.globl	_parse_XML
	.data
	.align	2
	.type	_$S7,object
_$S7:
	.long	0x0
	.align	2
	.type	_$S8,object
_$S8:
	.long	0x0
	.text
; 1145 LRESULT CALLBACK install_Hook (int code, WPARAM wParam, LPARAM lParam)
	.type	_install_Hook@12,function
_install_Hook@12:
	pushl	%ebp
	movl	%esp,%ebp
	subl	$280,%esp
	pushl	%ebx
	pushl	%esi
	pushl	%edi
; 1146 {
	.line	1146
	movl	8(%ebp),%ebx
; 1147 	CWPSTRUCT* pStruct = (CWPSTRUCT*)lParam;
	.line	1147
	movl	16(%ebp),%edi
; 1148 
; 1149 	static HWND acthMenu = NULL;
; 1150     	static HWND acthWnd = NULL;
; 1151 
; 1152 	int x = 0;
	.line	1152
	xor	%esi,%esi
; 1153 
; 1154 	x++;
	.line	1154
	incl	%esi
; 1155 	damm_handle = pStruct->hwnd;
	.line	1155
	movl	12(%edi),%eax
	movl	%eax,_damm_handle
	jmp	_$523
_$522:
; 1156 
; 1157 	//MessageBox(main_window,"hookinged  !!!","Error de tipo:",MB_OK);
; 1158 
; 1159 	//return CallNextHookEx (hoo_handle, code, wParam, lParam);
; 1160 
; 1161 	while ( code == HC_ACTION )
; 1162     	{
; 1163         HWND hWnd = pStruct->hwnd;
	.line	1163
	movl	12(%edi),%eax
	movl	%eax,-8(%ebp)
; 1164 
; 1165         // Normal and special handling for menu 0x10012
; 1166         if ( pStruct->message != WM_CREATE && pStruct->message != 0x01E2 )
	.line	1166
	movl	8(%edi),%eax
	cmpl	$1,%eax
	je	_$525
	cmpl	$482,%eax
	jne	_$524
; 1167         {
; 1168 		break;
	.line	1168
_$525:
; 1169         }
; 1170 
; 1171 		//4402762 chorrus
; 1172 	HWND pFrame = GetForegroundWindow();
	.line	1172
	call	_GetForegroundWindow@0
	movl	%eax,-268(%ebp)
; 1173 
; 1174 	x = 10;
	.line	1174
	movl	$10,%esi
; 1175 	//si es que activamos el skin para esta ventana con handle
; 1176 	//if ( pFrame == NULL || !GetXPLookNFeel (pFrame) )
; 1177 	if ( pFrame == NULL )
	.line	1177
	test	%eax,%eax
	je	_$524
; 1178         {
; 1179             break;
	.line	1179
_$527:
; 1180         }
; 1181 	char sClassName[256];
; 1182         int Count = GetClassName (hWnd, sClassName, MAX_CLASS_NAME);
	.line	1182
	pushl	_MAX_CLASS_NAME
	leal	-264(%ebp),%eax
	pushl	%eax
	pushl	-8(%ebp)
	call	_GetClassNameA@12
	movl	%eax,-272(%ebp)
; 1183 	// Check for the menu-class
; 1184         if ( Count != 6 || strcmp (sClassName, "#32768") != 0 )
	.line	1184
	cmpl	$6,%eax
	jne	_$532
	pushl	$_$531
	leal	-264(%ebp),%eax
	pushl	%eax
	call	_strcmp
	addl	$8,%esp
	or	%eax,%eax
	je	_$529
_$532:
; 1185         {
; 1186 		acthWnd = hWnd;
	.line	1186
	movl	-8(%ebp),%eax
	movl	%eax,_$S8
; 1187         	break;
	.line	1187
	jmp	_$524
_$529:
; 1188         }
; 1189 	else
; 1190 	{
	.line	1190
; 1191 		acthMenu = hWnd;
	.line	1191
	movl	-8(%ebp),%eax
	movl	%eax,_$S7
; 1192 	}
; 1193 	//hasta aca estamos seguros Safe point
; 1194 	x = 10;
	.line	1194
	movl	$10,%esi
; 1195 	HANDLE resu = GetProp (pStruct->hwnd, _WndPropName_OldProc);
	.line	1195
	pushl	$__WndPropName_OldProc
	pushl	12(%edi)
	call	_GetPropA@8
	movl	%eax,-280(%ebp)
; 1196 	if ( GetProp (pStruct->hwnd, _WndPropName_OldProc) != NULL )
	.line	1196
	pushl	$__WndPropName_OldProc
	pushl	12(%edi)
	call	_GetPropA@8
	or	%eax,%eax
	jne	_$524
; 1197         {
; 1198             // Already subclassed
; 1199             break;
	.line	1199
_$533:
; 1200         }
; 1201         // Subclass the window
; 1202         //get the code pointer
; 1203 
; 1204         WNDPROC oldWndProc = (WNDPROC)(LONG_PTR)GetWindowLong (pStruct->hwnd, GWL_WNDPROC);
	.line	1204
	pushl	$-4
	pushl	12(%edi)
	call	_GetWindowLongA@8
	movl	%eax,-4(%ebp)
; 1205 	old_menu_proc = oldWndProc;
	.line	1205
	movl	%eax,_old_menu_proc
; 1206 	if ( oldWndProc == NULL )
	.line	1206
	cmpl	$0,-4(%ebp)
	je	_$524
; 1207         {
; 1208         	//oops GPF we'r on the edge !!!
; 1209         break;
	.line	1209
_$535:
; 1210         }
; 1211 
; 1212         mnu_hwnd = pStruct->hwnd;
	.line	1212
	movl	12(%edi),%eax
	movl	%eax,_mnu_hwnd
; 1213         if ( !SetProp (pStruct->hwnd, _WndPropName_OldProc, oldWndProc) )
	.line	1213
	pushl	-4(%ebp)
	pushl	$__WndPropName_OldProc
	pushl	12(%edi)
	call	_SetPropA@12
	or	%eax,%eax
	je	_$524
; 1214         {
; 1215             break;
	.line	1215
_$537:
; 1216         }
; 1217 	long set_wl = SetWindowLong (pStruct->hwnd, GWL_WNDPROC,(DWORD)(DWORD_PTR)menuproc_sclass);
	.line	1217
	pushl	$_menuproc_sclass@16
	pushl	$-4
	pushl	12(%edi)
	call	_SetWindowLongA@12
	movl	%eax,-276(%ebp)
; 1218 	if(!set_wl)
	.line	1218
	test	%eax,%eax
	jne	_$524
; 1219         {
; 1220             RemoveProp (pStruct->hwnd, _WndPropName_OldProc);
	.line	1220
	pushl	$__WndPropName_OldProc
	pushl	12(%edi)
	call	_RemovePropA@8
; 1221             break;
	.line	1221
	jmp	_$524
; 1222         }
; 1223 
; 1224 
; 1225         // Success !
; 1226 	break;
	.line	1226
_$523:
	.line	1161
	or	%ebx,%ebx
	je	_$522
_$524:
; 1227 	}
; 1228 
; 1229 
; 1230 	return CallNextHookEx (hoo_handle, code, wParam, lParam);
	.line	1230
	pushl	16(%ebp)
	pushl	12(%ebp)
	pushl	%ebx
	pushl	_hoo_handle
	call	_CallNextHookEx@16
; 1231 }
	.line	1231
	popl	%edi
	popl	%esi
	popl	%ebx
	leave
	ret	$12
_$550:
	.size	_install_Hook@12,_$550-_install_Hook@12
	.globl	_install_Hook@12
	.data
	.align	2
	.type	_$S9,object
_$S9:
	.long	0
	.text
; 1232 LRESULT CALLBACK menuproc_sclass    (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	.type	_menuproc_sclass@16,function
_menuproc_sclass@16:
	pushl	%ebp
	movl	%esp,%ebp
	subl	$76,%esp
	pushl	%ebx
	pushl	%esi
	pushl	%edi
; 1233 {
	.line	1233
	movl	8(%ebp),%ebx
; 1234 	WNDPROC oldWndProc = (WNDPROC)GetProp (hWnd, _WndPropName_OldProc);
	.line	1234
	pushl	$__WndPropName_OldProc
	pushl	%ebx
	call	_GetPropA@8
	movl	%eax,-4(%ebp)
; 1235 
; 1236 switch ( uMsg ){
	.line	1236
	movl	12(%ebp),%eax
	cmpl	$24,%eax
	je	_$596
	jg	_$608
	cmpl	$2,%eax
	je	_$602
	jl	_$552
	cmpl	$16,%eax
	je	_$603
	jmp	_$552
_$608:
	cmpl	$130,%eax
	je	_$604
	cmpl	$133,%eax
	je	_$557
	jg	_$611
	cmpl	$43,%eax
	je	_$557
	jmp	_$552
_$611:
	cmpl	$279,%eax
	jne	_$552
_$555:
; 1237 
; 1238 	case WM_INITMENUPOPUP:
; 1239 		  {
	.line	1239
; 1240 			//  set_newTheme(hWnd,TRUE);
; 1241 		  }
; 1242 	case WM_DRAWITEM:
; 1243 		  {
	.line	1243
_$557:
; 1244 
; 1245 	      	  }
; 1246 
; 1247 
; 1248 		/*
; 1249 	case WM_NCCALCSIZE:
; 1250 		{
; 1251 		NCCALCSIZE_PARAMS* lpncsp = (NCCALCSIZE_PARAMS*)lParam;
; 1252 
; 1253 		//lpncsp->lppos->x +=  30;
; 1254 		lpncsp->rgrc[0].right  -= 1;
; 1255 		lpncsp->rgrc[2].bottom -= 10;
; 1256 		lpncsp->rgrc[0].top    += 1;
; 1257 		lpncsp->rgrc[0].left   += 1;
; 1258 		lpncsp->lppos->cx +=  10;
; 1259 		return 0;
; 1260 		}
; 1261 		*/
; 1262 	case WM_NCPAINT:
; 1263 		  {
	.line	1263
; 1264 	//return 0;
; 1265 	HDC hDC;
; 1266 	RECT rcTwo;
; 1267 	RECT rcSep;
; 1268 	RECT rcOrg;
; 1269 	int bdr_w = 30;   //main menu sidebar w
	.line	1269
	movl	$30,-52(%ebp)
; 1270 	int bdr_h = 150;
	.line	1270
	movl	$150,-60(%ebp)
; 1271 
; 1272 	static int main_wnd = 0;
; 1273 
; 1274 	hDC = GetWindowDC (hWnd);
	.line	1274
	pushl	%ebx
	call	_GetWindowDC@4
	movl	%eax,-48(%ebp)
; 1275 	GetWindowRect(hWnd,&rcTwo);
	.line	1275
	leal	-24(%ebp),%eax
	pushl	%eax
	pushl	%ebx
	call	_GetWindowRect@8
; 1276 
; 1277 	//SetWindowPos(hWnd,hWnd,10,10,10,10,0);
; 1278 
; 1279 	GetClipBox(hDC,&rcTwo);
	.line	1279
	leal	-24(%ebp),%eax
	pushl	%eax
	pushl	-48(%ebp)
	call	_GetClipBox@8
; 1280 	rcOrg = rcTwo;
	.line	1280
	leal	-44(%ebp),%edi
	leal	-24(%ebp),%esi
	movl	$4,%ecx
	rep
	movsl
; 1281 
; 1282 	//return 0;
; 1283 	//LPDRAWITEMSTRUCT	lpDs = (LPDRAWITEMSTRUCT)lParam;
; 1284 	rcSep.top 	= rcTwo.top 	+2;
	.line	1284
	movl	-20(%ebp),%eax
	addl	$2,%eax
	movl	%eax,-72(%ebp)
; 1285 	rcSep.left	= rcTwo.left	-2;
	.line	1285
	movl	-24(%ebp),%eax
	subl	$2,%eax
	movl	%eax,-76(%ebp)
; 1286 	rcSep.bottom	= rcTwo.bottom  -2;
	.line	1286
	movl	-12(%ebp),%eax
	subl	$2,%eax
	movl	%eax,-64(%ebp)
; 1287 	rcSep.right	= rcTwo.right   +2;
	.line	1287
	movl	-16(%ebp),%eax
	addl	$2,%eax
	movl	%eax,-68(%ebp)
; 1288 
; 1289 	HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0));
	.line	1289
	pushl	$0
	call	_CreateSolidBrush@4
	movl	%eax,-28(%ebp)
; 1290 	FrameRect(hDC, &rcTwo, hBrush);
	.line	1290
	pushl	%eax
	leal	-24(%ebp),%eax
	pushl	%eax
	pushl	-48(%ebp)
	call	_FrameRect@12
; 1291 
; 1292 	rcTwo.top 	 = rcTwo.top 	 +1;
	.line	1292
	movl	-20(%ebp),%eax
	incl	%eax
	movl	%eax,-20(%ebp)
; 1293 	rcTwo.left	 = rcTwo.left	 +1;
	.line	1293
	incl	-24(%ebp)
; 1294 	rcTwo.bottom 	 = rcTwo.bottom  -1;
	.line	1294
	movl	-12(%ebp),%eax
	decl	%eax
	movl	%eax,-12(%ebp)
; 1295 	rcTwo.right	 = rcTwo.right	 -1;
	.line	1295
	movl	-16(%ebp),%eax
	decl	%eax
	movl	%eax,-16(%ebp)
; 1296 	DeleteObject(hBrush);
	.line	1296
	pushl	-28(%ebp)
	call	_DeleteObject@4
; 1297 
; 1298 	//cambiar color: borde mas fino
; 1299 	hBrush = CreateSolidBrush(RGB(195,195, 195));
	.line	1299
	pushl	$0xc3c3c3
	call	_CreateSolidBrush@4
	movl	%eax,-28(%ebp)
; 1300 	FrameRect(hDC, &rcTwo, hBrush);
	.line	1300
	pushl	%eax
	leal	-24(%ebp),%eax
	pushl	%eax
	pushl	-48(%ebp)
	call	_FrameRect@12
; 1301 
; 1302 	rcTwo.top 	= rcTwo.top 	+1;
	.line	1302
	movl	-20(%ebp),%eax
	incl	%eax
	movl	%eax,-20(%ebp)
; 1303 	rcTwo.left	= rcTwo.left	+1;
	.line	1303
	incl	-24(%ebp)
; 1304 	rcTwo.bottom	= rcTwo.bottom	-1;
	.line	1304
	movl	-12(%ebp),%eax
	decl	%eax
	movl	%eax,-12(%ebp)
; 1305 	rcTwo.right	= rcTwo.right	-1;
	.line	1305
	movl	-16(%ebp),%eax
	decl	%eax
	movl	%eax,-16(%ebp)
; 1306 
; 1307 	FrameRect(hDC, &rcTwo, hBrush);
	.line	1307
	pushl	-28(%ebp)
	leal	-24(%ebp),%eax
	pushl	%eax
	pushl	-48(%ebp)
	call	_FrameRect@12
; 1308 	DeleteObject(hBrush);
	.line	1308
	pushl	-28(%ebp)
	call	_DeleteObject@4
; 1309 
; 1310 	//Side bar
; 1311 	rcTwo.top 	 = rcOrg.top 	+1;
	.line	1311
	movl	-40(%ebp),%eax
	incl	%eax
	movl	%eax,-20(%ebp)
; 1312 	rcTwo.left	 = rcOrg.left	+1;
	.line	1312
	movl	-44(%ebp),%eax
	incl	%eax
	movl	%eax,-24(%ebp)
; 1313 	rcTwo.bottom 	 = rcOrg.bottom -1;
	.line	1313
	movl	-32(%ebp),%eax
	decl	%eax
	movl	%eax,-12(%ebp)
; 1314 	rcTwo.right	 = 3;
	.line	1314
	movl	$3,-16(%ebp)
; 1315 
; 1316 	hBrush = CreateSolidBrush(RGB(128,128, 128));
	.line	1316
	pushl	$0x808080
	call	_CreateSolidBrush@4
	movl	%eax,-28(%ebp)
; 1317 	//FillRect generates the overlaping
; 1318 	//by detecting painting all the time
; 1319 	//is better to draw only where the menuitem
; 1320 	//is not going to be drawing off
; 1321 	//that leaves the perimeter of the windows
; 1322 	FillRect(hDC, &rcTwo, hBrush);
	.line	1322
	pushl	%eax
	leal	-24(%ebp),%eax
	pushl	%eax
	pushl	-48(%ebp)
	call	_FillRect@12
; 1323 	rcTwo.top 	 = rcOrg.bottom	-3;
	.line	1323
	movl	-32(%ebp),%eax
	subl	$3,%eax
	movl	%eax,-20(%ebp)
; 1324 	rcTwo.left	 = rcOrg.left	+1;
	.line	1324
	movl	-44(%ebp),%eax
	incl	%eax
	movl	%eax,-24(%ebp)
; 1325 	rcTwo.bottom 	 = rcOrg.bottom -1;
	.line	1325
	movl	-32(%ebp),%eax
	decl	%eax
	movl	%eax,-12(%ebp)
; 1326 	rcTwo.right	 = 29;
	.line	1326
	movl	$29,-16(%ebp)
; 1327 
; 1328 	FillRect(hDC, &rcTwo, hBrush);
	.line	1328
	pushl	-28(%ebp)
	leal	-24(%ebp),%eax
	pushl	%eax
	pushl	-48(%ebp)
	call	_FillRect@12
; 1329 
; 1330 	rcTwo.top 	 = rcOrg.top	-1;
	.line	1330
	movl	-40(%ebp),%eax
	decl	%eax
	movl	%eax,-20(%ebp)
; 1331 	rcTwo.left	 = rcOrg.left	+1;
	.line	1331
	movl	-44(%ebp),%eax
	incl	%eax
	movl	%eax,-24(%ebp)
; 1332 	rcTwo.bottom 	 = rcOrg.top    +3;
	.line	1332
	movl	-40(%ebp),%eax
	addl	$3,%eax
	movl	%eax,-12(%ebp)
; 1333 	rcTwo.right	 = 29;
	.line	1333
	movl	$29,-16(%ebp)
; 1334 
; 1335 	FillRect(hDC, &rcTwo, hBrush);
	.line	1335
	pushl	-28(%ebp)
	leal	-24(%ebp),%eax
	pushl	%eax
	pushl	-48(%ebp)
	call	_FillRect@12
; 1336 
; 1337 	DeleteObject(hBrush);
	.line	1337
	pushl	-28(%ebp)
	call	_DeleteObject@4
; 1338 
; 1339 	//black perimeter
; 1340 	rcTwo.top 	 = rcOrg.top 	;
	.line	1340
	movl	-40(%ebp),%eax
	movl	%eax,-20(%ebp)
; 1341 	rcTwo.left	 = rcOrg.left	;
	.line	1341
	movl	-44(%ebp),%eax
	movl	%eax,-24(%ebp)
; 1342 	rcTwo.bottom 	 = rcOrg.bottom ;
	.line	1342
	movl	-32(%ebp),%eax
	movl	%eax,-12(%ebp)
; 1343 	rcTwo.right	 = bdr_w;
	.line	1343
	movl	$30,-16(%ebp)
; 1344 
; 1345 	hBrush = CreateSolidBrush(RGB(0,0, 0));
	.line	1345
	pushl	$0
	call	_CreateSolidBrush@4
	movl	%eax,-28(%ebp)
; 1346 	FrameRect(hDC, &rcTwo, hBrush);
	.line	1346
	pushl	%eax
	leal	-24(%ebp),%eax
	pushl	%eax
	pushl	-48(%ebp)
	call	_FrameRect@12
; 1347 	DeleteObject(hBrush);
	.line	1347
	pushl	-28(%ebp)
	call	_DeleteObject@4
; 1348 
; 1349 	HPEN saveObject = SelectObject(hDC,CreatePen(PS_SOLID, 1, RGB(255, 0, 0)));
	.line	1349
	pushl	$255
	pushl	$1
	pushl	$0
	call	_CreatePen@12
	pushl	%eax
	pushl	-48(%ebp)
	call	_SelectObject@8
	movl	%eax,-56(%ebp)
; 1350    	DeleteObject(SelectObject(hDC, saveObject));
	.line	1350
	pushl	%eax
	pushl	-48(%ebp)
	call	_SelectObject@8
	pushl	%eax
	call	_DeleteObject@4
; 1351 	return 0;
	.line	1351
	xor	%eax,%eax
	jmp	_$551
_$596:
; 1352 	}
; 1353 
; 1354 	case WM_SHOWWINDOW:
; 1355         {
	.line	1355
; 1356 	BOOL bShow = wParam != 0;
	.line	1356
	xorl	%eax,%eax
	cmpl	$0,16(%ebp)
	setne	%al
	movl	%eax,-12(%ebp)
; 1357 	 if (!bShow)
	.line	1357
	test	%eax,%eax
	jne	_$553
; 1358 		{
; 1359 	 SetWindowLong  (hWnd, GWL_WNDPROC, (DWORD)(DWORD_PTR)oldWndProc);
	.line	1359
	pushl	-4(%ebp)
	pushl	$-4
	pushl	%ebx
	call	_SetWindowLongA@12
; 1360 	 RemoveProp (hWnd, _WndPropName_OldProc);
	.line	1360
	pushl	$__WndPropName_OldProc
	pushl	%ebx
	call	_RemovePropA@8
; 1361 		}
; 1362 	break;
	.line	1362
	jmp	_$553
_$602:
; 1363         }
; 1364 	case WM_DESTROY:
; 1365 	{
	.line	1365
; 1366 	 SetWindowLong  (hWnd, GWL_WNDPROC, (DWORD)(DWORD_PTR)oldWndProc);
	.line	1366
	pushl	-4(%ebp)
	pushl	$-4
	pushl	%ebx
	call	_SetWindowLongA@12
; 1367          RemoveProp (hWnd, _WndPropName_OldProc);
	.line	1367
	pushl	$__WndPropName_OldProc
	pushl	%ebx
	call	_RemovePropA@8
; 1368          break;
	.line	1368
	jmp	_$553
_$603:
; 1369 	}
; 1370 	case WM_CLOSE:
; 1371 	{
	.line	1371
; 1372 	 SetWindowLong  (hWnd, GWL_WNDPROC, (DWORD)(DWORD_PTR)oldWndProc);
	.line	1372
	pushl	-4(%ebp)
	pushl	$-4
	pushl	%ebx
	call	_SetWindowLongA@12
; 1373          RemoveProp (hWnd, _WndPropName_OldProc);
	.line	1373
	pushl	$__WndPropName_OldProc
	pushl	%ebx
	call	_RemovePropA@8
; 1374          break;
	.line	1374
	jmp	_$553
_$604:
; 1375 	}
; 1376 	case WM_NCDESTROY:
; 1377 	{
	.line	1377
; 1378 	if ( oldWndProc != NULL )
	.line	1378
	cmpl	$0,-4(%ebp)
	je	_$553
; 1379     		{
; 1380 		WNDPROC NewWndProc = (WNDPROC)(LONG_PTR)GetWindowLong (hWnd, GWL_WNDPROC);
	.line	1380
	pushl	$-4
	pushl	%ebx
	call	_GetWindowLongA@8
	movl	%eax,-12(%ebp)
; 1381 		SetWindowLong (hWnd, GWL_WNDPROC, (DWORD)(DWORD_PTR)oldWndProc);
	.line	1381
	pushl	-4(%ebp)
	pushl	$-4
	pushl	%ebx
	call	_SetWindowLongA@12
; 1382         	RemoveProp (hWnd, _WndPropName_OldProc);
	.line	1382
	pushl	$__WndPropName_OldProc
	pushl	%ebx
	call	_RemovePropA@8
; 1383 	        NewWndProc = (WNDPROC)(LONG_PTR)GetWindowLong (hWnd, GWL_WNDPROC);
	.line	1383
	pushl	$-4
	pushl	%ebx
	call	_GetWindowLongA@8
	movl	%eax,-12(%ebp)
; 1384     	}
; 1385     	break;
	.line	1385
	jmp	_$553
_$552:
; 1386 	}
; 1387 
; 1388 	default:
; 1389 	return CallWindowProc (oldWndProc, hWnd, uMsg, wParam, lParam);
	.line	1389
	pushl	20(%ebp)
	pushl	16(%ebp)
	pushl	12(%ebp)
	pushl	%ebx
	pushl	-4(%ebp)
	call	_CallWindowProcA@20
	jmp	_$551
_$553:
; 1390 	}//End case
; 1391 	return CallWindowProc (oldWndProc, hWnd, uMsg, wParam, lParam);
	.line	1391
	pushl	20(%ebp)
	pushl	16(%ebp)
	pushl	12(%ebp)
	pushl	%ebx
	pushl	-4(%ebp)
	call	_CallWindowProcA@20
_$551:
; 1392 }
	.line	1392
	popl	%edi
	popl	%esi
	popl	%ebx
	leave
	ret	$16
_$623:
	.size	_menuproc_sclass@16,_$623-_menuproc_sclass@16
	.globl	_menuproc_sclass@16
; 1393 void initHook()
	.type	_initHook,function
_initHook:
	pushl	%ebp
	movl	%esp,%ebp
	pushl	%edi
; 1394 {
	.line	1394
; 1395 
; 1396 	hoo_handle = (HHOOK) SetWindowsHookEx(WH_CALLWNDPROC,&install_Hook,hInst,GetCurrentThreadId());
	.line	1396
	call	_GetCurrentThreadId@0
	pushl	%eax
	pushl	_hInst
	pushl	$_install_Hook@12
	pushl	$4
	call	_SetWindowsHookExA@16
	movl	%eax,_hoo_handle
; 1397 
; 1398 }
	.line	1398
	popl	%edi
	leave
	ret
_$627:
	.size	_initHook,_$627-_initHook
	.globl	_initHook
; 1399 
; 1400 void unitHook()
	.type	_unitHook,function
_unitHook:
	pushl	%ebp
	movl	%esp,%ebp
	pushl	%edi
; 1401 {
	.line	1401
; 1402 	WNDPROC NewWndProc = (WNDPROC)(LONG_PTR)GetWindowLong (mnu_hwnd, GWL_WNDPROC);
	.line	1402
	pushl	$-4
	pushl	_mnu_hwnd
	call	_GetWindowLongA@8
	movl	%eax,%edi
; 1403 
; 1404 	SetWindowLong (mnu_hwnd, GWL_WNDPROC,(DWORD)(DWORD_PTR)old_menu_proc);
	.line	1404
	pushl	_old_menu_proc
	pushl	$-4
	pushl	_mnu_hwnd
	call	_SetWindowLongA@12
; 1405 
; 1406 		    NewWndProc = (WNDPROC)(LONG_PTR)GetWindowLong (mnu_hwnd, GWL_WNDPROC);
	.line	1406
	pushl	$-4
	pushl	_mnu_hwnd
	call	_GetWindowLongA@8
	movl	%eax,%edi
; 1407 
; 1408 	RemoveProp (mnu_hwnd, _WndPropName_OldProc);
	.line	1408
	pushl	$__WndPropName_OldProc
	pushl	_mnu_hwnd
	call	_RemovePropA@8
; 1409 	UnhookWindowsHookEx(hoo_handle);
	.line	1409
	pushl	_hoo_handle
	call	_UnhookWindowsHookEx@4
; 1410 
; 1411 }
	.line	1411
	popl	%edi
	leave
	ret
_$631:
	.size	_unitHook,_$631-_unitHook
	.globl	_unitHook
; 1412 int draw_menu(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	.type	_draw_menu,function
_draw_menu:
	pushl	%ebp
	movl	%esp,%ebp
	subl	$296,%esp
	pushl	%ebx
	pushl	%esi
	pushl	%edi
; 1413 {
	.line	1413
; 1414 		RECT	rect;
; 1415 		RECT    rcSep;
; 1416 	    	HDC     mnu_hdc;
; 1417 		HDC     def_hdc;
; 1418 		BITMAP  bmp;
; 1419 		HBRUSH  hBrush;
; 1420 		//medidas
; 1421 
; 1422 		int border_w	= 26;
	.line	1422
	movl	$26,-28(%ebp)
; 1423 		int text_off	= 78;
	.line	1423
	movl	$78,-140(%ebp)
; 1424 		int inner_sq    = 4;
	.line	1424
	movl	$4,-144(%ebp)
; 1425 
; 1426 		int point_x	= 0;
	.line	1426
	andl	$0,-20(%ebp)
; 1427 		int point_y 	= 0;
	.line	1427
	xor	%ebx,%ebx
; 1428 
; 1429 		LPDRAWITEMSTRUCT lpDs = (LPDRAWITEMSTRUCT)lParam;
	.line	1429
	movl	20(%ebp),%esi
; 1430 		MENUITEMINFO mii;
; 1431 		MENU_DATA    rawi_data;
; 1432 		LPMENU_DATA  menu_data;
; 1433 
; 1434 		//copy before going reclusives search
; 1435 		DRAWITEMSTRUCT lpCopy;
; 1436 		CopyMemory(&lpCopy, lpDs, sizeof(DRAWITEMSTRUCT));
	.line	1436
	pushl	$48
	pushl	%esi
	leal	-136(%ebp),%eax
	pushl	%eax
	call	_memcpy
; 1437 
; 1438 		rawi_data = get_menu_formWID(lpDs->hwndItem,lpDs->itemID);
	.line	1438
	pushl	8(%esi)
	pushl	20(%esi)
	leal	-88(%ebp),%eax
	pushl	%eax
	call	_get_menu_formWID
	addl	$24,%esp
; 1439 		menu_data = & rawi_data;
	.line	1439
	leal	-88(%ebp),%eax
	movl	%eax,-32(%ebp)
; 1440 
; 1441 		lpDs = &lpCopy;
	.line	1441
	leal	-136(%ebp),%esi
; 1442 
; 1443 	if(lpDs->itemState & ODS_SELECTED)
	.line	1443
	testb	$1,16(%esi)
	je	_$634
; 1444 		{
; 1445 		rcSep.top 	= lpDs->rcItem.top    +1;
	.line	1445
	movl	32(%esi),%eax
	incl	%eax
	movl	%eax,-12(%ebp)
; 1446 		rcSep.left	= lpDs->rcItem.left   +1;
	.line	1446
	movl	28(%esi),%eax
	incl	%eax
	movl	%eax,-16(%ebp)
; 1447 		rcSep.bottom	= lpDs->rcItem.bottom -1;
	.line	1447
	movl	40(%esi),%eax
	decl	%eax
	movl	%eax,-4(%ebp)
; 1448 		rcSep.right	= lpDs->rcItem.right  -1;
	.line	1448
	movl	36(%esi),%eax
	decl	%eax
	movl	%eax,-8(%ebp)
; 1449 
; 1450 //	-----------------
; 1451 //	barra al costado
; 1452 //	-----------------
; 1453 
; 1454 		rcSep.top 	= lpDs->rcItem.top    ;
	.line	1454
	movl	32(%esi),%eax
	movl	%eax,-12(%ebp)
; 1455 		rcSep.left	= lpDs->rcItem.left   + border_w + 3;
	.line	1455
	movl	28(%esi),%eax
	addl	$29,%eax
	movl	%eax,-16(%ebp)
; 1456 		rcSep.bottom	= lpDs->rcItem.bottom ;
	.line	1456
	movl	40(%esi),%eax
	movl	%eax,-4(%ebp)
; 1457 		rcSep.right	= lpDs->rcItem.right  ;
	.line	1457
	movl	36(%esi),%eax
	movl	%eax,-8(%ebp)
; 1458 
; 1459 		hBrush = CreateSolidBrush(menu_data->menu_color);
	.line	1459
	movl	-32(%ebp),%eax
	pushl	40(%eax)
	call	_CreateSolidBrush@4
	movl	%eax,-24(%ebp)
; 1460 		FillRect(lpDs->hDC, &rcSep, hBrush);
	.line	1460
	pushl	%eax
	leal	-16(%ebp),%eax
	pushl	%eax
	pushl	24(%esi)
	call	_FillRect@12
; 1461 		DeleteObject(hBrush);
	.line	1461
	pushl	-24(%ebp)
	call	_DeleteObject@4
; 1462 
; 1463 //	---------------------
; 1464 //	perimetro mas obscuro
; 1465 //	---------------------
; 1466 
; 1467 		rcSep.top 	= lpDs->rcItem.top    ;
	.line	1467
	movl	32(%esi),%eax
	movl	%eax,-12(%ebp)
; 1468 		rcSep.left	= lpDs->rcItem.left   + border_w + 3;
	.line	1468
	movl	28(%esi),%eax
	addl	$29,%eax
	movl	%eax,-16(%ebp)
; 1469 		rcSep.bottom	= lpDs->rcItem.bottom ;
	.line	1469
	movl	40(%esi),%eax
	movl	%eax,-4(%ebp)
; 1470 		rcSep.right	= lpDs->rcItem.right   ;
	.line	1470
	movl	36(%esi),%eax
	movl	%eax,-8(%ebp)
; 1471 
; 1472 		hBrush = CreateSolidBrush(RGB(128, 128, 128));
	.line	1472
	pushl	$0x808080
	call	_CreateSolidBrush@4
	movl	%eax,-24(%ebp)
; 1473 		FrameRect(lpDs->hDC, &rcSep, hBrush);
	.line	1473
	pushl	%eax
	leal	-16(%ebp),%eax
	pushl	%eax
	pushl	24(%esi)
	call	_FrameRect@12
; 1474 		DeleteObject(hBrush);
	.line	1474
	pushl	-24(%ebp)
	call	_DeleteObject@4
; 1475 
; 1476 //	-----------------
; 1477 //	barra al costado
; 1478 //	-----------------
; 1479 		rcSep.top 	= lpDs->rcItem.top    ;
	.line	1479
	movl	32(%esi),%eax
	movl	%eax,-12(%ebp)
; 1480 		rcSep.left	= lpDs->rcItem.left   ;
	.line	1480
	movl	28(%esi),%eax
	movl	%eax,-16(%ebp)
; 1481 		rcSep.bottom	= lpDs->rcItem.bottom ;
	.line	1481
	movl	40(%esi),%edx
	movl	%edx,-4(%ebp)
; 1482 		rcSep.right	= rcSep.left + border_w  ;
	.line	1482
	addl	$26,%eax
	movl	%eax,-8(%ebp)
; 1483 
; 1484 		hBrush = CreateSolidBrush(RGB(128, 128, 128));
	.line	1484
	pushl	$0x808080
	call	_CreateSolidBrush@4
	movl	%eax,-24(%ebp)
; 1485 		FillRect(lpDs->hDC, &rcSep, hBrush);
	.line	1485
	pushl	%eax
	leal	-16(%ebp),%eax
	pushl	%eax
	pushl	24(%esi)
	call	_FillRect@12
; 1486 		DeleteObject(hBrush);
	.line	1486
	pushl	-24(%ebp)
	call	_DeleteObject@4
; 1487 
; 1488 //	-----------------------------------------------
; 1489 //	draws a line betwen the sidebar & the menuitem
; 1490 //	-----------------------------------------------
; 1491 		int y_t = lpDs->rcItem.top;
	.line	1491
	movl	32(%esi),%eax
	movl	%eax,-244(%ebp)
; 1492 		int x_t = border_w;
	.line	1492
	movl	$26,-240(%ebp)
; 1493 		HPEN saveObject = SelectObject(lpDs->hDC,CreatePen(PS_SOLID, 1, RGB(0, 0, 0)));
	.line	1493
	pushl	$0
	pushl	$1
	pushl	$0
	call	_CreatePen@12
	pushl	%eax
	pushl	24(%esi)
	call	_SelectObject@8
	movl	%eax,-248(%ebp)
; 1494 		MoveToEx(lpDs->hDC, x_t, y_t, NULL);
	.line	1494
	pushl	$0
	pushl	-244(%ebp)
	pushl	$26
	pushl	24(%esi)
	call	_MoveToEx@16
; 1495 		LineTo  (lpDs->hDC, x_t, y_t);
	.line	1495
	pushl	-244(%ebp)
	pushl	$26
	pushl	24(%esi)
	call	_LineTo@12
; 1496 		LineTo  (lpDs->hDC, x_t ,lpDs->rcItem.bottom);
	.line	1496
	pushl	40(%esi)
	pushl	$26
	pushl	24(%esi)
	call	_LineTo@12
; 1497 		DeleteObject(SelectObject(lpDs->hDC, saveObject));
	.line	1497
	pushl	-248(%ebp)
	pushl	24(%esi)
	call	_SelectObject@8
	pushl	%eax
	call	_DeleteObject@4
; 1498 
; 1499 //	--------------------------
; 1500 //	Icon Holder & mini sidedar
; 1501 //	--------------------------
; 1502 
; 1503 		if(lpDs->hwndItem == main_menu)
	.line	1503
	movl	_main_menu,%eax
	cmpl	%eax,20(%esi)
	jne	_$648
; 1504 		{
; 1505 		rcSep.top 	= lpDs->rcItem.top    +2;
	.line	1505
	movl	32(%esi),%eax
	addl	$2,%eax
	movl	%eax,-12(%ebp)
; 1506 		rcSep.left	= lpDs->rcItem.left   + border_w + 8;
	.line	1506
	movl	28(%esi),%eax
	addl	$34,%eax
	movl	%eax,-16(%ebp)
; 1507 		rcSep.bottom	= lpDs->rcItem.bottom -2;
	.line	1507
	movl	40(%esi),%edx
	subl	$2,%edx
	movl	%edx,-4(%ebp)
; 1508 		rcSep.right	= rcSep.left + 36 ;
	.line	1508
	addl	$36,%eax
	movl	%eax,-8(%ebp)
; 1509 
; 1510 			
; 1511 		DrawEdge(lpDs->hDC, &rcSep, BDR_SUNKENOUTER, BF_RECT);
	.line	1511
	pushl	$15
	pushl	$2
	leal	-16(%ebp),%eax
	pushl	%eax
	pushl	24(%esi)
	call	_DrawEdge@16
; 1512 		InflateRect(&rcSep, -1,-1);
	.line	1512
	pushl	$-1
	pushl	$-1
	leal	-16(%ebp),%eax
	pushl	%eax
	call	_InflateRect@12
; 1513 		hBrush = CreateSolidBrush(RGB(0, 0, 0));
	.line	1513
	pushl	$0
	call	_CreateSolidBrush@4
	movl	%eax,-24(%ebp)
; 1514 		FillRect(lpDs->hDC, &rcSep, hBrush);
	.line	1514
	pushl	%eax
	leal	-16(%ebp),%eax
	pushl	%eax
	pushl	24(%esi)
	call	_FillRect@12
; 1515 		DeleteObject(hBrush);
	.line	1515
	pushl	-24(%ebp)
	call	_DeleteObject@4
; 1516 
; 1517 		
; 1518 		BITMAP bm;
; 1519 		GetObject(menu_data->menu_image, sizeof(BITMAP), (LPSTR)&bm);
	.line	1519
	leal	-276(%ebp),%eax
	pushl	%eax
	pushl	$24
	movl	-32(%ebp),%eax
	pushl	24(%eax)
	call	_GetObjectA@12
; 1520  		point_y = ((lpDs->rcItem.bottom - lpDs->rcItem.top) / 2) - (bm.bmHeight/2);
	.line	1520
	movl	40(%esi),%edi
	movl	%edi,%eax
	subl	32(%esi),%eax
	xor	%edx,%edx
	testl	%eax,%eax
	setl	%dl
	addl	%edx,%eax
	sarl	$1,%eax
	movl	%eax,-280(%ebp)
	movl	-268(%ebp),%eax
	xor	%edx,%edx
	testl	%eax,%eax
	setl	%dl
	addl	%edx,%eax
	sarl	$1,%eax
	movl	-280(%ebp),%edi
	subl	%eax,%edi
	movl	%edi,%ebx
; 1521 		point_y += lpDs->rcItem.top;
	.line	1521
	addl	32(%esi),%ebx
; 1522 		point_x = (lpDs->rcItem.left + border_w + 10) + (((lpDs->rcItem.left + bm.bmWidth) / 2 ) - (bm.bmWidth / 2));
	.line	1522
	movl	28(%esi),%edi
	movl	%edi,%eax
	addl	-272(%ebp),%eax
	xor	%edx,%edx
	testl	%eax,%eax
	setl	%dl
	addl	%edx,%eax
	sarl	$1,%eax
	movl	%eax,-284(%ebp)
	movl	-272(%ebp),%eax
	xor	%edx,%edx
	testl	%eax,%eax
	setl	%dl
	addl	%edx,%eax
	sarl	$1,%eax
	movl	-284(%ebp),%ecx
	subl	%eax,%ecx
	leal	36(%edi,%ecx),%edi
	movl	%edi,-20(%ebp)
; 1523 		
; 1524 		DrawTransparentBitmap(lpDs->hDC,menu_data->menu_image,point_x,point_y,RGB(170,170,170));
	.line	1524
	pushl	$0xaaaaaa
	movl	%bx,%ax
	movswl	%ax,%eax
	pushl	%eax
	movl	%edi,%eax
	movswl	%ax,%eax
	pushl	%eax
	movl	-32(%ebp),%eax
	pushl	24(%eax)
	pushl	24(%esi)
	call	_DrawTransparentBitmap
	addl	$20,%esp
; 1525 //	-----------------		
; 1526 //	Separador obscuro
; 1527 //	-----------------
; 1528 		HPEN saveObject = SelectObject(lpDs->hDC,CreatePen(PS_SOLID, 1, RGB(100, 100, 100)));
	.line	1528
	pushl	$0x646464
	pushl	$1
	pushl	$0
	call	_CreatePen@12
	pushl	%eax
	pushl	24(%esi)
	call	_SelectObject@8
	movl	%eax,-252(%ebp)
; 1529 		point_x = rcSep.right + 4;
	.line	1529
	movl	-8(%ebp),%eax
	addl	$4,%eax
	movl	%eax,-20(%ebp)
; 1530 		point_y = rcSep.top;
	.line	1530
	movl	-12(%ebp),%ebx
; 1531 		MoveToEx(lpDs->hDC, point_x, point_y, NULL);
	.line	1531
	pushl	$0
	pushl	%ebx
	pushl	%eax
	pushl	24(%esi)
	call	_MoveToEx@16
; 1532 		LineTo  (lpDs->hDC, point_x, point_y);
	.line	1532
	pushl	%ebx
	pushl	-20(%ebp)
	pushl	24(%esi)
	call	_LineTo@12
; 1533 		LineTo  (lpDs->hDC, point_x, rcSep.bottom);
	.line	1533
	pushl	-4(%ebp)
	pushl	-20(%ebp)
	pushl	24(%esi)
	call	_LineTo@12
; 1534 		DeleteObject(SelectObject(lpDs->hDC, saveObject));
	.line	1534
	pushl	-252(%ebp)
	pushl	24(%esi)
	call	_SelectObject@8
	pushl	%eax
	call	_DeleteObject@4
; 1535 		//claro
; 1536 		saveObject = SelectObject(lpDs->hDC,CreatePen(PS_SOLID, 1, RGB(230, 230, 230)));
	.line	1536
	pushl	$0xe6e6e6
	pushl	$1
	pushl	$0
	call	_CreatePen@12
	pushl	%eax
	pushl	24(%esi)
	call	_SelectObject@8
	movl	%eax,-252(%ebp)
; 1537 		point_x += 1;
	.line	1537
	incl	-20(%ebp)
; 1538 		point_y = rcSep.top;
	.line	1538
	movl	-12(%ebp),%ebx
; 1539 		MoveToEx(lpDs->hDC, point_x, point_y, NULL);
	.line	1539
	pushl	$0
	pushl	%ebx
	pushl	-20(%ebp)
	pushl	24(%esi)
	call	_MoveToEx@16
; 1540 		LineTo  (lpDs->hDC, point_x, point_y);
	.line	1540
	pushl	%ebx
	pushl	-20(%ebp)
	pushl	24(%esi)
	call	_LineTo@12
; 1541 		LineTo  (lpDs->hDC, point_x, rcSep.bottom);
	.line	1541
	pushl	-4(%ebp)
	pushl	-20(%ebp)
	pushl	24(%esi)
	call	_LineTo@12
; 1542 		DeleteObject(SelectObject(lpDs->hDC, saveObject));
	.line	1542
	pushl	-252(%ebp)
	pushl	24(%esi)
	call	_SelectObject@8
	pushl	%eax
	call	_DeleteObject@4
	jmp	_$649
_$648:
; 1543 		}
; 1544 //	-------------------
; 1545 //	child drawind here
; 1546 //	-------------------
; 1547 		else
; 1548 		{
	.line	1548
; 1549 		rcSep.top 	= lpDs->rcItem.top     +1;
	.line	1549
	movl	32(%esi),%eax
	incl	%eax
	movl	%eax,-12(%ebp)
; 1550 		rcSep.left	= lpDs->rcItem.left    +1;
	.line	1550
	movl	28(%esi),%eax
	incl	%eax
	movl	%eax,-16(%ebp)
; 1551 		rcSep.bottom	= lpDs->rcItem.bottom  -1;
	.line	1551
	movl	40(%esi),%eax
	decl	%eax
	movl	%eax,-4(%ebp)
; 1552 		rcSep.right	= lpDs->rcItem.left   +24;
	.line	1552
	movl	28(%esi),%eax
	addl	$24,%eax
	movl	%eax,-8(%ebp)
; 1553 
; 1554 		hBrush = CreateSolidBrush(RGB(90, 90, 90));
	.line	1554
	pushl	$0x5a5a5a
	call	_CreateSolidBrush@4
	movl	%eax,-24(%ebp)
; 1555 		FrameRect(lpDs->hDC, &rcSep, hBrush);
	.line	1555
	pushl	%eax
	leal	-16(%ebp),%eax
	pushl	%eax
	pushl	24(%esi)
	call	_FrameRect@12
; 1556 		DeleteObject(hBrush);
	.line	1556
	pushl	-24(%ebp)
	call	_DeleteObject@4
; 1557 
; 1558 		InflateRect(&rcSep, -1,-1);
	.line	1558
	pushl	$-1
	pushl	$-1
	leal	-16(%ebp),%eax
	pushl	%eax
	call	_InflateRect@12
; 1559 		hBrush = CreateSolidBrush(RGB(170, 170, 170));
	.line	1559
	pushl	$0xaaaaaa
	call	_CreateSolidBrush@4
	movl	%eax,-24(%ebp)
; 1560 		FillRect(lpDs->hDC, &rcSep, hBrush);
	.line	1560
	pushl	%eax
	leal	-16(%ebp),%eax
	pushl	%eax
	pushl	24(%esi)
	call	_FillRect@12
; 1561 		DeleteObject(hBrush);
	.line	1561
	pushl	-24(%ebp)
	call	_DeleteObject@4
; 1562 
; 1563 		BITMAP bm;
; 1564 		GetObject(menu_data->menu_image, sizeof(BITMAP), (LPSTR)&bm);
	.line	1564
	leal	-272(%ebp),%eax
	pushl	%eax
	pushl	$24
	movl	-32(%ebp),%eax
	pushl	24(%eax)
	call	_GetObjectA@12
; 1565  		point_y = ((lpDs->rcItem.bottom - lpDs->rcItem.top) / 2) - (bm.bmHeight/2);
	.line	1565
	movl	40(%esi),%edi
	movl	%edi,%eax
	subl	32(%esi),%eax
	xor	%edx,%edx
	testl	%eax,%eax
	setl	%dl
	addl	%edx,%eax
	sarl	$1,%eax
	movl	%eax,-276(%ebp)
	movl	-264(%ebp),%eax
	xor	%edx,%edx
	testl	%eax,%eax
	setl	%dl
	addl	%edx,%eax
	sarl	$1,%eax
	movl	-276(%ebp),%edi
	subl	%eax,%edi
	movl	%edi,%ebx
; 1566 		point_y += lpDs->rcItem.top;
	.line	1566
	addl	32(%esi),%ebx
; 1567 		point_x = lpDs->rcItem.left + 5;
	.line	1567
	movl	28(%esi),%eax
	addl	$5,%eax
	movl	%eax,-20(%ebp)
; 1568 
; 1569 		DrawTransparentBitmap(lpDs->hDC,menu_data->menu_image,point_x,point_y,RGB(170,170,170));
	.line	1569
	pushl	$0xaaaaaa
	movl	%bx,%ax
	movswl	%ax,%eax
	pushl	%eax
	movl	-20(%ebp),%eax
	movswl	%ax,%eax
	pushl	%eax
	movl	-32(%ebp),%eax
	pushl	24(%eax)
	pushl	24(%esi)
	call	_DrawTransparentBitmap
	addl	$20,%esp
_$649:
; 1570 		//DrawTransparentBitmap(lpDs->hDC,menu_data->menu_image,rcSep.left-1,rcSep.top-1,RGB(170,170,170));
; 1571 		
; 1572 		
; 1573 		}
; 1574 //	--------
; 1575 //	Put logo
; 1576 //	--------
; 1577 		if(lpDs->hwndItem == main_menu)
	.line	1577
	movl	_main_menu,%eax
	cmpl	%eax,20(%esi)
	jne	_$665
; 1578 		{
; 1579 		HBITMAP numBMP = LoadImage(hInst, MAKEINTRESOURCE(ID_LOGO), IMAGE_BITMAP,0, 0, LR_LOADTRANSPARENT);
	.line	1579
	pushl	$32
	pushl	$0
	pushl	$0
	pushl	$0
	pushl	$0x320
	pushl	_hInst
	call	_LoadImageA@24
	movl	%eax,-256(%ebp)
; 1580 		GetObject(numBMP, sizeof(BITMAP), &bmp);
	.line	1580
	leal	-168(%ebp),%eax
	pushl	%eax
	pushl	$24
	pushl	-256(%ebp)
	call	_GetObjectA@12
; 1581 
; 1582 		// if need to know bmp size use this
; 1583 		int BmpWidth  = bmp.bmWidth;
	.line	1583
	movl	-164(%ebp),%eax
	movl	%eax,-260(%ebp)
; 1584 		int BmpHeight = bmp.bmHeight;
	.line	1584
	movl	-160(%ebp),%eax
	movl	%eax,-264(%ebp)
; 1585 
; 1586 		// get the drawing handle for the objects
; 1587 		HDC hDC = CreateCompatibleDC(lpDs->hDC);
	.line	1587
	pushl	24(%esi)
	call	_CreateCompatibleDC@4
	movl	%eax,-252(%ebp)
; 1588     		SelectObject(hDC, numBMP);
	.line	1588
	pushl	-256(%ebp)
	pushl	%eax
	call	_SelectObject@8
; 1589     		BitBlt(lpDs->hDC, 0, 0, 25, 25, hDC,0,0, SRCCOPY);
	.line	1589
	pushl	$0xcc0020
	pushl	$0
	pushl	$0
	pushl	-252(%ebp)
	pushl	$25
	pushl	$25
	pushl	$0
	pushl	$0
	pushl	24(%esi)
	call	_BitBlt@36
; 1590 		DeleteDC(hDC);
	.line	1590
	pushl	-252(%ebp)
	call	_DeleteDC@4
_$665:
; 1591 		}
; 1592 //	--------------
; 1593 //	texto caption
; 1594 //	--------------
; 1595 		if(lpDs->hwndItem == main_menu)
	.line	1595
	movl	_main_menu,%eax
	cmpl	%eax,20(%esi)
	jne	_$669
; 1596 		{
; 1597 		SIZE extent;
; 1598 		char* captionText = (char*)lpDs->itemData;
	.line	1598
	movl	44(%esi),%eax
	movl	%eax,-252(%ebp)
; 1599 		GetTextExtentPoint32(lpDs->hDC,captionText, lstrlen(captionText), &extent);
	.line	1599
	pushl	%eax
	call	_lstrlenA@4
	leal	-260(%ebp),%edi
	pushl	%edi
	pushl	%eax
	pushl	-252(%ebp)
	pushl	24(%esi)
	call	_GetTextExtentPoint32A@16
; 1600 		point_y = ((lpDs->rcItem.bottom - lpDs->rcItem.top) / 2) - (extent.cy/2);
	.line	1600
	movl	40(%esi),%edi
	movl	%edi,%eax
	subl	32(%esi),%eax
	xor	%edx,%edx
	testl	%eax,%eax
	setl	%dl
	addl	%edx,%eax
	sarl	$1,%eax
	movl	%eax,-264(%ebp)
	movl	-256(%ebp),%eax
	xor	%edx,%edx
	testl	%eax,%eax
	setl	%dl
	addl	%edx,%eax
	sarl	$1,%eax
	movl	-264(%ebp),%edi
	subl	%eax,%edi
	movl	%edi,%ebx
; 1601 		point_y += lpDs->rcItem.top;
	.line	1601
	addl	32(%esi),%ebx
; 1602 		
; 1603 		rcSep.top 	= lpDs->rcItem.top;
	.line	1603
	movl	32(%esi),%eax
	movl	%eax,-12(%ebp)
; 1604 		rcSep.left	= lpDs->rcItem.left   + text_off;
	.line	1604
	movl	28(%esi),%eax
	addl	-140(%ebp),%eax
	movl	%eax,-16(%ebp)
; 1605 		rcSep.bottom	= lpDs->rcItem.bottom;
	.line	1605
	movl	40(%esi),%eax
	movl	%eax,-4(%ebp)
; 1606 		rcSep.right	= lpDs->rcItem.right;
	.line	1606
	movl	36(%esi),%eax
	movl	%eax,-8(%ebp)
; 1607 		
; 1608 		//point_x = (lpDs->rcItem.left + border_w + 10) + (((lpDs->rcItem.left + bm.bmWidth) / 2 ) - (bm.bmWidth / 2));
; 1609 		
; 1610 		SetBkMode(lpDs->hDC,TRANSPARENT);
	.line	1610
	pushl	$1
	pushl	24(%esi)
	call	_SetBkMode@8
; 1611 		DrawText (lpDs->hDC,captionText, -1, &rcSep,
	.line	1611
	pushl	$36
	leal	-16(%ebp),%eax
	pushl	%eax
	pushl	$-1
	pushl	-252(%ebp)
	pushl	24(%esi)
	call	_DrawTextA@20
	jmp	_$635
_$669:
; 1612 		DT_SINGLELINE | DT_LEFT | DT_VCENTER);
; 1613 		}
; 1614 		else
; 1615 		{
	.line	1615
; 1616 		char* captionText = (char*)lpDs->itemData;	
	.line	1616
	movl	44(%esi),%eax
	movl	%eax,-252(%ebp)
; 1617 		rcSep.top 	= lpDs->rcItem.top;
	.line	1617
	movl	32(%esi),%eax
	movl	%eax,-12(%ebp)
; 1618 		rcSep.left	= lpDs->rcItem.left   + text_off - 45 ;
	.line	1618
	movl	28(%esi),%eax
	addl	-140(%ebp),%eax
	subl	$45,%eax
	movl	%eax,-16(%ebp)
; 1619 		rcSep.bottom	= lpDs->rcItem.bottom;
	.line	1619
	movl	40(%esi),%eax
	movl	%eax,-4(%ebp)
; 1620 		rcSep.right	= lpDs->rcItem.right;
	.line	1620
	movl	36(%esi),%eax
	movl	%eax,-8(%ebp)
; 1621 				
; 1622 		int PointSize = 8;
	.line	1622
	movl	$8,-256(%ebp)
; 1623 		int DPI = 72;
	.line	1623
	movl	$72,-260(%ebp)
; 1624 		int lfHeight = -MulDiv(PointSize, GetDeviceCaps(lpDs->hDC, LOGPIXELSY), DPI);
	.line	1624
	pushl	$90
	pushl	24(%esi)
	call	_GetDeviceCaps@8
	pushl	$72
	pushl	%eax
	pushl	$8
	call	_MulDiv@12
	movl	%eax,%edi
	negl	%edi
	movl	%edi,-264(%ebp)
; 1625 
; 1626 		HFONT hfnt = CreateAngledFont(lfHeight, 0, "Verdana");
	.line	1626
	pushl	$_$678
	pushl	$0
	pushl	%edi
	call	_CreateAngledFont
	addl	$12,%esp
	movl	%eax,-268(%ebp)
; 1627 		
; 1628 		HFONT hfntPrev = SelectObject(lpDs->hDC, hfnt);
	.line	1628
	pushl	%eax
	pushl	24(%esi)
	call	_SelectObject@8
	movl	%eax,-272(%ebp)
; 1629 	
; 1630 		SetBkMode(lpDs->hDC,TRANSPARENT);
	.line	1630
	pushl	$1
	pushl	24(%esi)
	call	_SetBkMode@8
; 1631 		DrawText (lpDs->hDC,captionText, -1, &rcSep,
	.line	1631
	pushl	$36
	leal	-16(%ebp),%eax
	pushl	%eax
	pushl	$-1
	pushl	-252(%ebp)
	pushl	24(%esi)
	call	_DrawTextA@20
; 1632 		DT_SINGLELINE | DT_LEFT | DT_VCENTER);
; 1633 		SelectObject(lpDs->hDC, hfntPrev);
	.line	1633
	pushl	-272(%ebp)
	pushl	24(%esi)
	call	_SelectObject@8
; 1634 		}
; 1635 	}
	.line	1635
	jmp	_$635
_$634:
; 1636 //	-------------------------------------------------
; 1637 //	State not selected no seleccionado
; 1638 //	menuitems disabed, separators, and bears go here
; 1639 //	-------------------------------------------------
; 1640 	else
; 1641 		{
	.line	1641
; 1642 //	------------------------
; 1643 //	Fill in the menu window
; 1644 //	------------------------
; 1645 		rcSep.top 	= lpDs->rcItem.top    ;
	.line	1645
	movl	32(%esi),%eax
	movl	%eax,-12(%ebp)
; 1646 		rcSep.left	= lpDs->rcItem.left   ;
	.line	1646
	movl	28(%esi),%eax
	movl	%eax,-16(%ebp)
; 1647 		rcSep.bottom	= lpDs->rcItem.bottom ;
	.line	1647
	movl	40(%esi),%eax
	movl	%eax,-4(%ebp)
; 1648 		rcSep.right	= lpDs->rcItem.right  ;
	.line	1648
	movl	36(%esi),%eax
	movl	%eax,-8(%ebp)
; 1649 
; 1650 
; 1651 		HBRUSH hBrush = CreateSolidBrush(RGB(195, 195, 195));
	.line	1651
	pushl	$0xc3c3c3
	call	_CreateSolidBrush@4
	movl	%eax,-240(%ebp)
; 1652 		FillRect(lpDs->hDC, &rcSep, hBrush);
	.line	1652
	pushl	%eax
	leal	-16(%ebp),%eax
	pushl	%eax
	pushl	24(%esi)
	call	_FillRect@12
; 1653 		DeleteObject(hBrush);
	.line	1653
	pushl	-240(%ebp)
	call	_DeleteObject@4
; 1654 //	-----------------
; 1655 //	barra al costado
; 1656 //	-----------------
; 1657 		rcSep.top 	= lpDs->rcItem.top    ;
	.line	1657
	movl	32(%esi),%eax
	movl	%eax,-12(%ebp)
; 1658 		rcSep.left	= lpDs->rcItem.left   ;
	.line	1658
	movl	28(%esi),%eax
	movl	%eax,-16(%ebp)
; 1659 		rcSep.bottom	= lpDs->rcItem.bottom ;
	.line	1659
	movl	40(%esi),%ecx
	movl	%ecx,-4(%ebp)
; 1660 		rcSep.right	= rcSep.left + border_w  ;
	.line	1660
	addl	-28(%ebp),%eax
	movl	%eax,-8(%ebp)
; 1661 
; 1662 		hBrush = CreateSolidBrush(RGB(128, 128, 128));
	.line	1662
	pushl	$0x808080
	call	_CreateSolidBrush@4
	movl	%eax,-240(%ebp)
; 1663 		FillRect(lpDs->hDC, &rcSep, hBrush);
	.line	1663
	pushl	%eax
	leal	-16(%ebp),%eax
	pushl	%eax
	pushl	24(%esi)
	call	_FillRect@12
; 1664 		DeleteObject(hBrush);
	.line	1664
	pushl	-240(%ebp)
	call	_DeleteObject@4
; 1665 
; 1666 //	-----------------------------------------------
; 1667 //	draws a line betwen the sidebar & the menuitem
; 1668 //	-----------------------------------------------
; 1669 		if(lpDs->hwndItem != main_menu)
	.line	1669
	movl	_main_menu,%eax
	cmpl	%eax,20(%esi)
	je	_$685
_$685:
; 1670 		{
; 1671 		//border_w = 30;
; 1672 		}
; 1673 		int y_t = lpDs->rcItem.top;
	.line	1673
	movl	32(%esi),%eax
	movl	%eax,-248(%ebp)
; 1674 		int x_t = border_w;
	.line	1674
	movl	-28(%ebp),%eax
	movl	%eax,-244(%ebp)
; 1675 		HPEN saveObject = SelectObject(lpDs->hDC,CreatePen(PS_SOLID, 1, RGB(0, 0, 0)));
	.line	1675
	pushl	$0
	pushl	$1
	pushl	$0
	call	_CreatePen@12
	pushl	%eax
	pushl	24(%esi)
	call	_SelectObject@8
	movl	%eax,-252(%ebp)
; 1676 		MoveToEx(lpDs->hDC, x_t, y_t, NULL);
	.line	1676
	pushl	$0
	pushl	-248(%ebp)
	pushl	-244(%ebp)
	pushl	24(%esi)
	call	_MoveToEx@16
; 1677 		LineTo  (lpDs->hDC, x_t, y_t);
	.line	1677
	pushl	-248(%ebp)
	pushl	-244(%ebp)
	pushl	24(%esi)
	call	_LineTo@12
; 1678 		LineTo  (lpDs->hDC, x_t ,lpDs->rcItem.bottom);
	.line	1678
	pushl	40(%esi)
	pushl	-244(%ebp)
	pushl	24(%esi)
	call	_LineTo@12
; 1679 		DeleteObject(SelectObject(lpDs->hDC, saveObject));
	.line	1679
	pushl	-252(%ebp)
	pushl	24(%esi)
	call	_SelectObject@8
	pushl	%eax
	call	_DeleteObject@4
; 1680 
; 1681 //	--------------------------------------
; 1682 //	Draws the icon holder & the separator
; 1683 //	--------------------------------------
; 1684 		if(lpDs->hwndItem == main_menu && lpDs->itemData > 0)
	.line	1684
	movl	_main_menu,%eax
	cmpl	%eax,20(%esi)
	jne	_$687
	cmpl	$0,44(%esi)
	je	_$687
; 1685 		{
; 1686 		rcSep.top 	= lpDs->rcItem.top    +2;
	.line	1686
	movl	32(%esi),%eax
	addl	$2,%eax
	movl	%eax,-12(%ebp)
; 1687 		rcSep.left	= lpDs->rcItem.left   + border_w + 8;
	.line	1687
	movl	28(%esi),%eax
	movl	-28(%ebp),%edx
	leal	8(%eax,%edx),%eax
	movl	%eax,-16(%ebp)
; 1688 		rcSep.bottom	= lpDs->rcItem.bottom -2;
	.line	1688
	movl	40(%esi),%eax
	subl	$2,%eax
	movl	%eax,-4(%ebp)
; 1689 		rcSep.right	= rcSep.left + 36 ;
	.line	1689
	movl	-16(%ebp),%eax
	addl	$36,%eax
	movl	%eax,-8(%ebp)
; 1690 
; 1691 			
; 1692 		DrawEdge(lpDs->hDC, &rcSep, BDR_SUNKENOUTER, BF_RECT);
	.line	1692
	pushl	$15
	pushl	$2
	leal	-16(%ebp),%eax
	pushl	%eax
	pushl	24(%esi)
	call	_DrawEdge@16
; 1693 		InflateRect(&rcSep, -1,-1);
	.line	1693
	pushl	$-1
	pushl	$-1
	leal	-16(%ebp),%eax
	pushl	%eax
	call	_InflateRect@12
; 1694 		hBrush = CreateSolidBrush(RGB(0, 0, 0));
	.line	1694
	pushl	$0
	call	_CreateSolidBrush@4
	movl	%eax,-240(%ebp)
; 1695 		FillRect(lpDs->hDC, &rcSep, hBrush);
	.line	1695
	pushl	%eax
	leal	-16(%ebp),%eax
	pushl	%eax
	pushl	24(%esi)
	call	_FillRect@12
; 1696 		DeleteObject(hBrush);
	.line	1696
	pushl	-240(%ebp)
	call	_DeleteObject@4
; 1697 		
; 1698 		
; 1699 		BITMAP bm;
; 1700 		GetObject(menu_data->menu_image, sizeof(BITMAP), (LPSTR)&bm);
	.line	1700
	leal	-280(%ebp),%eax
	pushl	%eax
	pushl	$24
	movl	-32(%ebp),%eax
	pushl	24(%eax)
	call	_GetObjectA@12
; 1701  		point_y = ((lpDs->rcItem.bottom - lpDs->rcItem.top) / 2) - (bm.bmHeight/2);
	.line	1701
	movl	40(%esi),%edi
	movl	%edi,%eax
	subl	32(%esi),%eax
	xor	%edx,%edx
	testl	%eax,%eax
	setl	%dl
	addl	%edx,%eax
	sarl	$1,%eax
	movl	%eax,-284(%ebp)
	movl	-272(%ebp),%eax
	xor	%edx,%edx
	testl	%eax,%eax
	setl	%dl
	addl	%edx,%eax
	sarl	$1,%eax
	movl	-284(%ebp),%edi
	subl	%eax,%edi
	movl	%edi,%ebx
; 1702 		point_y += lpDs->rcItem.top;
	.line	1702
	addl	32(%esi),%ebx
; 1703 		point_x = (lpDs->rcItem.left + border_w + 10) + (((lpDs->rcItem.left + bm.bmWidth) / 2 ) - (bm.bmWidth / 2));
	.line	1703
	movl	28(%esi),%edi
	movl	-28(%ebp),%edx
	leal	10(%edi,%edx),%edx
	movl	%edx,-292(%ebp)
	movl	%edi,%eax
	addl	-276(%ebp),%eax
	movl	%eax,-288(%ebp)
	xor	%edx,%edx
	testl	%eax,%eax
	setl	%dl
	addl	%edx,%eax
	sarl	$1,%eax
	movl	%eax,-296(%ebp)
	movl	-276(%ebp),%eax
	xor	%edx,%edx
	testl	%eax,%eax
	setl	%dl
	addl	%edx,%eax
	sarl	$1,%eax
	movl	-296(%ebp),%edi
	subl	%eax,%edi
	movl	-292(%ebp),%edx
	addl	%edi,%edx
	movl	%edx,-20(%ebp)
; 1704 		
; 1705 		DrawTransparentBitmap(lpDs->hDC,menu_data->menu_image,point_x,point_y,RGB(170,170,170));
	.line	1705
	pushl	$0xaaaaaa
	movl	%bx,%ax
	movswl	%ax,%eax
	pushl	%eax
	movl	%edx,%eax
	movswl	%ax,%eax
	pushl	%eax
	movl	-32(%ebp),%eax
	pushl	24(%eax)
	pushl	24(%esi)
	call	_DrawTransparentBitmap
	addl	$20,%esp
; 1706 		
; 1707 //	-----------------		
; 1708 //	Separador obscuro
; 1709 //	-----------------
; 1710 		HPEN saveObject = SelectObject(lpDs->hDC,CreatePen(PS_SOLID, 1, RGB(128, 128, 128)));
	.line	1710
	pushl	$0x808080
	pushl	$1
	pushl	$0
	call	_CreatePen@12
	pushl	%eax
	pushl	24(%esi)
	call	_SelectObject@8
	movl	%eax,-256(%ebp)
; 1711 		point_x = rcSep.right + 4;
	.line	1711
	movl	-8(%ebp),%eax
	addl	$4,%eax
	movl	%eax,-20(%ebp)
; 1712 		point_y = rcSep.top;
	.line	1712
	movl	-12(%ebp),%ebx
; 1713 		MoveToEx(lpDs->hDC, point_x, point_y, NULL);
	.line	1713
	pushl	$0
	pushl	%ebx
	pushl	%eax
	pushl	24(%esi)
	call	_MoveToEx@16
; 1714 		LineTo  (lpDs->hDC, point_x, point_y);
	.line	1714
	pushl	%ebx
	pushl	-20(%ebp)
	pushl	24(%esi)
	call	_LineTo@12
; 1715 		LineTo  (lpDs->hDC, point_x, rcSep.bottom);
	.line	1715
	pushl	-4(%ebp)
	pushl	-20(%ebp)
	pushl	24(%esi)
	call	_LineTo@12
; 1716 		DeleteObject(SelectObject(lpDs->hDC, saveObject));
	.line	1716
	pushl	-256(%ebp)
	pushl	24(%esi)
	call	_SelectObject@8
	pushl	%eax
	call	_DeleteObject@4
; 1717 		//claro
; 1718 		saveObject = SelectObject(lpDs->hDC,CreatePen(PS_SOLID, 1, RGB(230, 230, 230)));
	.line	1718
	pushl	$0xe6e6e6
	pushl	$1
	pushl	$0
	call	_CreatePen@12
	pushl	%eax
	pushl	24(%esi)
	call	_SelectObject@8
	movl	%eax,-256(%ebp)
; 1719 		point_x += 1;
	.line	1719
	incl	-20(%ebp)
; 1720 		MoveToEx(lpDs->hDC, point_x, point_y, NULL);
	.line	1720
	pushl	$0
	pushl	%ebx
	pushl	-20(%ebp)
	pushl	24(%esi)
	call	_MoveToEx@16
; 1721 		LineTo  (lpDs->hDC, point_x, point_y);
	.line	1721
	pushl	%ebx
	pushl	-20(%ebp)
	pushl	24(%esi)
	call	_LineTo@12
; 1722 		LineTo  (lpDs->hDC, point_x, rcSep.bottom);
	.line	1722
	pushl	-4(%ebp)
	pushl	-20(%ebp)
	pushl	24(%esi)
	call	_LineTo@12
; 1723 		DeleteObject(SelectObject(lpDs->hDC, saveObject));
	.line	1723
	pushl	-256(%ebp)
	pushl	24(%esi)
	call	_SelectObject@8
	pushl	%eax
	call	_DeleteObject@4
_$687:
; 1724 		}
; 1725 		if(lpDs->hwndItem != main_menu && lpDs->itemData > 0)
	.line	1725
	movl	_main_menu,%eax
	cmpl	%eax,20(%esi)
	je	_$699
	cmpl	$0,44(%esi)
	je	_$699
; 1726 		{
; 1727 		rcSep.top 	= lpDs->rcItem.top     +1;
	.line	1727
	movl	32(%esi),%eax
	incl	%eax
	movl	%eax,-12(%ebp)
; 1728 		rcSep.left	= lpDs->rcItem.left    +1;
	.line	1728
	movl	28(%esi),%eax
	incl	%eax
	movl	%eax,-16(%ebp)
; 1729 		rcSep.bottom	= lpDs->rcItem.bottom  -1;
	.line	1729
	movl	40(%esi),%eax
	decl	%eax
	movl	%eax,-4(%ebp)
; 1730 		rcSep.right	= lpDs->rcItem.left   +24;
	.line	1730
	movl	28(%esi),%eax
	addl	$24,%eax
	movl	%eax,-8(%ebp)
; 1731 //	---------
; 1732 //	mini menu
; 1733 //	----------
; 1734 		BITMAP bm;
; 1735 		GetObject(menu_data->menu_image, sizeof(BITMAP), (LPSTR)&bm);
	.line	1735
	leal	-276(%ebp),%eax
	pushl	%eax
	pushl	$24
	movl	-32(%ebp),%eax
	pushl	24(%eax)
	call	_GetObjectA@12
; 1736  		//ptSize.x = bm.bmWidth;            // Get width of bitmap
; 1737    		//ptSize.y = bm.bmHeight;           // Get height of bitmap
; 1738 		point_y = ((lpDs->rcItem.bottom - lpDs->rcItem.top) / 2) - (bm.bmHeight/2);
	.line	1738
	movl	40(%esi),%edi
	movl	%edi,%eax
	subl	32(%esi),%eax
	xor	%edx,%edx
	testl	%eax,%eax
	setl	%dl
	addl	%edx,%eax
	sarl	$1,%eax
	movl	%eax,-280(%ebp)
	movl	-268(%ebp),%eax
	xor	%edx,%edx
	testl	%eax,%eax
	setl	%dl
	addl	%edx,%eax
	sarl	$1,%eax
	movl	-280(%ebp),%edi
	subl	%eax,%edi
	movl	%edi,%ebx
; 1739 		point_y += lpDs->rcItem.top;
	.line	1739
	addl	32(%esi),%ebx
; 1740 
; 1741 		//point_x = 15 - (bm.bmWidth/2);
; 1742 		point_x = lpDs->rcItem.left + 5;
	.line	1742
	movl	28(%esi),%eax
	addl	$5,%eax
	movl	%eax,-20(%ebp)
; 1743 
; 1744 		//HDC hDC    = CreateCompatibleDC(lpDs->hDC);
; 1745     		//SelectObject(hDC, menu_data->menu_image);
; 1746     		//BitBlt(lpDs->hDC,rcSep.left, rcSep.top, 22, 22, hDC, 0, 0, SRCCOPY);
; 1747 	  	DrawTransparentBitmap(lpDs->hDC,menu_data->menu_image,point_x,point_y,RGB(170,170,170));
	.line	1747
	pushl	$0xaaaaaa
	movl	%bx,%ax
	movswl	%ax,%eax
	pushl	%eax
	movl	-20(%ebp),%eax
	movswl	%ax,%eax
	pushl	%eax
	movl	-32(%ebp),%eax
	pushl	24(%eax)
	pushl	24(%esi)
	call	_DrawTransparentBitmap
	addl	$20,%esp
_$699:
; 1748 		//DeleteDC(hDC);
; 1749 
; 1750 	}
; 1751 //	----------------------
; 1752 //	Side bar text & logo
; 1753 //	----------------------
; 1754 		if(lpDs->hwndItem == main_menu)
	.line	1754
	movl	_main_menu,%eax
	cmpl	%eax,20(%esi)
	jne	_$705
; 1755 		{
; 1756 		int PointSize = 7;
	.line	1756
	movl	$7,-264(%ebp)
; 1757 		int DPI = 72;
	.line	1757
	movl	$72,-268(%ebp)
; 1758 		int lfHeight = -MulDiv(PointSize, GetDeviceCaps(lpDs->hDC, LOGPIXELSY), DPI);
	.line	1758
	pushl	$90
	pushl	24(%esi)
	call	_GetDeviceCaps@8
	pushl	$72
	pushl	%eax
	pushl	$7
	call	_MulDiv@12
	movl	%eax,%edi
	negl	%edi
	movl	%edi,-272(%ebp)
; 1759 
; 1760 		HFONT hfnt = CreateAngledFont(lfHeight, 90, "Verdana");
	.line	1760
	pushl	$_$678
	pushl	$90
	pushl	%edi
	call	_CreateAngledFont
	addl	$12,%esp
	movl	%eax,-276(%ebp)
; 1761 		LPCTSTR lpszLable = "MENU HELPER";
	.line	1761
	leal	_$707,%eax
	movl	%eax,-260(%ebp)
; 1762 		SetBkMode(lpDs->hDC, TRANSPARENT);
	.line	1762
	pushl	$1
	pushl	24(%esi)
	call	_SetBkMode@8
; 1763 		COLORREF prevColor = SetTextColor(lpDs->hDC, RGB(190,190,190));
	.line	1763
	pushl	$0xbebebe
	pushl	24(%esi)
	call	_SetTextColor@8
	movl	%eax,-280(%ebp)
; 1764 		HFONT hfntPrev = SelectObject(lpDs->hDC, hfnt);
	.line	1764
	pushl	-276(%ebp)
	pushl	24(%esi)
	call	_SelectObject@8
	movl	%eax,-284(%ebp)
; 1765 
; 1766 		BOOL ok = TextOut(lpDs->hDC, 0 , 200 , lpszLable, lstrlen(lpszLable));		//Restore defaults
	.line	1766
	pushl	-260(%ebp)
	call	_lstrlenA@4
	pushl	%eax
	pushl	-260(%ebp)
	pushl	$200
	pushl	$0
	pushl	24(%esi)
	call	_TextOutA@20
	movl	%eax,-292(%ebp)
; 1767 		SelectObject(lpDs->hDC, hfntPrev);
	.line	1767
	pushl	-284(%ebp)
	pushl	24(%esi)
	call	_SelectObject@8
; 1768 		SetTextColor(lpDs->hDC, prevColor);
	.line	1768
	pushl	-280(%ebp)
	pushl	24(%esi)
	call	_SetTextColor@8
; 1769 		SetBkMode(lpDs->hDC, OPAQUE);
	.line	1769
	pushl	$2
	pushl	24(%esi)
	call	_SetBkMode@8
; 1770 		//
; 1771 		//Put logo
; 1772 		//
; 1773 		HBITMAP BMPlogo = LoadImage(hInst, MAKEINTRESOURCE(ID_LOGO), IMAGE_BITMAP,0, 0, LR_LOADMAP3DCOLORS);
	.line	1773
	pushl	$4096
	pushl	$0
	pushl	$0
	pushl	$0
	pushl	$0x320
	pushl	_hInst
	call	_LoadImageA@24
	movl	%eax,-288(%ebp)
; 1774 
; 1775 
; 1776 		HDC hDC = CreateCompatibleDC(lpDs->hDC);
	.line	1776
	pushl	24(%esi)
	call	_CreateCompatibleDC@4
	movl	%eax,-256(%ebp)
; 1777     		SelectObject(hDC, BMPlogo);
	.line	1777
	pushl	-288(%ebp)
	pushl	%eax
	call	_SelectObject@8
; 1778     		BitBlt(lpDs->hDC, 0, 0, 25, 25, hDC,0, 0, SRCCOPY);
	.line	1778
	pushl	$0xcc0020
	pushl	$0
	pushl	$0
	pushl	-256(%ebp)
	pushl	$25
	pushl	$25
	pushl	$0
	pushl	$0
	pushl	24(%esi)
	call	_BitBlt@36
; 1779 
; 1780     		DeleteDC(hDC);
	.line	1780
	pushl	-256(%ebp)
	call	_DeleteDC@4
_$705:
; 1781     		}
; 1782 
; 1783 //	-------------------
; 1784 //	MF_SEPARATOR
; 1785 //	-------------------
; 1786 	if(lpDs->itemData == 0)
	.line	1786
	cmpl	$0,44(%esi)
	jne	_$708
; 1787 		{
; 1788 		int y_t = (lpDs->rcItem.top + lpDs->rcItem.bottom - 1) / 2;
	.line	1788
	movl	32(%esi),%edi
	addl	40(%esi),%edi
	movl	%edi,%eax
	decl	%eax
	xor	%edx,%edx
	testl	%eax,%eax
	setl	%dl
	addl	%edx,%eax
	sarl	$1,%eax
	movl	%eax,-256(%ebp)
; 1789 		int x_t = lpDs->rcItem.left + 32;
	.line	1789
	movl	28(%esi),%eax
	addl	$32,%eax
	movl	%eax,-260(%ebp)
; 1790 		HPEN saveObject = SelectObject(lpDs->hDC,CreatePen(PS_SOLID, 1, RGB(130, 130, 130)));
	.line	1790
	pushl	$0x828282
	pushl	$1
	pushl	$0
	call	_CreatePen@12
	pushl	%eax
	pushl	24(%esi)
	call	_SelectObject@8
	movl	%eax,-264(%ebp)
; 1791 		MoveToEx(lpDs->hDC, x_t, y_t, NULL);
	.line	1791
	pushl	$0
	pushl	-256(%ebp)
	pushl	-260(%ebp)
	pushl	24(%esi)
	call	_MoveToEx@16
; 1792 		LineTo  (lpDs->hDC, x_t, y_t);
	.line	1792
	pushl	-256(%ebp)
	pushl	-260(%ebp)
	pushl	24(%esi)
	call	_LineTo@12
; 1793 		LineTo  (lpDs->hDC, x_t + lpDs->rcItem.right - 5, y_t);
	.line	1793
	pushl	-256(%ebp)
	movl	-260(%ebp),%eax
	addl	36(%esi),%eax
	subl	$5,%eax
	pushl	%eax
	pushl	24(%esi)
	call	_LineTo@12
; 1794 		DeleteObject(SelectObject(lpDs->hDC, saveObject));
	.line	1794
	pushl	-264(%ebp)
	pushl	24(%esi)
	call	_SelectObject@8
	pushl	%eax
	call	_DeleteObject@4
; 1795 		saveObject = SelectObject(lpDs->hDC,CreatePen(PS_SOLID, 1, RGB(220, 220, 220)));
	.line	1795
	pushl	$0xdcdcdc
	pushl	$1
	pushl	$0
	call	_CreatePen@12
	pushl	%eax
	pushl	24(%esi)
	call	_SelectObject@8
	movl	%eax,-264(%ebp)
; 1796 
; 1797 		MoveToEx(lpDs->hDC, x_t, y_t+1, NULL);
	.line	1797
	pushl	$0
	movl	-256(%ebp),%eax
	incl	%eax
	pushl	%eax
	pushl	-260(%ebp)
	pushl	24(%esi)
	call	_MoveToEx@16
; 1798 		LineTo  (lpDs->hDC, x_t, y_t+1);
	.line	1798
	movl	-256(%ebp),%eax
	incl	%eax
	pushl	%eax
	pushl	-260(%ebp)
	pushl	24(%esi)
	call	_LineTo@12
; 1799 		LineTo  (lpDs->hDC, x_t + lpDs->rcItem.right - 5, y_t+1);
	.line	1799
	movl	-256(%ebp),%eax
	incl	%eax
	pushl	%eax
	movl	-260(%ebp),%eax
	addl	36(%esi),%eax
	subl	$5,%eax
	pushl	%eax
	pushl	24(%esi)
	call	_LineTo@12
; 1800 		DeleteObject(SelectObject(lpDs->hDC, saveObject));
	.line	1800
	pushl	-264(%ebp)
	pushl	24(%esi)
	call	_SelectObject@8
	pushl	%eax
	call	_DeleteObject@4
	jmp	_$635
_$708:
; 1801 		}
; 1802 //finalemente si todo esta ok dibujo el texto solo si:
; 1803 //no es un separador y al futuro chequear que no este tachado
; 1804 	else
; 1805 	if(lpDs->hwndItem == main_menu)
	.line	1805
	movl	_main_menu,%eax
	cmpl	%eax,20(%esi)
	jne	_$710
; 1806 		{
; 1807 		rcSep.top 	= lpDs->rcItem.top    ;
	.line	1807
	movl	32(%esi),%eax
	movl	%eax,-12(%ebp)
; 1808 		rcSep.left	= lpDs->rcItem.left   + text_off;
	.line	1808
	movl	28(%esi),%eax
	addl	-140(%ebp),%eax
	movl	%eax,-16(%ebp)
; 1809 		rcSep.bottom	= lpDs->rcItem.bottom ;
	.line	1809
	movl	40(%esi),%eax
	movl	%eax,-4(%ebp)
; 1810 		rcSep.right	= lpDs->rcItem.right   ;
	.line	1810
	movl	36(%esi),%eax
	movl	%eax,-8(%ebp)
; 1811 		//Fondo transparente
; 1812 		SetBkMode(lpDs->hDC,TRANSPARENT);
	.line	1812
	pushl	$1
	pushl	24(%esi)
	call	_SetBkMode@8
; 1813 		char* statusText = (char*)lpDs->itemData;
	.line	1813
	movl	44(%esi),%eax
	movl	%eax,-256(%ebp)
; 1814 		DrawText (lpDs->hDC, statusText, -1, &rcSep, DT_SINGLELINE | DT_LEFT | DT_VCENTER);
	.line	1814
	pushl	$36
	leal	-16(%ebp),%eax
	pushl	%eax
	pushl	$-1
	pushl	-256(%ebp)
	pushl	24(%esi)
	call	_DrawTextA@20
	jmp	_$635
_$710:
; 1815 		}
; 1816 	else
; 1817 		{
	.line	1817
; 1818 		char* captionText = (char*)lpDs->itemData;
	.line	1818
	movl	44(%esi),%eax
	movl	%eax,-256(%ebp)
; 1819 		rcSep.top 	= lpDs->rcItem.top;
	.line	1819
	movl	32(%esi),%eax
	movl	%eax,-12(%ebp)
; 1820 		rcSep.left	= lpDs->rcItem.left   + text_off - 45 ;
	.line	1820
	movl	28(%esi),%eax
	addl	-140(%ebp),%eax
	subl	$45,%eax
	movl	%eax,-16(%ebp)
; 1821 		rcSep.bottom	= lpDs->rcItem.bottom;
	.line	1821
	movl	40(%esi),%eax
	movl	%eax,-4(%ebp)
; 1822 		rcSep.right	= lpDs->rcItem.right;
	.line	1822
	movl	36(%esi),%eax
	movl	%eax,-8(%ebp)
; 1823 		
; 1824 		int PointSize = 8;
	.line	1824
	movl	$8,-260(%ebp)
; 1825 		int DPI = 72;
	.line	1825
	movl	$72,-264(%ebp)
; 1826 		int lfHeight = -MulDiv(PointSize, GetDeviceCaps(lpDs->hDC, LOGPIXELSY), DPI);
	.line	1826
	pushl	$90
	pushl	24(%esi)
	call	_GetDeviceCaps@8
	pushl	$72
	pushl	%eax
	pushl	$8
	call	_MulDiv@12
	movl	%eax,%edi
	negl	%edi
	movl	%edi,-268(%ebp)
; 1827 
; 1828 		HFONT hfnt = CreateAngledFont(lfHeight, 0, "Verdana");
	.line	1828
	pushl	$_$678
	pushl	$0
	pushl	%edi
	call	_CreateAngledFont
	addl	$12,%esp
	movl	%eax,-272(%ebp)
; 1829 		HFONT hfntPrev = SelectObject(lpDs->hDC, hfnt);
	.line	1829
	pushl	%eax
	pushl	24(%esi)
	call	_SelectObject@8
	movl	%eax,-276(%ebp)
; 1830 	
; 1831 		SetBkMode(lpDs->hDC,TRANSPARENT);
	.line	1831
	pushl	$1
	pushl	24(%esi)
	call	_SetBkMode@8
; 1832 		
; 1833 		DrawText (lpDs->hDC,captionText, -1, &rcSep,
	.line	1833
	pushl	$36
	leal	-16(%ebp),%eax
	pushl	%eax
	pushl	$-1
	pushl	-256(%ebp)
	pushl	24(%esi)
	call	_DrawTextA@20
; 1834 		DT_SINGLELINE | DT_LEFT | DT_VCENTER);
; 1835 		SelectObject(lpDs->hDC, hfntPrev);
	.line	1835
	pushl	-276(%ebp)
	pushl	24(%esi)
	call	_SelectObject@8
; 1836 		}
; 1837 	}
	.line	1837
_$635:
; 1838 if (lpDs->itemState & ODS_GRAYED )
	.line	1838
	testb	$2,16(%esi)
	je	_$718
_$718:
; 1839 {
; 1840 	
; 1841 	
; 1842 	
; 1843 }
; 1844 
; 1845 freeObjects:
; 1846 	free(menu_data);
	.line	1846
	pushl	-32(%ebp)
	call	_free
	popl	%ecx
; 1847 	return 0;
	.line	1847
	xor	%eax,%eax
; 1848 }
	.line	1848
	popl	%edi
	popl	%esi
	popl	%ebx
	leave
	ret
_$785:
	.size	_draw_menu,_$785-_draw_menu
	.globl	_draw_menu
; 1849 int draw_buttons(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	.type	_draw_buttons,function
_draw_buttons:
	pushl	%ebp
	movl	%esp,%ebp
	subl	$40,%esp
	pushl	%ebx
	pushl	%esi
	pushl	%edi
; 1850 {
	.line	1850
; 1851 
; 1852 	LPDRAWITEMSTRUCT lpDs = (LPDRAWITEMSTRUCT)lParam;
	.line	1852
	movl	20(%ebp),%edi
; 1853 	HDC 	hDC;
; 1854 	RECT 	rcSep;
; 1855 	HBRUSH  hBrush;
; 1856 	HWND	temp_hwnd;
; 1857 	HBITMAP temp_bmp[3];
; 1858 	int pic_w;
; 1859 	int pic_h;
; 1860 	//	copia
; 1861 
; 1862 	temp_hwnd = lpDs->hwndItem;
	.line	1862
	movl	20(%edi),%esi
; 1863 
; 1864 	hDC = CreateCompatibleDC(lpDs->hDC);	// create a memory DC
	.line	1864
	pushl	24(%edi)
	call	_CreateCompatibleDC@4
	movl	%eax,%esi
; 1865 
; 1866 	switch(lpDs->CtlID)
	.line	1866
	movl	4(%edi),%eax
	cmpl	$330,%eax
	je	_$790
	cmpl	$331,%eax
	je	_$793
	cmpl	$332,%eax
	je	_$796
	jmp	_$788
_$790:
; 1867 		{
; 1868 
; 1869 		case 330: //main_menu
; 1870 			temp_bmp[0] = g_hBmpUp;
	.line	1870
	movl	_g_hBmpUp,%eax
	movl	%eax,-12(%ebp)
; 1871 			temp_bmp[1] = g_hBmpDown;
	.line	1871
	movl	_g_hBmpDown,%eax
	movl	%eax,-8(%ebp)
; 1872 			temp_bmp[2] = g_hBmpHot;
	.line	1872
	movl	_g_hBmpHot,%eax
	movl	%eax,-4(%ebp)
; 1873 			pic_w		= 75;
	.line	1873
	movl	$75,-16(%ebp)
; 1874 			pic_h		= 30;
	.line	1874
	movl	$30,-20(%ebp)
; 1875 			break;
	.line	1875
	jmp	_$788
_$793:
; 1876 		case 331: //opti_button:
; 1877 			temp_bmp[0] = plus_up;
	.line	1877
	movl	_plus_up,%eax
	movl	%eax,-12(%ebp)
; 1878 			temp_bmp[1] = plus_dw;
	.line	1878
	movl	_plus_dw,%eax
	movl	%eax,-8(%ebp)
; 1879 			temp_bmp[2] = plus_ht;
	.line	1879
	movl	_plus_ht,%eax
	movl	%eax,-4(%ebp)
; 1880 			pic_w		= 9;
	.line	1880
	movl	$9,-16(%ebp)
; 1881 			pic_h		= 9;
	.line	1881
	movl	$9,-20(%ebp)
; 1882 			break;
	.line	1882
	jmp	_$788
_$796:
; 1883 		case 332: //plus_button:
; 1884 			temp_bmp[0] = opti_up;
	.line	1884
	movl	_opti_up,%eax
	movl	%eax,-12(%ebp)
; 1885 			temp_bmp[1] = opti_dw;
	.line	1885
	movl	_opti_dw,%eax
	movl	%eax,-8(%ebp)
; 1886 			temp_bmp[2] = opti_ht;
	.line	1886
	movl	_opti_ht,%eax
	movl	%eax,-4(%ebp)
; 1887 			pic_w		= 9;
	.line	1887
	movl	$9,-16(%ebp)
; 1888 			pic_h		= 9;
	.line	1888
	movl	$9,-20(%ebp)
; 1889 			break;
	.line	1889
_$788:
; 1890 
; 1891 		}
; 1892 	if(hDC){
	.line	1892
	or	%esi,%esi
	je	_$799
; 1893 		if(lpDs->itemState & ODS_SELECTED) //  if button is down
	.line	1893
	testb	$1,16(%edi)
	je	_$801
; 1894 		{
; 1895 			SelectObject(hDC, temp_bmp[1]);
	.line	1895
	pushl	-8(%ebp)
	pushl	%esi
	call	_SelectObject@8
	jmp	_$802
_$801:
; 1896 		}
; 1897 		else
; 1898 		{
	.line	1898
; 1899 			SelectObject(hDC, temp_bmp[0]);
	.line	1899
	pushl	-12(%ebp)
	pushl	%esi
	call	_SelectObject@8
_$802:
; 1900 		}
; 1901 		BitBlt(lpDs->hDC, 0, 0, pic_w, pic_h, hDC, 0, 0, SRCCOPY);
	.line	1901
	pushl	$0xcc0020
	pushl	$0
	pushl	$0
	pushl	%esi
	pushl	-20(%ebp)
	pushl	-16(%ebp)
	pushl	$0
	pushl	$0
	pushl	24(%edi)
	call	_BitBlt@36
; 1902 		DeleteDC(hDC);
	.line	1902
	pushl	%esi
	call	_DeleteDC@4
_$799:
; 1903 		}
; 1904 
; 1905 return 0;
	.line	1905
	xor	%eax,%eax
; 1906 }
	.line	1906
	popl	%edi
	popl	%esi
	popl	%ebx
	leave
	ret
_$805:
	.size	_draw_buttons,_$805-_draw_buttons
	.globl	_draw_buttons
; 1907 static HFONT CreateAngledFont(int fontHeight, int fontAngle, LPCTSTR lpfontFace)
	.type	_CreateAngledFont,function
_CreateAngledFont:
	pushl	%ebp
	movl	%esp,%ebp
	subl	$60,%esp
	pushl	%edi
; 1908 {
	.line	1908
	movl	12(%ebp),%edi
; 1909 	// prep logical font
; 1910 
; 1911 	LOGFONT lf;
; 1912 	ZeroMemory(&lf, sizeof(lf));
	.line	1912
	pushl	$60
	leal	-60(%ebp),%eax
	pushl	%eax
	call	_RtlZeroMemory@8
; 1913 	lf.lfHeight = fontHeight;
	.line	1913
	movl	8(%ebp),%eax
	movl	%eax,-60(%ebp)
; 1914 	lf.lfEscapement = ( 10 * fontAngle );   // 90' ccw
	.line	1914
	imul	$10,%edi,%eax
	movl	%eax,-52(%ebp)
; 1915 	lf.lfOrientation = ( 10 * fontAngle );  // 90' ccw
	.line	1915
	imul	$10,%edi,%eax
	movl	%eax,-48(%ebp)
; 1916 	lf.lfWeight = FW_NORMAL;	// 600
	.line	1916
	movl	$400,-44(%ebp)
; 1917 	lf.lfCharSet = ANSI_CHARSET;
	.line	1917
	movb	$0,-37(%ebp)
; 1918 	lf.lfQuality = ANTIALIASED_QUALITY;
	.line	1918
	movb	$4,-34(%ebp)
; 1919 	lstrcpy(lf.lfFaceName, lpfontFace);
	.line	1919
	pushl	16(%ebp)
	leal	-32(%ebp),%eax
	pushl	%eax
	call	_lstrcpyA@8
; 1920 
; 1921 	// instance logical font
; 1922 	return CreateFontIndirect(&lf);
	.line	1922
	leal	-60(%ebp),%eax
	pushl	%eax
	call	_CreateFontIndirectA@4
; 1923 }
	.line	1923
	popl	%edi
	leave
	ret
_$814:
	.size	_CreateAngledFont,_$814-_CreateAngledFont
; 1924 static HMENU FindMenuFromID(HMENU hMenu, UINT id)
	.type	_FindMenuFromID,function
_FindMenuFromID:
	pushl	%ebp
	movl	%esp,%ebp
	subl	$52,%esp
	pushl	%ebx
	pushl	%esi
	pushl	%edi
; 1925 {
	.line	1925
	movl	8(%ebp),%ebx
; 1926 	HMENU hSubMenu = NULL;
	.line	1926
	xor	%esi,%esi
; 1927 	MENUITEMINFO mii;
; 1928 	int isDone = 0;
	.line	1928
	movl	%esi,-52(%ebp)
; 1929 
; 1930 	int iCount = GetMenuItemCount(hMenu);
	.line	1930
	pushl	%ebx
	call	_GetMenuItemCount@4
	movl	%eax,-4(%ebp)
; 1931 	for ( int i = 0; i < iCount; i++) {
	.line	1931
	xor	%edi,%edi
	jmp	_$819
_$816:
; 1932 		if ( id == GetMenuItemID(hMenu, i) ) {
	.line	1932
	pushl	%edi
	pushl	%ebx
	call	_GetMenuItemID@8
	cmpl	%eax,12(%ebp)
	jne	_$820
; 1933 			//wID found we'r on a normal menuitem
; 1934 			GetMenuItemInfo(hMenu, i, TRUE, &mii);
	.line	1934
	leal	-48(%ebp),%eax
	pushl	%eax
	pushl	$1
	pushl	%edi
	pushl	%ebx
	call	_GetMenuItemInfoA@16
; 1935 			return hMenu;
	.line	1935
	movl	%ebx,%eax
	jmp	_$815
_$820:
; 1936 		}
; 1937 		else {
	.line	1937
; 1938 			hSubMenu = GetSubMenu(hMenu, i);
	.line	1938
	pushl	%edi
	pushl	%ebx
	call	_GetSubMenu@8
	movl	%eax,%esi
; 1939 			if((UINT)hSubMenu == id)
	.line	1939
	cmpl	12(%ebp),%esi
	jne	_$822
; 1940 			{
; 1941 				GetMenuItemInfo(hSubMenu, 0, TRUE, &mii);
	.line	1941
	leal	-48(%ebp),%eax
	pushl	%eax
	pushl	$1
	pushl	$0
	pushl	%esi
	call	_GetMenuItemInfoA@16
; 1942 				isDone = id;
	.line	1942
	movl	12(%ebp),%eax
	movl	%eax,-52(%ebp)
_$822:
; 1943 			}
; 1944 			if ( NULL != hSubMenu ) {
	.line	1944
	or	%esi,%esi
	je	_$824
; 1945 				// recurse
; 1946 				hSubMenu = FindMenuFromID(hSubMenu, id);
	.line	1946
	pushl	12(%ebp)
	pushl	%esi
	call	_FindMenuFromID
	addl	$8,%esp
	movl	%eax,%esi
; 1947 				if ( NULL != hSubMenu ) {
	.line	1947
	or	%esi,%esi
	je	_$824
; 1948 					return hSubMenu;
	.line	1948
	movl	%esi,%eax
	jmp	_$815
; 1949 				}
; 1950 			}
	.line	1950
_$824:
; 1951 		}
	.line	1951
; 1952 	}
	.line	1952
	.line	1931
	incl	%edi
_$819:
	cmpl	-4(%ebp),%edi
	jl	_$816
; 1953 
; 1954 	// no match
; 1955 	return NULL;
	.line	1955
	xor	%eax,%eax
_$815:
; 1956 }
	.line	1956
	popl	%edi
	popl	%esi
	popl	%ebx
	leave
	ret
_$832:
	.size	_FindMenuFromID,_$832-_FindMenuFromID
; 1957 int menuType_fromID(HMENU hMenu, UINT id)
	.type	_menuType_fromID,function
_menuType_fromID:
	pushl	%ebp
	movl	%esp,%ebp
	pushl	%ecx
	pushl	%eax
	pushl	%ebx
	pushl	%esi
	pushl	%edi
; 1958 {
	.line	1958
	movl	12(%ebp),%ebx
; 1959 	HMENU hSubMenu = NULL;
	.line	1959
	xor	%esi,%esi
; 1960 	int isDone = 0;
	.line	1960
	movl	%esi,-8(%ebp)
; 1961 	/*
; 1962 	fprintf(outfile,"-----------------------------------------------------------------------\n");
; 1963 	fprintf(outfile,"PIDE MENUID:\t%d HWND:\t%d\n",id,hMenu);
; 1964 	fprintf(outfile,"-----------------------------------------------------------------------\n");
; 1965 	*/
; 1966 	int iCount = GetMenuItemCount(hMenu);
	.line	1966
	pushl	8(%ebp)
	call	_GetMenuItemCount@4
	movl	%eax,-4(%ebp)
; 1967 	for ( int i = 0; i < iCount; i++) {
	.line	1967
	xor	%edi,%edi
	jmp	_$837
_$834:
; 1968 		if ( id == GetMenuItemID(hMenu, i) ) {
	.line	1968
	pushl	%edi
	pushl	8(%ebp)
	call	_GetMenuItemID@8
	cmpl	%eax,%ebx
	jne	_$838
; 1969 			//wID found we'r on a normal menuitem
; 1970 			//Normal menu item
; 1971 			return MF_STRING;
	.line	1971
	xor	%eax,%eax
	jmp	_$833
_$838:
; 1972 		}
; 1973 		else {
	.line	1973
; 1974 			hSubMenu = GetSubMenu(hMenu, i);
	.line	1974
	pushl	%edi
	pushl	8(%ebp)
	call	_GetSubMenu@8
	movl	%eax,%esi
; 1975 			if((UINT)hSubMenu == id)
	.line	1975
	cmpl	%ebx,%esi
	jne	_$840
; 1976 			{
; 1977 				return MF_POPUP;
	.line	1977
	movl	$16,%eax
	jmp	_$833
_$840:
; 1978 			}
; 1979 			if ( NULL != hSubMenu ) {
	.line	1979
	or	%esi,%esi
	je	_$842
; 1980 				hSubMenu = FindMenuFromID(hSubMenu, id);
	.line	1980
	pushl	%ebx
	pushl	%esi
	call	_FindMenuFromID
	addl	$8,%esp
	movl	%eax,%esi
; 1981 				if ( NULL != hSubMenu ) {
	.line	1981
	or	%esi,%esi
	je	_$842
; 1982 					return MF_STRING;
	.line	1982
	xor	%eax,%eax
	jmp	_$833
; 1983 				}
; 1984 			}
	.line	1984
_$842:
; 1985 		}
	.line	1985
; 1986 	}
	.line	1986
	.line	1967
	incl	%edi
_$837:
	cmpl	-4(%ebp),%edi
	jl	_$834
; 1987 
; 1988 	return -1;
	.line	1988
	orl	$-1,%eax
_$833:
; 1989 }
	.line	1989
	popl	%edi
	popl	%esi
	popl	%ebx
	leave
	ret
_$850:
	.size	_menuType_fromID,_$850-_menuType_fromID
	.globl	_menuType_fromID
; 1990 UINT ISmenu_root(UINT id)
	.type	_ISmenu_root,function
_ISmenu_root:
	pushl	%ebp
	movl	%esp,%ebp
	pushl	%ecx
	pushl	%ebx
	pushl	%esi
	pushl	%edi
; 1991 {
	.line	1991
	movl	8(%ebp),%esi
; 1992 	HMENU hSubMenu = NULL;
	.line	1992
	xor	%ebx,%ebx
; 1993 	int iCount = GetMenuItemCount(main_menu);
	.line	1993
	pushl	_main_menu
	call	_GetMenuItemCount@4
	movl	%eax,-4(%ebp)
; 1994 	for ( int i = 0; i < iCount; i++)
	.line	1994
	xor	%edi,%edi
	jmp	_$855
_$852:
; 1995 		{
; 1996 		if ( id == GetMenuItemID(main_menu, i) )
	.line	1996
	pushl	%edi
	pushl	_main_menu
	call	_GetMenuItemID@8
	cmpl	%eax,%esi
	jne	_$856
; 1997 		{
; 1998 			return 1;
	.line	1998
	xor	%eax,%eax
	inc	%eax
	jmp	_$851
_$856:
; 1999 		}
; 2000 		else
; 2001 		{
	.line	2001
; 2002 		hSubMenu = GetSubMenu(main_menu, i);
	.line	2002
	pushl	%edi
	pushl	_main_menu
	call	_GetSubMenu@8
	movl	%eax,%ebx
; 2003 			if((UINT)hSubMenu == id)
	.line	2003
	cmpl	%esi,%ebx
	jne	_$858
; 2004 			{
; 2005 				return 1;
	.line	2005
	xor	%eax,%eax
	inc	%eax
	jmp	_$851
_$858:
; 2006 			}
; 2007 		}
	.line	2007
; 2008 	}
	.line	2008
	.line	1994
	incl	%edi
_$855:
	cmpl	-4(%ebp),%edi
	jl	_$852
; 2009 
; 2010 return 0;
	.line	2010
	xor	%eax,%eax
_$851:
; 2011 }
	.line	2011
	popl	%edi
	popl	%esi
	popl	%ebx
	leave
	ret
_$863:
	.size	_ISmenu_root,_$863-_ISmenu_root
	.globl	_ISmenu_root
; 2012 void DrawTransparentBitmap(HDC hdc, HBITMAP hBitmap, short xStart,
	.type	_DrawTransparentBitmap,function
_DrawTransparentBitmap:
	pushl	%ebp
	movl	%esp,%ebp
	subl	$80,%esp
	pushl	%ebx
	pushl	%esi
	pushl	%edi
; 2013                            short yStart, COLORREF cTransparentColor)
; 2014    {
	.line	2014
	movl	8(%ebp),%esi
	movl	16(%ebp),%eax
	movw	%eax,16(%ebp)
	movl	20(%ebp),%eax
	movw	%eax,20(%ebp)
; 2015    BITMAP     bm;
; 2016    COLORREF   cColor;
; 2017    HBITMAP    bmAndBack, bmAndObject, bmAndMem, bmSave;
; 2018    HBITMAP    bmBackOld, bmObjectOld, bmMemOld, bmSaveOld;
; 2019    HDC        hdcMem, hdcBack, hdcObject, hdcTemp, hdcSave;
; 2020    POINT      ptSize;
; 2021 
; 2022    hdcTemp = CreateCompatibleDC(hdc);
	.line	2022
	pushl	%esi
	call	_CreateCompatibleDC@4
	movl	%eax,%edi
; 2023    SelectObject(hdcTemp, hBitmap);   // Select the bitmap
	.line	2023
	pushl	12(%ebp)
	pushl	%edi
	call	_SelectObject@8
; 2024 
; 2025    GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&bm);
	.line	2025
	leal	-44(%ebp),%eax
	pushl	%eax
	pushl	$24
	pushl	12(%ebp)
	call	_GetObjectA@12
; 2026    ptSize.x = bm.bmWidth;            // Get width of bitmap
	.line	2026
	movl	-40(%ebp),%eax
	movl	%eax,-8(%ebp)
; 2027    ptSize.y = bm.bmHeight;           // Get height of bitmap
	.line	2027
	movl	-36(%ebp),%eax
	movl	%eax,-4(%ebp)
; 2028    DPtoLP(hdcTemp, &ptSize, 1);      // Convert from device
	.line	2028
	pushl	$1
	leal	-8(%ebp),%eax
	pushl	%eax
	pushl	%edi
	call	_DPtoLP@12
; 2029 
; 2030                                      // to logical points
; 2031 
; 2032    // Create some DCs to hold temporary data.
; 2033    hdcBack   = CreateCompatibleDC(hdc);
	.line	2033
	pushl	%esi
	call	_CreateCompatibleDC@4
	movl	%eax,-16(%ebp)
; 2034    hdcObject = CreateCompatibleDC(hdc);
	.line	2034
	pushl	%esi
	call	_CreateCompatibleDC@4
	movl	%eax,-12(%ebp)
; 2035    hdcMem    = CreateCompatibleDC(hdc);
	.line	2035
	pushl	%esi
	call	_CreateCompatibleDC@4
	movl	%eax,%ebx
; 2036    hdcSave   = CreateCompatibleDC(hdc);
	.line	2036
	pushl	%esi
	call	_CreateCompatibleDC@4
	movl	%eax,-20(%ebp)
; 2037 
; 2038    // Create a bitmap for each DC. DCs are required for a number of
; 2039    // GDI functions.
; 2040 
; 2041    // Monochrome DC
; 2042    bmAndBack   = CreateBitmap(ptSize.x, ptSize.y, 1, 1, NULL);
	.line	2042
	pushl	$0
	pushl	$1
	pushl	$1
	pushl	-4(%ebp)
	pushl	-8(%ebp)
	call	_CreateBitmap@20
	movl	%eax,-52(%ebp)
; 2043 
; 2044    // Monochrome DC
; 2045    bmAndObject = CreateBitmap(ptSize.x, ptSize.y, 1, 1, NULL);
	.line	2045
	pushl	$0
	pushl	$1
	pushl	$1
	pushl	-4(%ebp)
	pushl	-8(%ebp)
	call	_CreateBitmap@20
	movl	%eax,-56(%ebp)
; 2046 
; 2047    bmAndMem    = CreateCompatibleBitmap(hdc, ptSize.x, ptSize.y);
	.line	2047
	pushl	-4(%ebp)
	pushl	-8(%ebp)
	pushl	%esi
	call	_CreateCompatibleBitmap@12
	movl	%eax,-60(%ebp)
; 2048    bmSave      = CreateCompatibleBitmap(hdc, ptSize.x, ptSize.y);
	.line	2048
	pushl	-4(%ebp)
	pushl	-8(%ebp)
	pushl	%esi
	call	_CreateCompatibleBitmap@12
	movl	%eax,-64(%ebp)
; 2049 
; 2050    // Each DC must select a bitmap object to store pixel data.
; 2051    bmBackOld   = SelectObject(hdcBack, bmAndBack);
	.line	2051
	pushl	-52(%ebp)
	pushl	-16(%ebp)
	call	_SelectObject@8
	movl	%eax,-68(%ebp)
; 2052    bmObjectOld = SelectObject(hdcObject, bmAndObject);
	.line	2052
	pushl	-56(%ebp)
	pushl	-12(%ebp)
	call	_SelectObject@8
	movl	%eax,-72(%ebp)
; 2053    bmMemOld    = SelectObject(hdcMem, bmAndMem);
	.line	2053
	pushl	-60(%ebp)
	pushl	%ebx
	call	_SelectObject@8
	movl	%eax,-76(%ebp)
; 2054    bmSaveOld   = SelectObject(hdcSave, bmSave);
	.line	2054
	pushl	-64(%ebp)
	pushl	-20(%ebp)
	call	_SelectObject@8
	movl	%eax,-80(%ebp)
; 2055 
; 2056    // Set proper mapping mode.
; 2057    SetMapMode(hdcTemp, GetMapMode(hdc));
	.line	2057
	pushl	%esi
	call	_GetMapMode@4
	pushl	%eax
	pushl	%edi
	call	_SetMapMode@8
; 2058 
; 2059    // Save the bitmap sent here, because it will be overwritten.
; 2060    BitBlt(hdcSave, 0, 0, ptSize.x, ptSize.y, hdcTemp, 0, 0, SRCCOPY);
	.line	2060
	pushl	$0xcc0020
	pushl	$0
	pushl	$0
	pushl	%edi
	pushl	-4(%ebp)
	pushl	-8(%ebp)
	pushl	$0
	pushl	$0
	pushl	-20(%ebp)
	call	_BitBlt@36
; 2061 
; 2062    // Set the background color of the source DC to the color.
; 2063    // contained in the parts of the bitmap that should be transparent
; 2064    cColor = SetBkColor(hdcTemp, cTransparentColor);
	.line	2064
	pushl	24(%ebp)
	pushl	%edi
	call	_SetBkColor@8
	movl	%eax,-48(%ebp)
; 2065 
; 2066    // Create the object mask for the bitmap by performing a BitBlt
; 2067    // from the source bitmap to a monochrome bitmap.
; 2068    BitBlt(hdcObject, 0, 0, ptSize.x, ptSize.y, hdcTemp, 0, 0,
	.line	2068
	pushl	$0xcc0020
	pushl	$0
	pushl	$0
	pushl	%edi
	pushl	-4(%ebp)
	pushl	-8(%ebp)
	pushl	$0
	pushl	$0
	pushl	-12(%ebp)
	call	_BitBlt@36
; 2069           SRCCOPY);
; 2070 
; 2071    // Set the background color of the source DC back to the original
; 2072    // color.
; 2073    SetBkColor(hdcTemp, cColor);
	.line	2073
	pushl	-48(%ebp)
	pushl	%edi
	call	_SetBkColor@8
; 2074 
; 2075    // Create the inverse of the object mask.
; 2076    BitBlt(hdcBack, 0, 0, ptSize.x, ptSize.y, hdcObject, 0, 0,
	.line	2076
	pushl	$0x330008
	pushl	$0
	pushl	$0
	pushl	-12(%ebp)
	pushl	-4(%ebp)
	pushl	-8(%ebp)
	pushl	$0
	pushl	$0
	pushl	-16(%ebp)
	call	_BitBlt@36
; 2077           NOTSRCCOPY);
; 2078 
; 2079    // Copy the background of the main DC to the destination.
; 2080    BitBlt(hdcMem, 0, 0, ptSize.x, ptSize.y, hdc, xStart, yStart,
	.line	2080
	pushl	$0xcc0020
	movswl	20(%ebp),%eax
	pushl	%eax
	movswl	16(%ebp),%eax
	pushl	%eax
	pushl	%esi
	pushl	-4(%ebp)
	pushl	-8(%ebp)
	pushl	$0
	pushl	$0
	pushl	%ebx
	call	_BitBlt@36
; 2081           SRCCOPY);
; 2082 
; 2083    // Mask out the places where the bitmap will be placed.
; 2084    BitBlt(hdcMem, 0, 0, ptSize.x, ptSize.y, hdcObject, 0, 0, SRCAND);
	.line	2084
	pushl	$0x8800c6
	pushl	$0
	pushl	$0
	pushl	-12(%ebp)
	pushl	-4(%ebp)
	pushl	-8(%ebp)
	pushl	$0
	pushl	$0
	pushl	%ebx
	call	_BitBlt@36
; 2085 
; 2086    // Mask out the transparent colored pixels on the bitmap.
; 2087    BitBlt(hdcTemp, 0, 0, ptSize.x, ptSize.y, hdcBack, 0, 0, SRCAND);
	.line	2087
	pushl	$0x8800c6
	pushl	$0
	pushl	$0
	pushl	-16(%ebp)
	pushl	-4(%ebp)
	pushl	-8(%ebp)
	pushl	$0
	pushl	$0
	pushl	%edi
	call	_BitBlt@36
; 2088 
; 2089    // XOR the bitmap with the background on the destination DC.
; 2090    BitBlt(hdcMem, 0, 0, ptSize.x, ptSize.y, hdcTemp, 0, 0, SRCPAINT);
	.line	2090
	pushl	$0xee0086
	pushl	$0
	pushl	$0
	pushl	%edi
	pushl	-4(%ebp)
	pushl	-8(%ebp)
	pushl	$0
	pushl	$0
	pushl	%ebx
	call	_BitBlt@36
; 2091 
; 2092    // Copy the destination to the screen.
; 2093    BitBlt(hdc, xStart, yStart, ptSize.x, ptSize.y, hdcMem, 0, 0,
	.line	2093
	pushl	$0xcc0020
	pushl	$0
	pushl	$0
	pushl	%ebx
	pushl	-4(%ebp)
	pushl	-8(%ebp)
	movswl	20(%ebp),%eax
	pushl	%eax
	movswl	16(%ebp),%eax
	pushl	%eax
	pushl	%esi
	call	_BitBlt@36
; 2094           SRCCOPY);
; 2095 
; 2096    // Place the original bitmap back into the bitmap sent here.
; 2097    BitBlt(hdcTemp, 0, 0, ptSize.x, ptSize.y, hdcSave, 0, 0, SRCCOPY);
	.line	2097
	pushl	$0xcc0020
	pushl	$0
	pushl	$0
	pushl	-20(%ebp)
	pushl	-4(%ebp)
	pushl	-8(%ebp)
	pushl	$0
	pushl	$0
	pushl	%edi
	call	_BitBlt@36
; 2098 
; 2099    // Delete the memory bitmaps.
; 2100    DeleteObject(SelectObject(hdcBack, bmBackOld));
	.line	2100
	pushl	-68(%ebp)
	pushl	-16(%ebp)
	call	_SelectObject@8
	pushl	%eax
	call	_DeleteObject@4
; 2101    DeleteObject(SelectObject(hdcObject, bmObjectOld));
	.line	2101
	pushl	-72(%ebp)
	pushl	-12(%ebp)
	call	_SelectObject@8
	pushl	%eax
	call	_DeleteObject@4
; 2102    DeleteObject(SelectObject(hdcMem, bmMemOld));
	.line	2102
	pushl	-76(%ebp)
	pushl	%ebx
	call	_SelectObject@8
	pushl	%eax
	call	_DeleteObject@4
; 2103    DeleteObject(SelectObject(hdcSave, bmSaveOld));
	.line	2103
	pushl	-80(%ebp)
	pushl	-20(%ebp)
	call	_SelectObject@8
	pushl	%eax
	call	_DeleteObject@4
; 2104 
; 2105    // Delete the memory DCs.
; 2106    DeleteDC(hdcMem);
	.line	2106
	pushl	%ebx
	call	_DeleteDC@4
; 2107    DeleteDC(hdcBack);
	.line	2107
	pushl	-16(%ebp)
	call	_DeleteDC@4
; 2108    DeleteDC(hdcObject);
	.line	2108
	pushl	-12(%ebp)
	call	_DeleteDC@4
; 2109    DeleteDC(hdcSave);
	.line	2109
	pushl	-20(%ebp)
	call	_DeleteDC@4
; 2110    DeleteDC(hdcTemp);
	.line	2110
	pushl	%edi
	call	_DeleteDC@4
; 2111    }
	.line	2111
	popl	%edi
	popl	%esi
	popl	%ebx
	leave
	ret
_$902:
	.size	_DrawTransparentBitmap,_$902-_DrawTransparentBitmap
	.globl	_DrawTransparentBitmap
; 2112 
; 2113 HBITMAP CreateBitmapMask(HBITMAP hbmColour, COLORREF crTransparent)
	.type	_CreateBitmapMask,function
_CreateBitmapMask:
	pushl	%ebp
	movl	%esp,%ebp
	subl	$32,%esp
	pushl	%ebx
	pushl	%esi
	pushl	%edi
; 2114 {
	.line	2114
; 2115 
; 2116     HDC hdcMem, hdcMem2, hdcMem3;
; 2117     HBITMAP hbmMask;
; 2118     HBITMAP hbmBack;
; 2119     BITMAP bm;
; 2120 
; 2121     // Create monochrome (1 bit) mask bitmap.
; 2122 
; 2123     GetObject(hbmColour, sizeof(BITMAP), &bm);
	.line	2123
	leal	-24(%ebp),%eax
	pushl	%eax
	pushl	$24
	pushl	8(%ebp)
	call	_GetObjectA@12
; 2124     hbmMask = CreateBitmap(bm.bmWidth, bm.bmHeight, 1, 1, NULL);
	.line	2124
	pushl	$0
	pushl	$1
	pushl	$1
	pushl	-16(%ebp)
	pushl	-20(%ebp)
	call	_CreateBitmap@20
	movl	%eax,-28(%ebp)
; 2125     hbmBack = CreateBitmap(bm.bmWidth, bm.bmHeight, 1, 24, NULL);
	.line	2125
	pushl	$0
	pushl	$24
	pushl	$1
	pushl	-16(%ebp)
	pushl	-20(%ebp)
	call	_CreateBitmap@20
	movl	%eax,-32(%ebp)
; 2126 
; 2127 
; 2128     // Get some HDCs that are compatible with the display driver
; 2129 
; 2130     hdcMem = CreateCompatibleDC(0);
	.line	2130
	pushl	$0
	call	_CreateCompatibleDC@4
	movl	%eax,%edi
; 2131     hdcMem2 = CreateCompatibleDC(0);
	.line	2131
	pushl	$0
	call	_CreateCompatibleDC@4
	movl	%eax,%esi
; 2132     hdcMem3 = CreateCompatibleDC(0);
	.line	2132
	pushl	$0
	call	_CreateCompatibleDC@4
	movl	%eax,%ebx
; 2133 
; 2134     SelectBitmap(hdcMem, hbmColour);
	.line	2134
	pushl	8(%ebp)
	pushl	%edi
	call	_SelectObject@8
; 2135     SelectBitmap(hdcMem2, hbmMask);
	.line	2135
	pushl	-28(%ebp)
	pushl	%esi
	call	_SelectObject@8
; 2136     SelectBitmap(hdcMem3, hbmBack);
	.line	2136
	pushl	-32(%ebp)
	pushl	%ebx
	call	_SelectObject@8
; 2137 
; 2138 
; 2139     //Copy of original !!!
; 2140     BitBlt(hdcMem3, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);
	.line	2140
	pushl	$0xcc0020
	pushl	$0
	pushl	$0
	pushl	%edi
	pushl	-16(%ebp)
	pushl	-20(%ebp)
	pushl	$0
	pushl	$0
	pushl	%ebx
	call	_BitBlt@36
; 2141     // Set the background colour of the colour image to the colour
; 2142     // you want to be transparent.
; 2143     SetBkColor(hdcMem, crTransparent);
	.line	2143
	pushl	12(%ebp)
	pushl	%edi
	call	_SetBkColor@8
; 2144 
; 2145     // Copy the bits from the colour image to the B+W mask... everything
; 2146     // with the background colour ends up white while everythig else ends up
; 2147     // black...Just what we wanted.
; 2148 
; 2149     BitBlt(hdcMem2, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem3, 0, 0, SRCCOPY);
	.line	2149
	pushl	$0xcc0020
	pushl	$0
	pushl	$0
	pushl	%ebx
	pushl	-16(%ebp)
	pushl	-20(%ebp)
	pushl	$0
	pushl	$0
	pushl	%esi
	call	_BitBlt@36
; 2150 
; 2151     // Take our new mask and use it to turn the transparent colour in our
; 2152     // original colour image to black so the transparency effect will
; 2153     // work right.
; 2154     BitBlt(hdcMem, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem2, 0, 0, SRCINVERT);
	.line	2154
	pushl	$0x660046
	pushl	$0
	pushl	$0
	pushl	%esi
	pushl	-16(%ebp)
	pushl	-20(%ebp)
	pushl	$0
	pushl	$0
	pushl	%edi
	call	_BitBlt@36
; 2155 
; 2156     // Clean up.
; 2157 
; 2158     DeleteDC(hdcMem);
	.line	2158
	pushl	%edi
	call	_DeleteDC@4
; 2159     DeleteDC(hdcMem2);
	.line	2159
	pushl	%esi
	call	_DeleteDC@4
; 2160 
; 2161     return hbmMask;
	.line	2161
	movl	-28(%ebp),%eax
; 2162 
; 2163 }
	.line	2163
	popl	%edi
	popl	%esi
	popl	%ebx
	leave
	ret
_$920:
	.size	_CreateBitmapMask,_$920-_CreateBitmapMask
	.globl	_CreateBitmapMask
; 2164 void drawIconToBitmap(HBITMAP bmSrc, char * FileName, int size )
	.type	_drawIconToBitmap,function
_drawIconToBitmap:
	pushl	%ebp
	movl	%esp,%ebp
	subl	$376,%esp
	pushl	%ebx
	pushl	%esi
	pushl	%edi
; 2165 {
	.line	2165
	movl	16(%ebp),%esi
; 2166   SHFILEINFO sfi;
; 2167   HDC        hMemDC;
; 2168   HBITMAP    hBitmap, hbmOld,bufferh;
; 2169   HDC        hDCDesktop;
; 2170   HBRUSH     hOldBrush;
; 2171   RECT       rc_a;
; 2172 
; 2173   rc_a.top 	= 0;
	.line	2173
	andl	$0,-12(%ebp)
; 2174   rc_a.left	= 0;
	.line	2174
	andl	$0,-16(%ebp)
; 2175   rc_a.bottom	= size;
	.line	2175
	movl	%esi,-4(%ebp)
; 2176   rc_a.right	= size;
	.line	2176
	movl	%esi,-8(%ebp)
; 2177 
; 2178   //MessageBox(main_window,FileName,"Error de tipo:",MB_OK);
; 2179   //LPSTR pFileName = "c:\windows\calc.exe";
; 2180   hDCDesktop = GetDC( GetDesktopWindow() );
	.line	2180
	call	_GetDesktopWindow@0
	pushl	%eax
	call	_GetDC@4
	movl	%eax,%edi
; 2181 
; 2182   // Create bitmap -----------------------------------------------------
; 2183   hBitmap = bmSrc; //CreateCompatibleBitmap( hDCDesktop, 96, iNumFiles * 32 );
	.line	2183
	movl	8(%ebp),%ebx
; 2184 
; 2185   ReleaseDC( GetDesktopWindow(), hDCDesktop );
	.line	2185
	call	_GetDesktopWindow@0
	pushl	%edi
	pushl	%eax
	call	_ReleaseDC@8
; 2186   hMemDC = CreateCompatibleDC( NULL );               // Create screen DC
	.line	2186
	pushl	$0
	call	_CreateCompatibleDC@4
	movl	%eax,%edi
; 2187   bufferh = CreateCompatibleBitmap(hMemDC, size,size);
	.line	2187
	pushl	%esi
	pushl	%esi
	pushl	%edi
	call	_CreateCompatibleBitmap@12
	movl	%eax,-376(%ebp)
; 2188 
; 2189   hbmOld = SelectObject( hMemDC, hBitmap );      // Select memory bitmap
	.line	2189
	pushl	%ebx
	pushl	%edi
	call	_SelectObject@8
	movl	%eax,-372(%ebp)
; 2190 
; 2191 
; 2192 
; 2193   // Select brush for deleting the memory bitmap -----------------------
; 2194   hOldBrush = CreateSolidBrush(RGB(170, 170, 170)); //SelectObject( hMemDC, GetStockObject( GRAY_BRUSH ) );
	.line	2194
	pushl	$0xaaaaaa
	call	_CreateSolidBrush@4
	movl	%eax,%ebx
; 2195   // Delete memory bitmap ----------------------------------------------
; 2196   FillRect(hMemDC,&rc_a,hOldBrush);
	.line	2196
	pushl	%ebx
	leal	-16(%ebp),%eax
	pushl	%eax
	pushl	%edi
	call	_FillRect@12
; 2197   //Rectangle( hMemDC, 0, 0, 32,32 );
; 2198   SelectObject( hMemDC, hOldBrush );
	.line	2198
	pushl	%ebx
	pushl	%edi
	call	_SelectObject@8
; 2199 if(size == 16)
	.line	2199
	cmpl	$16,%esi
	jne	_$925
; 2200 {
; 2201    //Small icon
; 2202    SHGetFileInfo( FileName,                          // Get small icon
	.line	2202
	pushl	$257
	pushl	$352
	leal	-368(%ebp),%eax
	pushl	%eax
	pushl	$0
	pushl	12(%ebp)
	call	_SHGetFileInfoA@20
; 2203                    0,
; 2204                    &sfi,
; 2205                    sizeof( sfi ),
; 2206                    SHGFI_ICON | SHGFI_SMALLICON );
; 2207     DrawIconEx( hMemDC,	0, 0,sfi.hIcon, size, size, 0,NULL, DI_NORMAL );
	.line	2207
	pushl	$3
	pushl	$0
	pushl	$0
	pushl	%esi
	pushl	%esi
	pushl	-368(%ebp)
	pushl	$0
	pushl	$0
	pushl	%edi
	call	_DrawIconEx@36
; 2208     DestroyIcon( sfi.hIcon );                            // Destroy icon
	.line	2208
	pushl	-368(%ebp)
	call	_DestroyIcon@4
_$925:
; 2209  }
; 2210 if(size == 32)
	.line	2210
	cmpl	$32,%esi
	jne	_$927
; 2211  {
; 2212     //Large icon
; 2213     SHGetFileInfo( FileName,0,&sfi,sizeof( sfi ),SHGFI_ICON | SHGFI_LARGEICON ); // SHGFI_LARGEICON = 0
	.line	2213
	pushl	$256
	pushl	$352
	leal	-368(%ebp),%eax
	pushl	%eax
	pushl	$0
	pushl	12(%ebp)
	call	_SHGetFileInfoA@20
; 2214     DrawIconEx( hMemDC,	0, 0,sfi.hIcon, size, size, 0, NULL, DI_NORMAL );
	.line	2214
	pushl	$3
	pushl	$0
	pushl	$0
	pushl	%esi
	pushl	%esi
	pushl	-368(%ebp)
	pushl	$0
	pushl	$0
	pushl	%edi
	call	_DrawIconEx@36
; 2215     DestroyIcon( sfi.hIcon );                            // Destroy icon
	.line	2215
	pushl	-368(%ebp)
	call	_DestroyIcon@4
_$927:
; 2216 
; 2217   }
; 2218 
; 2219   SelectObject( hMemDC, hbmOld );        // Old bitmap back to memory DC
	.line	2219
	pushl	-372(%ebp)
	pushl	%edi
	call	_SelectObject@8
; 2220   DeleteDC    ( hMemDC );                // Destroy memory DC
	.line	2220
	pushl	%edi
	call	_DeleteDC@4
; 2221   					 //Preserve bitmap
; 2222 }
	.line	2222
	popl	%edi
	popl	%esi
	popl	%ebx
	leave
	ret
_$936:
	.size	_drawIconToBitmap,_$936-_drawIconToBitmap
	.globl	_drawIconToBitmap
	.bss
	.globl	_damm_handle
	.align	2
	.type	_damm_handle,object
	.comm	_damm_handle,4
	.globl	_mnu_hwnd
	.align	2
	.type	_mnu_hwnd,object
	.comm	_mnu_hwnd,4
	.globl	_old_menu_proc
	.align	2
	.type	_old_menu_proc,object
	.comm	_old_menu_proc,4
	.globl	_hoo_handle
	.align	2
	.type	_hoo_handle,object
	.comm	_hoo_handle,4
	.globl	_g_BmpHeight
	.align	2
	.type	_g_BmpHeight,object
	.comm	_g_BmpHeight,4
	.globl	_g_BmpWidth
	.align	2
	.type	_g_BmpWidth,object
	.comm	_g_BmpWidth,4
	.globl	_MainBitmap
	.align	2
	.type	_MainBitmap,object
	.comm	_MainBitmap,4
	.globl	_lpBits
	.align	2
	.type	_lpBits,object
	.comm	_lpBits,4
	.globl	_lpBitmap
	.align	2
	.type	_lpBitmap,object
	.comm	_lpBitmap,4
	.globl	_opti_ht
	.align	2
	.type	_opti_ht,object
	.comm	_opti_ht,4
	.globl	_opti_dw
	.align	2
	.type	_opti_dw,object
	.comm	_opti_dw,4
	.globl	_opti_up
	.align	2
	.type	_opti_up,object
	.comm	_opti_up,4
	.globl	_outfile
	.align	2
	.type	_outfile,object
	.comm	_outfile,4
	.globl	_plus_ht
	.align	2
	.type	_plus_ht,object
	.comm	_plus_ht,4
	.globl	_plus_dw
	.align	2
	.type	_plus_dw,object
	.comm	_plus_dw,4
	.globl	_plus_up
	.align	2
	.type	_plus_up,object
	.comm	_plus_up,4
	.globl	_g_hBmpHot
	.align	2
	.type	_g_hBmpHot,object
	.comm	_g_hBmpHot,4
	.globl	_g_hBmpDown
	.align	2
	.type	_g_hBmpDown,object
	.comm	_g_hBmpDown,4
	.globl	_g_hBmpUp
	.align	2
	.type	_g_hBmpUp,object
	.comm	_g_hBmpUp,4
	.globl	_buttons_proc
	.align	2
	.type	_buttons_proc,object
	.comm	_buttons_proc,40
	.globl	_main_menu
	.align	2
	.type	_main_menu,object
	.comm	_main_menu,4
	.globl	_plus_button
	.align	2
	.type	_plus_button,object
	.comm	_plus_button,4
	.globl	_opti_button
	.align	2
	.type	_opti_button,object
	.comm	_opti_button,4
	.globl	_menu_msg
	.align	2
	.type	_menu_msg,object
	.comm	_menu_msg,4
	.globl	_menu_objects
	.align	2
	.type	_menu_objects,object
	.comm	_menu_objects,4
	.globl	_menu_button
	.align	2
	.type	_menu_button,object
	.comm	_menu_button,4
	.globl	_main_window
	.align	2
	.type	_main_window,object
	.comm	_main_window,4
	.globl	_hwndMain
	.align	2
	.type	_hwndMain,object
	.comm	_hwndMain,4
	.globl	_hInst
	.align	2
	.type	_hInst,object
	.comm	_hInst,4
	.globl	_Menu_item
	.align	2
	.type	_Menu_item,object
	.comm	_Menu_item,40
	.globl	_first_menu_data
	.align	2
	.type	_first_menu_data,object
	.comm	_first_menu_data,4
	.globl	_primary_menu_data
	.align	2
	.type	_primary_menu_data,object
	.comm	_primary_menu_data,4
	.extern	_dhToggleExceptions
	.extern	_dhUninitialize
	.extern	_dhInitializeImp
	.extern	_dhGetValue
	.extern	_dhPutValue
	.extern	_dhCallMethod
	.extern	_dhCreateObject
	.extern	_SysFreeString@4
	.extern	_SHGetFileInfoA@20
	.extern	_ShellExecuteA@24
	.extern	_DPtoLP@12
	.extern	_MoveToEx@16
	.extern	_SetTextColor@8
	.extern	_SetStretchBltMode@8
	.extern	_SetMapMode@8
	.extern	_SetBkMode@8
	.extern	_SetBkColor@8
	.extern	_SelectObject@8
	.extern	_LineTo@12
	.extern	_GetStockObject@4
	.extern	_GetMapMode@4
	.extern	_GetDeviceCaps@8
	.extern	_GetClipBox@8
	.extern	_DeleteObject@4
	.extern	_DeleteDC@4
	.extern	_CreateSolidBrush@4
	.extern	_CreatePen@12
	.extern	_CreateCompatibleDC@4
	.extern	_CreateCompatibleBitmap@12
	.extern	_CreateBitmap@20
	.extern	_BitBlt@36
	.extern	_DrawIconEx@36
	.extern	_TrackPopupMenuEx@24
	.extern	_DrawEdge@16
	.extern	_DestroyIcon@4
	.extern	_CallNextHookEx@16
	.extern	_UnhookWindowsHookEx@4
	.extern	_GetDesktopWindow@0
	.extern	_InflateRect@12
	.extern	_FrameRect@12
	.extern	_FillRect@12
	.extern	_ClientToScreen@8
	.extern	_GetWindowRect@8
	.extern	_GetClientRect@8
	.extern	_EndPaint@8
	.extern	_BeginPaint@8
	.extern	_ReleaseDC@8
	.extern	_GetWindowDC@4
	.extern	_GetDC@4
	.extern	_GetForegroundWindow@0
	.extern	_TrackPopupMenu@28
	.extern	_GetMenuItemCount@4
	.extern	_GetMenuItemID@8
	.extern	_GetSubMenu@8
	.extern	_CreatePopupMenu@0
	.extern	_GetSystemMetrics@4
	.extern	_ReleaseCapture@0
	.extern	_GetDlgItem@8
	.extern	_ShowWindow@8
	.extern	_PostQuitMessage@4
	.extern	_TranslateMessage@4
	.extern	_MulDiv@12
	.extern	_LoadResource@8
	.extern	_GetCurrentThreadId@0
	.extern	_LockResource@4
	.extern	_TrackMouseEvent@4
	.extern	_TextOutA@20
	.extern	_GetObjectA@12
	.extern	_GetTextMetricsA@8
	.extern	_GetTextExtentPoint32A@16
	.extern	_CreateFontIndirectA@4
	.extern	_LoadImageA@24
	.extern	_LoadCursorA@8
	.extern	_SetWindowsHookExA@16
	.extern	_GetClassNameA@12
	.extern	_SetWindowLongA@12
	.extern	_GetWindowLongA@8
	.extern	_MessageBoxA@16
	.extern	_RemovePropA@8
	.extern	_GetPropA@8
	.extern	_SetPropA@12
	.extern	_DrawTextA@20
	.extern	_GetMenuItemInfoA@16
	.extern	_AppendMenuA@16
	.extern	_TranslateAcceleratorA@12
	.extern	_LoadAcceleratorsA@8
	.extern	_CreateWindowExA@48
	.extern	_RegisterClassA@4
	.extern	_CallWindowProcA@20
	.extern	_DefWindowProcA@16
	.extern	_PostMessageA@16
	.extern	_SendMessageA@16
	.extern	_DispatchMessageA@4
	.extern	_GetMessageA@16
	.extern	_FindResourceA@12
	.extern	_lstrlenA@4
	.extern	_lstrcpyA@8
	.extern	_RtlZeroMemory@8
	.extern	_strcmp
	.extern	_memset
	.extern	_memcpy
	.extern	_malloc
	.extern	_free
	.extern	_exit
	.data
_$707:
; "MENU HELPER\x0"
	.byte	77,69,78,85,32,72,69,76,80,69,82,0
_$678:
; "Verdana\x0"
	.byte	86,101,114,100,97,110,97,0
_$531:
; "#32768\x0"
	.byte	35,51,50,55,54,56,0
_$491:
; "-\x0"
	.byte	45,0
	.align	2
_$471:
; "M\x0E\x0N\x0U\x0I"
	.word	77,69,78,85,73,84,69,77,0
	.align	2
_$470:
; ".\x0s\x0e\x0l\x0e\x0c\x0t\x0N\x0o"
	.word	46,115,101,108,101,99,116,78,111,100,101,115,40,37,83,41
	.word	0
	.align	2
_$469:
; "E\x0X\x0E\x0C\x0"
	.word	69,88,69,67,85,84,69,0
	.align	2
_$468:
; "C\x0O\x0L\x0"
	.word	67,79,76,79,82,0
	.align	2
_$467:
; "%\x0u"
	.word	37,117,0
	.align	2
_$466:
; "I\x0M\x0A\x0"
	.word	73,77,65,71,69,0
	.align	2
_$465:
; "C\x0A\x0P\x0T\x0"
	.word	67,65,80,84,73,79,78,0
	.align	2
_$464:
; ".\x0s\x0e\x0l\x0e\x0c\x0t\x0S\x0i\x0n\x0g\x0l\x0e\x0N"
	.word	46,115,101,108,101,99,116,83,105,110,103,108,101,78,111,100
	.word	101,40,37,83,41,46,116,101,120,116,0
	.align	2
_$463:
; "%\x0s"
	.word	37,115,0
	.align	2
_$462:
; ".\x0i\x0t\x0e\x0m\x0"
	.word	46,105,116,101,109,40,37,100,41,0
	.align	2
_$457:
; ".\x0l\x0e\x0n\x0"
	.word	46,108,101,110,103,116,104,0
	.align	2
_$456:
; "%\x0d"
	.word	37,100,0
	.align	2
_$414:
; ".\x0d\x0o\x0c\x0u\x0m\x0e\x0n\x0t\x0E\x0l\x0e\x0m\x0e\x0"
	.word	46,100,111,99,117,109,101,110,116,69,108,101,109,101,110,116
	.word	46,99,104,105,108,100,78,111,100,101,115,0
	.align	2
_$413:
; "%\x0o"
	.word	37,111,0
	.align	2
_$409:
; "m\x0e\x0n\x0u\x0."
	.word	109,101,110,117,46,120,109,108,0
	.align	2
_$408:
; ".\x0L\x0o\x0a\x0d\x0"
	.word	46,76,111,97,100,40,37,83,41,0
	.align	2
_$402:
; ".\x0A\x0s\x0y\x0n\x0c\x0"
	.word	46,65,115,121,110,99,32,61,32,37,98,0
	.align	2
_$395:
; "M\x0S\x0X\x0M\x0L\x0.\x0D\x0O\x0M\x0"
	.word	77,83,88,77,76,46,68,79,77,68,111,99,117,109,101,110
	.word	116,0
_$388:
; "menu.xml\x0"
	.byte	109,101,110,117,46,120,109,108,0
_$321:
; "No HDC !!!\x0"
	.byte	78,111,32,72,68,67,32,33,33,33,0
_$317:
; "Error de tipo:\x0"
	.byte	69,114,114,111,114,32,100,101,32,116,105,112,111,58,0
_$316:
; "Dibujando el Componente\x0"
	.byte	68,105,98,117,106,97,110,100,111,32,101,108,32,67,111,109
	.byte	112,111,110,101,110,116,101,0
_$302:
; "button\x0"
	.byte	98,117,116,116,111,110,0
_$238:
; "#12346\x0"
	.byte	35,49,50,51,52,54,0
_$226:
; "\x0"
	.byte	0
_$224:
; "open\x0"
	.byte	111,112,101,110,0
_$207:
; "Título\x0"
	.byte	84,237,116,117,108,111,0
_$185:
; "main_menu_WndClass\x0"
	.byte	109,97,105,110,95,109,101,110,117,95,87,110,100,67,108,97
	.byte	115,115,0
_$129:
; "://\x0"
	.byte	58,47,47,0
