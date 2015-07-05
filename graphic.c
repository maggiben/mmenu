///////////////////////////////////////////////////////////////////////////////////
// Título:      		graphic.c                                                //
// Author:		        Benjamin Maggi                                           //
// Descripcion: 		XML Handling functions                                   //
// Org. Date:           06/04/2008                                               //
// Last Modification:   06/04/2008                                               //
// Ver:                 0.0.2                                                    //
// compiler:            uses ansi-C / C99 tested with LCC & Pellesc              //
// Author mail:         benjaminmaggi@gmail.com                                  //
// License:             GNU                                                      //
//                                                                               //
// License:                                                                      //
// This program is free software; you can redistribute it                        //
// and/or modify it under the terms of the GNU General Public                    //
// License as published by the Free Software Foundation;                         //
// either version 2 of the License, or (at your option) any          	         //
// later version.                                                    	         //
//                                                                    	         //
// This program is distributed in the hope that it will be useful,   	         //
// but WITHOUT ANY WARRANTY; without even the implied warranty of    	         //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the     	         //
// GNU General Public License for more details.                      	         //
//                                                                               //
///////////////////////////////////////////////////////////////////////////////////

#include <windows.h>		// windows standar
#include <windowsx.h>       // Extra features 
#include <winuser.h>		// Windows User Module definitions
#include <tchar.h>          
#include <wchar.h>			// Unicode support
#include "graphic.h"		// Graphic Functions draw masks, bitmaps etc..
#include "FreeImage.h"      // FreeImage protos & delares
#include "./png/code/png.h"


	/// health flag
	BOOL m_good;

	/// the image's width
	int m_width;

	/// the image's height
	int m_height;

	/// the color values (blue, green, red, alpha)
	unsigned char* m_bgra;

	HBITMAP m_hOldBgBitmap;
	HBITMAP m_hBgBitmap;
	HDC m_hBgDC;
	HBITMAP m_hOldShadowBitmap;
	HBITMAP m_hShadowBitmap;
	HDC m_hShadowDC;
	HBITMAP m_hOldGlowBitmap;
	HBITMAP m_hGlowBitmap;
	HDC m_hGlowDC;
	HBITMAP m_hOldMemBitmap;
	HBITMAP m_hMemBitmap;
	HDC m_hMemDC;

void CreateGlow(COLORREF transparentColor, COLORREF glowColor);
void CreateDrawing(HDC hDrawingDC);


BOOL doCheckFileHeader(const char* Path);
BOOL doExtractCanonicData (png_structp PngPtr, png_infop InfoPtr);
unsigned char* doConvertRGBA8(png_structp PngPtr, png_infop InfoPtr);

HICON iconFromPng(void)
{
	HDC hMemDC;
    DWORD dwWidth, dwHeight;
    BITMAPV5HEADER bi;
    HBITMAP hBitmap, hOldBitmap;
    void *lpBits;
    DWORD x,y;
    HCURSOR hAlphaCursor = NULL;

    dwWidth  = GetSystemMetrics(SM_CXICON);  // width of cursor
    dwHeight = GetSystemMetrics(SM_CYICON);  // height of cursor
	
	ZeroMemory(&bi,sizeof(BITMAPV5HEADER));
    bi.bV5Size           = sizeof(BITMAPV5HEADER);
    bi.bV5Width           = dwWidth;
    bi.bV5Height          = dwHeight;
    bi.bV5Planes = 1;
    bi.bV5BitCount = 32;
    bi.bV5Compression = BI_BITFIELDS;
    // The following mask specification specifies a supported 32 BPP
    // alpha format for Windows XP.
    bi.bV5RedMask   =  0x00FF0000;
    bi.bV5GreenMask =  0x0000FF00;
    bi.bV5BlueMask  =  0x000000FF;
    bi.bV5AlphaMask =  0xFF000000; 

    HDC hdc;
    hdc = GetDC(NULL);

    // Create the DIB section with an alpha channel.
    hBitmap = CreateDIBSection(hdc, (BITMAPINFO *)&bi, DIB_RGB_COLORS, (void **)&lpBits, NULL, (DWORD)0);

    hMemDC = CreateCompatibleDC(hdc);
    ReleaseDC(NULL,hdc);

	// Draw something on the DIB section.
	/*
    hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);
    PatBlt(hMemDC,0,0,dwWidth,dwHeight,WHITENESS);
    SetTextColor(hMemDC,RGB(255,0,0));
    SetBkMode(hMemDC,TRANSPARENT);
    TextOut(hMemDC,0,9,"rgba",4);


		RECT rcItem;
		
		rcItem.left  = 0;
		rcItem.right = 10;
		rcItem.top   = 0;
		rcItem.bottom = 10;

		HBRUSH hBrush = CreateSolidBrush(RGB(255,9,9));		// Menu BG Color
		FillRect(hMemDC, &rcItem, hBrush);
		DeleteObject(hBrush);

	SelectObject(hMemDC, hOldBitmap);
    DeleteDC(hMemDC);
	*/

    // Create an empty mask bitmap.
    HBITMAP hMonoBitmap = CreateBitmap(dwWidth,dwHeight,1,1,NULL);

    // Set the alpha values for each pixel in the cursor so that
    // the complete cursor is semi-transparent.
	/*
    DWORD *lpdwPixel;
    lpdwPixel = (DWORD *)lpBits;
    for (x=0;x<dwWidth;x++)
       for (y=0;y<dwHeight;y++)
       {
           // Clear the alpha bits
           *lpdwPixel &= 0x00FFFFFF;
           // Set the alpha bits to 0x9F (semi-transparent)
           *lpdwPixel |= 0x9F000000;
           lpdwPixel++;
       }
   */
	static BOOL pp = FALSE;
	static unsigned char* data;
	if(!pp)
	{
		loadPNG("basic2.png");
	  	lpBits = getBGRA();
	  	pp = TRUE;
	 }
	/*
	for (int y=0; y<32; y++)
	{
		BYTE * pPixel = (BYTE *) data + 32 * 4 * y;
		BYTE * oPixel = (BYTE *) lpBits + 32 * 4 * y;

			for (int x=0; x<32; x++)
			{
                oPixel[0] = pPixel[0] * pPixel[3] / 255; 
                oPixel[1] = 0;//pPixel[1] * pPixel[3] / 255; 
                oPixel[2] = 0;//pPixel[2] * pPixel[3] / 255; 

                pPixel += 4;
				oPixel += 4;
            }
	}
	*/
	HDC hDC = GetDC(NULL);
	HBITMAP bitmap2 = CreateCompatibleBitmap(hDC, 32, 32);
	// RGB + Alpha
	int bsize = 32 * 32 * 4;

	SetBitmapBits(bitmap2, bsize, data);

    ICONINFO ii;
    ii.fIcon = TRUE;  // Change fIcon to TRUE to create an alpha icon
    ii.xHotspot = 0;
    ii.yHotspot = 0;
    ii.hbmMask = hMonoBitmap;
    ii.hbmColor = hBitmap;

    // Create the alpha cursor with the alpha DIB section.
    hAlphaCursor = CreateIconIndirect(&ii);

	ReleaseDC(NULL, hDC);
    DeleteObject(hBitmap);          
    DeleteObject(hMonoBitmap); 

    return hAlphaCursor;

}

