#include "filters.h"

int nCalc(int n) { //Make sure we address the right index
	if (n < 0) {
		return 40 + n;
	}
	return n;
}

void lowPassFilter(int n, int *x, int *y, int *min) {

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
	//y[n] = 2 * y[nCalc(n-1)] - y[nCalc(n-2)] + (x[n] - 2*x[nCalc(n-6)]+x[nCalc(n-12)])/32; This is deprecated
	//Better to just calculate the the differences that are use in the highPassFilter. These are
	//relative and will not go towards INT_MIN.

	
}


void highPassFilter(int n, int *x, int *y) { //Calculate high pass filter after the specifications
	y[n] = y[nCalc(n - 1)] - x[n]/32+x[nCalc(n-16)]- x[nCalc(n - 17)]+x[nCalc(n-32)]/32;
}


void derivativeFilter(int n, int *x, int *y) { //Calculate derivative filter after the specifications
	y[n] = (2 * x[n] + x[nCalc(n - 1)] - x[nCalc(n - 3)] - 2 * x[nCalc(n - 4)])/8;
}

void sqrFilter(int n, int *x, int *y) { //Square the data and make sure the output is not negativ 
	int temp = x[n] * x[n];
	if (temp < 0) {
		y[n] = -temp;
	}
	else y[n] = temp;
}

void mwiFilter(int n, int *x, int *y) { //Moving-Window-Integration. Dividing in every step
	int mwiWindow = 30;
	y[n] = 0;
	for (int i = 1; i <= mwiWindow; i++) {
		y[n] += x[nCalc(n -(mwiWindow - i))] / mwiWindow;
	}
	
}