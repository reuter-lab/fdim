#include "LineView.h"
#include "Line.h"
#include "utils.h"

DEFINE_EVENT_TYPE(myEVT_LINEVIEW_CHANGED)

BEGIN_EVENT_TABLE(LineView, wxWindow)
	EVT_PAINT(LineView::OnPaint)
	EVT_LEFT_DOWN(LineView::OnLeftDown)
END_EVENT_TABLE()

LineView::LineView(wxWindow *parent, wxWindowID id)
	: wxWindow(parent, id, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE)
{
	SetBackgroundColour(wxColour(0,0,0));
	setLine(0);
}


void LineView::setLine(Line *line_)
{
	line = line_;
	sel = 0;
	if(line) {
		indices[0] = line->getfirst();
		indices[1] = line->getlast();

		wxCommandEvent event(myEVT_LINEVIEW_CHANGED, GetId());
		event.SetEventObject(this);
		GetEventHandler()->ProcessEvent(event);
	}
	Refresh();
}


void LineView::transform(double x, double y, int& tx, int& ty)
{
	int cw, ch; GetClientSize(&cw, &ch);
	tx = int((x - line->getminx()) * (cw - 2*PAD) / (line->getmaxx() - line->getminx()) + PAD);
	ty = ch-int((y - line->getminy()) * (ch - 2*PAD) / (line->getmaxy() - line->getminy()) + PAD);
}

void LineView::inverseTransform(int tx, int ty, double& x, double& y)
{
	int cw, ch; GetClientSize(&cw, &ch);
	x = (tx      - PAD)*(line->getmaxx() - line->getminx())/(cw - 2*PAD) + line->getminx();
	y = ((ch-ty) - PAD)*(line->getmaxy() - line->getminy())/(ch - 2*PAD) + line->getminy();
}

void LineView::OnPaint(wxPaintEvent& event)
{
	wxPaintDC dc(this);
	if(!line)
		return;

	int x1, y1, x2, y2;
	transform(line->getminx(), line->getf(line->getminx()), x1, y1);
	transform(line->getmaxx(), line->getf(line->getmaxx()), x2, y2);
	dc.SetPen(*wxWHITE_PEN);
	dc.DrawLine(x1, y1, x2, y2);

	dc.SetPen(*wxTRANSPARENT_PEN);
	for(int i = 0; i < line->getLen(); ++i) {
		int x, y;
		transform(line->getx(i), line->gety(i), x, y);
		if(sel == 1 && i == indices[0]) 
			dc.SetBrush(*wxRED_BRUSH);
		else  if(i == line->getfirst() || i == line->getlast())
			dc.SetBrush(*wxGREEN_BRUSH);
		else
			dc.SetBrush(*wxWHITE_BRUSH);
		dc.DrawRectangle(x-2,y-2,5,5);
	}
}

void LineView::OnLeftDown(wxMouseEvent& event)
{
	if(!line)
		return;

	double x, y;
	inverseTransform(event.GetX(), event.GetY(), x, y);
	int i = line->getindex(x, y, 0.5);
	if(i >= 0) {
		indices[sel] = i;
		sel = 1 - sel;
		if(sel == 0) {
			if(indices[0] > indices[1]) 
				mySwap(indices[0],indices[1]);
			if(indices[0] != indices[1]) {
				line->calc(indices[0],indices[1]);
			
				wxCommandEvent event(myEVT_LINEVIEW_CHANGED, GetId());
				event.SetEventObject(this);
				GetEventHandler()->ProcessEvent(event);
			}
		}
		Refresh();
	}
}
