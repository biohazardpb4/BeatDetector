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
private:
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
    static unsigned char* getSampleData();
    static long getSampleDataSize();
    virtual void process() = 0;
};

#endif
