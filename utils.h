#ifndef UTILS_H
#define UTILS_H

/// return the absolute value
template<class T> T abs(T a) { return a > 0 ? a : -a; }

/// return the minimum of two values
template<class T> T min(T a, T b) { return a < b ? a : b; }

/// return the maximum of two values
template<class T> T max(T a, T b) { return a > b ? a : b; }

/// return v clamped to the interval [a;b]
template<class T> T clamp(T v, T a, T b) { return v < a ? a : v > b ? b : v; }

/// same as max(v,b)
template<class T> T clampLow(T v, T b) { return max(v,b); }

/// same as min(v,b)
template<class T> T clampHigh(T v, T b) { return min(v,b); }

template<class T> void mySwap( T& a, T& b) {
	T t(a);
	a = b;
	b = t;
}


#endif
