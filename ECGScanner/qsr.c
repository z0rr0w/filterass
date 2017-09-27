#include "qsr.h"
#include <stdio.h>
#include <stdlib.h>


struct peakTuple PEAKS[5000];
struct peakTuple rPeaks[5000];
int peakCount = 0,
rPeakCount = 0,
peakX1 = 0,
peakX2 = 0,
RR = 0,
RR_LOW = 0,
RR_HIGH = 0,
RR_Average1 = 0,
RR_Average2 = 0,
RR_MISS = 0;

int RecentRR[8] = { 0 };
int RecentRR_OK[8] = { 0 };

//int NPKF = 0;
//int SPKF = 0;

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
	for (int i = peakCount; i > 0; i--) {
		if (PEAKS[i].peakVal > (*params).THRESHOLD2)
			return PEAKS[i];
	}
	peakTuple error = { -1 };
	return error;
	//IF INFINITE LOOP CHANGE RETURN STATEMENT TO HANDLE MISSING PEAK
}

void peakDetection(QRS_params *params, int* postMWI, int n)
{
	if (peakX2 > peakX1 && peakX2 > postMWI[n % 40]) {  //if a peak is found, save value and position (position being the n'th data input)
		PEAKS[peakCount].peakPos = n;
		PEAKS[peakCount].peakVal = peakX2;
		if (peakX2 > (*params).THRESHOLD1) { //remember to do fun stuff with threshold1 it needs to be calgulated.
			
			if (rPeakCount = 0) {
				RR = PEAKS[peakCount].peakPos;
				
			}
			else {
				RR = PEAKS[peakCount].peakPos - rPeaks[rPeakCount].peakPos;
			}
			if (RR_LOW < RR && RR < RR_HIGH) {
				rPeaks[rPeakCount] = PEAKS[peakCount];
				//printf("%d\n", rPeaks[rPeakCount].peakPos);
				rPeakCount++;
				(*params).SPKF = (125 * PEAKS[peakCount].peakVal + 875*(*params).SPKF)/ 1000;
				RecentRR[rCalc(rPeakCount%8)], RecentRR_OK[rCalc(rPeakCount%8)] = RR; //rCalc might be superfluous (redundant)
				RR_Average2 = avg2();
				RR_Average1 = avg1();
				RR_LOW = 92 * RR_Average2 / 100;
				RR_HIGH = 116 * RR_Average2 / 100;
				RR_MISS = 166 * RR_Average2 / 100;
				(*params).THRESHOLD1 = (*params).NPKF + 25 * ((*params).SPKF - (*params).NPKF) / 100;
				(*params).THRESHOLD2 = (*params).THRESHOLD1 / 2;
			}
			else {
				if (RR > RR_MISS) {
					peakTuple temp = searchBack(params);
					if (temp.peakPos != -1) {
						rPeaks[rPeakCount] = temp;
						printf("%d\n", rPeaks[rPeakCount].peakPos);
						(*params).SPKF = (25 * (temp.peakVal) + 75 * ((*params).SPKF)) / 100;
						RecentRR[rCalc(rPeakCount % 8)];
						rPeakCount++;
						RR_Average1 = avg1();
						RR_LOW = 92 * RR_Average1 / 100;
						RR_HIGH = 116 * RR_Average1 / 100;
						RR_MISS = 166 * RR_Average1 / 100;
						(*params).THRESHOLD1 = (*params).NPKF + 25 * ((*params).SPKF - (*params).NPKF) / 100;
						(*params).THRESHOLD2 = (*params).THRESHOLD1 / 2;
					}
				}
			}
		}
		else {
			(*params).NPKF = (125 * peakX2 + 875 * (*params).NPKF) / 1000;
			(*params).THRESHOLD1 = (*params).NPKF + ((*params).SPKF - (*params).NPKF)*25/100; //TODO: find out if it's smart to round before division.
			(*params).THRESHOLD2 = (*params).THRESHOLD1 / 2;
		}
		peakCount++;
	}
	peakX1 = peakX2; //rearrange numbers to find peaks
	peakX2 = postMWI[n % 40];
}
