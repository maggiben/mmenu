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
#include <wincodec.h>		// Windows Imaging Component
#include "graphic.h"		// Graphic Functions draw masks, bitmaps etc..
#include "FreeImage.h"      // FreeImage protos & delares
#include "VistaIcons.h"

#pragma comment( lib, "windowscodecs" )

//#define SafeRelease(pUnk) if (pUnk) { pUnk->lpVtbl->Release(pUnk); pUnk = NULL; }
#define SafeRelease(pUnk) if (pUnk) { pUnk->Release(); pUnk = NULL; }

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

HBITMAP ScaleImage(HBITMAP hBitmap, LONG width, LONG height)
{
	HRESULT hr;
	IWICImagingFactory *pFactory = NULL;
	IWICBitmapScaler *pIScaler = NULL;
	IWICBitmap *outputBitmap = NULL;
	HBITMAP hbmp = NULL;

	hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER | CLSCTX_LOCAL_SERVER, IID_IWICImagingFactory, (LPVOID*)&pFactory);
	if (SUCCEEDED(hr))
	{
		if (SUCCEEDED(hr)) hr = pFactory->CreateBitmapFromHBITMAP(hBitmap, NULL, WICBitmapUsePremultipliedAlpha, &outputBitmap);

		BITMAPINFO bmi = { 0 };
		bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bmi.bmiHeader.biWidth = width;
		bmi.bmiHeader.biHeight = -(height);
		bmi.bmiHeader.biPlanes = 1;
		bmi.bmiHeader.biBitCount = 32;
		bmi.bmiHeader.biCompression = BI_RGB;

		BYTE *pBits;
		hbmp = CreateDIBSection(NULL, &bmi, DIB_RGB_COLORS, (void**)&pBits, NULL, 0);
		if (SUCCEEDED(hr)) hr = hbmp ? S_OK : E_OUTOFMEMORY;

		if (SUCCEEDED(hr)) hr = pFactory->CreateBitmapScaler( &pIScaler);
		if (SUCCEEDED(hr)) hr = pIScaler->Initialize(outputBitmap, width, height, WICBitmapInterpolationModeFant);
		WICRect rect = { 0, 0, width, height };
		if (SUCCEEDED(hr)) hr = pIScaler->CopyPixels(&rect, width * 4, width * height * 4, pBits);

		if (SUCCEEDED(hr))
			hr = S_OK;
		else
			DeleteObject(hbmp);

		pIScaler->Release();
		outputBitmap->Release();
		pFactory->Release();
	}
	return hbmp;
}

SIZE ScaleBox(SIZE * origin, LONG width, LONG height) {
	DOUBLE ratio = (DOUBLE)origin->cx / (DOUBLE)origin->cy;
	SIZE result;
	if (origin->cx > origin->cy) {
		result.cy = width / (LONG)ratio;
		result.cx = width;
	}
	else {
		result.cx = height * (LONG)ratio;
		result.cy = height;
	}
	return result;
}

