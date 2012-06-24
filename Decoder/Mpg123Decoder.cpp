#include "Mpg123Decoder.h"
//#include "FFT.h"
//@line:69

	extern "C" {
	#include "mpg123.h"
	}
	#include <stdio.h>
	#include <string.h>

	struct Mp3File
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

	void cleanup( mpg123_handle *handle )
	{
	        mpg123_close(handle);
	        mpg123_delete(handle);
	        mpg123_exit();
	}

	static inline int readBuffer( Mp3File* mp3 )
	{
		size_t done = 0;
		int err = mpg123_read( mp3->handle, mp3->buffer, mp3->buffer_size, &done );
	
		mp3->leftSamples = done / 2;
		mp3->offset = 0;
	
		if( err != MPG123_OK )
			return 0;
		else
			return done;
	}
	
	static inline jlong wrapped_Java_FPR_audio_BeatzDecoder_openFile
(JNIEnv* env, jobject object, jstring obj_filename, char* filename) {

//@line:111

		mpg123_handle *mh = NULL;
		int  channels = 0, encoding = 0;
		long rate = 0;
		int  err  = MPG123_OK;
	
		err = mpg123_init();
		if( err != MPG123_OK || (mh = mpg123_new(NULL, &err)) == NULL
				|| mpg123_open(mh, filename) != MPG123_OK
				|| mpg123_getformat(mh, &rate, &channels, &encoding) != MPG123_OK )
		{
///////////////////////////////////////////////////////////////////////////////
			char err_str[256];
			sprintf( err_str, "Trouble with mpg123: %s with file: %s\n",
					mh==NULL ? mpg123_plain_strerror(err) : mpg123_strerror(mh),
					filename);
			__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "%s", err_str);
///////////////////////////////////////////////////////////////////////////////
			cleanup(mh);
			return 0;
		}
	
		if(encoding != MPG123_ENC_SIGNED_16)
		{ 
			// Signed 16 is the default output format anyways; it would actually by only different if we forced it.
		    // So this check is here just for this explanation.
			cleanup(mh);
			return 0;
		}
		// Ensure that this output format will not change (it could, when we allow it).
		mpg123_format_none(mh);
		mpg123_format(mh, rate, channels, encoding);
	
		size_t buffer_size = mpg123_outblock( mh );
		unsigned char* buffer = (unsigned char*)malloc(buffer_size);
		size_t done = 0;
		int samples = 0;
	
		Mp3File* mp3 = new Mp3File();
		mp3->handle = mh;
		mp3->channels = channels;
		mp3->rate = rate;
		mp3->buffer = buffer;
		mp3->buffer_size = buffer_size;
		int length = mpg123_length( mh );
		if( length == MPG123_ERR )
			mp3->length = 0;
		else
			mp3->length = length / rate;

		__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "File info: %d, %f, %ld", mp3->channels, mp3->length, mp3->rate);
	
		return (jlong)mp3;
	
}

jlong Java_FPR_audio_BeatzDecoder_openFile(JNIEnv* env, jobject object, jstring obj_filename) {
	char* filename = (char*)env->GetStringUTFChars(obj_filename, 0);

	jlong JNI_returnValue = wrapped_Java_FPR_audio_BeatzDecoder_openFile(env, object, obj_filename, filename);

	env->ReleaseStringUTFChars(obj_filename, filename);

	return JNI_returnValue;
}

static inline jint wrapped_Java_FPR_audio_BeatzDecoder_readSamples
(JNIEnv* env, jobject object, jlong handle, jshortArray obj_buffer, jint offset, jint numSamples, short* buffer) {

//@line:159
	
		Mp3File* mp3 = (Mp3File*)handle;
		short* target = buffer + offset;
	
		int idx = 0;
		while( idx != numSamples )
		{
			//__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "left samples: %d ", mp3->leftSamples);
			if( mp3->leftSamples > 0 )
			{
				short* src = ((short*)mp3->buffer) + mp3->offset;
				for( ; idx < numSamples && mp3->offset < mp3->buffer_size / 2; mp3->leftSamples--, mp3->offset++, target++, src++, idx++ )
				{
					*target = *src;
					//__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "0x%X ", *src);
				}
			}
			else
			{
				int result = readBuffer( mp3 );
				if( result == 0 )
					return 0;
			}
	
		}
	
		if( idx > numSamples )
			return 0;
	
		return idx;
	
}

jint Java_FPR_audio_BeatzDecoder_readSamples(JNIEnv* env, jobject object, jlong handle, jshortArray obj_buffer, jint offset, jint numSamples) {
	short* buffer = (short*)env->GetPrimitiveArrayCritical(obj_buffer, 0);

	jint JNI_returnValue = wrapped_Java_FPR_audio_BeatzDecoder_readSamples(env, object, handle, obj_buffer, offset, numSamples, buffer);

	env->ReleasePrimitiveArrayCritical(obj_buffer, buffer, 0);

	return JNI_returnValue;
}

jint Java_FPR_audio_BeatzDecoder_skipSamples(JNIEnv* env, jobject object, jlong handle, jint numSamples) {


//@line:189

		Mp3File* mp3 = (Mp3File*)handle;
	
		int idx = 0;
		while( idx != numSamples )
		{
			if( mp3->leftSamples > 0 )
			{
				for( ; idx < numSamples && mp3->offset < mp3->buffer_size / 2; mp3->leftSamples--, mp3->offset++, idx++ );
			}
			else
			{
				int result = readBuffer( mp3 );
				if( result == 0 )
					return 0;
			}
	
		}
	
		if( idx > numSamples )
			return 0;
	
		return idx;
	

}

