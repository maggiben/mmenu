#include "stdafx.h"
#include "SeekerWnd.h"


#ifdef _DEBUG
   #define new DEBUG_NEW
   #undef THIS_FILE
   static char THIS_FILE[] = __FILE__;
#endif


// internal helpers
// 
static void FrameWindow(HWND hWnd)
{
   if (!::IsWindow(hWnd))
      return;

   CRect wndRect;
   ::GetWindowRect(hWnd, wndRect);
   wndRect.NormalizeRect();
   wndRect.right = wndRect.right - wndRect.left;
   wndRect.left = 0;
   wndRect.bottom = wndRect.bottom - wndRect.top;
   wndRect.top = 0;

   HDC      hDC;
   int      iOldROP;
   HPEN     hPen;
   HPEN     hOldPen;
   HBRUSH   hOldBrush;

   hDC = ::GetWindowDC(hWnd);
   iOldROP = ::SetROP2(hDC, R2_XORPEN);
   hPen = ::CreatePen(PS_SOLID, ::GetSystemMetrics(SM_CXSIZEFRAME), RGB(255, 255, 255));
   hOldPen = (HPEN)::SelectObject(hDC, hPen);
   hOldBrush = (HBRUSH)::SelectObject(hDC, ::GetStockObject(NULL_BRUSH));

   ::Rectangle(hDC, wndRect.left, wndRect.top, wndRect.right, wndRect.bottom);
   
   ::SelectObject(hDC, hOldBrush);
   ::SelectObject(hDC, hOldPen);
   ::DeleteObject(hPen);
   ::SetROP2(hDC, iOldROP);
   ::ReleaseDC(hWnd, hDC);
}





// CSeekerEdit
//
BEGIN_MESSAGE_MAP(CSeekerEdit, CEdit)
//{{AFX_MSG_MAP(CSeekerEdit)
   ON_WM_LBUTTONDOWN()
   ON_WM_LBUTTONUP()
   ON_WM_MOUSEMOVE()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()



CSeekerEdit::CSeekerEdit()
   :  m_hSeekedWindow(NULL), 
      m_bSeeking(FALSE)
{
}

CSeekerEdit::~CSeekerEdit()
{
}


void CSeekerEdit::SetCursorID(DWORD CursorID)
{
// load seeking cursor
   m_hSeekingCursor = ::LoadCursor(AfxGetResourceHandle(), MAKEINTRESOURCE(CursorID));
}


void CSeekerEdit::OnLButtonDown(UINT nFlags, CPoint point) 
{
   if ((MK_CONTROL & nFlags) == MK_CONTROL)
   {
   // start seeking
      m_bSeeking = TRUE;
      m_hSeekedWindow = m_hWnd;
      FrameWindow(m_hSeekedWindow);
      SetCapture();

   // change the cursor
      ::SetCursor(m_hSeekingCursor);
   }
   else
      CEdit::OnLButtonDown(nFlags, point);
}


void CSeekerEdit::OnLButtonUp(UINT nFlags, CPoint point) 
{
   if (m_bSeeking)
   {
   // end seeking
      m_bSeeking = FALSE;
      FrameWindow(m_hSeekedWindow);
      ReleaseCapture();
   }

   CEdit::OnLButtonUp(nFlags, point);
}


void CSeekerEdit::OnMouseMove(UINT nFlags, CPoint point) 
{
   if (m_bSeeking)
   {
   // change the cursor
      SetCursor(m_hSeekingCursor);
   
   // see if there is a different window under the mouse
      HWND hWindowUnderTheMouse;
      ClientToScreen(&point);
      hWindowUnderTheMouse = ::WindowFromPoint(point);
      if (hWindowUnderTheMouse != m_hSeekedWindow)
      {
         FrameWindow(m_hSeekedWindow);
         m_hSeekedWindow = hWindowUnderTheMouse;
         FrameWindow(m_hSeekedWindow);

         CString szTitle;
         szTitle.Format(_T("0x%lX"), m_hSeekedWindow);
         SetWindowText(szTitle);
      }
   }
   else
      CEdit::OnMouseMove(nFlags, point);
}




// CSeekerStatic
//
BEGIN_MESSAGE_MAP(CSeekerStatic, CStatic)
//{{AFX_MSG_MAP(CSeekerStatic)
   ON_WM_LBUTTONDOWN()
   ON_WM_LBUTTONUP()
   ON_WM_MOUSEMOVE()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CSeekerStatic::CSeekerStatic()
   :  m_hSeekedWindow(NULL), 
      m_bSeeking(FALSE)
{
   m_hTwinWnd        = NULL;
   m_hSeekingCursor  = NULL;
   m_hIconUp         = NULL;
   m_hIconDown       = NULL;
}


CSeekerStatic::~CSeekerStatic()
{
}


void CSeekerStatic::SetCursorIDs(DWORD CursorID, DWORD IconUp, DWORD IconDown)
{
// load needed resources
   m_hSeekingCursor = ::LoadCursor(AfxGetResourceHandle(), MAKEINTRESOURCE(CursorID));
   m_hIconUp = ::LoadIcon(AfxGetResourceHandle(), MAKEINTRESOURCE(IconUp));
   m_hIconDown = ::LoadIcon(AfxGetResourceHandle(), MAKEINTRESOURCE(IconDown));
}


void CSeekerStatic::SetTwinWnd(HWND hWnd)
{
// sanity check
   ASSERT(::IsWindow(hWnd));
   if (::IsWindow(hWnd))
      m_hTwinWnd = hWnd;
   else
      m_hTwinWnd = NULL;
}


void CSeekerStatic::OnLButtonDown(UINT nFlags, CPoint point) 
{
// default processing
   CStatic::OnLButtonDown(nFlags, point);

// change the cursor
   ::SetCursor(m_hSeekingCursor);
   
// change the icon itself
   SetIcon(m_hIconDown);

// start seeking
   m_bSeeking = TRUE;
   m_hSeekedWindow = m_hWnd;
   FrameWindow(m_hSeekedWindow);
   SetCapture();
}


void CSeekerStatic::OnLButtonUp(UINT nFlags, CPoint point) 
{
   if (m_bSeeking)
   {
   // end seeking
      m_bSeeking = FALSE;
      FrameWindow(m_hSeekedWindow);
      ReleaseCapture();

   // Restore the Finder Tool icon
      SetIcon(m_hIconUp);
   }

   CStatic::OnLButtonUp(nFlags, point);
}


void CSeekerStatic::OnMouseMove(UINT nFlags, CPoint point) 
{
   if (m_bSeeking)
   {
   // !!! if you change the cursor, you change the icon !!!
   // change the cursor
   // SetCursor(m_hSeekingCursor);
   
   // see if there is a different window under the mouse
      HWND hWindowUnderTheMouse;
      ClientToScreen(&point);
      hWindowUnderTheMouse = ::WindowFromPoint(point);
      if (hWindowUnderTheMouse != m_hSeekedWindow)
      {
         FrameWindow(m_hSeekedWindow);
         m_hSeekedWindow = hWindowUnderTheMouse;
         FrameWindow(m_hSeekedWindow);

      // update the twin window
         CString szTitle;
         szTitle.Format(_T("0x%lX"), m_hSeekedWindow);
         ::SetWindowText(m_hTwinWnd, szTitle);
      }
   }
   else
      CStatic::OnMouseMove(nFlags, point);
}
