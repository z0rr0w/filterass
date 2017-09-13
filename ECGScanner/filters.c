#include "filters.h"

// Feel free to change return statement and arguments

int nCalc(int n) {
	if (n < 0) {
		return 40 + n;
	}
	return n;
}

void lowPassFilter(int n, int *x, int *y)
{
	
	float temp = (2 * y[nCalc(n-1)]) - y[nCalc(n-2)] + 0.03125f*(x[n] - (2*x[nCalc(n-6)])+x[nCalc(n-12)]);
	y[n] = (int)temp;
	




}
