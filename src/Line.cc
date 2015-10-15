/*
 * Original Author: Martin Reuter
 * Copyright 2014 Martin Reuter
 * 
 * Terms and conditions for use, reproduction, distribution and contribution
 * are found in the file 'LICENSE'
 *
 */

#include <cmath>
#include "Line.h"

Line::Line() 
{
	clear();
}

void Line::clear()
{
	len=0;
}


void Line::add(double xi, double yi)
{
	// TODO:
	if(len >= MAX_LEN)
		return;

	if( len == 0 ) {
		minx=xi; miny=yi;
		maxx=xi; maxy=yi;
	}else {
		if (xi < minx) minx=xi;
		if (yi < miny) miny=yi;
		if (xi > maxx) maxx=xi;
		if (yi > maxy) maxy=yi;
	}
	x[len]=xi; y[len]=yi; 
	++len;
}


void Line::calc(int first_, int last_)
{
	// TODO:
	first = first_;
	last  = last_;
	if(first < 0)
		first = 0;
	else if(first >= len)
		first = len - 1;
	if(last < 0)
		last = 0;
	else if(last >= len)
		last = len - 1;

	double xsum = 0, ysum = 0, xysum=0, xxsum=0;
	for (int i = first; i <= last; ++i)	{
		xsum += x[i];
		ysum += y[i];
		xysum += x[i] * y[i];
		xxsum += x[i] * x[i];
	}
	int anz = last - first + 1;
	double xm = xsum / anz; 
	double ym = ysum / anz;
	m = (xysum - xsum * ym) / (xxsum - xsum * xm);
	b = ym - m * xm;

	// calc cc
	double ytm = 0;
	for (int i = first; i<=last; ++i) 
		ytm += m * x[i] + b;
	ytm /= anz;

	double sumyt = 0, sumy = 0;
	for (int i = first; i <= last; ++i) {
		double yt = m * x[i] + b;
		sumyt += (yt - ytm) * (yt - ytm);
		sumy  += (y[i] - ytm) * (y[i] - ytm);
	}
	cc = sqrt(sumyt/sumy);

}


int Line::getindex(double xt, double yt, double e)
{
	double min = (x[0]-xt)*(x[0]-xt) + (y[0]-yt)*(y[0]-yt);
	int    temp = 0;
	for (int i = 1; i < len; i++)	{
		double d = (x[i]-xt)*(x[i]-xt) + (y[i]-yt)*(y[i]-yt);
		if (d < min) {
			min = d;
			temp = i;
		}
	}
	
	if (min > e)
		temp = -1;
	
	return temp;
}