HBITMAP create_alpha_bitmap(int width, int height, void **outdata)
{
	BITMAPV5HEADER bi;
	HDC hdc;
	HBITMAP hBitmap;

	ZeroMemory (&bi, sizeof (BITMAPV5HEADER));
	bi.bV5Size = sizeof (BITMAPV5HEADER);
	bi.bV5Width = width;
	bi.bV5Height = height;
	bi.bV5Planes = 1;
	bi.bV5BitCount = 32;
	bi.bV5Compression = BI_BITFIELDS;
	/* The following mask specification specifies a supported 32 BPP
	* alpha format for Windows XP (BGRA format).
	*/
	bi.bV5RedMask   = 0x00FF0000;
	bi.bV5GreenMask = 0x0000FF00;
	bi.bV5BlueMask  = 0x000000FF;
	bi.bV5AlphaMask = 0xFF000000;

	/* Create the DIB section with an alpha channel. */
	hdc = GetDC (NULL);
	if (!hdc)
	{
      return NULL;
	}
	hBitmap = CreateDIBSection(hdc, (BITMAPINFO *)&bi, DIB_RGB_COLORS, (PVOID *) outdata, NULL, (DWORD)0);

	ReleaseDC (NULL, hdc);
	return hBitmap;
}
///////////////////////////////////////////////////////////////////////////////////
// This draws a bitmap in a solid backgound color of the same                    //
// size than the icon and returns an HBITMAP                                     //
///////////////////////////////////////////////////////////////////////////////////
void drawIconToBitmap(HBITMAP bmSrc, char * FileName, int size, COLORREF compositeColor)
{
	SHFILEINFO sfi;
	HDC        hMemDC;
	HBITMAP    hBitmap, hbmOld, bufferh;
	HDC        hDCDesktop;
	HBRUSH     hOldBrush;
	RECT       rc_a;

	rc_a.top		= 0;
	rc_a.left		= 0;
	rc_a.bottom		= size;
	rc_a.right		= size;

	#define BASIC_SHGFI_FLAGS | SHGFI_TYPENAME | SHGFI_SHELLICONSIZE | SHGFI_SYSICONINDEX | SHGFI_DISPLAYNAME | SHGFI_EXETYPE
	hDCDesktop = GetDC( GetDesktopWindow() );

	///////////////////////////////////////////////////////////////////////////////////
	// Create bitmap                                                                 //
	///////////////////////////////////////////////////////////////////////////////////
	hBitmap = bmSrc; //CreateCompatibleBitmap( hDCDesktop, 96, iNumFiles * 32 );

	ReleaseDC( GetDesktopWindow(), hDCDesktop );
	hMemDC	= CreateCompatibleDC( NULL );				// Create screen DC

	bufferh	= CreateCompatibleBitmap(hMemDC, size,size);
	hbmOld	= SelectObject( hMemDC, hBitmap );			// Select memory bitmap


	///////////////////////////////////////////////////////////////////////////////////
	// Select brush for deleting the memory bitmap                                   //
	///////////////////////////////////////////////////////////////////////////////////
	hOldBrush = CreateSolidBrush(compositeColor);
	///////////////////////////////////////////////////////////////////////////////////
	// Delete memory bitmap                                                          //
	///////////////////////////////////////////////////////////////////////////////////
	FillRect(hMemDC,&rc_a,hOldBrush);
	//Rectangle( hMemDC, 0, 0, 32,32 );
	SelectObject( hMemDC, hOldBrush );


	if(size == 16)
	{
		//Small icon
		SHGetFileInfo( 
					FileName,
					0,
					&sfi,
					sizeof( sfi ),
					SHGFI_ICON | SHGFI_SMALLICON 
					);

    	DrawIconEx(hMemDC,0,0,sfi.hIcon,size,size,0,NULL,DI_NORMAL);
		
		ICONINFO icInfo;
		if(GetIconInfo(sfi.hIcon, &icInfo))
		{
			BITMAPINFO bmpInfo        = { 0 };
			HDC hdc = GetDC(NULL);
			if (GetDIBits(hdc, icInfo.hbmColor, 0, 0, NULL, &bmpInfo, DIB_RGB_COLORS) != 0)
			{

			}

		}

		// Destroy icon
		DestroyIcon(sfi.hIcon);
	}
	if(size == 32)
	{
    	//Large icon
    	SHGetFileInfo(
					FileName,
					0,
					&sfi,
					sizeof( sfi ),
					SHGFI_ICON | SHGFI_LARGEICON
					); 
		// SHGFI_LARGEICON = 0
    	DrawIconEx( hMemDC,	0, 0,sfi.hIcon, size, size, 0, NULL, DI_NORMAL );
		// Destroy icon
    	DestroyIcon( sfi.hIcon );                           
	}
	SelectObject( hMemDC, hbmOld );        // Old bitmap back to memory DC
	DeleteDC    ( hMemDC );                // Destroy memory DC
	//Preserve bitmap
}
///////////////////////////////////////////////////////////////////////////////////
// Creates a 1bpp image, crTransparent will be used as a mask color              //
///////////////////////////////////////////////////////////////////////////////////
HBITMAP CreateBitmapMask(HBITMAP hbmColour, COLORREF crTransparent)
{

    HDC hdcMem, hdcMem2, hdcMem3;
    HBITMAP hbmMask;
    HBITMAP hbmBack;
    BITMAP bm;
	
	///////////////////////////////////////////////////////////////////////////////////
    // Create monochrome (1 bit) mask bitmap.                                        //
	///////////////////////////////////////////////////////////////////////////////////
    GetObject(hbmColour, sizeof(BITMAP), &bm);
    hbmMask = CreateBitmap(bm.bmWidth, bm.bmHeight, 1, 1, NULL);
    hbmBack = CreateBitmap(bm.bmWidth, bm.bmHeight, 1, 24, NULL);

	///////////////////////////////////////////////////////////////////////////////////
    // Get some HDCs that are compatible with the display driver                     //
	///////////////////////////////////////////////////////////////////////////////////
    hdcMem	= CreateCompatibleDC(0);
    hdcMem2	= CreateCompatibleDC(0);
    hdcMem3	= CreateCompatibleDC(0);

    SelectBitmap(hdcMem, hbmColour);
    SelectBitmap(hdcMem2, hbmMask);
    SelectBitmap(hdcMem3, hbmBack);

	///////////////////////////////////////////////////////////////////////////////////
    //Copy of original !!!                                                           //
	///////////////////////////////////////////////////////////////////////////////////
    BitBlt(hdcMem3, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);
    // Set the background colour of the colour image to the colour
    // you want to be transparent.
    SetBkColor(hdcMem, crTransparent);

    // Copy the bits from the colour image to the B+W mask... everything
    // with the background colour ends up white while everythig else ends up
    // black...Just what we wanted.

    BitBlt(hdcMem2, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem3, 0, 0, SRCCOPY);

    // Take our new mask and use it to turn the transparent colour in our
    // original colour image to black so the transparency effect will
    // work right.
    BitBlt(hdcMem, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem2, 0, 0, SRCINVERT);

    // Clean up.

    DeleteDC(hdcMem);
    DeleteDC(hdcMem2);

    return hbmMask;

}
///////////////////////////////////////////////////////////////////////////////////
// Draws an HBITMAP with a bg color cTransparentColor on the target DC           //
// Works better with paletized images and cannot use alpha channels              //
///////////////////////////////////////////////////////////////////////////////////
void DrawTransparentBitmap(HDC hdc, HBITMAP hBitmap, short xStart,short yStart, COLORREF cTransparentColor)
{

   BITMAP     bm;
   COLORREF   cColor;
   HBITMAP    bmAndBack, bmAndObject, bmAndMem, bmSave;
   HBITMAP    bmBackOld, bmObjectOld, bmMemOld, bmSaveOld;
   HDC        hdcMem, hdcBack, hdcObject, hdcTemp, hdcSave;
   POINT      ptSize;

   hdcTemp = CreateCompatibleDC(hdc);
   SelectObject(hdcTemp, hBitmap);   // Select the bitmap

   GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&bm);
   ptSize.x = bm.bmWidth;            // Get width of bitmap
   ptSize.y = bm.bmHeight;           // Get height of bitmap
   DPtoLP(hdcTemp, &ptSize, 1);      // Convert from device

                                     // to logical points

   // Create some DCs to hold temporary data.
   hdcBack   = CreateCompatibleDC(hdc);
   hdcObject = CreateCompatibleDC(hdc);
   hdcMem    = CreateCompatibleDC(hdc);
   hdcSave   = CreateCompatibleDC(hdc);

   // Create a bitmap for each DC. DCs are required for a number of
   // GDI functions.

   // Monochrome DC
   bmAndBack   = CreateBitmap(ptSize.x, ptSize.y, 1, 1, NULL);

   // Monochrome DC
   bmAndObject = CreateBitmap(ptSize.x, ptSize.y, 1, 1, NULL);

   bmAndMem    = CreateCompatibleBitmap(hdc, ptSize.x, ptSize.y);
   bmSave      = CreateCompatibleBitmap(hdc, ptSize.x, ptSize.y);

   // Each DC must select a bitmap object to store pixel data.
   bmBackOld   = SelectObject(hdcBack, bmAndBack);
   bmObjectOld = SelectObject(hdcObject, bmAndObject);
   bmMemOld    = SelectObject(hdcMem, bmAndMem);
   bmSaveOld   = SelectObject(hdcSave, bmSave);

   // Set proper mapping mode.
   SetMapMode(hdcTemp, GetMapMode(hdc));

   // Save the bitmap sent here, because it will be overwritten.
   BitBlt(hdcSave, 0, 0, ptSize.x, ptSize.y, hdcTemp, 0, 0, SRCCOPY);

   // Set the background color of the source DC to the color.
   // contained in the parts of the bitmap that should be transparent
   cColor = SetBkColor(hdcTemp, cTransparentColor);

   // Create the object mask for the bitmap by performing a BitBlt
   // from the source bitmap to a monochrome bitmap.
   BitBlt(hdcObject, 0, 0, ptSize.x, ptSize.y, hdcTemp, 0, 0,SRCCOPY);

   // Set the background color of the source DC back to the original
   // color.
   SetBkColor(hdcTemp, cColor);

   // Create the inverse of the object mask.
   BitBlt(hdcBack, 0, 0, ptSize.x, ptSize.y, hdcObject, 0, 0,NOTSRCCOPY);

   // Copy the background of the main DC to the destination.
   BitBlt(hdcMem, 0, 0, ptSize.x, ptSize.y, hdc, xStart, yStart,SRCCOPY);

   // Mask out the places where the bitmap will be placed.
   BitBlt(hdcMem, 0, 0, ptSize.x, ptSize.y, hdcObject, 0, 0, SRCAND);

   // Mask out the transparent colored pixels on the bitmap.
   BitBlt(hdcTemp, 0, 0, ptSize.x, ptSize.y, hdcBack, 0, 0, SRCAND);

   // XOR the bitmap with the background on the destination DC.
   BitBlt(hdcMem, 0, 0, ptSize.x, ptSize.y, hdcTemp, 0, 0, SRCPAINT);

   // Copy the destination to the screen.
   BitBlt(hdc, xStart, yStart, ptSize.x, ptSize.y, hdcMem, 0, 0,SRCCOPY);

   // Place the original bitmap back into the bitmap sent here.
   BitBlt(hdcTemp, 0, 0, ptSize.x, ptSize.y, hdcSave, 0, 0, SRCCOPY);

   // Delete the memory bitmaps.
   DeleteObject(SelectObject(hdcBack, bmBackOld));
   DeleteObject(SelectObject(hdcObject, bmObjectOld));
   DeleteObject(SelectObject(hdcMem, bmMemOld));
   DeleteObject(SelectObject(hdcSave, bmSaveOld));

   // Delete the memory DCs.
   DeleteDC(hdcMem);
   DeleteDC(hdcBack);
   DeleteDC(hdcObject);
   DeleteDC(hdcSave);
   DeleteDC(hdcTemp);
}

///////////////////////////////////////////////////////////////////////////////////
// Composite 2 HBITMAPS                                                          //
///////////////////////////////////////////////////////////////////////////////////
HBITMAP compositeImage(HBITMAP fgImage, HBITMAP bgImage)
{
	// ...
	// the following code assumes that you have a valid HBITMAP loaded into the memory
	FIBITMAP *bgDib = NULL;
	FIBITMAP *fgDib = NULL;
	if(bgImage)
	{

		BITMAP bm;
		GetObject(bgImage, sizeof(BITMAP), (LPSTR) &bm);
		bgDib = FreeImage_Allocate(bm.bmWidth, bm.bmHeight, bm.bmBitsPixel,0,0,0);
		// The GetDIBits function clears the biClrUsed and biClrImportant BITMAPINFO members (dont't know why) 
		// So we save these infos below. This is needed for palettized images only. 
		int nColors = FreeImage_GetColorsUsed(bgDib);
		HDC dc = GetDC(NULL);
		int Success = GetDIBits(dc, bgImage, 0, FreeImage_GetHeight(bgDib), 
		FreeImage_GetBits(bgDib), FreeImage_GetInfo(bgDib), DIB_RGB_COLORS);
		ReleaseDC(NULL, dc);
		// restore BITMAPINFO members
		FreeImage_GetInfoHeader(bgDib)->biClrUsed = nColors;
		FreeImage_GetInfoHeader(bgDib)->biClrImportant = nColors;
	}
	if(fgImage)
	{
		BITMAP bm;
		GetObject(fgImage, sizeof(BITMAP), (LPSTR) &bm);
		fgDib = FreeImage_Allocate(bm.bmWidth, bm.bmHeight, bm.bmBitsPixel,0,0,0);
		// The GetDIBits function clears the biClrUsed and biClrImportant BITMAPINFO members (dont't know why)
		// So we save these infos below. This is needed for palettized images only.
		int nColors = FreeImage_GetColorsUsed(fgDib);
		HDC dc = GetDC(NULL);
		int Success = GetDIBits(dc, fgImage, 0, FreeImage_GetHeight(fgDib),
		FreeImage_GetBits(fgDib), FreeImage_GetInfo(fgDib), DIB_RGB_COLORS);
		ReleaseDC(NULL, dc);
		// restore BITMAPINFO members
		FreeImage_GetInfoHeader(fgDib)->biClrUsed = nColors;
		FreeImage_GetInfoHeader(fgDib)->biClrImportant = nColors;
	}
	HDC hDC = GetDC(NULL);
	FIBITMAP *display_dib_a = FreeImage_Composite(bgDib,TRUE,NULL,NULL); 
	HBITMAP bitmap = CreateDIBitmap(hDC, FreeImage_GetInfoHeader(fgDib), CBM_INIT, FreeImage_GetBits(fgDib), FreeImage_GetInfo(fgDib), DIB_RGB_COLORS);
	// don't forget to call FreeImage_Unload when you no longer need the dib
	FreeImage_Unload(bgDib);
	FreeImage_Unload(fgDib);
	return bitmap;
}

