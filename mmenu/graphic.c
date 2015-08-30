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

#define SafeRelease(pUnk) if (pUnk) { pUnk->Release(); pUnk = NULL; }

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
	GRADIENT pfnGradientFill = NULL;
	pfnGradientFill = (GRADIENT)GetProcAddress(hLibrary, "dGradientFill");
	
	if (!pfnGradientFill)
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


HFONT CreateAngledFont(int fontHeight, int fontAngle, LPCWSTR lpfontFace, LOGFONT * ftAttr)
{
	////////////////////////////////////////////////////////////////////////////
	// Prepare logical font                                                   //
	////////////////////////////////////////////////////////////////////////////
	if (!ftAttr)
	{
		LOGFONT lf;
		ZeroMemory(&lf, sizeof(lf));

		lf.lfHeight = fontHeight;
		lf.lfEscapement = (10 * fontAngle);		// 90' ccw
		lf.lfOrientation = (10 * fontAngle);	// 90' ccw
		lf.lfWeight = FW_NORMAL;				// 600
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

HFONT Graphics::CreateAngledFont(LONG fontSize, LONG fontAngle, LPWSTR pFontFamily) {
	
	HDC hDC = GetDC(NULL);
	int DPI = 72;
	int lfHeight = -MulDiv(fontSize, GetDeviceCaps(hDC, LOGPIXELSY), DPI);
	LOGFONT lf;
	ZeroMemory(&lf, sizeof(LOGFONT));

	lf.lfHeight = lfHeight;
	lf.lfEscapement = (10 * fontAngle);		// 90' ccw
	lf.lfOrientation = (10 * fontAngle);	// 90' ccw
	lf.lfWeight = FW_NORMAL;				// 600
	lf.lfCharSet = ANSI_CHARSET;
	lf.lfQuality = PROOF_QUALITY | ANTIALIASED_QUALITY;

	lstrcpy(lf.lfFaceName, pFontFamily);

	////////////////////////////////////////////////////////////////////////////
	// instance logical font                                                  //
	////////////////////////////////////////////////////////////////////////////
	return CreateFontIndirect(&lf);
}

// END SOURCE
