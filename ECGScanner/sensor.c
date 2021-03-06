#include "sensor.h"
#include <stdio.h>
#include <stdlib.h>

int getNextData(FILE *file)
{
	int nextData;
	if (fscanf(file, "%d", &nextData) != EOF) { //Read the next datapoint unless it is the end of the file
		return nextData;
	}

	return INT_MIN; //end of file
}

FILE* openfile(const char* filename)
{
   FILE *file = fopen(filename,"r"); // Opens the requested file, if it exists
   return file;
}