static inline jint wrapped_Java_FPR_audio_BeatzDecoder_flagBeats
(JNIEnv* env, jobject object, jlong handle, jfloatArray obj_buffer, jint numSamples, jfloat tolerance, float* beat_flag_buffer) {

	const int SAMPLES_PER_INTERVAL = 1024;
	const int TRAILING_AVERAGE_LENGTH = 10;
	// approximately 200 BPM max
	const int MIN_AVERAGES_BETWEEN = 50;
	float ENERGY_CUTOFF = tolerance;

	// the average of every SAMPLES_PER_INTERVAL samples will be recorded for analysis
	int *interval_averages = new int[numSamples / SAMPLES_PER_INTERVAL];
	short *sample_buffer = new short[SAMPLES_PER_INTERVAL];
	short sample;


	__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "sample and interval buffer created");

	for(int i = 0; i < numSamples/SAMPLES_PER_INTERVAL; i++) {
		// read individual samples
		int samples_read = wrapped_Java_FPR_audio_BeatzDecoder_readSamples
				(env, object, handle, NULL, 0, SAMPLES_PER_INTERVAL, sample_buffer);
		for(int j = 0; j < SAMPLES_PER_INTERVAL; j++) {
			sample = sample_buffer[j];
			interval_averages[i] += sample >= 0 ? sample : -sample;
		}
		interval_averages[i] /= SAMPLES_PER_INTERVAL;
	}

///////////////////////////////////////////////////////////////////////////////

	float average = 0;
	for(int i = 0; i < numSamples/SAMPLES_PER_INTERVAL; i++)
		average += interval_averages[i];
	average /= float(numSamples)/SAMPLES_PER_INTERVAL;
	__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "num intervals: %d  average: %f", numSamples/SAMPLES_PER_INTERVAL, average);


///////////////////////////////////////////////////////////////////////////////
	int trailing_average = 0;
	int beat_marker_index = 0;
	int beat_flag_bound = 1024;//sizeof(beat_flag_buffer)/4;
	int last_sample_index = 0;

	__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "beat_flag_bound: %d ", beat_flag_bound);

	if(numSamples/SAMPLES_PER_INTERVAL > TRAILING_AVERAGE_LENGTH) {
		// start the history
		for(int i = 0; i < TRAILING_AVERAGE_LENGTH; i++)
			trailing_average += interval_averages[i];

		// If instantEnergy / samples in buffer < instantEnergy for the next sample then add beatmarker.
		for(int i = TRAILING_AVERAGE_LENGTH; i < numSamples/SAMPLES_PER_INTERVAL; i++)
		{
			if(i - last_sample_index > MIN_AVERAGES_BETWEEN && interval_averages[i + 1] > (trailing_average / TRAILING_AVERAGE_LENGTH) * ENERGY_CUTOFF) {
				beat_flag_buffer[beat_marker_index++] = float(i)/(numSamples/SAMPLES_PER_INTERVAL);
				last_sample_index = i;
			}
			trailing_average -= interval_averages[i - TRAILING_AVERAGE_LENGTH];
			trailing_average += interval_averages[i + 1];

			// protect teh memory
			if(beat_marker_index >= beat_flag_bound)
				break;
		}
	}

	// free memory
	delete[] sample_buffer;
	delete[] interval_averages;

	__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "beats flagged: %d ", beat_marker_index);

	// return the number of "beats" flagged
	return beat_marker_index;
}

jint Java_FPR_audio_BeatzDecoder_flagBeats(JNIEnv* env, jobject object, jlong handle, jfloatArray obj_buffer, jint numSamples, jfloat tolerance) {
	float* buffer = (float*)env->GetPrimitiveArrayCritical(obj_buffer, 0);

	jint JNI_returnValue = wrapped_Java_FPR_audio_BeatzDecoder_flagBeats(env, object, handle, obj_buffer, numSamples, tolerance, buffer);

	env->ReleasePrimitiveArrayCritical(obj_buffer, buffer, 0);

	return JNI_returnValue;
}

jint Java_FPR_audio_BeatzDecoder_getNumChannels(JNIEnv* env, jobject object, jlong handle) {


//@line:214

		Mp3File* mp3 = (Mp3File*)handle;
		return mp3->channels;
	

}

jint Java_FPR_audio_BeatzDecoder_getRate(JNIEnv* env, jobject object, jlong handle) {


//@line:219

		Mp3File* mp3 = (Mp3File*)handle;
		return mp3->rate;
	

}

jfloat Java_FPR_audio_BeatzDecoder_getLength(JNIEnv* env, jobject object, jlong handle) {


//@line:224

		Mp3File* mp3 = (Mp3File*)handle;
		return mp3->length;
	

}

void Java_FPR_audio_BeatzDecoder_closeFile(JNIEnv* env, jobject object, jlong handle) {


//@line:229

		Mp3File* mp3 = (Mp3File*)handle;
		free(mp3->buffer);
		cleanup(mp3->handle);
	

}

