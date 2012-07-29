#ifndef COMPRESSEDPOWERHISTORY_H
#define COMPRESSEDPOWERHISTORY_H

#include "Algorithm.h"

class CompressedPowerHistory : public Algorithm {
private:
    float cutoff;
    float compression;
    int trailingAverageLength;
public:
    CompressedPowerHistory(float, float, int);
    virtual void process();
};

#endif // COMPRESSEDPOWERHISTORY_H
