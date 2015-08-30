///////////////////////////////////////////////////////////////////////////////////
// Título:      		graphic.h                                                //
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
// either version 2 of the License, or (at your option) any                      //
// later version.                                                                //
//                                                                               //
// This program is distributed in the hope that it will be useful,               //
// but WITHOUT ANY WARRANTY; without even the implied warranty of                //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                 //
// GNU General Public License for more details.                                  //
//                                                                               //
///////////////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __GRAPHIC_H__
#define __GRAPHIC_H__

HFONT		CreateAngledFont(int fontHeight, int fontAngle, LPCWSTR lpfontFace, LOGFONT * ftAttr);

void		drawIconToBitmap(HBITMAP bmSrc, LPWSTR FileName, int size, COLORREF compositeColor);
HBITMAP		CreateBitmapMask(HBITMAP, COLORREF);
void		DrawTransparentBitmap(HDC, HBITMAP, short, short, COLORREF);
HBITMAP		compositeImage(HBITMAP fgImage, HBITMAP bgImage);
// Gradient Generator
BOOL		gradientFill(HDC hdc, RECT objBox, COLORREF fromClr, COLORREF toClr);
HBITMAP		skinedBox(HBITMAP leftHbm, HBITMAP middleHbm, HBITMAP rightHbm, int bWidth, int bHeight);
COLORREF	InterpolateLinear(COLORREF first, COLORREF second, long position, long start, long end);
// Others
void		wallpaper(HDC hdc, RECT *lprc, int idbitmap);
void		doGlow(HDC hDC);
void		blurBitmap(HDC hSrcDc, HBITMAP hSrcBmp, BYTE bDeltaMax);

// Image Loading & Manipulation
HBITMAP		LoadImageSmart(LPWSTR imagePath);
HBITMAP		ScaleImage(HBITMAP hBitmap, LONG width, LONG height);

class Graphics
{
private:
public:
	HFONT CreateAngledFont(LONG fontSize, LONG fontAngle, LPWSTR pFontFamily);
	Graphics() {};
	~Graphics() {};
};

#endif
