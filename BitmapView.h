#ifndef BITMAPVIEW_H
#define BITMAPVIEW_H

#include <wx/wx.h>
#include <wx/scrolwin.h>

class BitmapView : public wxScrolledWindow
{
public:
	BitmapView(wxWindow* parent, const wxWindowID id);
	void setImage(const wxImage& image);
	bool isActiveSelection();
	wxRect getSelection();
	
private:
	wxBitmap bitmap;
	bool     isDragging;
	bool     activeSelection;
	wxPoint  sel1, sel2;

	void OnEraseBackground(wxEraseEvent& event);
	void OnPaint(wxPaintEvent& event);
	void OnLeftDown(wxMouseEvent& event);
	void OnLeftUp(wxMouseEvent& event);
	void OnMotion(wxMouseEvent& event);
	
	DECLARE_EVENT_TABLE()
};

#endif