///////////////////////////////////////////////////////////////////////////////////////////
// Gradients the api way and non api                                                     //
///////////////////////////////////////////////////////////////////////////////////////////
BOOL gradientFill(HDC hdc, RECT objBox, COLORREF fromClr, COLORREF toClr)
{
	BOOL		result;
	COLORREF	gradient;	// Used to store the gradient value
	HPEN 		oldPen;
	HPEN 		grdPen;
	/////////////////////////////////////////////////////////////////////////////
	// "msimg32.dll", "GradientFill", Not available on W95/NT4 -> W98/W2K Only //
	/////////////////////////////////////////////////////////////////////////////
	HINSTANCE hLibrary = LoadLibrary("msimg32.dll");
	if ( NULL == hLibrary )
	{
		goto NoApi;
	}

	BOOL (__stdcall *pfnGradientFill)(HDC,PTRIVERTEX,ULONG,PVOID,ULONG,ULONG);

	pfnGradientFill = GetProcAddress(hLibrary,"dGradientFill");
	if ( NULL == pfnGradientFill )
	{
		FreeLibrary(hLibrary);
		goto NoApi;
	}

	// establish starting color
	DWORD rgb0 = fromClr; //GetSysColor(COLOR_GRADIENTACTIVECAPTION);

	// establish ending color
	DWORD rgb1 = toClr;	//GetSysColor(COLOR_ACTIVECAPTION);

	// setup gradient fill array
	TRIVERTEX vert[2];

	// starting
	vert[0].x      = objBox.left;	// Put it at X
	vert[0].y      = objBox.top;	// Put it at Y
	vert[0].Red    = MAKEWORD(0,GetRValue(rgb0));
	vert[0].Green  = MAKEWORD(0,GetGValue(rgb0));
	vert[0].Blue   = MAKEWORD(0,GetBValue(rgb0));
	vert[0].Alpha  = 0x0000;

	// ending
	vert[1].x      = objBox.right;
	vert[1].y      = objBox.bottom;
	vert[1].Red    = MAKEWORD(0,GetRValue(rgb1));
	vert[1].Green  = MAKEWORD(0,GetGValue(rgb1));
	vert[1].Blue   = MAKEWORD(0,GetBValue(rgb1));
	vert[1].Alpha  = 0x0000;

	GRADIENT_RECT gRect;
	gRect.UpperLeft  = 0;
	gRect.LowerRight = 1;

	result = pfnGradientFill(hdc, vert, 2, &gRect, 1, GRADIENT_FILL_RECT_H);
	result = FreeLibrary(hLibrary);

	return result;
NoApi:
	///////////////////////////////////////////
	// Do the gradient without using any api //
	///////////////////////////////////////////
	for(int i = objBox.top; i < objBox.bottom; i++)
	{
		gradient = InterpolateLinear(fromClr,toClr,i,objBox.top,objBox.bottom);
		grdPen = CreatePen(PS_SOLID,1,gradient);
		oldPen = SelectObject(hdc,grdPen);

		MoveToEx(hdc,objBox.left,i,NULL);
		LineTo(hdc,objBox.right,i);

		SelectObject(hdc,oldPen);
		DeleteObject(grdPen);
	}
	return TRUE;
}
HBITMAP compositeMask(HBITMAP fgBitmap, HBITMAP bgBitmap, HBITMAP mask)
{
	HDC hMemDC;
    DWORD dwWidth, dwHeight;
    HBITMAP hBitmap, hOldBitmap;
    void *lpBits;
    DWORD x,y;
}
RGBQUAD *bitMask8bits(int iWidth,int iHeight)
{
	BITMAPINFO		bmi;
	RGBQUAD			*pixelData; //[GRAPHICWINDOW_WIDTH * GRAPHICWINDOW_HEIGHT];
	void			*ptPixels;
	void			*lpBits;
	DWORD			dibData[iWidth * iHeight];


	pixelData = (RGBQUAD*)malloc(iWidth * iHeight * sizeof(RGBQUAD));
	//memset(pixelData, 0, iWidth * iHeight * sizeof(RGBQUAD));

	ZeroMemory(pixelData, (iWidth * iHeight * sizeof(RGBQUAD)));
	ZeroMemory(&bmi, sizeof(BITMAPINFO));

    bmi.bmiHeader.biSize			= sizeof( bmi.bmiHeader );
    bmi.bmiHeader.biWidth			= iWidth;
    bmi.bmiHeader.biHeight			= iHeight;
    bmi.bmiHeader.biPlanes			= 1;
    bmi.bmiHeader.biBitCount		= 32;
    bmi.bmiHeader.biCompression		= BI_RGB;
    bmi.bmiHeader.biSizeImage		= 0;
    bmi.bmiHeader.biXPelsPerMeter	= 0;
    bmi.bmiHeader.biYPelsPerMeter	= 0;
    bmi.bmiHeader.biClrUsed			= 0;
    bmi.bmiHeader.biClrImportant	= 0;
	
	BITMAPV5HEADER bi;
    ZeroMemory(&bi,sizeof(BITMAPV5HEADER));
    bi.bV5Size			= sizeof(BITMAPV5HEADER);
    bi.bV5Width			= iWidth;
    bi.bV5Height		= iHeight;
    bi.bV5Planes		= 1;
    bi.bV5BitCount		= 32;
    bi.bV5Compression	= BI_BITFIELDS;
    bi.bV5RedMask		= 0x00FF0000;
    bi.bV5GreenMask		= 0x0000FF00;
    bi.bV5BlueMask		= 0x000000FF;
    bi.bV5AlphaMask		= 0xFF000000; 


	//HDC hDC = CreateCompatibleDC(NULL);
	//HBITMAP DirectBitmap = CreateDIBSection(hdc, (BITMAPINFO *)&bmi, DIB_RGB_COLORS, (void **)&ptPixels, NULL, 0);
	HDC hdc;
	hdc =CreateCompatibleDC(NULL);
    // Create the DIB section with an alpha channel.
	HBITMAP hBitmap = CreateDIBSection(hdc, (BITMAPINFO *)&bi, DIB_RGB_COLORS, (void **)&lpBits, NULL, (DWORD)0);
    
    for (int x = 0; x < iWidth; x++)
       for (int y = 0; y < iHeight; y++)
       {
			((DWORD *)lpBits)[x + y * iWidth] 
					= (0xAF << 24) |             	//0xaa000000 Alpha
                 ((BYTE)(0xFF	* 0xAF) << 16)	|	//0x00rr0000 Red
                 ((BYTE)(0x1F	* 0xAF) << 8)	|	//0x0000gg00 Green
                 ((BYTE)(0x1F	* 0xAF));			//0x000000bb Blue
       }
	
	/*
    for (int y = 0; y < iHeight; y++)
        for (int x = 0; x < iWidth; x++)
        {
			RGBQUAD *pixel;
			int y;

			pixel  = &pixelData[x + y * iWidth];
            pixel->rgbRed   = y << 1;
            pixel->rgbGreen = 0xFF - (y << 1);
            pixel->rgbBlue  = 0x1F;


			//BYTE * pPixel = (BYTE *) ptPixels + x + y * ulBitmapWidth;
			//myAlpha = ((UINT32 *)ptPixels)[x + y * ulBitmapWidth];
            //ubAlpha = (BYTE)mask[x][y]; //(UCHAR)((float)ptPixels[x + y * ulBitmapWidth]); //mask[y][x]);//x / (float)ulBitmapWidth * 255);
            //calculate the factor by which we multiply each component
            //fAlphaFactor = (float)ubAlpha / (float)0xff; 
            // multiply each pixel by fAlphaFactor, so each component 
            // is less than or equal to the alpha value.
            //((UINT32 *)pvBits)[x + y * ulBitmapWidth] 
            //    = (ubAlpha << 24) |                       //0xaa000000
            //     ((UCHAR)(ubRed * fAlphaFactor) << 16) |  //0x00rr0000
            //     ((UCHAR)(ubGreen * fAlphaFactor) << 8) | //0x0000gg00
            //     ((UCHAR)(ubBlue   * fAlphaFactor));      //0x000000bb
			//bData = ((UINT32 *)pvBits)[x + y * ulBitmapWidth];
        }

	*/
/*
    for (int count=0; count < GRAPHICWINDOW_WIDTH; count++)
    {
        RGBQUAD *pixel;
        int y;
        
        #define calcoffset(_x, _y) (GRAPHICWINDOW_WIDTH * _y) + _x

        y = (int)(spectrum[count] * 4.0f * (float)GRAPHICWINDOW_HEIGHT);
        if (y >= GRAPHICWINDOW_HEIGHT)
        {
            y = GRAPHICWINDOW_HEIGHT - 1;
        }

        for (count2=0; count2<y; count2++)
        {
            pixel = &GraphicWindowBitmapData[calcoffset(count, count2)];
            pixel->rgbRed   = count2 << 1;
            pixel->rgbGreen = 0xFF - (count2 << 1);
            pixel->rgbBlue  = 0x1F;
        }
    }
*/
	return 0;
}
void DrawAlphaBlend (HWND hWnd, HDC hdcwnd)
{
	BYTE mask[32][32] = 
	{
		{ 255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255 },
		{ 255,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,255 },
		{ 255,240,225,225,225,225,225,225,225,225,225,225,225,225,225,225,225,225,225,225,225,225,225,225,225,225,225,225,225,225,225,225 },
		{ 255,240,225,210,210,210,210,210,210,210,210,210,210,210,210,210,210,210,210,210,210,210,210,210,210,210,210,210,210,225,240,255 },
		{ 255,240,225,210,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,210,225,240,255 },
		{ 255,240,225,210,195,180,180,180,180,180,180,180,180,180,180,180,180,180,180,180,180,180,180,180,180,180,180,195,210,225,240,255 },
		{ 255,240,225,210,195,180,165,165,165,165,165,165,165,165,165,165,165,165,165,165,165,165,165,165,165,165,180,195,210,225,240,255 },
		{ 255,240,225,210,195,180,165,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,165,180,195,210,225,240,255 },
		{ 255,240,225,210,195,180,165,150,135,135,135,135,135,135,135,135,135,135,135,135,135,135,135,135,150,165,180,195,210,225,240,255 },
		{ 255,240,225,210,195,180,165,150,135,120,120,120,120,120,120,120,120,120,120,120,120,120,120,135,150,165,180,195,210,225,240,255 },
		{ 255,240,225,210,195,180,165,150,135,120,105,105,105,105,105,105,105,105,105,105,105,105,120,135,150,165,180,195,210,225,240,255 },
		{ 255,240,225,210,195,180,165,150,135,120,105,075,075,075,075,075,075,075,075,075,075,105,120,135,150,165,180,195,210,225,240,255 },
		{ 255,240,225,210,195,180,165,150,135,120,105,075,045,045,045,045,045,045,045,045,075,105,120,135,150,165,180,195,210,225,240,255 },
		{ 255,240,225,210,195,180,165,150,135,120,105,075,045,030,030,030,030,030,030,045,075,105,120,135,150,165,180,195,210,225,240,255 },
		{ 255,240,225,210,195,180,165,150,135,120,105,075,045,030,020,020,020,020,030,045,075,105,120,135,150,165,180,195,210,225,240,255 },
		{ 255,240,225,210,195,180,165,150,135,120,105,075,045,030,020,010,010,020,030,045,075,105,120,135,150,165,180,195,210,225,240,255 },
		{ 255,240,225,210,195,180,165,150,135,120,105,075,045,030,020,010,010,020,030,045,075,105,120,135,150,165,180,195,210,225,240,255 },
		{ 255,240,225,210,195,180,165,150,135,120,105,075,045,030,020,020,020,020,030,045,075,105,120,135,150,165,180,195,210,225,240,255 },
		{ 255,240,225,210,195,180,165,150,135,120,105,075,045,030,030,030,030,030,030,045,075,105,120,135,150,165,180,195,210,225,240,255 },
		{ 255,240,225,210,195,180,165,150,135,120,105,075,045,045,045,045,045,045,045,045,075,105,120,135,150,165,180,195,210,225,240,255 },
		{ 255,240,225,210,195,180,165,150,135,120,105,075,075,075,075,075,075,075,075,075,075,105,120,135,150,165,180,195,210,225,240,255 },
		{ 255,240,225,210,195,180,165,150,135,120,105,105,105,105,105,105,105,105,105,105,105,105,120,135,150,165,180,195,210,225,240,255 },
		{ 255,240,225,210,195,180,165,150,135,120,120,120,120,120,120,120,120,120,120,120,120,120,120,135,150,165,180,195,210,225,240,255 },
		{ 255,240,225,210,195,180,165,150,135,135,135,135,135,135,135,135,135,135,135,135,135,135,135,135,150,165,180,195,210,225,240,255 },
		{ 255,240,225,210,195,180,165,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,165,180,195,210,225,240,255 },
		{ 255,240,225,210,195,180,165,165,165,165,165,165,165,165,165,165,165,165,165,165,165,165,165,165,165,165,180,195,210,225,240,255 },
		{ 255,240,225,210,195,180,180,180,180,180,180,180,180,180,180,180,180,180,180,180,180,180,180,180,180,180,180,195,210,225,240,255 },
		{ 255,240,225,210,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,210,225,240,255 },
		{ 255,240,225,210,210,210,210,210,210,210,210,210,210,210,210,210,210,210,210,210,210,210,210,210,210,210,210,210,210,225,240,255 },
		{ 255,240,225,225,225,225,225,225,225,225,225,225,225,225,225,225,225,225,225,225,225,225,225,225,225,225,225,225,225,225,225,225 },
		{ 255,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,255 },
		{ 255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255 }
	};
    
	HDC hdc;               // handle of the DC we will create 
    BLENDFUNCTION bf;      // structure for alpha blending
    HBITMAP hbitmap;       // bitmap handle
    BITMAPINFO bmi;        // bitmap header
    VOID *pvBits;          // pointer to DIB section
    ULONG   ulWindowWidth, ulWindowHeight;      // window width/height
    ULONG   ulBitmapWidth, ulBitmapHeight;      // bitmap width/height
    RECT    rt;            // used for getting window dimensions
    UINT32   x,y;          // stepping variables
    UCHAR ubAlpha;         // used for doing transparent gradient
    UCHAR ubRed;        
    UCHAR ubGreen;
    UCHAR ubBlue;
    float fAlphaFactor;    // used to do premultiply
            
    // get window dimensions
    GetClientRect(hWnd, &rt);
    
    // calculate window width/height
    ulWindowWidth = 32; //rt.right - rt.left;  
    ulWindowHeight = 32; //rt.bottom - rt.top;  

    // make sure we have at least some window size
    if ((!ulWindowWidth) || (!ulWindowHeight))
        return;

    // divide the window into 3 horizontal areas
    //ulWindowHeight = ulWindowHeight / 3;

    // create a DC for our bitmap -- the source DC for AlphaBlend 
    hdc = CreateCompatibleDC(hdcwnd);
    
    // zero the memory for the bitmap info
    ZeroMemory(&bmi, sizeof(BITMAPINFO));

    // setup bitmap info 
    // set the bitmap width and height to 60% of the width and height of each of the three horizontal areas. Later on, the blending will occur in the center of each of the three areas.
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = ulBitmapWidth = ulWindowWidth;// - (ulWindowWidth/5)*2;
    bmi.bmiHeader.biHeight = ulBitmapHeight = ulWindowHeight;// - (ulWindowHeight/5)*2;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;         // four 8-bit components
    bmi.bmiHeader.biCompression = BI_RGB;
    bmi.bmiHeader.biSizeImage = ulBitmapWidth * ulBitmapHeight * 4;

    // create our DIB section and select the bitmap into the dc
    hbitmap = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, &pvBits, NULL, 0x0);
    SelectObject(hdc, hbitmap);

	BITMAP bmp;
	HINSTANCE hinst = GetModuleHandle(NULL);
	HBITMAP hMask = NULL;
	hMask = LoadImage(GetModuleHandle(NULL),"alpha.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE); //LoadBitmap(hinst, MAKEINTRESOURCE(idbitmap));
	GetObject(hMask, sizeof(BITMAP), &bmp);

	BITMAPINFO RGB32BitsBITMAPINFO; 
	ZeroMemory(&RGB32BitsBITMAPINFO,sizeof(BITMAPINFO));
	RGB32BitsBITMAPINFO.bmiHeader.biSize		= sizeof(BITMAPINFOHEADER);
	RGB32BitsBITMAPINFO.bmiHeader.biWidth		= bmp.bmWidth;
	RGB32BitsBITMAPINFO.bmiHeader.biHeight		= bmp.bmHeight;
	RGB32BitsBITMAPINFO.bmiHeader.biPlanes		= 1;
	RGB32BitsBITMAPINFO.bmiHeader.biBitCount	= 32;

	void  * ptPixels;
	UINT32  myAlpha;
	UINT32	bData;

	HDC hDC = CreateCompatibleDC(hdcwnd);
	HBITMAP DirectBitmap = CreateDIBSection(hdc, (BITMAPINFO *)&RGB32BitsBITMAPINFO, DIB_RGB_COLORS, (void **)&ptPixels, NULL, 0);

	/*
	HDC mDC = CreateCompatibleDC(hdcwnd);
	HBITMAP oldBmp = SelectObject(mDC, hMask);
	//BitBlt(hdc, 0, 0, bmp.bmWidth, bmp.bmHeight, mDC,0,0, SRCCOPY);
	//DeleteDC(mDC);

	for (y = 0; y < ulBitmapHeight; y++)
	for (x = 0; x < ulBitmapWidth; x++)
	{
		COLORREF myColor = GetPixel(mDC,x,y);
	}
	SelectObject(mDC,oldBmp);
	DeleteDC(hDC);
	*/
    // bottom window area, use constant alpha = 75% and a changing
    // source alpha. Create a gradient effect using source alpha, and 
    // then fade it even more with constant alpha
    ubRed = 0x00;
    ubGreen = 0x00;
    ubBlue = 0xff;
    
    for (y = 0; y < ulBitmapHeight; y++)
        for (x = 0; x < ulBitmapWidth; x++)
        {

            // for a simple gradient, base the alpha value on the x 
            // value of the pixel
			//BYTE pp = (UINT) ptPixels[x + y * ulBitmapWidth];
			BYTE * pPixel = (BYTE *) ptPixels + x + y * ulBitmapWidth;
			myAlpha = ((UINT32 *)ptPixels)[x + y * ulBitmapWidth];
            ubAlpha = (BYTE)mask[x][y]; //(UCHAR)((float)ptPixels[x + y * ulBitmapWidth]); //mask[y][x]);//x / (float)ulBitmapWidth * 255);
            //calculate the factor by which we multiply each component
            fAlphaFactor = (float)ubAlpha / (float)0xff; 
            // multiply each pixel by fAlphaFactor, so each component 
            // is less than or equal to the alpha value.
            ((UINT32 *)pvBits)[x + y * ulBitmapWidth] 
                = (ubAlpha << 24) |                       //0xaa000000
                 ((UCHAR)(0xFF	* fAlphaFactor) << 16) |  //0x00rr0000
                 ((UCHAR)(0x1F	* fAlphaFactor) << 8) | //0x0000gg00
                 ((UCHAR)(0x1F	* fAlphaFactor));      //0x000000bb
			bData = ((UINT32 *)pvBits)[x + y * ulBitmapWidth];
        }

    bf.BlendOp = AC_SRC_OVER;
    bf.BlendFlags = 0;
    bf.AlphaFormat = AC_SRC_ALPHA;   // use source alpha 
    bf.SourceConstantAlpha = 0xFF;   // use constant alpha, with 
                                     // 75% opaqueness
    AlphaBlend(hdcwnd, ulWindowWidth, 0, ulBitmapWidth, ulBitmapHeight, hdc, 0, 0, ulBitmapWidth, ulBitmapHeight, bf);

	//SelectObject(hdcwnd, oldP);
    // do cleanup
    DeleteObject(hbitmap);
    DeleteDC(hdc);

}
HBITMAP skinedBox(HBITMAP leftHbm, HBITMAP middleHbm, HBITMAP rightHbm, int bWidth, int bHeight)
{
	BITMAP	bmpLeft;
	BITMAP	bmpMiddle;
	BITMAP	bmpRight;
	HBITMAP oldBmp		= NULL;
	HDC		drawSrfc	= CreateCompatibleDC(NULL);		// Desktop Compatible DC
	HDC		hdcMem		= CreateCompatibleDC(NULL);		// Desktop Compatible DC
	HBITMAP	hbmp		= CreateCompatibleBitmap(drawSrfc, bWidth, bHeight);
	int		offset		= 0;


	GetObject(leftHbm, sizeof(BITMAP), &bmpLeft);
	GetObject(middleHbm, sizeof(BITMAP), &bmpMiddle);
	GetObject(rightHbm, sizeof(BITMAP), &bmpRight);
	oldBmp = SelectObject(hdcMem, leftHbm);
	BitBlt(drawSrfc, 0, 0, bmpLeft.bmWidth, bmpLeft.bmHeight, hdcMem, 0, 0, SRCCOPY);
	SelectObject(hdcMem,oldBmp);
	offset += bmpLeft.bmWidth;


	oldBmp = SelectObject(hdcMem, middleHbm);
	for(int x = offset; x < (bWidth - bmpLeft.bmWidth); x++)
	{
		BitBlt(drawSrfc, x, 0, bmpMiddle.bmWidth, bmpMiddle.bmHeight, hdcMem, 0, 0, SRCCOPY);
		offset++;
	}
	SelectObject(hdcMem,oldBmp);

	oldBmp = SelectObject(hdcMem, rightHbm);
	BitBlt(drawSrfc, offset, 0, bmpRight.bmWidth, bmpRight.bmHeight, hdcMem, 0, 0, SRCCOPY);

	SelectObject(hdcMem,oldBmp);

	oldBmp = SelectObject(hdcMem, hbmp);
	BitBlt(drawSrfc, offset, 0, bWidth, bHeight, hdcMem, 0, 0, SRCCOPY);
	SelectObject(hdcMem,oldBmp);


	DeleteDC(hdcMem);
	DeleteDC(drawSrfc);
	/*
	BITMAPINFO bmi; 
	ZeroMemory(&bmi,sizeof(BITMAPINFO));
	bmi.bmiHeader.biSize		= sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth		= bWidth;
	bmi.bmiHeader.biHeight		= bHeight;
	bmi.bmiHeader.biPlanes		= 1;
	bmi.bmiHeader.biBitCount	= 32;
	bmi.bmiHeader.biCompression = BI_RGB;
	bmi.bmiHeader.biSizeImage	= bWidth * bHeight * 4;
	*/
	return hbmp;
}
///////////////////////////////////////////////////////////////////////////////////////////
// Linear Interpolation the easy way to do gradients author unknow                       //
///////////////////////////////////////////////////////////////////////////////////////////
COLORREF InterpolateLinear(COLORREF first, COLORREF second, float position, float start, float end)
{
	if(start == end) return first;
	if(end - start == 0) return second;
	if(position == start) return first;
	if(position == end) return second;
	return RGB
		(
			(BYTE)((GetRValue(second)*(position - start) + GetRValue(first)*(end-position))/(end-start)),
			(BYTE)((GetGValue(second)*(position - start) + GetGValue(first)*(end-position))/(end-start)),
			(BYTE)((GetBValue(second)*(position - start) + GetBValue(first)*(end-position))/(end-start))
		);
}
void wallpaper(HDC hdc,RECT *lprc,int idbitmap)
{
	HDC shdc = NULL;
	BITMAP bmp;
	HBITMAP hbmp = NULL;

	long bmx, bmy;

	if (idbitmap >0)
	{
		HINSTANCE hinst	= GetModuleHandle(NULL);
		hbmp	= LoadImage(GetModuleHandle(NULL),"rayado.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE); //LoadBitmap(hinst, MAKEINTRESOURCE(idbitmap));
		if(hbmp == NULL)
		{
			MessageBoxW(NULL,L"LoadBitmap()",L"Error",MB_ICONINFORMATION);
		}
		shdc	= CreateCompatibleDC(hdc);
		SelectObject(shdc, hbmp);
		GetObject(hbmp, sizeof(BITMAP), &bmp);
		bmx = bmp.bmWidth;
		bmy = bmp.bmHeight;

		long r = lprc->right;
		long l = lprc->left;
		long b = lprc->bottom;
		long t = lprc->top;

		long x,y;
		for(x=l; x<r; x += bmx)
		{
			for(y=t; y<b; y += bmy)
			{
				BitBlt(hdc, x,y, r-x,b-y, shdc, 0, 0, SRCCOPY);
			}
		}

		DeleteObject(hbmp);
		DeleteDC(shdc);
	}
}

///////////////////////////////////////////////////////////////////////////////////
//                                                                               //
// loadPNG                                                                       //
// -----------------                                                             //
//                                                                               //
// Reads a png file.                                                             //
// @return true if the file operation succeeded                                  //
// @return false if the file operation failed                                    //
///////////////////////////////////////////////////////////////////////////////////
BOOL loadPNG(const char* Path)
{
	// reset health flag
	m_good = FALSE;

	// check filetype
	 if (!doCheckFileHeader(Path)) return FALSE;

	// try to open file
	FILE* file = fopen(Path, "rb");

	// unable to open
	if (file == 0) return FALSE;

	// create read struct
	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);

	// check pointer
	if (png_ptr == 0)
	{
		fclose(file);
		return FALSE;
	}

	// create info struct
    png_infop info_ptr = png_create_info_struct(png_ptr);

	// check pointer
    if (info_ptr == 0)
    {
        png_destroy_read_struct(&png_ptr, 0, 0);
		fclose(file);
        return FALSE;
    }

	// set error handling
	if (setjmp(png_jmpbuf(png_ptr)))
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, 0);
		fclose(file);
		return FALSE;
	}

	// I/O initialization using standard C streams
	png_init_io(png_ptr, file);

	// read entire image (high level)
	png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_EXPAND, 0);

	// convert the png bytes to BGRA
	if (!doExtractCanonicData(png_ptr, info_ptr))
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, 0);

		fclose(file);
		return FALSE;
	}

	// free memory
	png_destroy_read_struct(&png_ptr, &info_ptr, 0);

	// close file
	fclose(file);

	// reset health flag
	return (m_good = TRUE);
}

