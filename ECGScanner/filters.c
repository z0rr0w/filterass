#include "filters.h"

int nCalc(int n) {
	if (n < 0) {
		return 40 + n;
	}
	return n;
}

void lowPassFilter(int n, int *x, int *y, int *min) {
	//Writing by Jacob Bruun (s164410)

	int y1 = y[nCalc(n - 1)] + *min;
	int y2 = y[nCalc(n - 2)] + *min;
	

	y[n] = 2 * y1 - y2 + (x[n] - 2 * x[nCalc(n - 6)] + x[nCalc(n - 12)]) / 32 - *min;
	if (n == 34) {
		*min = y[0];
		for (int i = 0; i < 40; i++) { //Find minimum in array
			if (y[i] < *min)
			{
				*min = y[i];
			}
		}
		for (int i = 0; i < 40; i++)
		{ //Subtract minimum from all elements in array;
			if (y[i] - *min >= 0) {
				y[i] = y[i] - *min;
			}
		}
	}
	//y[n] = 2 * y[nCalc(n-1)] - y[nCalc(n-2)] + (x[n] - 2*x[nCalc(n-6)]+x[nCalc(n-12)])/32;
	//Better to just calculate the the differences that are use in the highPassFilter. These are
	//relative and will not go towards INT_MIN.

	
}


void highPassFilter(int n, int *x, int *y) {
	y[n] = y[nCalc(n - 1)] - x[n]/32+x[nCalc(n-16)]- x[nCalc(n - 17)]+x[nCalc(n-32)]/32;
}


void derivativeFilter(int n, int *x, int *y) {
	y[n] = (2 * x[n] + x[nCalc(n - 1)] - x[nCalc(n - 3)] - 2 * x[nCalc(n - 4)])/8;
}

void sqrFilter(int n, int *x, int *y) {
	int temp = x[n] * x[n];
	if (temp < 0) {
		y[n] = -temp;
	}
	else y[n] = temp;
}

void mwiFilter(int n, int *x, int *y) {
	int mwiWindow = 30;
	y[n] = 0;
	for (int i = 1; i <= mwiWindow; i++) {
		y[n] += x[nCalc(n -(mwiWindow - i))] / mwiWindow;
	}
	
}