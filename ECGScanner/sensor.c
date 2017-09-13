#include "sensor.h"
#include <stdio.h>
#include <stdlib.h>

int getNextData(FILE *file)
{

	int nextData;
	if (fscanf(file, "%d", &nextData) != EOF) {
		return nextData;
	}

	return 9999; //end of file
}

FILE* openfile(const char* filename)
{
   FILE *file = fopen(filename,"r"); // Opens the requested file, if it exists
   return file;
}