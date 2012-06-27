/*
	BeatDetector works as a beat detection algorithm tester for Fist Pump Revolution!
*/

#include <stdio.h>
#include <iostream>
#include <phonon>
#include <phonon/mediaobject.h>

#include "Decoder/MP3Decoder.h"
#include "Algorithm/Algorithm.h"

static const int NUM_ALGOS = 1;

int main(int argc, char **argv)
{
	// decode the mp3
	MP3Decoder d = MP3Decoder();
	d.loadFile(argv[1]);
	unsigned char * samples = d.getSampleBuffer();
	int numSamples = d.getSampleBufferSize();

	// make an array of algos
	Algorithm* algos[NUM_ALGOS];

	// set different algos
	algos[0] = new SimplePowerHistory();

	/*for(int i = 0; i < NUM_ALGOS; i++) {
		Algorithm::setSampleBuffer(samples, numSamples);
		algos[i]->process();
	}*/

	Phonon::MediaObject *music =
		Phonon::createPlayer(Phonon::MusicCategory,
			Phonon::MediaSource(argv[1]));
	music->play();

	std::cin.get();

	return EXIT_SUCCESS;
}
