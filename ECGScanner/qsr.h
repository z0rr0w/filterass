#include <stdio.h>
#ifndef QSR_H
#define QSR_H

// Header file for QRS functionality 
// it is recommended to use the structure "QRS_parameters"
// to organize all variables and parameters

typedef struct QRS_params
{ // Structure for QRS parameters
   int SPKF;
   int NPKF; 
   int THRESHOLD1;
   int THRESHOLD2;
   // Add parameters that are missing
   // or of use to you
   
} QRS_params;

typedef struct peakTuple {
	int peakVal; // or char strVal[20];
	int peakPos;
} peakTuple;

// Feel free to change and add methods
void peakDetection(QRS_params *, int*, int);

void fileSetup();
void fileClose();

int rCalc(int n);
int avg1();
int avg2();
peakTuple searchBack(QRS_params *params);

#endif // QSR_H
