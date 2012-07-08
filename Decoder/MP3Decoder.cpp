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
	if(&mp3File != NULL && mp3File.buffer != NULL)
		free(mp3File.buffer);
	this->cleanup(mp3File.handle);
}

void MP3Decoder::loadFile(const char *fpath) {
	printf("loading %s\n", fpath);
	this->openFile(fpath);
	printf("MP3 Length: %f\n", mp3File.length);
}

unsigned char* MP3Decoder::getSampleBuffer() {
	printf("retrieving sample buffer\n");
	size_t readSize = this->readBuffer();
	printf("sample buffer read in, %ld samples read\n", readSize);
	return mp3File.buffer;
}

size_t MP3Decoder::getSampleBufferSize() {
	return mp3File.buffer_size;
}

///////////////////////////// end of public functions /////////////////////////
///////////////////////////// private functions below /////////////////////////

inline int MP3Decoder::readBuffer() {
	size_t done = 0;
    int err = mpg123_read(mp3File.handle, mp3File.buffer, mp3File.buffer_size, &done);
	mp3File.leftSamples = done/2;
	mp3File.offset = 0;

	if( err != MPG123_OK) {
		printf("file not read into buffer correctly/n");
		return 0;
	}
	else
		return done;
}

inline void MP3Decoder::openFile(const char *filename) {
	mpg123_handle *mh = NULL;
	int  channels = 0, encoding = 0;
	long rate = 0;
	int  err  = MPG123_OK;
        
	err = mpg123_init();
	if( err != MPG123_OK || (mh = mpg123_new(NULL, &err)) == NULL
	|| mpg123_open(mh, filename) != MPG123_OK
	|| mpg123_getformat(mh, &rate, &channels, &encoding) != MPG123_OK ) {
		printf( "Trouble with mpg123: %s with file: %s\n",
		mh==NULL ? mpg123_plain_strerror(err) : mpg123_strerror(mh), filename);
		this->cleanup(mh);
		return;
	}
        
	// Ensure that this output format will not change (it could, when we allow it).
	mpg123_format_none(mh);
	mpg123_format(mh, rate, channels, encoding);
        
	size_t buffer_size = mpg123_length(mh) * channels;//mpg123_outblock( mh );
	printf("buffer size: %ld\n", buffer_size);
    unsigned char* buffer = (unsigned char*)malloc(buffer_size);
	size_t done = 0;
	int samples = 0;
        
	mp3File = MP3File();
	mp3File.handle = mh;
	mp3File.channels = channels;
	mp3File.rate = rate;
	mp3File.buffer = buffer;
	mp3File.buffer_size = buffer_size;
	int length = mpg123_length( mh );
	if( length == MPG123_ERR )
		mp3File.length = 0;
	else
		mp3File.length = length / rate;
}

void MP3Decoder::cleanup(mpg123_handle * handle) {
	mpg123_close(handle);
	mpg123_delete(handle);
	mpg123_exit();
}
