/*
	BeatDetector works as a beat detection algorithm tester for Fist Pump Revolution!
*/

#include <stdio.h>

#include "Decoder/MP3Decoder.h"

int main(int argc, char **argv)
{
	MP3Decoder d = MP3Decoder();
	d.loadFile(argv[1]);
	short * samples = d.getSampleBuffer();

	return EXIT_SUCCESS;
}