///////////////////////////////////////////////////////////////////////////////////
//                                                                               //
// doCheckFileHeader                                                             //
// -----------------                                                             //
//                                                                               //
// Returns true if the specified file is a png file.                             //
///////////////////////////////////////////////////////////////////////////////////
BOOL doCheckFileHeader(const char* Path)
{
	// try to open file
	FILE* file = fopen(Path, "rb");

	// unable to open
	if (file == 0) return FALSE;

	// the number of bytes to read
	const int nread = 8;

	unsigned char buffer[nread];

	// try to read header
	if (fread(buffer, 1, nread, file) != nread)
	{
		fclose(file);
		return 0;
	}

	// close file
	fclose(file);

	// compare headers
	int result = png_sig_cmp(buffer, 0, nread);

	return(result == 0);
}
/*
HBITMAP loadPngImage(TCHAR filePath)
{
	// the image's width
	int png_width;
	// the image's height
	int png_height;
	// the color values (blue, green, red, alpha)
	unsigned char* png_bgra;
	// check filetype
	if (!doCheckFileHeader(filePath))
	{
		return FALSE;
	}

	// try to open file
	FILE* file = fopen(Path, "rb");

	// unable to open
	if (file == 0) return FALSE;

	// create read struct
	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);

	// check pointer
	if (png_ptr == 0)
	{
		fclose(file);
		return FALSE;
	}

	// create info struct
    png_infop info_ptr = png_create_info_struct(png_ptr);

	// check pointer
    if (info_ptr == 0)
    {
        png_destroy_read_struct(&png_ptr, 0, 0);
		fclose(file);
        return FALSE;
    }

	// set error handling
	if (setjmp(png_jmpbuf(png_ptr)))
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, 0);
		fclose(file);
		return FALSE;
	}

	// I/O initialization using standard C streams
	png_init_io(png_ptr, file);

	// read entire image (high level)
	png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_EXPAND, 0);

	// convert the png bytes to BGRA
	if (!doExtractCanonicData(png_ptr, info_ptr))
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, 0);

		fclose(file);
		return FALSE;
	}

	// free memory
	png_destroy_read_struct(&png_ptr, &info_ptr, 0);

	// close file
	fclose(file);

	// reset health flag
	return (m_good = TRUE);

	loadPNG("inter.png");
	unsigned char* data = getBGRA();
		for (int y=0; y<48; y++)
		{
			BYTE * pPixel = (BYTE *) data + 48 * 4 * y;

			for (int x=0; x<48; x++)
			{
                pPixel[0] = pPixel[0] * pPixel[3] / 255; 
                pPixel[1] = pPixel[1] * pPixel[3] / 255; 
                pPixel[2] = pPixel[2] * pPixel[3] / 255; 

                pPixel += 4;
            }
		}

		HBITMAP bitmap2 = CreateCompatibleBitmap(hDC, 48, 48);
		// RGB + Alpha
		int bsize = 48 * 48 * 4;

		SetBitmapBits(bitmap2, bsize, data);

}
*/
///////////////////////////////////////////////////////////////////////////////////
//                                                                               //
// doExtractCanonicData                                                          //
// --------------------                                                          //
//                                                                               //
// Converts the png bytes to BGRA.                                               //
//                                                                               //
// @par Assumptions                                                              //
// - png_read_png() has been called with the PNG_TRANSFORM_EXPAND flag           //
///////////////////////////////////////////////////////////////////////////////////
BOOL doExtractCanonicData(png_structp PngPtr, png_infop InfoPtr)
{
	// check pointer
	if (m_bgra)
	{
		// free memory
		free(m_bgra);

		// reset pointer
		m_bgra = 0;
	}

	// get dimensions
	m_width = png_get_image_width(PngPtr, InfoPtr);
    m_height = png_get_image_height(PngPtr, InfoPtr);

	// get color information
	int color_type = png_get_color_type(PngPtr, InfoPtr);

	// rgb
	if (color_type == PNG_COLOR_TYPE_RGB)
	{
		//m_bgra = (UINT)doConvertRGB8(PngPtr, InfoPtr);
	}

	// rgb with opacity
	else if (color_type == PNG_COLOR_TYPE_RGB_ALPHA)
	{
		m_bgra = doConvertRGBA8(PngPtr, InfoPtr);
	}

	// 256 grey values
	else if (color_type == PNG_COLOR_TYPE_GRAY)
	{
		//m_bgra = doConvertGrey8(PngPtr, InfoPtr);
	}

	// 256 grey values with opacity
	else if (color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
	{
		//m_bgra = doConvertGreyA8(PngPtr, InfoPtr);
	}

	// check pointer
	return (m_bgra != 0);
}
///////////////////////////////////////////////////////////////////////////////////
//                                                                               //
// doConvertRGBA8                                                                //
// --------------                                                                //
//                                                                               //
// Gets the data from an 8-bit rgb image with alpha values.                      //
//                                                                               //
// @par Memory                                                                   //
// - The returned pointer is created by using the new[] operator.                //
// - You have to free the allocated memory yourself.                             //
//                                                                               //
// @par Structure                                                                //
// - The color-sequence is Blue-Green-Red-Alpha (8 bit each).                    //
// - The first 4 val. (RGBA) are located in the top-left corner of the image.    //
// - The last 4 val. (RGBA) are located in the bottom-right corner of the image. //
///////////////////////////////////////////////////////////////////////////////////
unsigned char* doConvertRGBA8(png_structp PngPtr, png_infop InfoPtr)
{
	// calculate needed memory
	int size = m_height * m_width * 4;

	// allocate memory
	unsigned char* bgra = (unsigned char *) malloc(sizeof(unsigned char) * size);

	// get image rows
	png_bytep* row_pointers = png_get_rows(PngPtr, InfoPtr);

	int pos = 0;

	// get color values
	for(int i = 0; i < m_height; i++)
	{
		for(int j = 0; j < (4 * m_width); j += 4)
		{
			bgra[pos++] = row_pointers[i][j + 2];	// blue
			bgra[pos++] = row_pointers[i][j + 1];	// green
			bgra[pos++] = row_pointers[i][j];		// red
			bgra[pos++] = row_pointers[i][j + 3];	// alpha
		}
	}

	return bgra;
}

///////////////////////////////////////////////////////////////////////////////////
//                                                                               //
// doConvertGrey8                                                                //
// --------------                                                                //
//                                                                               //
// Gets the data from an 8-bit monochrome image.                                 //
//                                                                               //
// @par Memory                                                                   //
// - The returned pointer is created by using the new[] operator.                //
// - You have to free the allocated memory yourself.                             //
//                                                                               //
// @par Structure                                                                //
// - The color-sequence is Blue-Green-Red-Alpha (8 bit each).                    //
// - The first 4 val. (RGBA) are located in the top-left corner of the image.    //
// - The last 4 val. (RGBA) are located in the bottom-right corner of the image. //
//                                                                               //
///////////////////////////////////////////////////////////////////////////////////
unsigned char* doConvertGrey8(png_structp PngPtr, png_infop InfoPtr)
{
	// calculate needed memory
	int size = m_height * m_width * 4;

	// allocate memory
	unsigned char* bgra = (unsigned char *) malloc(sizeof(unsigned char) * size);

	// get image rows
	png_bytep* row_pointers = png_get_rows(PngPtr, InfoPtr);

	int pos = 0;

	// get color values
	for(int i = 0; i < m_height; i++)
	{
		for(int j = 0; j < m_width; j++)
		{
			bgra[pos++] = row_pointers[i][j];	// blue
			bgra[pos++] = row_pointers[i][j];	// green
			bgra[pos++] = row_pointers[i][j];	// red
			bgra[pos++] = 0;					// alpha
		}
	}

	return bgra;
}

///////////////////////////////////////////////////////////////////////////////////
//                                                                               //
// doConvertGreyA8                                                               //
// ---------------                                                               //
//                                                                               //
// Gets the data from an 8-bit monochrome image with alpha values.               //
//                                                                               //
///////////////////////////////////////////////////////////////////////////////////
unsigned char* doConvertGreyA8(png_structp PngPtr, png_infop InfoPtr)
{
	// calculate needed memory
	int size = m_height * m_width * 4;

	// allocate memory
	unsigned char* bgra = (unsigned char *) malloc(sizeof(unsigned char) * size);

	// get image rows
	png_bytep* row_pointers = png_get_rows(PngPtr, InfoPtr);

	int pos = 0;

	// get color values
	for(int i = 0; i < m_height; i++)
	{
		for(int j = 0; j < (2 * m_width); j += 2)
		{
			bgra[pos++] = row_pointers[i][j];		// blue
			bgra[pos++] = row_pointers[i][j];		// green
			bgra[pos++] = row_pointers[i][j];		// red
			bgra[pos++] = row_pointers[i][j + 1];	// alpha
		}
	}

	return bgra;
}
unsigned char* getBGRA()
{
	return m_bgra;
}

void doGlow(HDC hDC)
{
	// Get screen DC
	//HDC hDC = GetDC(m_hWnd);

	// Image rectangle
	RECT rect = {0, 0, 300, 200};
	// Set transparent and glowing color
	COLORREF transparentColor = RGB(128,128,128);
	COLORREF glowColor = RGB(0,0,255);
	COLORREF shadowColor = RGB(0,0,0);

	// Create background brush
	HBRUSH hBgBrush = CreateSolidBrush(transparentColor);

	FillRect(hDC, &rect, hBgBrush);

	// Create background DC and bitmap
	m_hBgDC = CreateCompatibleDC(hDC);
	m_hBgBitmap = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(11274), IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE);
	m_hOldBgBitmap = (HBITMAP)SelectObject(m_hBgDC, m_hBgBitmap);

	// Create memory DC and bitmap
	m_hMemDC = CreateCompatibleDC(hDC);
	m_hMemBitmap = CreateCompatibleBitmap(hDC, 300, 200);
	m_hOldMemBitmap = (HBITMAP)SelectObject(m_hMemDC, m_hMemBitmap);
	BitBlt(m_hMemDC, 0, 0, 300, 200, m_hBgDC, 0, 0, SRCCOPY);

	// Create glow DC and bitmap
	m_hGlowDC = CreateCompatibleDC(hDC);
	m_hGlowBitmap = CreateCompatibleBitmap(hDC, 300, 200);
	m_hOldGlowBitmap = (HBITMAP)SelectObject(m_hGlowDC, m_hGlowBitmap);
	BitBlt(m_hGlowDC, 0, 0, 300, 200, m_hBgDC, 0, 0, SRCCOPY);

	// Destroy background brush
	DeleteObject(hBgBrush);
	
	// Release screen DC
	//ReleaseDC(m_hWnd, hDC);

	// Create drawing on memory DC
	CreateDrawing(m_hMemDC);

	// Create glow effect
	CreateGlow(transparentColor, glowColor);
	blurBitmap(hDC, m_hGlowBitmap, 1);
	m_hOldGlowBitmap = (HBITMAP)SelectObject(m_hMemDC, m_hGlowBitmap);

	BitBlt(hDC, 0, 0, 200, 200, m_hMemDC, 0, 0, SRCCOPY);
}
void CreateDrawing(HDC hDrawingDC)
{
	COLORREF drawingColor = RGB(0,0,255);

	// Draw some text
	LOGFONT lf;
	lf.lfHeight = -MulDiv(24, GetDeviceCaps(GetDC(NULL), LOGPIXELSY), 72);
	lf.lfWidth = 0;
	lf.lfEscapement = 0;
	lf.lfOrientation = 0;
	lf.lfWeight = FW_BOLD;
	lf.lfItalic = FALSE;
	lf.lfUnderline = FALSE;
	lf.lfStrikeOut = FALSE;
	lf.lfCharSet = DEFAULT_CHARSET;
	lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
	lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	lf.lfQuality = DEFAULT_QUALITY;
	lf.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
	_tcscpy(lf.lfFaceName, _T("Times New Roman"));
	HFONT hFont = CreateFontIndirect(&lf);
	HFONT hOldFont = (HFONT)SelectObject(hDrawingDC, hFont);
	COLORREF oldTextColor = SetTextColor(hDrawingDC, drawingColor);
	int oldTextMode = SetBkMode(hDrawingDC, TRANSPARENT);
	_TCHAR lpszText[] = _T("Hello, World !!!");
	TextOut(hDrawingDC,35, 75, lpszText, _tcslen(lpszText));
	SelectObject(hDrawingDC, hOldFont);
	DeleteObject(hFont);
	SetBkMode(hDrawingDC, oldTextMode);
	SetTextColor(hDrawingDC, oldTextColor);

	// Draw some shapes
	HPEN hPen = CreatePen(PS_SOLID, 5, drawingColor);
	HPEN hOldPen = (HPEN)SelectObject(hDrawingDC, hPen);
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hDrawingDC, (HBRUSH)GetStockObject(HOLLOW_BRUSH));
	MoveToEx(hDrawingDC, 30, 30, NULL);
	LineTo(hDrawingDC, 200, 60);
	MoveToEx(hDrawingDC, 30, 60, NULL);
	LineTo(hDrawingDC, 200, 30);
	Rectangle(hDrawingDC, 80, 130, 140, 180);
	Ellipse(hDrawingDC, 120, 140, 180, 170);
	Rectangle(hDrawingDC, 160, 130, 220, 180);
	SelectObject(hDrawingDC, hOldBrush);
	SelectObject(hDrawingDC, hOldPen);
	DeleteObject(hPen);
}

