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
public:
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

        int numSamples = Algorithm::sampleDataSize;
        const int SAMPLES_PER_INTERVAL = 1024;
        const int TRAILING_AVERAGE_LENGTH = 10;
        // approximately 200 BPM max
        const int MIN_AVERAGES_BETWEEN = 50;
        float ENERGY_CUTOFF = 2.0;

        // the average of every SAMPLES_PER_INTERVAL samples will be recorded for analysis
        int *interval_averages = new int[numSamples / SAMPLES_PER_INTERVAL];
        unsigned char *sample_buffer = Algorithm::sampleData;
        unsigned short sample;

        for(int i = 0; i < numSamples/SAMPLES_PER_INTERVAL; i++) {
            for(int j = 0; j < SAMPLES_PER_INTERVAL; j++) {
                sample = sample_buffer[j*2];
                interval_averages[i] += sample;
            }
            interval_averages[i] /= SAMPLES_PER_INTERVAL;
        }

        int trailing_average = 0;
        int last_sample_index = 0;

        if(numSamples/SAMPLES_PER_INTERVAL > TRAILING_AVERAGE_LENGTH) {
            // start the history
            for(int i = 0; i < TRAILING_AVERAGE_LENGTH; i++)
                trailing_average += interval_averages[i];

            // If instantEnergy / samples in buffer < instantEnergy for the next sample then add beatmarker.
            for(int i = TRAILING_AVERAGE_LENGTH; i < numSamples/SAMPLES_PER_INTERVAL; i++)
            {
                if(i - last_sample_index > MIN_AVERAGES_BETWEEN &&
                        interval_averages[i + 1] > (trailing_average / TRAILING_AVERAGE_LENGTH) * ENERGY_CUTOFF) {
                    beatOutput->push_back(float(i)/(numSamples/SAMPLES_PER_INTERVAL));
                    last_sample_index = i;
                }
                trailing_average -= interval_averages[i - TRAILING_AVERAGE_LENGTH];
                trailing_average += interval_averages[i + 1];
            }
        }

        // free memory
        delete[] interval_averages;
    }
};

#endif