HBITMAP	LoadImageSmart(LPWSTR imagePath)
{
	HBITMAP tmpBMP = NULL;

	HDC hDCDesktop = GetDC(0);
	FREE_IMAGE_FORMAT format = FreeImage_GetFileTypeU(imagePath, 0);
	
	switch (format)
	{
		case FIF_BMP:
		{
			
			HBITMAP hBitmap = static_cast<HBITMAP>(LoadImage(NULL, imagePath, IMAGE_BITMAP, 0, 0, LR_LOADTRANSPARENT | LR_LOADFROMFILE));
			return hBitmap;
			break;
		}
		case FIF_ICO:
		{
			HICON hIcon = (HICON)LoadImage(NULL, imagePath, IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
			HBITMAP hBitmap = IconToBitmapPARGB32(hIcon);
			DeleteObject(hIcon);
			return hBitmap;
			break;
		}
		case FIF_EXR:
		{
			HBITMAP hBmp = NULL;
			IWICImagingFactory *pFactory = NULL;
			IWICBitmapDecoder *pDecoder = NULL;
			HRESULT hr;
			hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER | CLSCTX_LOCAL_SERVER, IID_IWICImagingFactory, (LPVOID*)&pFactory);
			if (SUCCEEDED(hr))
			{
				hr = pFactory->CreateDecoderFromFilename(imagePath, NULL, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &pDecoder);
				if (SUCCEEDED(hr)) {
					GUID guidFormat;
					hr = pDecoder->GetContainerFormat(&guidFormat);
					/*if (SUCCEEDED(hr)) {
						if (IsEqualGUID(&guidFormat, &GUID_ContainerFormatPng)) {
							MessageBox(NULL, L"Is PNG", L"Error !", MB_ICONWARNING);
						}
					}*/
					IWICBitmapFrameDecode *pFrame = NULL;
					if (SUCCEEDED(hr)) hr = pDecoder->GetFrame(0, &pFrame);
					UINT width;
					UINT height;
					if (SUCCEEDED(hr)) hr = pFrame->GetSize(&width, &height);

					IWICFormatConverter *pConverter = NULL;
					if (SUCCEEDED(hr)) hr = pFactory->CreateFormatConverter(&pConverter);
					if (SUCCEEDED(hr)) hr = pConverter->Initialize((IWICBitmapSource *)pFrame, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.0, WICBitmapPaletteTypeCustom);

					BITMAPINFO bmi = { 0 };
					bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
					bmi.bmiHeader.biWidth = width;
					bmi.bmiHeader.biHeight = -static_cast<int>(height);
					bmi.bmiHeader.biPlanes = 1;
					bmi.bmiHeader.biBitCount = 32;
					bmi.bmiHeader.biCompression = BI_RGB;

					LPVOID pvPixels = NULL;
					HDC hDC = GetDC(NULL);
					hBmp = CreateDIBSection(hDC, &bmi, DIB_RGB_COLORS, &pvPixels, NULL, 0);
					ReleaseDC(NULL, hDC);

					UINT nStride = width * 4;
					UINT nSize = nStride * height;

					if (SUCCEEDED(hr)) hr = pConverter->CopyPixels(NULL, nStride, nSize, (LPBYTE)pvPixels);

					SafeRelease(pConverter);
					SafeRelease(pFrame);
					SafeRelease(pDecoder);
					SafeRelease(pFactory);
				}
			}
			HBITMAP hFinalBmp = ScaleImage(hBmp, GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CXSMICON));
			DeleteObject(hBmp);
			return hFinalBmp;
		}
		case FIF_PNG:
		{
			FIBITMAP *image = FreeImage_LoadU(format, imagePath, PNG_DEFAULT);
			if (!image)
			{
				// Could not load image
				return NULL;
			}

			image = FreeImage_Rescale(image, 16, 16, FILTER_CATMULLROM);

			int width = FreeImage_GetWidth(image);
			int height = FreeImage_GetHeight(image);
			BOOL bHasBackground = FreeImage_HasBackgroundColor(image);
			BOOL bIsTransparent = FreeImage_IsTransparent(image);
			
			BITMAPINFO  bmi;
			ZeroMemory(&bmi, sizeof(BITMAPINFO));

			bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
			bmi.bmiHeader.biPlanes = 1;
			bmi.bmiHeader.biBitCount = 32;
			bmi.bmiHeader.biCompression = BI_RGB;

			bmi.bmiHeader.biWidth = GetSystemMetrics(SM_CXSMICON);
			bmi.bmiHeader.biHeight = GetSystemMetrics(SM_CXSMICON);

			BYTE *pBits;
			HBITMAP hBitmap = CreateDIBSection(NULL, &bmi, DIB_RGB_COLORS, (void**)&pBits, NULL, 0);

			FreeImage_PreMultiplyWithAlpha(image);
			
			CopyMemory(pBits, FreeImage_GetBits(image), FreeImage_GetPitch(image) * FreeImage_GetHeight(image));
			FreeImage_Unload(image);
			ReleaseDC(NULL, hDCDesktop);
			return hBitmap;
			break;
		}
	}
	return NULL;
}
///////////////////////////////////////////////////////////////////////////////////
// This draws a bitmap in a solid backgound color of the same                    //
// size than the icon and returns an HBITMAP                                     //
///////////////////////////////////////////////////////////////////////////////////
void drawIconToBitmap(HBITMAP bmSrc, LPWSTR FileName, int size, COLORREF compositeColor)
{
	SHFILEINFO sfi;
	HDC        hMemDC;
	HBITMAP    hBitmap, hbmOld, bufferh;
	HDC        hDCDesktop;
	HBRUSH     hOldBrush;
	RECT       rc_a;

	rc_a.top = 0;
	rc_a.left = 0;
	rc_a.bottom = size;
	rc_a.right = size;

#define BASIC_SHGFI_FLAGS | SHGFI_TYPENAME | SHGFI_SHELLICONSIZE | SHGFI_SYSICONINDEX | SHGFI_DISPLAYNAME | SHGFI_EXETYPE
	hDCDesktop = GetDC(GetDesktopWindow());

	///////////////////////////////////////////////////////////////////////////////////
	// Create bitmap                                                                 //
	///////////////////////////////////////////////////////////////////////////////////
	hBitmap = bmSrc; //CreateCompatibleBitmap( hDCDesktop, 96, iNumFiles * 32 );

	ReleaseDC(GetDesktopWindow(), hDCDesktop);
	hMemDC = CreateCompatibleDC(NULL);				// Create screen DC

	bufferh = CreateCompatibleBitmap(hMemDC, size, size);
	hbmOld = (HBITMAP)SelectObject(hMemDC, hBitmap);			// Select memory bitmap


	///////////////////////////////////////////////////////////////////////////////////
	// Select brush for deleting the memory bitmap                                   //
	///////////////////////////////////////////////////////////////////////////////////
	hOldBrush = CreateSolidBrush(compositeColor);
	///////////////////////////////////////////////////////////////////////////////////
	// Delete memory bitmap                                                          //
	///////////////////////////////////////////////////////////////////////////////////
	FillRect(hMemDC, &rc_a, hOldBrush);
	//Rectangle( hMemDC, 0, 0, 32,32 );
	SelectObject(hMemDC, hOldBrush);


	if (size == 16)
	{
		//Small icon
		SHGetFileInfo(
			FileName,
			0,
			&sfi,
			sizeof(sfi),
			SHGFI_ICON | SHGFI_SMALLICON
			);

		DrawIconEx(hMemDC, 0, 0, sfi.hIcon, size, size, 0, NULL, DI_NORMAL);

		ICONINFO icInfo;
		if (GetIconInfo(sfi.hIcon, &icInfo))
		{
			BITMAPINFO bmpInfo = { 0 };
			HDC hdc = GetDC(NULL);
			if (GetDIBits(hdc, icInfo.hbmColor, 0, 0, NULL, &bmpInfo, DIB_RGB_COLORS) != 0)
			{

			}

		}

		// Destroy icon
		DestroyIcon(sfi.hIcon);
	}
	if (size == 32)
	{
		//Large icon
		SHGetFileInfo(
			FileName,
			0,
			&sfi,
			sizeof(sfi),
			SHGFI_ICON | SHGFI_LARGEICON
			);
		// SHGFI_LARGEICON = 0
		DrawIconEx(hMemDC, 0, 0, sfi.hIcon, size, size, 0, NULL, DI_NORMAL);
		// Destroy icon
		DestroyIcon(sfi.hIcon);
	}
	SelectObject(hMemDC, hbmOld);        // Old bitmap back to memory DC
	DeleteDC(hMemDC);                // Destroy memory DC
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
	hdcMem = CreateCompatibleDC(0);
	hdcMem2 = CreateCompatibleDC(0);
	hdcMem3 = CreateCompatibleDC(0);

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
	HINSTANCE hLibrary = LoadLibrary(L"msimg32.dll");
	if (NULL == hLibrary)
	{
		goto NoApi;
	}

	typedef BOOL(CALLBACK *GRADIENT)(HDC, PTRIVERTEX, ULONG, PVOID, ULONG, ULONG);
	GRADIENT pfnGradientFill;
	pfnGradientFill = (GRADIENT)GetProcAddress(hLibrary, "dGradientFill");

	if (NULL == pfnGradientFill)
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
	vert[0].x = objBox.left;	// Put it at X
	vert[0].y = objBox.top;	// Put it at Y
	vert[0].Red = MAKEWORD(0, GetRValue(rgb0));
	vert[0].Green = MAKEWORD(0, GetGValue(rgb0));
	vert[0].Blue = MAKEWORD(0, GetBValue(rgb0));
	vert[0].Alpha = 0x0000;

	// ending
	vert[1].x = objBox.right;
	vert[1].y = objBox.bottom;
	vert[1].Red = MAKEWORD(0, GetRValue(rgb1));
	vert[1].Green = MAKEWORD(0, GetGValue(rgb1));
	vert[1].Blue = MAKEWORD(0, GetBValue(rgb1));
	vert[1].Alpha = 0x0000;

	GRADIENT_RECT gRect;
	gRect.UpperLeft = 0;
	gRect.LowerRight = 1;

	result = pfnGradientFill(hdc, vert, 2, &gRect, 1, GRADIENT_FILL_RECT_H);
	result = FreeLibrary(hLibrary);

	return result;
NoApi:
	///////////////////////////////////////////
	// Do the gradient without using any api //
	///////////////////////////////////////////
	for (int i = objBox.top; i < objBox.bottom; i++)
	{
		gradient = InterpolateLinear(fromClr, toClr, i, objBox.top, objBox.bottom);
		grdPen = CreatePen(PS_SOLID, 1, gradient);
		oldPen = (HPEN)SelectObject(hdc, grdPen);

		MoveToEx(hdc, objBox.left, i, NULL);
		LineTo(hdc, objBox.right, i);

		SelectObject(hdc, oldPen);
		DeleteObject(grdPen);
	}

	return TRUE;
}

