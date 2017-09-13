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

// Feel free to change and add methods
void peakDetection(QRS_params *params);

#endif // QSR_H
