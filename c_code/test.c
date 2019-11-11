#include "portaudio.h"
#include <stdio.h>
#include <stdbool.h>

// gcc test.c -lrt -lasound -lpthread -lportaudio -o test

typedef struct
{
    float left_phase;
    float right_phase;
}   
paTestData;


#define SAMPLE_RATE (44100)
#define SECONDS (10)
static paTestData data;

static float audio[SAMPLE_RATE * SECONDS] = {};
static int current_position = 0;
static volatile int count = 0;
static bool flag = false;

/* This routine will be called by the PortAudio engine when audio is needed.
   It may called at interrupt level on some machines so don't do anything
   that could mess up the system like calling malloc() or free().
*/ 
static int patestCallback( const void *inputBuffer, void *outputBuffer,
                           unsigned long framesPerBuffer,
                           const PaStreamCallbackTimeInfo* timeInfo,
                           PaStreamCallbackFlags statusFlags,
                           void *userData )
{
    float* in = (float*) inputBuffer;
    float* out = (float*) outputBuffer;
    unsigned int i;
    for( i=0; i<framesPerBuffer; i++ )
    {
        *out = *in;
        // audio[current_position] += *in;
        // *out = audio[current_position];
        // current_position++;
        // if (current_position >= SAMPLE_RATE * SECONDS)
        // {
        //     current_position = 0;
        //     count++;
        //     flag = true;
        // }
        out++;
        in++;
    }
    return 0;
}

int main()
{
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
                printf("%s\r\n", deviceInfo->name);
            }
        }
        


        PaStream *stream;
        PaError err;
        PaDeviceIndex index = 2;
        PaStreamParameters input = {2, 2, paFloat32, (Pa_GetDeviceInfo(2))->defaultLowInputLatency, NULL};
        PaStreamParameters output = {2, 2, paFloat32, (Pa_GetDeviceInfo(2))->defaultLowOutputLatency, NULL};

        /* Open an audio I/O stream. */
        // err = Pa_OpenDefaultStream( &stream,
        //                             2,          /* stereo input */
        //                             2,          /* stereo output */
        //                             paFloat32,  /* 32 bit floating point output */
        //                             SAMPLE_RATE,
        //                             256,        /* frames per buffer, i.e. the number
        //                                             of sample frames that PortAudio will
        //                                             request from the callback. Many apps
        //                                             may want to use
        //                                             paFramesPerBufferUnspecified, which
        //                                             tells PortAudio to pick the best,
        //                                             possibly changing, buffer size.*/
        //                             patestCallback, /* this is your callback function */
        //                             &data ); /*This is a pointer that will be passed to
        //                                             your callback*/

                                                    
        err = Pa_OpenStream	(	&stream,
                                    &input,
                                    &output,
                                    SAMPLE_RATE,
                                    2048,
                                    paNoFlag,
                                    patestCallback,
                                    &data );	
        if( err != paNoError ) printf(  "PortAudio error: %s\n", Pa_GetErrorText( err ) );

        err = Pa_StartStream( stream );
        if( err != paNoError ) printf(  "PortAudio error: %s\n", Pa_GetErrorText( err ) );

        /* Sleep for several seconds. */
        //Pa_Sleep(5*1000);
        while (count < 5)
        {
            if (flag)
            {
                printf("wrap\r\n");
                flag = false;
            }
        }

        err = Pa_StopStream( stream );
        if( err != paNoError ) printf(  "PortAudio error: %s\n", Pa_GetErrorText( err ) );

        err = Pa_CloseStream( stream );
        if( err != paNoError ) printf(  "PortAudio error: %s\n", Pa_GetErrorText( err ) );
    }

    err = Pa_Terminate();
    
}