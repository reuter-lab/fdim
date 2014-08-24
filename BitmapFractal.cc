#include "BitmapFractal.h"
#include "Line.h"

using namespace std;

BitmapFractal::BitmapFractal()
	: wxImage()
{
    //AddHandler(new wxPNGHandler);
}

void BitmapFractal::convertToGrey()
{
    unsigned char *data = GetData();
    int size = GetHeight() * GetWidth();
    for (int i = 0; i < size; i++) {
		unsigned char grey = (data[i * 3] + data[i * 3 + 1] + data[i * 3 + 2]) / 3;
		data[i * 3] = grey;
		data[i * 3 + 1] = grey;
		data[i * 3 + 2] = grey;
    }
}

void BitmapFractal::setSelection(const wxRect& sel)
{
	selection = sel;
}

inline int BitmapFractal::getWidth()
{
	return selection.width;
}


inline int BitmapFractal::getHeight()
{
	return selection.height;
}

inline unsigned char BitmapFractal::getGray(int x, int y)
{
	return GetRed(selection.x+x,selection.y+y);
}



double BitmapFractal::capacity(const wxRect& sel, Line& L, int lmax, int step)
{
	setSelection(sel);
	// Laeuft ueber Gitter aus Boxen und Potenzen von 2
	// ueber Hoehe (max-min) wird Boxcount N berechnet
    printf("Calculating Capactiy Dimension:\n");
    double N;
    int boxsize = 2;
    int max, min;
    register int k, l, m, n;
    lmax = 64;
    step = 2;
    int hn = int (getWidth() / lmax) * lmax;	// neue Hoehe ohne Rand
    int wn = int (getHeight() / lmax) * lmax;	// neue Breite ohne Rand
    int i;
    printf("Calculating over h=%d and w=%d\n", hn, wn);
	L.clear();
    for (boxsize = 2; boxsize <= lmax; boxsize *= step) {
		N = 0;
		i = 0;
		for (k = 0; k <= hn - boxsize; k += boxsize) {
			for (l = 0; l <= wn - boxsize; l += boxsize) {
				max = min = getGray(l, k);
				for (m = k; m < k + boxsize; m++) {
					for (n = l; n < l + boxsize; n++) {
						if (getGray(n, m) > max)
							max = getGray(n, m);
						if (getGray(n, m) < min)
							min = getGray(n, m);
					}
				}
				N += (max - min) / boxsize + 1;	//anz boxen in int aufgerundet
				i++;		// zaehlt Quadrate auf Grundflaeche
			}
		}
		printf("Quadrate: %5d ", i);
		printf("N=%7.1f and -ln(N)=%10.6f for boxsize %3d\n", N,
			   -log((double) N), boxsize);
		L.add(log((double) boxsize), -log((double) N));
    }
	L.calc(1,40);
    double D;
	D = L.slope();
    fractdim = D;
    printf("\nSlope (Fract. Dim) is %f with correlation %f\n", D,
		   L.correlation());
    return fractdim;
}


double BitmapFractal::information(const wxRect& sel, Line& L, int lmax, int step)
{
	setSelection(sel);
	// Laeuft ueber Gitter aus Boxen und Potenzen von 2
	// inf: Information ueber die Verteilung der Punkte auf die Boxen
	// ueber freq(rel. Haeufigkeit) wird inf=freq*log(freq) berechnet
	// boxsize^2 Pixel werden auf je N Boxen gemittelt
    printf("Calculating Information Dimension:\n");
    double inf;
    double freq;
    double N;
    int boxsize = 2;
    int max, min;
    lmax = 64;
    step = 2;
    register  int k, l, m, n;
    int hn = int (getWidth() / lmax) * lmax;	// neue Hoehe ohne Rand
    int wn = int (getHeight() / lmax) * lmax;	// neue Breite ohne Rand
    printf("Calculating over h=%d and w=%d\n", hn, wn);
	L.clear();
    double loghw = log((double) (hn * wn));
    for (boxsize = 2; boxsize <= lmax; boxsize *= step) {
		inf = 0;
		int te = 0;
		for (k = 0; k <= hn - boxsize; k += boxsize) {
			for (l = 0; l <= wn - boxsize; l += boxsize) {
				max = min = getGray(l, k);
				for (m = k; m < k + boxsize; m++) {
					for (n = l; n < l + boxsize; n++) {
						if (getGray(n, m) > max)
							max = getGray(n, m);
						if (getGray(n, m) < min)
							min = getGray(n, m);
					}
				}
				N = (max - min) / boxsize + 1;	//anz boxen in int aufgerundet
				te += int (N);
				freq = (boxsize * boxsize);	// durchschnittl. Anzahl Punkte in Box
				inf += freq * (log(freq / N) - loghw);	//Summe bezogen auf N Boxen
			}
		}
		printf("(N: %5d) ", te);
		// te summiert die Boxen auf. Hier liesse sich die Cap.Dim berrechnen !!!
		inf = inf / double (hn * wn);	// bezogen auf alle Punkte
		printf("inf=%10.6f for boxsize %3d\n", inf, boxsize);
		L.add(log((double) boxsize), inf);
    }
	L.calc(1,40);
    double D;
    D = L.slope();
    fractdim = D;
    printf("\nSlope (Fract. Dim) is %f with correlation %f\n", D,
		   L.correlation());
    return fractdim;
}

