/*
 * MP3Decoder.cpp
 *
 *  Created on: Jun 23, 2012
 *      Author: jay
 */
#include <stdio.h>

#include "MP3Decoder.h"

///////////////////////////// public functions below //////////////////////////

MP3Decoder::MP3Decoder() {
	// initialize the mp3 file
	mp3File.handle = NULL;
	mp3File.buffer = NULL;

	printf("MP3Decoder created\n");
}

MP3Decoder::~MP3Decoder() {
	this->closeFile();
}

void MP3Decoder::loadFile(char * fpath) {
	printf("loading %s\n", fpath);
}

short* MP3Decoder::getSampleBuffer() {
	printf("retrieving sample buffer\n");
	return 0;
}

///////////////////////////// end of public functions /////////////////////////
///////////////////////////// private functions below /////////////////////////

inline int MP3Decoder::readBuffer() {
	return 0;
}

inline long MP3Decoder::openFile(char *) {
	return 0;
}

// close the currently stored mp3 file
void MP3Decoder::closeFile() {
	if(mp3File.handle != NULL) {
		free(mp3File.buffer);
		mpg123_close(mp3File.handle);
		mpg123_delete(mp3File.handle);
	}
	mpg123_exit();
}
