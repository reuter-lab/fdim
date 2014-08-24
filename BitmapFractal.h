#ifndef BITMAPFRACTAL_H
#define BITMAPFRACTAL_H

#include <wx/wx.h>
class Line;


class BitmapFractal : public wxImage
{
public:
	BitmapFractal();
	void convertToGrey();
	double capacity(const wxRect& sel, Line& L, int lmax = 21, int step=1);
	double information(const wxRect& sel, Line& L, int lmax = 21, int step=1);
	double correlation(const wxRect& sel, Line& L, int lmax = 21, int step=1);
	double probability(const wxRect& sel, Line& L);
private:
	wxRect   selection;
	void setSelection(const wxRect& sel);
	int getWidth();
	int getHeight();
	unsigned char getGray(int x, int y);
	double fractdim;

};

#endif
