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
#define filterCap 40
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
	FILE *timeFile = fopen("timeOutput.text", "w");




	int minLowPass = 0;
	
	int* preLow[filterCap] = { 0 };
	int* postLow[filterCap] = { 0 };
	int* postHigh[filterCap] = { 0 };
	int* postDer[filterCap] = { 0 };
	int* postSqr[filterCap] = { 0 };
	int* postMWI[filterCap] = { 0 };



	int n = 0;
	int nextData;
	int nMod = n % filterCap;

	fileSetup();

	while((nextData=getNextData(file))!= INT_MIN){        // Read Data from Sensor
		preLow[nMod] = nextData;

		lowPassFilter(nMod,preLow, postLow, &minLowPass);            // Filter Data
		//fprintf(lowPassFile, "preLow: %d \t postLow: %d \n",preLow[nMod], postLow[nMod]);

		highPassFilter(nMod, postLow, postHigh);
		//fprintf(highPassFile, "postLow: %lf \t postHigh: %lf \n", postLow[nMod], postHigh[nMod]);

		derivativeFilter(nMod, postHigh, postDer);
		//fprintf(derFile, "postHigh: %d \t postDer %d \n", postHigh[nMod], postDer[nMod]);

		sqrFilter(nMod, postDer, postSqr);
		//fprintf(sqrFile, "postDer: %d \t postSqr: %d \n", postDer[nMod], postSqr[nMod]);


		mwiFilter(nMod, postSqr, postMWI);
		//fprintf(sqrFile, "postSqr: %d \t postMwi: %d \n", postSqr[nMod], postMWI[nMod]);

		peakDetection(&qsr_params,postMWI,n); // Perform Peak Detection
		n++;
		nMod = n % filterCap;
		
	}
	fclose(lowPassFile);
	fclose(highPassFile);
	fclose(derFile);
	fclose(sqrFile);
	fclose(mwiFile);
	getchar();
	
	return 0;
}
