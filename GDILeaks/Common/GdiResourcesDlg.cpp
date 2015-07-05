#include "stdafx.h"
#include "GdiResourcesDlg.h"

#ifdef _DEBUG
   #define new DEBUG_NEW
   #undef THIS_FILE
   static char THIS_FILE[] = __FILE__;
#endif



BEGIN_MESSAGE_MAP(CGdiResourcesDlg, CDialog)
//{{AFX_MSG_MAP(CGdiResourcesDlg)
   ON_LBN_SELCHANGE(IDC_ITEM_LIST, OnSelChangeItemList)
   ON_WM_SIZE()
   ON_WM_GETMINMAXINFO()
   ON_WM_PAINT()
   ON_WM_NCCREATE()
	ON_LBN_DBLCLK(IDC_ITEM_LIST, OnDblclkItemList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



CGdiResourcesDlg::CGdiResourcesDlg(CWnd* pParent, CGdiResources* pResources)
   : CDialog(CGdiResourcesDlg::IDD, pParent)
{
// init members
   m_pResources = pResources;
   ASSERT(m_pResources != NULL);
   m_pNotifications = NULL;

// set default dialog dimension
   m_DlgRect.left    = 0;
   m_DlgRect.right   = 400;
   m_DlgRect.top     = 0;
   m_DlgRect.bottom  = 200;

// MFC stuff
//
//{{AFX_DATA_INIT(CGdiResourcesDlg)
   // NOTE: the ClassWizard will add member initialization here
//}}AFX_DATA_INIT
}


void CGdiResourcesDlg::SetNotificationCallBack(INotificationCallBack* pNotifications)
{
   m_pNotifications = pNotifications;
}


void CGdiResourcesDlg::DoDataExchange(CDataExchange* pDX)
{
   CDialog::DoDataExchange(pDX);

//{{AFX_DATA_MAP(CGdiResourcesDlg)
   // NOTE: the ClassWizard will add DDX and DDV calls here
//}}AFX_DATA_MAP
}


BOOL CGdiResourcesDlg::OnInitDialog() 
{
// default MFC processing
   CDialog::OnInitDialog();
   
// fill the listbox with resources 
   ASSERT(m_pResources != NULL);
   if (m_pResources != NULL)
   {
      POSITION Pos;
      CGdiObj* pGdiObject = NULL;
      CGdiObj* pObj = NULL;
      CString  szText;
      CString  szInfo;
      int      iReturn;
      for (
            pGdiObject = m_pResources->GetFirst(Pos); 
            (pGdiObject != NULL); 
            pGdiObject = m_pResources->GetNext(Pos)
          )
      {
      // add each GDI object into the list
         if (pGdiObject != NULL)
         {
            pGdiObject->GetTypeString(szInfo);
            szText.Format(
               "%s%s\t%#04lx",
               (LPCSTR)szInfo, 
               (IsStockObject(pGdiObject->GetHandle())) ? _T("*") : _T(""),
               pGdiObject->GetHandle()
               );

            iReturn = 
               SendDlgItemMessage(
                  IDC_ITEM_LIST, 
                  LB_ADDSTRING, 
                  0, 
                  (LPARAM)(LPCSTR)szText
                  );
            if (iReturn != LB_ERR)
               SendDlgItemMessage(IDC_ITEM_LIST, LB_SETITEMDATA, (WPARAM)iReturn, (LPARAM)pGdiObject);
         }
      }      
   }

// resize the list
   CRect WindowRect;
   GetClientRect(WindowRect);
   ::SetWindowPos(::GetDlgItem(m_hWnd, IDC_ITEM_LIST), NULL, 0, 0, 160, WindowRect.Height(), SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);

// put it on top of the other windows
   ::SetForegroundWindow(m_hWnd);

   return TRUE;  // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}


void CGdiResourcesDlg::OnSize(UINT nType, int cx, int cy) 
{
// resize the list 
   ::SetWindowPos(::GetDlgItem(m_hWnd, IDC_ITEM_LIST), NULL, 0, 0, 160, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);

// default processing
   CDialog::OnSize(nType, cx, cy);

// force a refresh
   InvalidateRect(NULL);
}


void CGdiResourcesDlg::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
// default processing
   CDialog::OnGetMinMaxInfo(lpMMI);

// set the minimun width and height for resize
   lpMMI->ptMinTrackSize.y = m_DlgRect.Width();
   lpMMI->ptMinTrackSize.x = m_DlgRect.Height();
}



void CGdiResourcesDlg::OnPaint() 
{
   CPaintDC dc(this); // device context for painting
   
// draw GDI object selected in the listbox
   int iReturn = SendDlgItemMessage(IDC_ITEM_LIST, LB_GETCURSEL, 0, 0);
   if (iReturn != LB_ERR)
   {
      iReturn = SendDlgItemMessage(IDC_ITEM_LIST, LB_GETITEMDATA, iReturn, (LPARAM)NULL);
      CGdiObj* pObject = (CGdiObj*)iReturn;
      DrawResource(&dc, pObject);
   }
}


void DisplayPaletteColors(CDC* pDC, CRect DrawingRectangle, DWORD PaletteColorCount, PALETTEENTRY* pPaletteEntries)
{
// sanity checks
   ASSERT(pDC != NULL);
   if ((pDC == NULL) || (PaletteColorCount == 0) || (pPaletteEntries == NULL))
      return;


// display a palette "wheel"
// --> 16x16 blocks side by side (plus 1 pixel separator)
   DWORD BlockWidth        = 16;
   DWORD BlockHeight       = 16;
   DWORD SeparatorWidth    = 1;
   DWORD SeparatorHeight   = 1;
   CRect ColorBlock;
   HBRUSH hColorBrush;

// 1. compute the number blocks of per line
   DWORD ColorPerLine = DrawingRectangle.Width() / (BlockWidth + SeparatorWidth);
   
// 2. iterate until the last color
   DWORD CurrentColor = 0;
   DWORD CurrentLine  = 0;
   do
   {
   // draw current color
      ColorBlock.left = 
         DrawingRectangle.left + (CurrentColor % ColorPerLine) * (BlockWidth + SeparatorWidth);
      ColorBlock.top = 
         DrawingRectangle.top + CurrentLine * (BlockHeight + SeparatorHeight);
      ColorBlock.right = 
         ColorBlock.left + BlockWidth;
      ColorBlock.bottom = 
         ColorBlock.top + BlockHeight;

      hColorBrush = 
         ::CreateSolidBrush(
            RGB(
               pPaletteEntries[CurrentColor].peRed, 
               pPaletteEntries[CurrentColor].peGreen, 
               pPaletteEntries[CurrentColor].peBlue, 
               )
            );
      if (hColorBrush != NULL)
      {
         ::FillRect(pDC->m_hDC, ColorBlock, hColorBrush);
         ::DeleteObject(hColorBrush);
      }

   // next color
      CurrentColor++;

   // next line?
      if ((CurrentColor % ColorPerLine) == 0)
         CurrentLine++;
   } while (CurrentColor < PaletteColorCount);
}


BOOL DisplayDCBitmap(CDC* pDC, CRect Rectangle, HDC hDC)
{
   HBITMAP hBitmap = (HBITMAP)::GetCurrentObject(hDC, OBJ_BITMAP);
   BITMAP bmInfo;
   if (
         (::GetObject(hBitmap, sizeof(BITMAP), &bmInfo) == sizeof(BITMAP)) &&
         (bmInfo.bmWidth - bmInfo.bmHeight != 0) // 1x1 bitmap
      )
   {
   // Copy the bitmap bits to the screen DC
      int OldMode = pDC->SetBkMode(OPAQUE);
      ::BitBlt(
         pDC->m_hDC,
         Rectangle.left, 
         Rectangle.top, 
         bmInfo.bmWidth, 
         bmInfo.bmHeight, 
         hDC, 
         0,
         0,
         SRCCOPY
         );
      pDC->SetBkMode(OldMode);

      return(TRUE);
   }
   else
   {
      return(FALSE);
   }
}


void CGdiResourcesDlg::DrawResource(CDC* pDC, CGdiObj* pObject)
{
// sanity checks
   ASSERT(pDC != NULL);
   if (pDC == NULL)
         return;

   CRect ListRect;
   ::GetClientRect(::GetDlgItem(m_hWnd, IDC_ITEM_LIST), ListRect);

   CRect DlgRect;
   GetClientRect(DlgRect);
   DlgRect.left = ListRect.right+2;

// erase the background
   ::FillRect(pDC->m_hDC, DlgRect, (HBRUSH)::GetStockObject(LTGRAY_BRUSH));
   pDC->SetBkMode(TRANSPARENT);

   CRect Rectangle = DlgRect;
   if (pObject != NULL)
   {
      CString szInfo;
      CString szLine;
      switch(pObject->GetType())
      {
      // BitBlt() the bitmap
         case OBJ_BITMAP:
         {
            Rectangle.left    += 32;
            Rectangle.top     += 24;
            Rectangle.right   -= 12;
            Rectangle.bottom  -= 12;

            HBITMAP hBitmap = (HBITMAP)pObject->GetHandle();
            szInfo.Format("(%#04lx) Bitmap", hBitmap);
            BITMAP      bmInfo;
            if (
                  (::GetObjectType(hBitmap) == OBJ_BITMAP) && 
                  (::GetObject(hBitmap, sizeof(BITMAP), &bmInfo) == sizeof(BITMAP))
               )
            {
               CBitmap Bitmap;
               Bitmap.Attach(hBitmap);
               CDC dcMem;

            // Create a memory DC compatible with the screen DC
               dcMem.CreateCompatibleDC(pDC);

               pDC->SaveDC();

            // Put the bitmap bits into the memory DC
               CBitmap* pbmOld = dcMem.SelectObject(&Bitmap);

            // Copy the bitmap bits to the screen DC
               pDC->BitBlt(
                  Rectangle.left, 
                  Rectangle.top, 
                  bmInfo.bmWidth, 
                  bmInfo.bmHeight, 
                  &dcMem, 
                  0,
                  0,
                  SRCCOPY
                  );

            // Put the original bitmap back
               dcMem.SelectObject(pbmOld);

               Bitmap.Detach();

               pDC->RestoreDC(-1);
            }
            else
            {
               szLine = "Invalid bitmap handle";
               pDC->DrawText(szLine, Rectangle, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
            }
         }
         break;


      // fill a rectangle with the brush
         case OBJ_BRUSH:
         {
            try
            {
               Rectangle.left    += DlgRect.Width() / 4;
               Rectangle.top     += DlgRect.Height() / 4;
               Rectangle.right   -= DlgRect.Width() / 4;
               Rectangle.bottom  -= DlgRect.Height() / 4;

               HBRUSH   hBrush = (HBRUSH)pObject->GetHandle();
               LOGBRUSH logBrush;
               szInfo.Format("(%#04lx) Brush", hBrush);
               if (
                       (::GetObjectType(hBrush) == OBJ_BRUSH) &&
                       (::GetObject(hBrush, sizeof(LOGBRUSH), &logBrush) == sizeof(LOGBRUSH))
                   )
               {
                  ::FillRect(pDC->m_hDC, Rectangle, hBrush);
               }
               else
               {
                  szLine = "Invalid brush handle";
                  pDC->DrawText(szLine, Rectangle, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
               }
            }
            catch(...)
            {
               szLine = "Invalid brush handle";
               pDC->DrawText(szLine, Rectangle, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
            }
         }
         break;


      // frame a rectangle with the pen
         case OBJ_PEN:
         case OBJ_EXTPEN:
         {
            Rectangle.left    += DlgRect.Width() / 4;
            Rectangle.top     += DlgRect.Height() / 4;
            Rectangle.right   -= DlgRect.Width() / 4;
            Rectangle.bottom  -= DlgRect.Height() / 4;

            HPEN hPen = (HPEN)pObject->GetHandle();

            if (pObject->GetType() == OBJ_PEN)
               szInfo.Format("(%#04lx) Pen", hPen);
            else
            if (pObject->GetType() == OBJ_EXTPEN)
               szInfo.Format("(%#04lx) Ext Pen", hPen);

            if (
                  (::GetObjectType(hPen) == OBJ_PEN) ||
                  (::GetObjectType(hPen) == OBJ_EXTPEN)
               )
            {
               pDC->SaveDC();

               HPEN hOldPen = (HPEN)::SelectObject(pDC->m_hDC, hPen);
               pDC->Rectangle(Rectangle);
               ::SelectObject(pDC->m_hDC, (HPEN)hOldPen);

               pDC->RestoreDC(-1);
            }
            else
            {
               szLine = "Invalid Pen";
               pDC->DrawText(szLine, Rectangle, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
            }
         }   
         break;

      // write alphabet + digits using the font
         case OBJ_FONT:
         {
            LOGFONT logFont;
            HFONT hFont = (HFONT)pObject->GetHandle();
            if (
                  (::GetObjectType(hFont) == OBJ_FONT) && 
                  ((::GetObject(hFont, sizeof(LOGFONT), &logFont) == sizeof(LOGFONT)))
               )
            {
               szInfo.Format(
                  "(%#04lx) Font %s (%d)", 
                  hFont,
                  logFont.lfFaceName,
                  -MulDiv(logFont.lfHeight, 72, GetDeviceCaps(pDC->m_hDC, LOGPIXELSY))
                  );

               Rectangle.left    += 32;
               Rectangle.top     += 24;
               Rectangle.right   -= 12;
               Rectangle.bottom  -= 12;

               pDC->SaveDC();
               HFONT hOldFont = (HFONT)::SelectObject(pDC->m_hDC, hFont);
                  pDC->DrawText("abcdefghijklmnopqrstuvwxyz\nABCDEFGHIJKLMNOPQRSTUVWXYZ\n0123456789", Rectangle, DT_CENTER);
               ::SelectObject(pDC->m_hDC, hOldFont);
               pDC->RestoreDC(-1);
            }
            else
            {
               szInfo.Format("(%#04lx) Font ", pObject->GetHandle());
               szLine = "Invalid Font";
               pDC->DrawText(szLine, Rectangle, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
            }
         }
         break;


      // since the region might larger than the dialog, 
      // we just get the bounding rectangle but we could also
      // have tried to fill the region with a brush
         case OBJ_REGION:
         {
            Rectangle.left    += 32;
            Rectangle.top     += 24;
            Rectangle.right   -= 12;
            Rectangle.bottom  -= 12;

            HRGN hRegion = (HRGN)pObject->GetHandle();
            szInfo.Format("(%#04lx) Region", pObject->GetHandle());
            if (::GetObjectType(hRegion) == OBJ_REGION)
            {
               int   iResult;
               RECT  rgnRect;
               iResult = ::GetRgnBox(hRegion, &rgnRect);
               switch(iResult)
               {
                  case 0:
                     szLine = "\n\nInvalid region handle";
                  break;
                  
                  case NULLREGION:
                     szLine = "\n\nNULL region";
                  break;

                  case SIMPLEREGION:
                     szLine.Format(
                        "\n\nsimple region\n[%u, %u, %u, %u] of (%u x %u)", 
                        rgnRect.left, rgnRect.top, rgnRect.right, rgnRect.bottom,
                        rgnRect.right - rgnRect.left, rgnRect.bottom - rgnRect.top
                        );
                  break;

                  case COMPLEXREGION:
                     szLine.Format(
                        "\n\ncomplex\n[%u, %u, %u, %u] of (%u x %u)", 
                        rgnRect.left, rgnRect.top, rgnRect.right, rgnRect.bottom,
                        rgnRect.right - rgnRect.left, rgnRect.bottom - rgnRect.top
                        );
                  break;
               }
            }
            else
               szLine = "\n\nInvalid region handle";

            DrawTextEx(
               pDC->m_hDC, 
               (LPSTR)(LPCSTR)szLine, 
               szLine.GetLength(), 
               &Rectangle, 
               DT_CENTER,
               NULL);
         }
         break;


      // BitBlt() the bitmap of the DC
         case OBJ_DC:
         {
            Rectangle.left    += 32;
            Rectangle.top     += 24;
            Rectangle.right   -= 12;
            Rectangle.bottom  -= 12;

            HDC hDC = (HDC)pObject->GetHandle();
            if (::GetObjectType(hDC) == OBJ_DC)
            {
               szInfo.Format("(%#04lx) DC", hDC);
               if (!DisplayDCBitmap(pDC, Rectangle, hDC))
               {
                  szInfo.Format("(%#04lx) DC", hDC);
                  szLine = "Invalid bitmap handle";
                  pDC->DrawText(szLine, Rectangle, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
               }
            }
            else
            {
               szInfo.Format("(%#04lx) DC", hDC);
               szLine = "Invalid DC handle";
               pDC->DrawText(szLine, Rectangle, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
            }
         }
         break;


      // BitBlt() the bitmap of the Memory DC
         case OBJ_MEMDC:
         {
            Rectangle.left    += 32;
            Rectangle.top     += 24;
            Rectangle.right   -= 12;
            Rectangle.bottom  -= 12;

            HDC hDC = (HDC)pObject->GetHandle();
            if (::GetObjectType(hDC) == OBJ_MEMDC)
            {
               szInfo.Format("(%#04lx) Memory DC", hDC);
               if (!DisplayDCBitmap(pDC, Rectangle, hDC))
               {
                  szLine = "Invalid bitmap handle";
                  pDC->DrawText(szLine, Rectangle, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
               }
            }
            else
            {
               szInfo.Format("(%#04lx) Memory DC", hDC);
               szLine = "Invalid Memory DC handle";
               pDC->DrawText(szLine, Rectangle, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
            }
         }
         break;


      // get palette entries count and draw a color wheel
         case OBJ_PAL:
         {
            DWORD Count = 0;
            HPALETTE hPalette = (HPALETTE)pObject->GetHandle();
            if (::GetObject(hPalette, sizeof(WORD), &Count) == sizeof(WORD))
            {
                szInfo.Format("(%#04lx) Palette with %u entries", hPalette, Count);
                if (Count != NULL)
                {
                   PALETTEENTRY* pPaletteEntries = new PALETTEENTRY[Count];
                   if (pPaletteEntries != NULL)
                   {
                      DWORD PaletteColorCount = ::GetPaletteEntries(hPalette, 0, Count, pPaletteEntries);
                      if (PaletteColorCount != Count)
                      {
                      // strange...
                      }
                      else
                      {
                      // display each color
                         Rectangle.left    += 32;
                         Rectangle.top     += 24;
                         Rectangle.right   -= 12;
                         Rectangle.bottom  -= 12;

                         DisplayPaletteColors(pDC, Rectangle, PaletteColorCount, pPaletteEntries);
                      }
                   }
                   delete [] pPaletteEntries;
                }
            }
            else
                szInfo.Format("(%#04lx) Invalid Palette", hPalette);
         }
         break;


      // play enhanced metafile into our DC
         case OBJ_ENHMETAFILE:
         {
            szInfo.Format("(%#04lx) Enhanced Metafile", pObject->GetHandle());

            Rectangle.left    += 32;
            Rectangle.top     += 24;
            Rectangle.right   -= 12;
            Rectangle.bottom  -= 12;

            pDC->SaveDC();
            pDC->PlayMetaFile((HENHMETAFILE)pObject->GetHandle(), Rectangle);
            pDC->RestoreDC(-1);
         }
         break;


      // play metafile into our DC
         case OBJ_METAFILE:
         {
            szInfo.Format("(%#04lx) Metafile", pObject->GetHandle());

            Rectangle.left    += 32;
            Rectangle.top     += 24;
            Rectangle.right   -= 12;
            Rectangle.bottom  -= 12;

            pDC->SaveDC();

            pDC->SetMapMode(MM_ANISOTROPIC);   // Scalable metafile
            pDC->SetViewportExt(Rectangle.Width(), Rectangle.Height());   // Size of picture
            pDC->SetViewportOrg(Rectangle.left, Rectangle.right);         // Location of picture

            pDC->PlayMetaFile((HMETAFILE)pObject->GetHandle());

            pDC->RestoreDC(-1);
         }
         break;



      // find a way to display these objets
         case OBJ_METADC:
         {
            Rectangle.left    += 32;
            Rectangle.top     += 24;
            Rectangle.right   -= 12;
            Rectangle.bottom  -= 12;

            HDC hDC = (HDC)pObject->GetHandle();
            if (::GetObjectType(hDC) == OBJ_METADC)
            {
               szInfo.Format("(%#04lx) Metafile DC", hDC);

               if (!DisplayDCBitmap(pDC, Rectangle, hDC))
               {
                  szInfo.Format("(%#04lx) Metafile DC", hDC);
                  szLine = "Invalid bitmap handle";
                  pDC->DrawText(szLine, Rectangle, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
               }
            }
            else
            {
               szInfo.Format("(%#04lx) Meta DC", hDC);
               szLine = "Invalid DC handle";
               pDC->DrawText(szLine, Rectangle, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
            }
         }
         break;
         
         case OBJ_ENHMETADC:
         {
            Rectangle.left    += 32;
            Rectangle.top     += 24;
            Rectangle.right   -= 12;
            Rectangle.bottom  -= 12;

            HDC hDC = (HDC)pObject->GetHandle();
            if (::GetObjectType(hDC) == OBJ_METADC)
            {
               szInfo.Format("(%#04lx) Enhanced Metafile DC", hDC);

               if (!DisplayDCBitmap(pDC, Rectangle, hDC))
               {
                  szInfo.Format("(%#04lx) Enhanced Metafile DC", hDC);
                  szLine = "Invalid bitmap handle";
                  pDC->DrawText(szLine, Rectangle, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
               }
            }
            else
            {
               szInfo.Format("(%#04lx) Enhanced Metafile DC", hDC);
               szLine = "Invalid DC handle";
               pDC->DrawText(szLine, Rectangle, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
            }
         }
         break;

      
      // not covered here
         case OBJ_COLORSPACE:
            szInfo.Format("(%#04lx) Color Space", pObject->GetHandle());
         break;
      }

      pDC->DrawText(szInfo, DlgRect, DT_CENTER | DT_SINGLELINE);
   }
   else
   {
      pDC->DrawText(
         "Invalid Resource",
         strlen("Invalid Resource"),
         DlgRect,
         DT_CENTER | DT_SINGLELINE | DT_VCENTER
         );
   }
}


void CGdiResourcesDlg::OnSelChangeItemList() 
{
// show new selected GDI object
   int iReturn = SendDlgItemMessage(IDC_ITEM_LIST, LB_GETCURSEL, 0, 0);
   if (iReturn != LB_ERR)
   {
      iReturn = SendDlgItemMessage(IDC_ITEM_LIST, LB_GETITEMDATA, iReturn, (LPARAM)NULL);
      CGdiObj* pObject = (CGdiObj*)iReturn;

      CClientDC dc(this); 
      DrawResource(&dc, pObject);

   // notify event if needed
      if ((m_pNotifications != NULL) && (pObject != NULL))
      {
         m_pNotifications->OnSelect(m_hWnd, pObject->GetHandle());
      }
   }
}


void CGdiResourcesDlg::OnDblclkItemList() 
{
   if (m_pNotifications != NULL)
   {
      int iReturn = SendDlgItemMessage(IDC_ITEM_LIST, LB_GETCURSEL, 0, 0);
      if (iReturn != LB_ERR)
      {
         iReturn = SendDlgItemMessage(IDC_ITEM_LIST, LB_GETITEMDATA, iReturn, (LPARAM)NULL);
         CGdiObj* pObject = (CGdiObj*)iReturn;
         if (pObject != NULL)
            m_pNotifications->OnDoubleClick(m_hWnd, pObject->GetHandle());
      }
   }
}