void CreateGlow(COLORREF transparentColor, COLORREF glowColor)
{
	int i,j, k, l;
	RECT rect = {0, 0, 300, 200};


	// Create temporary DC and bitmap
	
	HDC hDC = GetDC(NULL);
	
	/*
	HDC m_hBgDC = CreateCompatibleDC(hDC);
	HBITMAP m_hMemBitmap = CreateCompatibleBitmap(hDC, 300, 200);
	*/
	HDC hTempDC = CreateCompatibleDC(hDC);
	HBITMAP hTempBitmap = CreateCompatibleBitmap(hDC, 300, 200);
	HBITMAP hOldTempBitmap = (HBITMAP)SelectObject(hTempDC, hTempBitmap);
	HDC hTempDC2 = CreateCompatibleDC(hDC);
	HBITMAP hTempBitmap2 = CreateCompatibleBitmap(hDC, 300, 200);
	HBITMAP hOldTempBitmap2 = (HBITMAP)SelectObject(hTempDC2, hTempBitmap2);
	HDC hTempDC3 = CreateCompatibleDC(hDC);
	HBITMAP hTempBitmap3 = CreateCompatibleBitmap(hDC, 300, 200);
	HBITMAP hOldTempBitmap3 = (HBITMAP)SelectObject(hTempDC3, hTempBitmap3);
	ReleaseDC(NULL, hDC);

	// Clear background
	HBRUSH hBgBrush = CreateSolidBrush(transparentColor);
	FillRect(hTempDC, &rect, hBgBrush);
	FillRect(hTempDC2, &rect, hBgBrush);
	BitBlt(hTempDC3, 0, 0, 300, 200, m_hBgDC, 0, 0, SRCCOPY);
	DeleteObject(hBgBrush);

	// Create drawing on glow DC
	// CreateDrawing(hTempDC2);

	// Draw memory DC on temporary DC
	int glowingOffset = 4;
	TransparentBlt(hTempDC, -glowingOffset, 0, 300, 200, hTempDC2, 0, 0, 300, 200, transparentColor);
	TransparentBlt(hTempDC, glowingOffset, 0, 300, 200, hTempDC2, 0, 0, 300, 200, transparentColor);
	TransparentBlt(hTempDC, 0, -glowingOffset, 300, 200, hTempDC2, 0, 0, 300, 200, transparentColor);
	TransparentBlt(hTempDC, 0, glowingOffset, 300, 200, hTempDC2, 0, 0, 300, 200, transparentColor);

	// Get original bitmap
	BITMAP bmpOrig;
	GetObject(m_hMemBitmap, sizeof(BITMAP), &bmpOrig);
	int sizeOrig = bmpOrig.bmWidthBytes * bmpOrig.bmHeight;
	BYTE* pDataOrig = (BYTE *) malloc(sizeof(BYTE) * sizeOrig);
	GetBitmapBits(m_hMemBitmap, sizeOrig, pDataOrig);
	int bppOrig = bmpOrig.bmBitsPixel >> 3;

	// Get source bitmap
	BITMAP bmpSrc;
	GetObject(m_hMemBitmap, sizeof(BITMAP), &bmpSrc);
	int sizeSrc = bmpSrc.bmWidthBytes * bmpSrc.bmHeight;
	BYTE* pDataSrc = (BYTE *) malloc(sizeof(BYTE) * sizeSrc);
	GetBitmapBits(hTempBitmap, sizeSrc, pDataSrc);
	int bppSrc = bmpSrc.bmBitsPixel >> 3;

	// Get source2 bitmap
	BITMAP bmpSrc2;
	GetObject(hTempBitmap2, sizeof(BITMAP), &bmpSrc2);
	int sizeSrc2 = bmpSrc2.bmWidthBytes * bmpSrc2.bmHeight;
	BYTE* pDataSrc2 = (BYTE *) malloc(sizeof(BYTE) * sizeSrc2);
	GetBitmapBits(hTempBitmap2, sizeSrc2, pDataSrc2);
	int bppSrc2 = bmpSrc2.bmBitsPixel >> 3;

	// Get source3 bitmap
	BITMAP bmpSrc3;
	GetObject(hTempBitmap3, sizeof(BITMAP), &bmpSrc3);
	int sizeSrc3 = bmpSrc3.bmWidthBytes * bmpSrc3.bmHeight;
	BYTE* pDataSrc3 = (BYTE *) malloc(sizeof(BYTE) * sizeSrc3);
	GetBitmapBits(hTempBitmap3, sizeSrc3, pDataSrc3);
	int bppSrc3 = bmpSrc3.bmBitsPixel >> 3;

	// Get destination bitmap
	BITMAP bmpDst;
	GetObject(m_hGlowBitmap, sizeof(BITMAP), &bmpDst);
	int sizeDst = bmpDst.bmWidthBytes * bmpDst.bmHeight;
	BYTE* pDataDst = (BYTE *) malloc(sizeof(BYTE) * sizeDst);
	GetBitmapBits(m_hGlowBitmap, sizeDst, pDataDst);
	int bppDst = bmpDst.bmBitsPixel >> 3;

	// Get transparent color
	BYTE redTransparent = GetRValue(transparentColor);
	BYTE greenTransparent = GetGValue(transparentColor);
	BYTE blueTransparent = GetBValue(transparentColor);

	// Get glow color
	BYTE redGlow = GetRValue(glowColor);
	BYTE greenGlow = GetGValue(glowColor);
	BYTE blueGlow = GetBValue(glowColor);

	// Copy source bitmap to destination bitmap using transparent color
	int verticalOffset = 0;
	int horizontalOffset;
	int totalOffset;
	BYTE red, green, blue;
	for (i=0; i<bmpSrc.bmHeight; i++)
	{
		horizontalOffset = 0;

		for (j=0; j<bmpSrc.bmWidth; j++)
		{
			// Calculate total offset
			totalOffset = verticalOffset + horizontalOffset;

			// Get source pixel
			blue = pDataSrc[totalOffset];
			green = pDataSrc[totalOffset+1];
			red = pDataSrc[totalOffset+2];
			// Check for transparent color
			if ((red != redTransparent) || (green != greenTransparent) || (blue != blueTransparent))
			{
				// Set destination pixel
				pDataSrc3[totalOffset] = blueGlow;
				pDataSrc3[totalOffset+1] = greenGlow;
				pDataSrc3[totalOffset+2] = redGlow;
			}

			// Increment horizontal offset
			horizontalOffset += bppSrc;
		}

		// Increment vertical offset
		verticalOffset += bmpSrc.bmWidthBytes;
	}

	// Create temporary bitmap
	BYTE* pDataTemp = (BYTE *) malloc(sizeof(BYTE)*sizeDst);
	memcpy(pDataTemp, pDataSrc3, sizeDst);
	BYTE* pDataTemp2 = (BYTE *) malloc(sizeof(BYTE)*sizeDst);
	memcpy(pDataTemp2, pDataSrc, sizeDst);

	// Apply blur effect
	int filterSize = 11;
	int filterHalfSize = filterSize >> 1;
	int filterHorizontalOffset = filterHalfSize * bppDst;
	int filterVerticalOffset = filterHalfSize * bmpSrc.bmWidthBytes;
	int filterTotalOffset = filterVerticalOffset + filterHorizontalOffset;
	int filterX, filterY, filterOffset;
	int resultRed, resultGreen, resultBlue;
	int resultRed2, resultGreen2, resultBlue2;
	verticalOffset = 0;
	for (i=filterHalfSize; i<bmpDst.bmHeight-filterHalfSize; i++)
	{
		horizontalOffset = 0;

		for (j=filterHalfSize; j<bmpDst.bmWidth-filterHalfSize; j++)
		{
			// Calculate total offset
			totalOffset = verticalOffset + horizontalOffset;

			if ((i>=filterHalfSize) && (i<bmpDst.bmHeight-filterHalfSize) && (j>=filterHalfSize) && (j<bmpDst.bmWidth-filterHalfSize))
			{
				// Clear result pixel
				resultRed = resultGreen = resultBlue = 0;
				resultRed2 = resultGreen2 = resultBlue2 = 0;

				// Set vertical filter offset
				filterY = verticalOffset;

				// Apply filter
				for (k=-filterHalfSize; k<=filterHalfSize; k++)
				{
					// Set horizontal filter offset
					filterX = horizontalOffset;

					for (l=-filterHalfSize; l<=filterHalfSize; l++)
					{
						// Calculate total filter offset
						filterOffset = filterY + filterX;

						// Calculate result pixel
						resultBlue += pDataSrc3[filterOffset];
						resultGreen += pDataSrc3[filterOffset+1];
						resultRed += pDataSrc3[filterOffset+2];
						resultBlue2 += pDataSrc[filterOffset];
						resultGreen2 += pDataSrc[filterOffset+1];
						resultRed2 += pDataSrc[filterOffset+2];

						// Increment horizontal filter offset
						filterX += bppDst;
					}

					// Increment vertical filter offset
					filterY += bmpDst.bmWidthBytes;
				}

				// Set destination pixel
				pDataTemp[totalOffset+filterTotalOffset] = resultBlue / (filterSize*filterSize);
				pDataTemp[totalOffset+1+filterTotalOffset] = resultGreen / (filterSize*filterSize);
				pDataTemp[totalOffset+2+filterTotalOffset] = resultRed / (filterSize*filterSize);

				pDataTemp2[totalOffset+filterTotalOffset] = resultBlue2 / (filterSize*filterSize);
				pDataTemp2[totalOffset+1+filterTotalOffset] = resultGreen2 / (filterSize*filterSize);
				pDataTemp2[totalOffset+2+filterTotalOffset] = resultRed2 / (filterSize*filterSize);
			}

			// Increment horizontal offset
			horizontalOffset += bppDst;
		}

		// Increment vertical offset
		verticalOffset += bmpDst.bmWidthBytes;
	}

	// Copy glow bitmap to destination bitmap
	verticalOffset = 0;
	double alpha=1.0, alpha_koef;
	double glow_default_intensity = (redGlow + greenGlow + blueGlow) / 3;
	double glow_intenzity, glow_koef;
	for (i=0; i<bmpDst.bmHeight; i++)
	{
		horizontalOffset = 0;

		for (j=0; j<bmpDst.bmWidth; j++)
		{
			// Calculate total offset
			totalOffset = verticalOffset + horizontalOffset;

			// Check for transparent color
			if ((pDataTemp2[totalOffset+2] != redTransparent) || (pDataTemp2[totalOffset+1] != greenTransparent) || (pDataTemp2[totalOffset] != blueTransparent))
			{
				// Calculate glow transparency
				glow_intenzity = (pDataTemp2[totalOffset] + pDataTemp2[totalOffset+1] + pDataTemp2[totalOffset+2]) / 3;
				glow_koef = (glow_intenzity - 255) / (glow_default_intensity - 255);
				if (fabs(glow_koef) > 0.5)
					glow_koef = 0.5 * (fabs(glow_koef) / glow_koef);
				if (glow_default_intensity == 255)
					alpha_koef = 0.0;
				else
					alpha_koef = alpha * glow_koef;

				// Calculate destination pixel
				blue = (BYTE)(alpha_koef*pDataTemp[totalOffset] + (1.0-alpha_koef)*pDataDst[totalOffset]);
				green = (BYTE)(alpha_koef*pDataTemp[totalOffset+1] + (1.0-alpha_koef)*pDataDst[totalOffset+1]);
				red = (BYTE)(alpha_koef*pDataTemp[totalOffset+2] + (1.0-alpha_koef)*pDataDst[totalOffset+2]);

				// Set destination pixel
				pDataSrc3[totalOffset] = blue;
				pDataSrc3[totalOffset+1] = green;
				pDataSrc3[totalOffset+2] = red;
			}
			else
			{
				// Set destination pixel
				pDataSrc3[totalOffset] = pDataDst[totalOffset];
				pDataSrc3[totalOffset+1] = pDataDst[totalOffset+1];
				pDataSrc3[totalOffset+2] = pDataDst[totalOffset+2];
			}

			// Increment horizontal offset
			horizontalOffset += bppDst;
		}

		// Increment vertical offset
		verticalOffset += bmpDst.bmWidthBytes;
	}

	// Set destination bitmap
	SetBitmapBits(m_hGlowBitmap, sizeDst, pDataSrc3);

	// Destroy buffers
	free(pDataOrig);
	free(pDataTemp);
	free(pDataTemp2);
	free(pDataSrc);
	free(pDataSrc2);
	free(pDataSrc3);
	free(pDataDst);

	// Destroy temporary DC and bitmap
	if (hTempDC)
	{
		SelectObject(hTempDC, hOldTempBitmap);
		DeleteDC(hTempDC);
		DeleteObject(hTempBitmap);
	}
	if (hTempDC2)
	{
		SelectObject(hTempDC2, hOldTempBitmap2);
		DeleteDC(hTempDC2);
		DeleteObject(hTempBitmap2);
	}
	if (hTempDC3)
	{
		SelectObject(hTempDC3, hOldTempBitmap3);
		DeleteDC(hTempDC3);
		DeleteObject(hTempBitmap3);
	}

	// Create drawing on glow DC
	CreateDrawing(m_hGlowDC);
}
/////////////////////////////////////////////////////////////////////
//
// Input  :  Source DC
//           Source Bitmap
//           Color Delta
//
// Output :  None
// return :  None
//
/////////////////////////////////////////////////////////////////////
//
// History : 05/2002 Jean-Rémi PINNA (jrpinna@noos.fr) Wrote it.
//
/////////////////////////////////////////////////////////////////////


