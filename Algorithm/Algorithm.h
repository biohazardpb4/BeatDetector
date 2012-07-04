/*
 * Algorithm.h
 *
 *  Created on: Jun 25, 2012
 *      Author: Jay
 */

#ifndef ALGORITHM_H_
#define ALGORITHM_H_

#include <stdio.h>
#include <vector>
#include <string>

using namespace std;

class Algorithm {
protected:
	static unsigned char * sampleData;
    static long sampleDataSize;

public:
    Algorithm(string);
	~Algorithm();

    string name;
    std::vector<float>* beatOutput;

	// pass the sample buffer
    static void cleanup();
	static void setSampleBuffer(unsigned char *, long);
    virtual void process() = 0;
};

// first try at the algorithm to detect teh beatz
class SimplePowerHistory : public Algorithm {
public:
    SimplePowerHistory():Algorithm("Simple Power History") {}

    virtual void process() {
        printf("running algorithm: %s\n", this->name.c_str());
        for(int i = 0; i < 100; i++)
            beatOutput->push_back(float(i)/100);
	}
};

#endif
