#include "filters.h"


int nCalc(int n) {
	if (n < 0) {
		return 40 + n;
		//return (40 + n) % 40;
	}
	return n;
}

void lowPassFilter(int n, int *x, int *y) {
	//y[n] = 2 * y[nCalc(n-1)] - y[nCalc(n-2)] + (x[n] - 2*x[nCalc(n-6)]+x[nCalc(n-12)])/32;
	y[n] = (2 * (y[nCalc(n - 1)]-7512) - (y[nCalc(n - 2)] - 2*7512)) + (x[n] - ((2 * x[nCalc(n - 6)])) + (x[nCalc(n - 12)])) / 32;
	//y[n] = ((2 * y[nCalc(n - 1)] - y[nCalc(n - 2)])+5000 + (x[n] - 2 * x[nCalc(n - 6)] + x[nCalc(n - 12)]) / 32)-5000;
	//y[n%40] = (2 * y[nCalc(n%40 - 1)] - y[nCalc(n%40 - 2)] + (x[n%40] - 2 * x[nCalc(n%40 - 6)] + x[nCalc(n%40 - 12)]) / 32)-n;
	//y[n] = (2 * y[nCalc(n - 1)] - y[nCalc(n - 2)] + (x[n] - 2 * x[nCalc(n - 6)] + x[nCalc(n - 12)]) / 32);
	//Please send halp
	
}


void highPassFilter(int n, int *x, int *y) {
	y[n] = y[nCalc(n - 1)] - x[n]/32+x[nCalc(n-16)]- x[nCalc(n - 17)]+x[nCalc(n-32)]/32;
	//y[n] = y[nCalc(n - 1)] - ((x[n]-5000 )/ (32 + x[nCalc(n - 16)]-5000)) - x[nCalc(n - 17)]-5000 + ((x[nCalc(n - 32)] - 5000)/ 32);
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