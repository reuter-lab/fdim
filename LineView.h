#ifndef LINEVIEW_H
#define LINEVIEW_H

#include <wx/wx.h>

class Line;

DECLARE_EVENT_TYPE(myEVT_LINEVIEW_CHANGED,-1)

class LineView: public wxWindow 
{
public:
	LineView(wxWindow* parent, const wxWindowID id);
	void setLine(Line *line);
private:
	static const int PAD = 20;
	Line *line;
	int   indices[2];
	int   sel;

	void OnPaint(wxPaintEvent& event);
	void OnLeftDown(wxMouseEvent& event);

	void transform(double x, double y, int& tx, int &ty);
	void inverseTransform(int tx, int ty, double& x, double &y);

	DECLARE_EVENT_TABLE()
};


#endif
