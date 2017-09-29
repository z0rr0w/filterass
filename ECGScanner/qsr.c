#include "qsr.h"
#include <stdio.h>
#include <stdlib.h>

struct peakTuple PEAKS[512];
struct peakTuple rPeaks[256]; //Dividable by 8. Important when inserting in RecentRR
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
FILE *mwiData;

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

void fileSetup() {
	output = fopen("Output.txt", "w");
	mwiData = fopen("mwiOut.txt", "w");
}
void fileClose() {
	fclose(output);
	fclose(mwiData);
}

void printInfo() {
	int diff;
	int bpm = 60;
	/*if (rPeakCount-1 >= 2) {
		diff = (rPeaks[rPeakCount].peakPos - rPeaks[rPeakCount - 4].peakPos) / (250);
		if (diff != 0) {
			bpm = (int)(240 / diff); // (2/diff)*60
		}
		
	} */
	int RRInSeconds = (avg2())*100 / (250);
	if (RRInSeconds != 0) {
		bpm = 6000 / RRInSeconds;
	}
	
	printf("TimeValue: %d \t PeakValue: %d \t Pulse: %d \n", rPeaks[rPeakCount].peakPos, rPeaks[rPeakCount].peakVal,bpm);
	if (rPeaks[rPeakCount].peakVal < 2000) {
		printf("WARNING! WEAK PULSE DETECTED!\n");
	}
	if (rrMissCount >= 5) {
		printf("WARNING! UNEVEN HEARTRATE DETECTED! \n");
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
	//IF INFINITE LOOP CHANGE RETURN STATEMENT TO HANDLE MISSING PEAK
}

void peakDetection(QRS_params *params, int* postMWI, int n)
{

	fprintf(mwiData, "%d %d\n",postMWI[n % 40],(*params).THRESHOLD1);
	if (peakX2 > peakX1 && peakX2 > postMWI[n % 40]) {  //if a peak is found, save value and position (position being the n'th data input)
		peakTuple temp;
		temp.peakPos = n;
		temp.peakVal = peakX2;
		addToPeaks(temp);
		//PEAKS[peakCount].peakPos = n;
		//PEAKS[peakCount].peakVal = peakX2;
		
		if (peakX2 > (*params).THRESHOLD1) { //remember to do fun stuff with threshold1 it needs to be calgulated.
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
				//rPeaks[rPeakCount] = PEAKS[peakCount];
				//printf("%d %d\n", rPeaks[rPeakCount].peakPos, rPeaks[rPeakCount].peakVal);
				fprintf(output, "%d %d\n", rPeaks[rPeakCount].peakPos, rPeaks[rPeakCount].peakVal);
				printInfo();
				rPeakCount++;
				(*params).SPKF = PEAKS[peakCount].peakVal/8 + 7*(*params).SPKF/8;
				RecentRR[rCalc((rPeakCount-1)%8)]=RR, RecentRR_OK[rCalc((rPeakCount-1)%8)] = RR; //rCalc might be superfluous (redundant)
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
					peakTuple temp = searchBack(params);
					if (temp.peakPos != -1) {
						addToRPeaks(temp);
						//rPeaks[rPeakCount] = temp;
						//printf("%d %d sb\n", rPeaks[rPeakCount].peakPos, rPeaks[rPeakCount].peakVal);
						fprintf(output, "%d %d\n", rPeaks[rPeakCount].peakPos, rPeaks[rPeakCount].peakVal);
						printInfo();
						(*params).SPKF =  (temp.peakVal)/4 + 3*(*params).SPKF/4;
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
			(*params).NPKF = (125 * peakX2 + 875 * (*params).NPKF) / 1000;
			(*params).THRESHOLD1 = (*params).NPKF + ((*params).SPKF - (*params).NPKF)*25/100; //TODO: find out if it's smart to round before division.
			(*params).THRESHOLD2 = (*params).THRESHOLD1 / 2;
		}
		peakCount++;
	}
	peakX1 = peakX2; //rearrange numbers to find peaks
	peakX2 = postMWI[n % 40];
	//printf("%d\n", postMWI[n % 40]);
}