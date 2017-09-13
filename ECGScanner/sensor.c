#include "sensor.h"
#include <stdio.h>
#include <stdlib.h>

int getNextData(FILE *file)
{

	int nextData;
	if (fscanf(file, "%d", &nextData) == 1) {
		return nextData;
	}

	return 0; //end of file
}

FILE* openfile(const char* filename)
{
   FILE *file = fopen(filename,"r"); // Opens the requested file, if it exists
   return file;
}