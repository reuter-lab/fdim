#ifndef LINE_H
#define LINE_H


class Line
{
public:
	Line();

	// loescht alle Punkte
	void  clear();
	
	// gibt die Anzahl der Punkte zurueck
	int   getLen() const { return len; }

	// gibt zurueck ob die Menge der Punkte leer ist
	bool   isEmpty() const { return len == 0; }

	// Fuegt neuen Punkt hinzu
	void add(double xi, double yi);

	// berechnet Ausgleichsgerade für die Punkte tfirst ... tlast
	void calc(int tfirst, int tlast);

	int getfirst() const  {return first;}
	int getlast()  const {return last;}

	// Gibt die Steigung aus
	double slope() { return m; }
		
	// Gibt den y-Achsen Abschnitt aus
	double intercept() { return b; }

	// Gibt den Correlation Coefficient aus
	double correlation() { return cc; }
	
	// bounding box
	double getminx() const  {return minx;} 
	double getminy() const  {return miny;}
	double getmaxx() const {return  maxx;} 
	double getmaxy() const  {return maxy;}

	double getx(int idx) const  { return x[idx]; }
	double gety(int idx) const  { return y[idx]; }

	// Berechnet f(x)
	double getf (double x) const {return (m*x+b);}


	int getindex(double xt,double yt,double e=0.01);
private:
	enum { MAX_LEN = 800 };
	double x[MAX_LEN];
	double y[MAX_LEN];
	int    len;
	double m, b;
	double cc;

	double minx, miny;
	double maxx, maxy;

	int first, last;

};

#endif
