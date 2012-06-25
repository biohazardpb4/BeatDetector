#include <stdio.h>

#include "Algorithm.h"

Algorithm::Algorithm() {
	printf("Algorithm created");
}

Algorithm::~Algorithm() {
	printf("Algorithm destroyed");
}

unsigned char * Algorithm::sampleData = NULL;
long Algorithm::sampleDataSize = 0;

void Algorithm::setSampleBuffer(unsigned char * buf, long size) {
	sampleData = buf;
	sampleDataSize = size;
}

void Algorithm::process() {
	for(int i = 0; i < sampleDataSize; i++)
		printf("algo sample: %d is %d\n", i, sampleData[i]);	
}
