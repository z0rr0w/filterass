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
	//file = openfile("ECG.txt");

	FILE *lowPassFile = fopen("lowPassOutput.txt","w");
	FILE *highPassFile = fopen("highPassOutput.txt","w");
	FILE *derFile = fopen("derOutput.txt","w");
	FILE *sqrFile = fopen("sqrOutput.txt","w");
	FILE *mwiFile = fopen("mwiOutput.txt", "w");


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
                                
		lowPassFilter(n % 40,preLow, postLow);            // Filter Data
		fprintf(lowPassFile, "preLow: %d \t postLow: %d \n",preLow[n % 40], postLow[n % 40]);
		highPassFilter(n % 40, postLow, postHigh);
		fprintf(highPassFile, "postLow: %d \t postHigh: %d \n", postLow[n%40], postHigh[n % 40]);
		derivativeFilter(n % 40, postHigh, postDer);
		fprintf(derFile, "postHigh: %d \t postDer %d \n", postHigh[n%40], postDer[n % 40]);
		sqrFilter(n % 40, postDer, postSqr);
		fprintf(sqrFile, "postDer: %d \t postSqr: %d \n", postDer[n%40], postSqr[n % 40]);
		mwiFilter(n % 40, postSqr, postMWI);
		fprintf(sqrFile, "postSqr: %d \t postMwi: %d \n", postSqr[n % 40], postMWI[n % 40]);

		peakDetection(&qsr_params,postMWI,n); // Perform Peak Detection

		
		//printf("%d\n", postLow[n % 40]); //for testing

		n++;
		
	}
	fclose(lowPassFile);
	fclose(highPassFile);
	fclose(derFile);
	fclose(sqrFile);
	fclose(mwiFile);
	getchar();
	
	return 0;
}
