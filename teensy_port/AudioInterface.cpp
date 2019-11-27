#include "AudioInterface.h"
#include "portaudio.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <sched.h>
#include <pthread.h>
#include "Globals.h"

// gcc test.c -lrt -lasound -lpthread -lportaudio -o test


#define SAMPLE_RATE (44100)
#define SECONDS (20)
#define AUDIO_LENGTH (441000)
#define CHUNK_SIZE (2048)
#define MAX_NUMBER_OF_TRACKS (4)
//static paTestData data;

static Track* tracks[MAX_NUMBER_OF_TRACKS] = {};
static int number_of_tracks = 0;

static int16_t audio[MAX_NUMBER_OF_TRACKS][AUDIO_LENGTH] = {};
static volatile long current_position = 0;
static volatile long write_position = 0;
static volatile int first_recording = 0;
static volatile long track_length = AUDIO_LENGTH;

void* audio_thread(void *arg);

static int paCallback( const void *inputBuffer, void *outputBuffer,
                           unsigned long framesPerBuffer,
                           const PaStreamCallbackTimeInfo* timeInfo,
                           PaStreamCallbackFlags statusFlags,
                           void *userData );



void audio_add_track(Track* track)
{
    if (number_of_tracks < MAX_NUMBER_OF_TRACKS)
    {
        tracks[number_of_tracks] = track;
        number_of_tracks++;
    }
    else
    {
        printf("Max number of tracks!\n");
    }
    
}

void audio_set_track_length(void)
{
    track_length = current_position;
}

void audio_set_track_position(int position)
{
    current_position = position;
    write_position = current_position - CHUNK_SIZE;// * 2;
    if (write_position < 0)
    {
        write_position += track_length;
    }
}

void audio_init(void)
{
    pthread_t audio_thread_id; 
    pthread_create(&audio_thread_id, NULL, audio_thread, NULL);    
}


void* audio_thread(void *arg)
{
    const struct sched_param priority = {80};
    sched_setscheduler(0, SCHED_FIFO, &priority);

    PaError err = Pa_Initialize();
    if( err != paNoError )
    {
        printf(  "PortAudio error: %s\n", Pa_GetErrorText( err ) );
    }
    else
    {
        int numDevices;
        numDevices = Pa_GetDeviceCount();
        if( numDevices < 0 )
        {
            printf( "ERROR: Pa_CountDevices returned 0x%x\n", numDevices );
            err = numDevices;
        }
        else
        {
            const   PaDeviceInfo *deviceInfo;
            int i;
            for( i=0; i<numDevices; i++ )
            {
                deviceInfo = Pa_GetDeviceInfo( i );
                printf("name: %s\r\n", deviceInfo->name);
		printf("hostApi: %d\r\n", deviceInfo->hostApi);
		printf("maxInputChannels: %d\r\n", deviceInfo->maxInputChannels);
		printf("maxOutputChannels: %d\r\n", deviceInfo->maxOutputChannels);
		printf("defaultLowInputLatency: %f\r\n", deviceInfo->defaultLowInputLatency);
		printf("defaultLowOutputLatency: %f\r\n", deviceInfo->defaultLowOutputLatency);
		printf("defaultHighInputLatency: %f\r\n", deviceInfo->defaultHighInputLatency);
		printf("defaultHighOutputLatency: %f\r\n", deviceInfo->defaultHighOutputLatency);
		printf("defaultSampleRate: %f\r\n\n", deviceInfo->defaultSampleRate);		
            }
        }
        


        PaStream *stream;
        PaError err;
        PaDeviceIndex index = 2;
        PaStreamParameters input = {index, 1, paInt16, (Pa_GetDeviceInfo(index))->defaultLowInputLatency, NULL};
        PaStreamParameters output = {index, 1, paInt16, (Pa_GetDeviceInfo(index))->defaultLowOutputLatency, NULL};

                                                    
        err = Pa_OpenStream	(	&stream,
                                    &input,
                                    &output,
                                    SAMPLE_RATE,
                                    CHUNK_SIZE,
                                    paNoFlag,
                                    paCallback,
                                    NULL );	
        if( err != paNoError ) printf(  "PortAudio error: %s\n", Pa_GetErrorText( err ) );

        err = Pa_StartStream( stream );
        if( err != paNoError ) printf(  "PortAudio error: %s\n", Pa_GetErrorText( err ) );

        while(1)
        {
            sched_yield();
        }

        err = Pa_StopStream( stream );
        if( err != paNoError ) printf(  "PortAudio error: %s\n", Pa_GetErrorText( err ) );

        err = Pa_CloseStream( stream );
        if( err != paNoError ) printf(  "PortAudio error: %s\n", Pa_GetErrorText( err ) );

    }

    err = Pa_Terminate();
    return NULL;
}

/* This routine will be called by the PortAudio engine when audio is needed.
   It may called at interrupt level on some machines so don't do anything
   that could mess up the system like calling malloc() or free().
*/ 
static int paCallback( const void *inputBuffer, void *outputBuffer,
                           unsigned long framesPerBuffer,
                           const PaStreamCallbackTimeInfo* timeInfo,
                           PaStreamCallbackFlags statusFlags,
                           void *userData )
{
    int16_t* in = (int16_t*) inputBuffer;
    int16_t* out = (int16_t*) outputBuffer;
    unsigned int i;
    //outputBuffer = (void*)&(audio[current_position]);
	//printf("tick\r\n");
    

    for( i=0; i<framesPerBuffer; i++ )
    {
        *out = *in;
//        for (int i = 0; i < number_of_tracks; i++)
//        {
//            if (tracks[i]->isRecording())
//            {
//                audio[i][write_position] += *in;
//            }
//
//            if (tracks[i]->isPlaying())
//            {
//                *out += audio[i][current_position];
//            }
//        }
//
//        current_position++;
//        if (current_position >= track_length || current_position >= AUDIO_LENGTH)
//        {
//            current_position = 0;
//            //count++;
//            //flag = true;
//        }
//        write_position++;
//        if (write_position >= track_length || write_position >= AUDIO_LENGTH)
//        {
//            write_position = 0;
//        }

        out++;
        in++;
    }
    return 0;
}

