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


void highPassFilter(int n, int *x, int *y){

	float temp = y[nCalc(n - 1)] - x[n]*0.03125f+x[nCalc(n-16)]- x[nCalc(n - 17)]+x[nCalc(n-32)]*0.03125f;
	y[n] = (int)temp;





}


void derivativeFilter(int n, int *x, int *y) {

	float temp = 0.125f*(2 * x[n] + x[nCalc(n - 1)] - x[nCalc(n - 3)] - 2 * x[nCalc(n - 4)]);
	y[n] = (int)temp;


}

void sqrFilter(int n, int *x, int *y)
{
	int temp = x[n]* x[n];
	y[n] = temp;


}



