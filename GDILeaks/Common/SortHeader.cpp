#include "stdafx.h"
#include "SortHeader.h"

#ifdef _DEBUG
   #define new DEBUG_NEW
   #undef THIS_FILE
   static char THIS_FILE[] = __FILE__;
#endif



BEGIN_MESSAGE_MAP(CSortHeader, CHeaderCtrl)
//{{AFX_MSG_MAP(CSortHeader)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()


//--------------------------------------------------------------------
// Construction/Destruction
//
CSortHeader::CSortHeader()
{
   m_nSortCol = -1;
}

CSortHeader::~CSortHeader()
{
}
//
//--------------------------------------------------------------------


//--------------------------------------------------------------------
// C++ Interface
//
int CSortHeader::SetSortImage( int nCol, BOOL bAsc )
{
   int nPrevCol = m_nSortCol;

   m_nSortCol = nCol;
   m_bSortAsc = bAsc;

// Change the item to ownder drawn
   HD_ITEM hditem;

   hditem.mask = HDI_FORMAT;
   GetItem(nCol, &hditem);
   hditem.fmt |= HDF_OWNERDRAW;
   SetItem(nCol, &hditem);

// Invalidate header control so that it gets redrawn
   Invalidate();
   return nPrevCol;
}
//
//--------------------------------------------------------------------


//--------------------------------------------------------------------
// Internal helpers
//
void CSortHeader::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
   CDC dc;
   dc.Attach(lpDrawItemStruct->hDC); 

// Get the column rect
   CRect rcLabel(lpDrawItemStruct->rcItem);

// Save DC
   int nSavedDC = dc.SaveDC();

// Set clipping region to limit drawing within column 
   CRgn rgn;
   rgn.CreateRectRgnIndirect(&rcLabel); 
   dc.SelectObject(&rgn);
   rgn.DeleteObject();

// Draw the background
   dc.FillRect(rcLabel, &CBrush(::GetSysColor(COLOR_3DFACE)));  
   
// Labels are offset by a certain amount  
// This offset is related to the width of a space character
   int offset;
   offset = dc.GetTextExtent(_T(" "), 1 ).cx*2;
//   offset = (rcLabel.bottom - rcLabel.top) / 4;

// Get the column text and format
   TCHAR    szInfo[_MAX_PATH];
   HD_ITEM  hditem;   
   hditem.mask = HDI_TEXT | HDI_FORMAT;
   hditem.pszText = szInfo;
   hditem.cchTextMax = _MAX_PATH;
   GetItem(lpDrawItemStruct->itemID, &hditem);

// Determine format for drawing column label
   UINT uFormat = DT_SINGLELINE | DT_NOPREFIX | DT_NOCLIP | DT_VCENTER | DT_END_ELLIPSIS; 
   if (hditem.fmt & HDF_CENTER)
      uFormat |= DT_CENTER;
   else if (hditem.fmt & HDF_RIGHT)
      uFormat |= DT_RIGHT;
   else
      uFormat |= DT_LEFT;

// Adjust the rect if the mouse button is pressed on it
   if (lpDrawItemStruct->itemState == ODS_SELECTED)
   {
      rcLabel.left++;
      rcLabel.top += 2;
      rcLabel.right++;  
   }

// Adjust the rect further if Sort arrow is to be displayed
   if (lpDrawItemStruct->itemID == (UINT)m_nSortCol) 
      rcLabel.right -= 3 * offset;

   rcLabel.left += offset;
   rcLabel.right -= offset;   

// Draw column label
   if (rcLabel.left < rcLabel.right)
      dc.DrawText(szInfo,-1,rcLabel, uFormat);

// Draw the Sort arrow  
   if (lpDrawItemStruct->itemID == (UINT)m_nSortCol) 
   {
      CRect rcIcon(lpDrawItemStruct->rcItem);

   // Set up pens to use for drawing the triangle
      CPen penLight(PS_SOLID, 1, GetSysColor(COLOR_3DHILIGHT));
      CPen penShadow(PS_SOLID, 1, GetSysColor(COLOR_3DSHADOW));
      CPen *pOldPen = dc.SelectObject( &penLight );
      if (m_bSortAsc)
      {
      // Draw triangle pointing upwards
         dc.MoveTo(rcIcon.right - 2*offset, offset-1);
         dc.LineTo(rcIcon.right - 3*offset/2, rcIcon.bottom - offset);
         dc.LineTo(rcIcon.right - 5*offset/2-2, rcIcon.bottom - offset);
         dc.MoveTo(rcIcon.right - 5*offset/2-1, rcIcon.bottom - offset-1);
         dc.SelectObject(&penShadow );
         dc.LineTo(rcIcon.right - 2*offset, offset-2);
      }
      else
      {
      // Draw triangle pointing downwords
         dc.MoveTo(rcIcon.right - 3*offset/2, offset-1);
         dc.LineTo(rcIcon.right - 2*offset-1, rcIcon.bottom - offset + 1);
         dc.MoveTo(rcIcon.right - 2*offset-1, rcIcon.bottom - offset);
         dc.SelectObject(&penShadow);
         dc.LineTo(rcIcon.right - 5*offset/2-1, offset -1);
         dc.LineTo(rcIcon.right - 3*offset/2, offset -1);
      }

   // Restore the pen
      dc.SelectObject(pOldPen);
   }

// Restore dc  
   dc.RestoreDC(nSavedDC);

// Detach the dc before returning
   dc.Detach();
}
//
//--------------------------------------------------------------------