HBITMAP skinedBox(HBITMAP leftHbm, HBITMAP middleHbm, HBITMAP rightHbm, int bWidth, int bHeight)
{
	BITMAP	bmpLeft;
	BITMAP	bmpMiddle;
	BITMAP	bmpRight;
	HBITMAP oldBmp = NULL;
	HDC		drawSrfc = CreateCompatibleDC(NULL);		// Desktop Compatible DC
	HDC		hdcMem = CreateCompatibleDC(NULL);		// Desktop Compatible DC
	HBITMAP	hbmp = CreateCompatibleBitmap(drawSrfc, bWidth, bHeight);
	int		offset = 0;


	GetObject(leftHbm, sizeof(BITMAP), &bmpLeft);
	GetObject(middleHbm, sizeof(BITMAP), &bmpMiddle);
	GetObject(rightHbm, sizeof(BITMAP), &bmpRight);
	oldBmp = (HBITMAP)SelectObject(hdcMem, leftHbm);
	BitBlt(drawSrfc, 0, 0, bmpLeft.bmWidth, bmpLeft.bmHeight, hdcMem, 0, 0, SRCCOPY);
	SelectObject(hdcMem, oldBmp);
	offset += bmpLeft.bmWidth;


	oldBmp = (HBITMAP)SelectObject(hdcMem, middleHbm);
	for (int x = offset; x < (bWidth - bmpLeft.bmWidth); x++)
	{
		BitBlt(drawSrfc, x, 0, bmpMiddle.bmWidth, bmpMiddle.bmHeight, hdcMem, 0, 0, SRCCOPY);
		offset++;
	}
	SelectObject(hdcMem, oldBmp);

	oldBmp = (HBITMAP)SelectObject(hdcMem, rightHbm);
	BitBlt(drawSrfc, offset, 0, bmpRight.bmWidth, bmpRight.bmHeight, hdcMem, 0, 0, SRCCOPY);

	SelectObject(hdcMem, oldBmp);

	oldBmp = (HBITMAP)SelectObject(hdcMem, hbmp);
	BitBlt(drawSrfc, offset, 0, bWidth, bHeight, hdcMem, 0, 0, SRCCOPY);
	SelectObject(hdcMem, oldBmp);


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
COLORREF InterpolateLinear(COLORREF first, COLORREF second, long position, long start, long end)
{
	if (start == end) return first;
	if (end - start == 0) return second;
	if (position == start) return first;
	if (position == end) return second;
	return RGB
		(
		(BYTE)((GetRValue(second)*(position - start) + GetRValue(first)*(end - position)) / (end - start)),
		(BYTE)((GetGValue(second)*(position - start) + GetGValue(first)*(end - position)) / (end - start)),
		(BYTE)((GetBValue(second)*(position - start) + GetBValue(first)*(end - position)) / (end - start))
		);
}

void wallpaper(HDC hdc, RECT *lprc, int idbitmap)
{
	HDC shdc = NULL;
	BITMAP bmp;
	HBITMAP hbmp = NULL;

	long bmx, bmy;

	if (idbitmap >0)
	{
		HINSTANCE hinst = GetModuleHandle(NULL);
		hbmp = (HBITMAP)LoadImage(GetModuleHandle(NULL), L"rayado.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE); //LoadBitmap(hinst, MAKEINTRESOURCE(idbitmap));
		if (hbmp == NULL)
		{
			MessageBoxW(NULL, L"LoadBitmap()", L"Error", MB_ICONINFORMATION);
		}
		shdc = CreateCompatibleDC(hdc);
		SelectObject(shdc, hbmp);
		GetObject(hbmp, sizeof(BITMAP), &bmp);
		bmx = bmp.bmWidth;
		bmy = bmp.bmHeight;

		long r = lprc->right;
		long l = lprc->left;
		long b = lprc->bottom;
		long t = lprc->top;

		long x, y;
		for (x = l; x<r; x += bmx)
		{
			for (y = t; y<b; y += bmy)
			{
				BitBlt(hdc, x, y, r - x, b - y, shdc, 0, 0, SRCCOPY);
			}
		}

		DeleteObject(hbmp);
		DeleteDC(shdc);
	}
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


void blurBitmap(HDC hSrcDc, HBITMAP hSrcBmp, BYTE bDeltaMax)
{

	BITMAPINFO  sBmpInfo;
	BYTE *      pBmpBytes;

	memset(&sBmpInfo, 0, sizeof(BITMAPINFO));

	BITMAP SrcBmp;
	GetObject(hSrcBmp, sizeof(BITMAP), &SrcBmp);

	if ((SrcBmp.bmBitsPixel != 24) && (SrcBmp.bmBitsPixel != 32))
		return;

	sBmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	sBmpInfo.bmiHeader.biWidth = SrcBmp.bmWidth;
	sBmpInfo.bmiHeader.biHeight = SrcBmp.bmHeight;
	sBmpInfo.bmiHeader.biBitCount = SrcBmp.bmBitsPixel;
	sBmpInfo.bmiHeader.biPlanes = 1;
	sBmpInfo.bmiHeader.biCompression = BI_RGB;
	sBmpInfo.bmiHeader.biSizeImage =
		SrcBmp.bmBitsPixel*SrcBmp.bmWidth*SrcBmp.bmHeight / 8;

	pBmpBytes = (BYTE *)malloc(sBmpInfo.bmiHeader.biSizeImage);//new BYTE[sBmpInfo.bmiHeader.biSizeImage];

	// Fill the buffer with a copy of the bitmap's bits

	GetDIBits(hSrcDc, hSrcBmp, 0, sBmpInfo.bmiHeader.biHeight, pBmpBytes, &sBmpInfo, DIB_RGB_COLORS);


	DWORD dwLineWidth, dwByteCount, dwBytesPixel, dwImageSize;

	dwLineWidth = SrcBmp.bmBitsPixel*SrcBmp.bmWidth / 8;
	dwBytesPixel = SrcBmp.bmBitsPixel / 8;
	dwImageSize = sBmpInfo.bmiHeader.biSizeImage;
	dwByteCount = dwBytesPixel;
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

	long nPosRange[9] = { -(long)(dwLineWidth - dwBytesPixel),
		-(long)(dwLineWidth),
		-(long)(dwLineWidth + dwBytesPixel),
		-(long)(dwBytesPixel),
		0,
		+dwBytesPixel,
		+(dwLineWidth - dwBytesPixel),
		+(dwLineWidth),
		+(dwLineWidth + dwBytesPixel) };


	// Sum Blending Matrix Elements
	//

	BYTE bMatrixSum = 0;

	for (int iSum = 0; iSum<9; iSum++)
		bMatrixSum += bBlurBlendingMatrix[iSum];

	while (dwByteCount < dwImageSize)
	{
		DWORD bDelta, dwAverage;

		// Create array of pixel's positions
		//
		long nPosNewRange[9] = { (long)(dwByteCount + nPosRange[0]),
			(long)(dwByteCount + nPosRange[1]),
			(long)(dwByteCount + nPosRange[2]),
			(long)(dwByteCount + nPosRange[3]),
			(long)(dwByteCount + nPosRange[4]),
			(long)(dwByteCount + nPosRange[5]),
			(long)(dwByteCount + nPosRange[6]),
			(long)(dwByteCount + nPosRange[7]),
			(long)(dwByteCount + nPosRange[8]) };

		// Check point validity : ensure points are in the bit array
		//
		for (int iPos = 0; iPos < 9; iPos++)
		{
			nPosNewRange[iPos] = nPosNewRange[iPos] < 0 ? 0 :
				nPosNewRange[iPos] >
				(long)dwImageSize ? dwByteCount : nPosNewRange[iPos];
		}

		// Compute method on each RGB color of the pixel
		//
		for (int iColor = 0; iColor < 3; iColor++)
		{
			dwAverage = 0;

			// Compute Color average
			//
			BYTE bColorValue[9] = { pBmpBytes[nPosNewRange[0] + iColor],
				pBmpBytes[nPosNewRange[1] + iColor],
				pBmpBytes[nPosNewRange[2] + iColor],
				pBmpBytes[nPosNewRange[3] + iColor],
				pBmpBytes[nPosNewRange[4] + iColor],
				pBmpBytes[nPosNewRange[5] + iColor],
				pBmpBytes[nPosNewRange[6] + iColor],
				pBmpBytes[nPosNewRange[7] + iColor],
				pBmpBytes[nPosNewRange[8] + iColor] };

			// Apply Blendind Matrix to compute color average
			//
			for (int iPos = 0; iPos < 9; iPos++)
				dwAverage += bColorValue[iPos] * bBlurBlendingMatrix[iPos];

			BYTE bAverage = (BYTE)(dwAverage / bMatrixSum);

			bDelta = abs(pBmpBytes[dwByteCount] - bAverage);
			pBmpBytes[dwByteCount + iColor] =
				bDelta > bDeltaMax ? bAverage :
				pBmpBytes[dwByteCount + iColor];
		}

		// Move to next pixel
		//
		dwByteCount += dwBytesPixel;
	}


	SetDIBits(hSrcDc, hSrcBmp, 0, sBmpInfo.bmiHeader.biHeight, pBmpBytes, &sBmpInfo, DIB_RGB_COLORS);

	free(pBmpBytes);
}

HFONT CreateAngledFont(int fontHeight, int fontAngle, LPCTSTR lpfontFace, LOGFONT * ftAttr)
{
	////////////////////////////////////////////////////////////////////////////
	// Prepare logical font                                                   //
	////////////////////////////////////////////////////////////////////////////
	if (!ftAttr)
	{
		LOGFONT lf;
		ZeroMemory(&lf, sizeof(lf));

		lf.lfHeight = fontHeight;
		lf.lfEscapement = (10 * fontAngle);	// 90' ccw
		lf.lfOrientation = (10 * fontAngle);	// 90' ccw
		lf.lfWeight = FW_NORMAL;			// 600
		lf.lfCharSet = ANSI_CHARSET;
		lf.lfQuality = PROOF_QUALITY | ANTIALIASED_QUALITY;

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
