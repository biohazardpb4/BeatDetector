/*
 * MP3Decoder.h
 *
 *  Created on: Jun 23, 2012
 *      Author: jay
 */
#include "mpg123.h"

#ifndef MP3DECODER_H_
#define MP3DECODER_H_

class MP3Decoder {
private:
	struct MP3File
	{
	        mpg123_handle* handle;
	        int channels;
	        long rate;
	        float length;
	        size_t buffer_size;
            unsigned char* buffer;
	        int leftSamples;
	        int offset;
	};
	
	// our current mp3 file
	MP3File mp3File;
	inline int readBuffer();
    inline void openFile(const char *);
	void cleanup(mpg123_handle *);

public:
	// da structors
	MP3Decoder();
	virtual ~MP3Decoder();

	// loads the file passed in	
    void loadFile(const char *);
    unsigned char * getSampleBuffer();
	size_t getSampleBufferSize();
};

#endif /* MP3DECODER_H_ */
