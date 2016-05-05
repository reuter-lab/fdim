/*
 * Original Author: Martin Reuter
 * Copyright 2014 Martin Reuter
 * 
 * Terms and conditions for use, reproduction, distribution and contribution
 * are found in the file 'LICENSE'
 *
 */

#include "BitmapView.h"
#include "utils.h"

BEGIN_EVENT_TABLE(BitmapView, wxScrolledWindow)
  EVT_ERASE_BACKGROUND(BitmapView::OnEraseBackground)
  EVT_PAINT(BitmapView::OnPaint)
  EVT_LEFT_DOWN(BitmapView::OnLeftDown)
  EVT_LEFT_UP(BitmapView::OnLeftUp)
  EVT_MOTION(BitmapView::OnMotion)
END_EVENT_TABLE()

BitmapView::BitmapView(wxWindow *parent, wxWindowID id)
  : wxScrolledWindow(parent, id, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE)
{
  SetBackgroundStyle(wxBG_STYLE_CUSTOM);
  activeSelection = false;
  isDragging = false;
  bitmap.Create(1,1);
}

void BitmapView::setImage(const wxImage& image)
{
  bitmap = wxBitmap(image);
  SetScrollbars(1, 1, bitmap.GetWidth(), bitmap.GetHeight());
  activeSelection = false;
  Refresh();
}

void BitmapView::OnEraseBackground(wxEraseEvent& event)
{
  // empty
}


void BitmapView::OnPaint(wxPaintEvent& event)
{
  wxPaintDC dc(this);  DoPrepareDC(dc);

  dc.SetPen(*wxBLACK_PEN);
  dc.SetBrush(*wxBLACK_BRUSH);
  int cw, ch; GetClientSize(&cw, &ch);
  int vx, vy; GetViewStart(&vx, &vy);
  if(cw > bitmap.GetWidth())
    dc.DrawRectangle(bitmap.GetWidth(), vy, cw - bitmap.GetWidth() + 1, bitmap.GetHeight());
  if(ch > bitmap.GetHeight())
    dc.DrawRectangle(vx, bitmap.GetHeight(), cw, ch - bitmap.GetHeight() + 1);

  dc.DrawBitmap(bitmap, 0, 0, false);

  if(activeSelection)
  {
    dc.SetBrush(*wxTRANSPARENT_BRUSH);
    dc.SetPen(*wxRED_PEN);
    wxRect rect = getSelection();
    dc.DrawRectangle(rect.x, rect.y, rect.width, rect.height);
  }
}

bool  BitmapView::isActiveSelection()
{
  return activeSelection;
}

wxRect BitmapView::getSelection()
{
  return wxRect(min(sel1.x, sel2.x), min(sel1.y,sel2.y), abs(sel2.x - sel1.x)+1, abs(sel2.y - sel1.y)+1);
}

void BitmapView::OnLeftDown(wxMouseEvent& event)
{
  CalcUnscrolledPosition(event.GetX(), event.GetY(), &sel1.x, &sel1.y);
  sel1.x = clamp(sel1.x, 0, bitmap.GetWidth()-1);
  sel1.y = clamp(sel1.y, 0, bitmap.GetHeight()-1);
  sel2 = sel1;
  activeSelection = true;
  isDragging = true;
  CaptureMouse();
  Refresh();
}

void BitmapView::OnLeftUp(wxMouseEvent& event)
{
  if (GetCapture() == this ) ReleaseMouse();
  isDragging = false;
  if(sel1 == sel2)
    activeSelection = false;
}


void BitmapView::OnMotion(wxMouseEvent& event)
{
  if(!isDragging)
    return;

  wxPoint pos = event.GetPosition();
  CalcUnscrolledPosition(pos.x, pos.y, &sel2.x, &sel2.y);
  sel2.x = clamp(sel2.x, 0, bitmap.GetWidth()-1);
  sel2.y = clamp(sel2.y, 0, bitmap.GetHeight()-1);

  // calc scrolling
  int cw, ch; GetClientSize(&cw, &ch);
  int dx = 0, dy = 0;
  if(pos.x < 0)       
    dx = pos.x;
  else if(pos.x > cw) 
    dx = pos.x - cw;
  
  if(pos.y < 0)      
    dy = pos.y;
  else if(pos.y > ch) 
    dy = pos.y - ch;
  if(dx || dy)
  {
    int vx, vy; GetViewStart(&vx, &vy);
    Scroll(vx + dx, vy + dy);
  }
  
  Refresh();
}



