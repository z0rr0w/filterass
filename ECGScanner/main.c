#include "sensor.h"
#include "filters.h"
#include "qsr.h"
#include <stdio.h>
#include <stdlib.h>

// Main function for organizing the program execution.
// The functions and object predefined are just for inspiration.
// Please change orden,names arguments to fit your solution.

int main()
{	
    QRS_params qsr_params;       // Instance of the made avaiable through: #include "qsr.h"
	FILE *file;                  // Pointer to a file object
	file = openfile("ECG.txt");

	
	int* preLow[40] = { 0 };
	int* postLow[40] = { 0 };
	int* postHigh[40] = { 0 };
	int* postDer[40] = { 0 };
	int* postSqr[40] = { 0 };
	int* postMWI[40] = { 0 };
	int n = 0;
	int nextData;

	while((nextData=getNextData(file))!= 9999){        // Read Data from Sensor
		preLow[n%40] = nextData;
                                
		lowPassFilter(n%40,preLow, postLow);            // Filter Data
		highPassFilter(n % 40, postLow, postHigh);
		derivativeFilter(n % 40, postHigh, postDer);
		sqrFilter(n % 40, postDer, postSqr);
		printf("%d\n", postSqr[n % 40]);



	    peakDetection(&qsr_params); // Perform Peak Detection


		++n;
	}
	getchar();
	return 0;
}
