#include "qsr.h"
#include <stdio.h>
#include <stdlib.h>

struct peakTuple PEAKS[512];
struct peakTuple rPeaks[16]; //Dividable by 8. Important when inserting in RecentRR
int peakCount = 0,
rPeakCount = 0,
peakX1 = 0,
peakX2 = 0,
RR = 0,
RR_LOW = -1,
RR_HIGH = INT_MAX,
RR_Average1 = 0,
RR_Average2 = 0,
RR_MISS = 180,
rrMissCount = 0;
FILE *output ;
FILE *mwiOut;
FILE *thresholds;
FILE *irreg;
FILE *searchBackFile;

void addToPeaks(peakTuple peak) {
	if (peakCount == sizeof(PEAKS)/sizeof(PEAKS[0])){
		peakCount = 0;
	}
	PEAKS[peakCount] = peak;
}

void addToRPeaks(peakTuple peak) {
	if (rPeakCount == sizeof(rPeaks) / sizeof(rPeaks[0])) {
		rPeakCount = 0;
	}
	rPeaks[rPeakCount] = peak;
}

int RecentRR[8] = { 0 }; 
int RecentRR_OK[8] = { 0 };

void fileSetup() { //Opens files for testing purposes
	output = fopen("Output.txt", "w");
	mwiOut = fopen("mwiOut.txt", "w");
	thresholds = fopen("threshholds.txt", "w");
	irreg = fopen("irreg.txt", "w");
	searchBackFile = fopen("searchBack.txt", "w");

}
void fileClose() { //Closes the test files
	fclose(output);
	fclose(mwiOut);
	fclose(thresholds);
	fclose(irreg);
	fclose(searchBackFile);
}

void printInfo() { //Prints info to the user
	int bpm = 60;
	bpm = (60 * 250) / avg2();
	
	printf("TimeValue: %d \t PeakValue: %d \t Pulse: %d \n", rPeaks[rPeakCount].peakPos, rPeaks[rPeakCount].peakVal,bpm);
	if (rPeaks[rPeakCount].peakVal < 2000) {
		printf("WARNING! LOW BLOOD PRESSURE DETECTED!\n");
		
	}
	if (rrMissCount >= 5) {
		printf("WARNING! IRREGULAR HEARTRATE DETECTED! \n");
		fprintf(irreg, "%d %d\n", rPeaks[rPeakCount].peakPos, 0);
	}
}


int rCalc(int n) {
	if (n < 0) {
		return 40 + n;
	}
	return n;
}

int avg1() {		//Averages RecentRR
	int ret=0;
	for (int i = 0; i < 8; i++) {
		ret += RecentRR[i];
	}
	return ret / 8;
}

int avg2() {		//Averages RecentRR_OK
	int ret = 0;
	for (int i = 0; i < 8; i++) {
		ret += RecentRR_OK[i];
	}
	return ret / 8;
}

peakTuple searchBack(QRS_params *params) {
	for (int i = peakCount-1; i >= 0; i--) {
		if (PEAKS[i].peakVal > (*params).THRESHOLD2)
			return PEAKS[i];
	}
	peakTuple error = { -1 };
	return error;
}

void peakDetection(QRS_params *params, int* postMWI, int n)
{

	fprintf(mwiOut, "%d \n",postMWI[n % 40]);
	fprintf(thresholds, "%d %d\n", (*params).THRESHOLD1, (*params).THRESHOLD2);

	if (peakX2 > peakX1 && peakX2 > postMWI[n % 40]) {  //if a peak is found, save value and position (position being the n'th data input)
		peakTuple peak;
		peak.peakPos = n;
		peak.peakVal = peakX2;
		addToPeaks(peak);
		
		if (peakX2 > (*params).THRESHOLD1) { //Check wheter the found peak is above threshold 1
			if (rPeakCount == 0) {
				RR = PEAKS[peakCount].peakPos;
				for (int i = 0; i < 8; i++) {
					RecentRR[i] = RR;
				}
				for (int i = 0; i < 8; i++) {
					RecentRR_OK[i] = RR;
				}
			}
			else {
				RR = PEAKS[peakCount].peakPos - rPeaks[rPeakCount-1].peakPos;
			}
			if (RR_LOW < RR && RR < RR_HIGH) {
				rrMissCount = 0;//An RR was between RR_LOW and RR_HIGH and the missCount is reset
				addToRPeaks(PEAKS[peakCount]);

				fprintf(output, "%d %d\n", rPeaks[rPeakCount].peakPos, rPeaks[rPeakCount].peakVal);
				printInfo();

				//Update the parameters
				rPeakCount++;
				(*params).SPKF = PEAKS[peakCount].peakVal/8 + 7*(*params).SPKF/8;
				RecentRR[rCalc((rPeakCount - 1) % 8)] = RR;
				RecentRR_OK[rCalc((rPeakCount - 1) % 8)] = RR;
				RR_Average2 = avg2();
				RR_Average1 = avg1();
				RR_LOW = 92 * RR_Average2 / 100;
				RR_HIGH = 116 * RR_Average2 / 100;
				RR_MISS = 166 * RR_Average2 / 100;
				(*params).THRESHOLD1 = (*params).NPKF + ((*params).SPKF - (*params).NPKF)/4;
				(*params).THRESHOLD2 = (*params).THRESHOLD1 / 2;
			}
			else {
				rrMissCount++; //RR missed both RR_LOW and RR_HIGH
				if (RR > RR_MISS) {
					peakTuple peak2 = searchBack(params); //Perform searchback
					fprintf(searchBackFile, "%d \n", n);
					if (peak2.peakVal != -1) {
						addToRPeaks(peak2);
						fprintf(output, "%d %d\n", rPeaks[rPeakCount].peakPos, rPeaks[rPeakCount].peakVal);
						printInfo();
						//Update parameters
						(*params).SPKF =  peak2.peakVal/4 + 3*(*params).SPKF/4;
						RR = peak.peakPos - peak2.peakPos;
						RecentRR[rCalc(rPeakCount % 8)] = RR;
						rPeakCount++;
						RR_Average1 = avg1();
						RR_LOW = 92 * RR_Average1 / 100;
						RR_HIGH = 116 * RR_Average1 / 100;
						RR_MISS = 166 * RR_Average1 / 100;
						(*params).THRESHOLD1 = (*params).NPKF + ((*params).SPKF - (*params).NPKF) /4;
						(*params).THRESHOLD2 = (*params).THRESHOLD1 / 2;
					}
				}
			}
		}
		else {
			//No peaks were found. Update params
			(*params).NPKF = (125 * peakX2 + 875 * (*params).NPKF) / 1000;
			(*params).THRESHOLD1 = (*params).NPKF + ((*params).SPKF - (*params).NPKF)*25/100;
			(*params).THRESHOLD2 = (*params).THRESHOLD1 / 2;
		}
		peakCount++;
	}
	peakX1 = peakX2; //rearrange numbers to find peaks
	peakX2 = postMWI[n % 40];
}