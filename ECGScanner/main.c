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
	QRS_params qsr_params = { 0 };       // Instance of the made avaiable through: #include "qsr.h"
	qsr_params.NPKF = 0;
	qsr_params.SPKF = 0;
	qsr_params.THRESHOLD1 = 1000;
	qsr_params.THRESHOLD2 = 500;

	FILE *file;                  // Pointer to a file object
	file = openfile("ECG900K.txt");

	//


	
	int* preLow[40] = { 0 };
	int* postLow[40] = { 0 };
	int* postHigh[40] = { 0 };
	int* postDer[40] = { 0 };
	int* postSqr[40] = { 0 };
	int* postMWI[40] = { 0 };


	int n = 0;
	int nextData;

	fileSetup();


	while((nextData=getNextData(file))!= INT_MIN){        // Read Data from Sensor
		preLow[n%40] = nextData;
                                
		lowPassFilter(n%40,preLow, postLow);            // Filter Data
		highPassFilter(n % 40, postLow, postHigh);
		derivativeFilter(n % 40, postHigh, postDer);
		sqrFilter(n % 40, postDer, postSqr);
		mwiFilter(n % 40, postSqr, postMWI);

		peakDetection(&qsr_params,postMWI,n); // Perform Peak Detection

		
		//printf("%d\n", postLow[n % 40]); //for testing


		++n;
	}
	getchar();
	
	return 0;
}
