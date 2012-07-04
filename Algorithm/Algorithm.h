/*
 * Algorithm.h
 *
 *  Created on: Jun 25, 2012
 *      Author: Jay
 */

#ifndef ALGORITHM_H_
#define ALGORITHM_H_

#include <stdio.h>

class Algorithm {
protected:
	static unsigned char * sampleData;
	static long sampleDataSize;
public:
	Algorithm();
	~Algorithm();

	// pass the sample buffer
	static void setSampleBuffer(unsigned char *, long);
	virtual void process() = 0;
};

// first try at the algorithm to detect teh beatz
class SimplePowerHistory : public Algorithm {
public:
	virtual void process() {
		for(int i = 0; i < sampleDataSize; i++)
            printf("power history algo: %d is %d\n", i, sampleData[i]);
	}
};

#endif
