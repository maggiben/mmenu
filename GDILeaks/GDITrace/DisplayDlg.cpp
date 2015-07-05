// DisplayDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DisplayDlg.h"

#ifdef _DEBUG
   #define new DEBUG_NEW
   #undef THIS_FILE
   static char THIS_FILE[] = __FILE__;
#endif


#ifdef __TRACE
   #define _TRACE TRACE
#else
   #define _TRACE 
#endif



CDisplayDlg::CDisplayDlg(CWnd* pParent, CGDIReflect* pGDIReflect)
   : CDialog(CDisplayDlg::IDD, pParent)
{
// init members
   m_DlgRect.left    = 0;
   m_DlgRect.right   = 400;
   m_DlgRect.top     = 0;
   m_DlgRect.bottom  = 200;
   m_pGDIReflect = pGDIReflect;


// MFC stuff
//
//{{AFX_DATA_INIT(CDisplayDlg)
   // NOTE: the ClassWizard will add member initialization here
//}}AFX_DATA_INIT
}


void CDisplayDlg::DoDataExchange(CDataExchange* pDX)
{
   CDialog::DoDataExchange(pDX);

//{{AFX_DATA_MAP(CDisplayDlg)
   // NOTE: the ClassWizard will add DDX and DDV calls here
//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDisplayDlg, CDialog)
//{{AFX_MSG_MAP(CDisplayDlg)
   ON_LBN_SELCHANGE(IDC_ITEM_LIST, OnSelChangeItemList)
   ON_WM_SIZE()
   ON_WM_GETMINMAXINFO()
   ON_WM_PAINT()
   ON_WM_NCCREATE()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()



BOOL CDisplayDlg::OnInitDialog() 
{
// default MFC processing
   CDialog::OnInitDialog();

// fill the list with the resources 
   ASSERT(m_pGDIReflect != NULL);
   if (m_pGDIReflect != NULL)
   {
      DWORD          dwType;
      HANDLE         hObject;
      CString        szText;
      CString        szInfo;
      int            iReturn;
      DWORD          GdiType;
      CHandleInfo*   pInfo;
      for (POSITION pos = m_pGDIReflect->GetFirstItem(hObject, pInfo); ; pos = m_pGDIReflect->GetNextItem(pos, hObject, pInfo))
      {
         GdiType = ::GetObjectType(hObject);
         if (GdiType != 0) 
         {
            szText.Format("%s\t%#04lx", GetGDIType(dwType), hObject);
            iReturn = ::SendDlgItemMessage(m_hWnd, IDC_ITEM_LIST, LB_ADDSTRING, 0, (LPARAM)(LPCTSTR)szText);
            if (iReturn != LB_ERR)
               ::SendDlgItemMessage(m_hWnd, IDC_ITEM_LIST, LB_SETITEMDATA, (WPARAM)iReturn, (LPARAM)hObject);
         }

         if (pos == NULL)
            break;
      }
   }


// resize the list
   CRect WindowRect;
   GetClientRect(WindowRect);
   ::SetWindowPos(::GetDlgItem(m_hWnd, IDC_ITEM_LIST), NULL, 0, 0, 160, WindowRect.Height(), SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);

   return TRUE;  // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}


void CDisplayDlg::OnSize(UINT nType, int cx, int cy) 
{
// resize the list 
   ::SetWindowPos(::GetDlgItem(m_hWnd, IDC_ITEM_LIST), NULL, 0, 0, 160, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);

// default processing
   CDialog::OnSize(nType, cx, cy);

// force a refresh
   InvalidateRect(NULL);
}


void CDisplayDlg::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
// default processing
   CDialog::OnGetMinMaxInfo(lpMMI);

   lpMMI->ptMinTrackSize.y = m_DlgRect.Width();
   lpMMI->ptMinTrackSize.x = m_DlgRect.Height();
}


void CDisplayDlg::OnPaint() 
{
   CPaintDC dc(this); // device context for painting
   
// draw selected GDI object
   int iReturn = SendDlgItemMessage(IDC_ITEM_LIST, LB_GETCURSEL, 0, 0);
   if (iReturn != LB_ERR)
   {
      iReturn = SendDlgItemMessage(IDC_ITEM_LIST, LB_GETITEMDATA, iReturn, (LPARAM)NULL);
      HANDLE hObject = (HANDLE)iReturn;
      DrawResource(&dc, hObject);
   }
}


void CDisplayDlg::OnSelChangeItemList() 
{
   int iReturn = SendDlgItemMessage(IDC_ITEM_LIST, LB_GETCURSEL, 0, 0);
   if (iReturn != LB_ERR)
   {
      iReturn = SendDlgItemMessage(IDC_ITEM_LIST, LB_GETITEMDATA, iReturn, (LPARAM)NULL);
      HANDLE hObject = (HANDLE)iReturn;

      CClientDC dc(this); 
      DrawResource(&dc, hObject);
   }
}


void CDisplayDlg::DrawResource(CDC* pDC, HGDIOBJ hObject)
{
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
   DWORD GdiType = ::GetObjectType(hObject);

   if (GdiType != 0)
   {
      CString szInfo;
      CString szLine;
      switch(GdiType)
      {
         case OBJ_BITMAP :
         {
            Rectangle.left    += 32;
            Rectangle.top     += 24;
            Rectangle.right   -= 12;
            Rectangle.bottom  -= 12;

            HBITMAP hBitmap = (HBITMAP)hObject;
            BITMAP bmInfo;
            if ((::GetObjectType(hBitmap)) && (::GetObject(hBitmap, sizeof(BITMAP), &bmInfo) == sizeof(BITMAP)))
            {
               szInfo.Format("(%#04lx) Bitmap (%u x %u)", hBitmap, bmInfo.bmWidth, bmInfo.bmHeight);
               CBitmap Bitmap;
               Bitmap.Attach(hBitmap);
               CDC dcMem;

            // Create a memory DC compatible with the screen DC
               dcMem.CreateCompatibleDC(pDC);

            // Put the bitmap bits into the memory DC
               CBitmap* pbmOld = dcMem.SelectObject(&Bitmap);

            // Copy the bitmap bits to the screen DC
               BOOL bSuccess = 
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

               if (!bSuccess)
                  TRACE("Impossible to BitBlt bitmap(0x%#04lx)\n", hBitmap);

            // Put the original bitmap back
               dcMem.SelectObject(pbmOld);

               Bitmap.Detach();


            // draw a framing rectangle
               RECT rcFrame;
               rcFrame.left   = Rectangle.left - 1;
               rcFrame.top    = Rectangle.top - 1;
               rcFrame.right  = rcFrame.left + bmInfo.bmWidth + 1;
               rcFrame.bottom = rcFrame.top + bmInfo.bmHeight + 1;
               ::DrawFocusRect(pDC->m_hDC, &rcFrame);
            }
            else
            {
               szInfo.Format("(%#04lx) invalid Bitmap", hBitmap);
               szLine = "Invalid bitmap handle";
               pDC->DrawText(szLine, Rectangle, DT_CENTER);
            }
         }
         break;

         case OBJ_BRUSH :
         {
            Rectangle.left    += DlgRect.Width() / 4;
            Rectangle.top     += DlgRect.Height() / 4;
            Rectangle.right   -= DlgRect.Width() / 4;
            Rectangle.bottom  -= DlgRect.Height() / 4;

            HBRUSH hBrush = (HBRUSH)hObject;
            szInfo.Format("(%#04lx) Brush", hBrush);
            if (::GetObjectType(hBrush) == OBJ_BRUSH)
            {
               ::FillRect(pDC->m_hDC, Rectangle, hBrush);

            // draw a framing rectangle
               RECT rcFrame;
               rcFrame.left   = Rectangle.left - 1;
               rcFrame.top    = Rectangle.top - 1;
               rcFrame.right  = Rectangle.right + 1;
               rcFrame.bottom = Rectangle.bottom + 1;
               ::DrawFocusRect(pDC->m_hDC, &rcFrame);
            }
            else
            {
               szLine = "Invalid brush handle";
               pDC->DrawText(szLine, Rectangle, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
            }
         }
         break;

         case OBJ_PAL :
            szInfo.Format("(%#04lx) Palette", hObject);
         break;

         case OBJ_PEN :
         {
            Rectangle.left    += DlgRect.Width() / 4;
            Rectangle.top     += DlgRect.Height() / 4;
            Rectangle.right   -= DlgRect.Width() / 4;
            Rectangle.bottom  -= DlgRect.Height() / 4;

            HPEN hPen = (HPEN)hObject;
            szInfo.Format("(%#04lx) Pen", hPen);
            if (::GetObjectType(hPen) == OBJ_PEN)
            {
               HPEN hOldPen = (HPEN)::SelectObject(pDC->m_hDC, hPen);
               pDC->Rectangle(Rectangle);
               ::SelectObject(pDC->m_hDC, (HPEN)hOldPen);
            }
            else
            {
               szLine = "Invalid Pen";
               pDC->DrawText(szLine, Rectangle, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
            }
         }   
         break;

         case OBJ_FONT :
         {
            LOGFONT logFont;
            HFONT hFont = (HFONT)hObject;
            if ((::GetObjectType(hFont) == OBJ_FONT) && ((::GetObject(hFont, sizeof(LOGFONT), &logFont) == sizeof(LOGFONT))))
            {
               szInfo.Format(
                  "(%#04lx) Font %s (%d)", 
                  hObject,
                  logFont.lfFaceName,
                  -MulDiv(logFont.lfHeight, 72, GetDeviceCaps(pDC->m_hDC, LOGPIXELSY))
                  );

               Rectangle.left    = DlgRect.left + 20;
               Rectangle.top     = DlgRect.top + 40;
               Rectangle.right   = DlgRect.right - 4;
               Rectangle.bottom  = DlgRect.bottom - 4;

               HFONT hOldFont = (HFONT)::SelectObject(pDC->m_hDC, hFont);
                  pDC->DrawText(_T("abcdefghijklmnopqrstuvwxyz\nABCDEFGHIJKLMNOPQRSTUVWXYZ\n0123456789"), Rectangle, DT_CENTER);
               ::SelectObject(pDC->m_hDC, hOldFont);
            }
            else
            {
               szInfo.Format("(%#04lx) Font ", hObject);
               szLine = "Invalid Font";
               pDC->DrawText(szLine, Rectangle, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
            }
         }
         break;

         case OBJ_REGION :
         {
            HRGN hRegion = (HRGN)hObject;
            szInfo.Format("(%#04lx) Region", hObject);
            if (::GetObjectType(hRegion) == OBJ_REGION)
            {
//               ::OffsetRgn(hRegion, ListRect.left, 40);
//               ::FillRgn(pDC->m_hDC, hRegion, (HBRUSH)::GetStockObject(BLACK_BRUSH));
            }
            else
            {
               Rectangle.left    += DlgRect.Width() / 4;
               Rectangle.top     += DlgRect.Height() / 4;
               Rectangle.right   -= DlgRect.Width() / 4;
               Rectangle.bottom  -= DlgRect.Height() / 4;
               szLine = "Invalid region handle";
               pDC->DrawText(szLine, Rectangle, DT_CENTER);
            }
         }
         break;


         case OBJ_DC :
         case OBJ_MEMDC :
         {
            Rectangle.left    += 32;
            Rectangle.top     += 24;
            Rectangle.right   -= 12;
            Rectangle.bottom  -= 12;

            HDC hDC = (HDC)hObject;
            if ((::GetObjectType(hDC) == OBJ_DC) || (::GetObjectType(hDC) == OBJ_MEMDC))
            {
               HBITMAP hBitmap = (HBITMAP)GetCurrentObject(hDC, OBJ_BITMAP);
               BITMAP bmInfo;
               if (
                     (::GetObject(hBitmap, sizeof(BITMAP), &bmInfo) == sizeof(BITMAP)) &&
                     (bmInfo.bmWidth - bmInfo.bmHeight != 0) // 1x1 bitmap
                  )
               {
                  if (GdiType == OBJ_MEMDC)
                     szInfo.Format("(%#04lx) Memory DC (%u x %u)", hDC, bmInfo.bmWidth, bmInfo.bmHeight);
                  else
                     szInfo.Format("(%#04lx) DC (%u x %u)", hDC, bmInfo.bmWidth, bmInfo.bmHeight);

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

               // draw a framing rectangle
                  RECT rcFrame;
                  rcFrame.left   = Rectangle.left - 1;
                  rcFrame.top    = Rectangle.top - 1;
                  rcFrame.right  = rcFrame.left + bmInfo.bmWidth + 1;
                  rcFrame.bottom = rcFrame.top + bmInfo.bmHeight + 1;
                  ::DrawFocusRect(pDC->m_hDC, &rcFrame);
               }
               else
               {
                  if (GdiType == OBJ_MEMDC)
                     szInfo.Format("(%#04lx) DC", hDC);
                  else
                     szInfo.Format("(%#04lx) Memory DC", hDC);

                  szLine = "Invalid bitmap handle";
                  pDC->DrawText(szLine, Rectangle, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
               }
            }
            else
            {
               if (GdiType == OBJ_MEMDC)
               {
                  szInfo.Format("(%#04lx) Memory DC", hDC);
                  szLine = "Invalid Memory DC handle";
               }
               else
               {
                  szInfo.Format("(%#04lx) DC", hDC);
                  szLine = "Invalid DC handle";
               }
               pDC->DrawText(szLine, Rectangle, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
            }
         }
         break;

         case OBJ_EXTPEN :
            szInfo.Format("(%#04lx) Ext Pen", hObject);
         break;

         case OBJ_METAFILE :
            szInfo.Format("(%#04lx) Metafile", hObject);
         break;

         case OBJ_METADC :
            szInfo.Format("(%#04lx) Meta DC", hObject);
         break;

         case OBJ_ENHMETAFILE :
            szInfo.Format("(%#04lx) Enhanced Metafile", hObject);
         break;

         case OBJ_ENHMETADC :
            szInfo.Format("(%#04lx) Enhanced Metafile DC", hObject);
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