void blurBitmap( HDC hSrcDc, HBITMAP hSrcBmp, BYTE bDeltaMax )
{

	BITMAPINFO  sBmpInfo;
	BYTE *      pBmpBytes;

	memset( &sBmpInfo, 0, sizeof(BITMAPINFO) );

	BITMAP SrcBmp;
	GetObject(hSrcBmp, sizeof(BITMAP), &SrcBmp);

	if( (SrcBmp.bmBitsPixel != 24) && (SrcBmp.bmBitsPixel != 32) )
    return;

  sBmpInfo.bmiHeader.biSize         = sizeof(BITMAPINFOHEADER);
  sBmpInfo.bmiHeader.biWidth        = SrcBmp.bmWidth;
  sBmpInfo.bmiHeader.biHeight       = SrcBmp.bmHeight;
  sBmpInfo.bmiHeader.biBitCount     = SrcBmp.bmBitsPixel;
  sBmpInfo.bmiHeader.biPlanes       = 1;
  sBmpInfo.bmiHeader.biCompression  = BI_RGB;
  sBmpInfo.bmiHeader.biSizeImage    =
        SrcBmp.bmBitsPixel*SrcBmp.bmWidth*SrcBmp.bmHeight/8;

  pBmpBytes = malloc(sBmpInfo.bmiHeader.biSizeImage);//new BYTE[sBmpInfo.bmiHeader.biSizeImage];

  // Fill the buffer with a copy of the bitmap's bits

  GetDIBits(hSrcDc, hSrcBmp, 0, sBmpInfo.bmiHeader.biHeight, pBmpBytes, &sBmpInfo, DIB_RGB_COLORS );


  DWORD dwLineWidth, dwByteCount, dwBytesPixel, dwImageSize;

  dwLineWidth   = SrcBmp.bmBitsPixel*SrcBmp.bmWidth/8;
  dwBytesPixel  = SrcBmp.bmBitsPixel/8;
  dwImageSize   = sBmpInfo.bmiHeader.biSizeImage;
  dwByteCount   = dwBytesPixel;
  // Start to second pixel to avoid bug on first pixel
  // (Previous bottom point coordinate is 1276)

  // Blending Matrix ---- This matrix correspond to the Blur More
  //                         blending method
  //                      The middle pixel value is the average
  //                        of all environnent pixels
  //                      Change value of an element modify his
  //                        weight in the computation of the average
  //
  // BYTE bBlurMoreBlendingMatrix[9] = { 1, 1, 1,
  //          1, 1, 1,
  //          1, 1, 1 };
  //
  // BYTE bHalfBlendingMatrix[9] = { 0, 1, 0,
  //          1, 1, 1,
  //          0, 1, 0 };


  // Blending Matrix ---- This matrix correspond to the Blur
  //                         blending method
  //                      The sum of the elements of the array
  //                         must not exceed 256
  //

  BYTE bBlurBlendingMatrix[9] = { 1, 2, 1,
          2, 4, 2,
          1, 2, 1 };

  // This matrix contains the values needed to point on
  // corresponding bit
  //

  long nPosRange[9] = { - (long)(dwLineWidth-dwBytesPixel) ,
                        - (long)(dwLineWidth) ,
                        - (long)(dwLineWidth+dwBytesPixel),
                        - (long)(dwBytesPixel) ,
                        0 ,
                        +dwBytesPixel,
                        + (dwLineWidth-dwBytesPixel) ,
                        + (dwLineWidth) ,
                        + (dwLineWidth+dwBytesPixel) };


  // Sum Blending Matrix Elements
  //

  BYTE bMatrixSum = 0;

  for( int iSum = 0 ; iSum<9 ; iSum++ )
    bMatrixSum += bBlurBlendingMatrix[iSum];

  while( dwByteCount < dwImageSize )
  {
    DWORD bDelta, dwAverage;

    // Create array of pixel's positions
    //
    long nPosNewRange[9]= { (long)(dwByteCount+nPosRange[0]),
          (long)(dwByteCount+nPosRange[1]),
          (long)(dwByteCount+nPosRange[2]),
          (long)(dwByteCount+nPosRange[3]),
          (long)(dwByteCount+nPosRange[4]),
          (long)(dwByteCount+nPosRange[5]),
          (long)(dwByteCount+nPosRange[6]),
          (long)(dwByteCount+nPosRange[7]),
          (long)(dwByteCount+nPosRange[8]) };

    // Check point validity : ensure points are in the bit array
    //
    for( int iPos = 0 ; iPos < 9 ; iPos++ )
    {
      nPosNewRange[iPos] = nPosNewRange[iPos] < 0 ? 0 :
              nPosNewRange[iPos] >
              (long)dwImageSize ? dwByteCount : nPosNewRange[iPos];
    }

    // Compute method on each RGB color of the pixel
    //
    for( int iColor = 0 ; iColor < 3 ; iColor++ )
    {
      dwAverage = 0;

      // Compute Color average
      //
      BYTE bColorValue[9] = { pBmpBytes[nPosNewRange[0]+iColor],
                              pBmpBytes[nPosNewRange[1]+iColor],
                              pBmpBytes[nPosNewRange[2]+iColor],
                              pBmpBytes[nPosNewRange[3]+iColor],
                              pBmpBytes[nPosNewRange[4]+iColor],
                              pBmpBytes[nPosNewRange[5]+iColor],
                              pBmpBytes[nPosNewRange[6]+iColor],
                              pBmpBytes[nPosNewRange[7]+iColor],
                              pBmpBytes[nPosNewRange[8]+iColor] };

      // Apply Blendind Matrix to compute color average
      //
      for( int iPos = 0 ; iPos < 9 ; iPos++ )
        dwAverage += bColorValue[iPos]*bBlurBlendingMatrix[iPos];

      BYTE bAverage = (BYTE)(dwAverage/bMatrixSum);

      bDelta = abs( pBmpBytes[dwByteCount]-bAverage );
      pBmpBytes[dwByteCount+iColor] =
           bDelta > bDeltaMax ? bAverage :
                                  pBmpBytes[dwByteCount+iColor];
    }

    // Move to next pixel
    //
    dwByteCount+=dwBytesPixel;
  }


  SetDIBits(hSrcDc, hSrcBmp, 0, sBmpInfo.bmiHeader.biHeight, pBmpBytes, &sBmpInfo, DIB_RGB_COLORS );

  free(pBmpBytes);
}

HFONT CreateAngledFont(int fontHeight, int fontAngle, LPCTSTR lpfontFace,LOGFONT * ftAttr)
{
	////////////////////////////////////////////////////////////////////////////
	// Prepare logical font                                                   //
	////////////////////////////////////////////////////////////////////////////
	if(!ftAttr)
	{
		LOGFONT lf;
		ZeroMemory(&lf, sizeof(lf));

		lf.lfHeight 		= fontHeight;
		lf.lfEscapement		= ( 10 * fontAngle );	// 90' ccw
		lf.lfOrientation	= ( 10 * fontAngle );	// 90' ccw
		lf.lfWeight 		= FW_NORMAL;			// 600
		lf.lfCharSet 		= ANSI_CHARSET;
		lf.lfQuality 		= PROOF_QUALITY|ANTIALIASED_QUALITY;

		lstrcpy(lf.lfFaceName, lpfontFace);

		////////////////////////////////////////////////////////////////////////////
		// instance logical font                                                  //
		////////////////////////////////////////////////////////////////////////////
		return CreateFontIndirect(&lf);
	}
	else
	{
		return CreateFontIndirect(ftAttr);
	}
}
// END SOURCE
