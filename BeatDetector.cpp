/*
	BeatDetector works as a beat detection algorithm tester for Fist Pump Revolution!
*/

#include <stdio.h>

#include "Decoder/MP3Decoder.h"
#include "Algorithm/Algorithm.h"

int main(int argc, char **argv)
{
	// decode the mp3
	MP3Decoder d = MP3Decoder();
	d.loadFile(argv[1]);
	unsigned char * samples = d.getSampleBuffer();
	int numSamples = d.getSampleBufferSize();

	// make an algorithm
	Algorithm a = Algorithm();
	Algorithm::setSampleBuffer(samples, numSamples);
	a.process();

	return EXIT_SUCCESS;
}
