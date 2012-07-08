#ifndef SIMPLEPOWERHISTORY_H
#define SIMPLEPOWERHISTORY_H

#include "Algorithm.h"

// first try at the algorithm to detect teh beatz
class SimplePowerHistory : public Algorithm {
private:
    float cutoff;
public:
    SimplePowerHistory(float);
    virtual void process();
};

#endif // SIMPLEPOWERHISTORY_H
