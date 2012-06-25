/*
 * Algorithm.h
 *
 *  Created on: Jun 25, 2012
 *      Author: Jay
 */

#ifndef ALGORITHM_H_
#define ALGORITHM_H_

class Algorithm {
private:
	static unsigned char * sampleData;
	static long sampleDataSize;
public:
	Algorithm();
	~Algorithm();

	// pass the sample buffer
	static void setSampleBuffer(unsigned char *, long);
	virtual void process();
};

#endif
