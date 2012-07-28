#include "SimplePowerHistory.h"

// first try at the algorithm to detect teh beatz
SimplePowerHistory::SimplePowerHistory(float cutoff, int trailingAverageLength):Algorithm("Simple Power History") {
    this->cutoff = cutoff;
    this->trailingAverageLength=trailingAverageLength;
}

void SimplePowerHistory::process() {

    printf("running algorithm: %s\n", this->name.c_str());

    int numSamples = Algorithm::getSampleDataSize();
    const int SAMPLES_PER_INTERVAL = 1024;
    int TRAILING_AVERAGE_LENGTH = trailingAverageLength;
    // approximately 200 BPM max
    const int MIN_AVERAGES_BETWEEN = 40;
    float ENERGY_CUTOFF = cutoff;

    // the average of every SAMPLES_PER_INTERVAL samples will be recorded for analysis
    float *interval_averages = new float[numSamples / SAMPLES_PER_INTERVAL];
    unsigned char *sample_buffer = Algorithm::getSampleData();
    short sample;
    int numIntervals = numSamples/SAMPLES_PER_INTERVAL;

    for(int i = 0; i < numIntervals; i++) {
        interval_averages[i] = 0;
        for(int j = 0; j < SAMPLES_PER_INTERVAL/2; j++) {
            sample = sample_buffer[SAMPLES_PER_INTERVAL*i + j*2];
            interval_averages[i] += sample > 0 ? sample : -sample;
        }
        interval_averages[i] /= SAMPLES_PER_INTERVAL;
        //printf("interval average for %d: %d\n", i, interval_averages[i]);
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