double BitmapFractal::correlation(const wxRect& sel, Line& L, int lmax, int step)
{
	setSelection(sel);
	// Laeuft ueber Gitter aus Boxen und Potenzen von 2
	// inf: Information ueber die Verteilung der Punkte auf die Boxen
	// ueber freq(rel. Haeufigkeit) wird freq*freq berechnet
	// boxsize^2 Pixel werden auf je N Boxen gemittelt
    printf("Calculating Correlation Dimension:\n");
    double sqrfreq;
    double freq;
    double N;
    int boxsize = 2;
    int max, min;
    lmax = 64;
    step = 2;
    register int k, l, m, n;
    int hn = int (getWidth() / lmax) * lmax;	// neue Hoehe ohne Rand
    int wn = int (getHeight() / lmax) * lmax;	// neue Breite ohne Rand
    printf("Calculating over h=%d and w=%d\n", hn, wn);
	L.clear();
    for (boxsize = 2; boxsize <= lmax; boxsize *= step) {
		sqrfreq = 0;
		int te = 0;
		for (k = 0; k <= hn - boxsize; k += boxsize) {
			for (l = 0; l <= wn - boxsize; l += boxsize) {
				max = min = getGray(l, k);
				for (m = k; m < k + boxsize; m++) {
					for (n = l; n < l + boxsize; n++) {
						if (getGray(n, m) > max)
							max = getGray(n, m);
						if (getGray(n, m) < min)
							min = getGray(n, m);
					}
				}
				N = (max - min) / boxsize + 1;	//anz boxen in int aufgerundet
				te += int (N);
				freq = (boxsize * boxsize);	// durchschnittl. Anzahl Punkte in Box
				sqrfreq += freq * freq / N;	// bez auf N Boxen
			}
		}
		printf("(N: %5d) ", te);
		// te summiert die Boxen auf. Hier liesse sich die Cap.Dim berrechnen !!!
		sqrfreq = sqrfreq / double (hn * wn);	// bezogen auf alle Abstaende (Punkte^2)
		sqrfreq = sqrfreq / double (hn * wn);
		printf("sqrfreq=%10.6f for boxsize %3d\n", sqrfreq, boxsize);
		L.add(log((double) boxsize), log(sqrfreq));
    }
	L.calc(1,40);
    double D;
    D = L.slope();
    fractdim = D;
    printf("\nSlope (Fract. Dim) is %f with correlation %f\n", D,
		   L.correlation());
    return fractdim;
}


double BitmapFractal::probability(const wxRect& sel, Line& L)
{
	setSelection(sel);
	printf("\nCalculating Probability Dimension:\n");
    int lmax = 31;
    int l, m = 0;
    double gewsum = 0;
    int x, y, xt, yt, r;
    // hier kann ein Bildausschnitt definiert werden
    int h1 = getWidth();
    int w1 = getHeight();
	printf("Calculating over h=%d and w=%d\n", h1, w1);
	printf("\n   l     log(l)        N(l)        -log(N(l))\n");
	printf("==================================================\n");
	L.clear();
    // Laufe ueber alle Punkte und alle l
    for (l = 1; l <= lmax; l += 2) {
		r = (l - 1) / 2;
		for (y = (lmax - 1) / 2; y < h1 - (lmax - 1) / 2; y++)
			for (x = (lmax - 1) / 2; x < w1 - (lmax - 1) / 2; x++) {
				// Laufe ueber jeden Punkt in Box um (x,y,f(x,y))
				m = 0;
				for (yt = y - r; yt <= y + r; yt++)
					for (xt = x - r; xt <= x + r; xt++) {
						// Zaehle Punkte in Box
						if ((getGray(xt, yt) <= getGray(x, y) + r)
							&& (getGray(xt, yt) >= getGray(x, y) - r))
							m++;
					}
				gewsum += (1 / double (m));
			}
		printf(" %3d    %f   %12.6f    %10.6f  \n", l, log((double) l),
			   gewsum, -log(gewsum));
		L.add(log((double) l), -log(gewsum));
		gewsum = 0;
    }
	L.calc(2,15);
    fractdim = L.slope();
	printf("Fractaldim  [5-31] ist %f\n", L.slope());
	printf("Correlation [5-31] ist %f\n", L.correlation());
    return fractdim;
}
