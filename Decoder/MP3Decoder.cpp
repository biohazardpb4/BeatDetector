/*
 * MP3Decoder.cpp
 *
 *  Created on: Jun 23, 2012
 *      Author: jay
 */
#include <stdio.h>

#include "MP3Decoder.h"

MP3Decoder::MP3Decoder(std::string fpath) {
	printf("MP3Decoder decoding file at: %s", fpath.c_str());
}

MP3Decoder::~MP3Decoder() {

}

// loads the mp3 file passed into the constructor
void MP3Decoder::load() {



	mpg123_close(mp3File.handle);
	mpg123_delete(mp3File.handle);
	mpg123_exit();
}
