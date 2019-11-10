#include "portaudio.h"
#include <stdio.h>

typedef struct
{
    float left_phase;
    float right_phase;
}   
paTestData;


#define SAMPLE_RATE (44100)
static paTestData data;

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
        /* Open an audio I/O stream. */
        err = Pa_OpenDefaultStream( &stream,
                                    2,          /* stereo input */
                                    2,          /* stereo output */
                                    paFloat32,  /* 32 bit floating point output */
                                    SAMPLE_RATE,
                                    256,        /* frames per buffer, i.e. the number
                                                    of sample frames that PortAudio will
                                                    request from the callback. Many apps
                                                    may want to use
                                                    paFramesPerBufferUnspecified, which
                                                    tells PortAudio to pick the best,
                                                    possibly changing, buffer size.*/
                                    patestCallback, /* this is your callback function */
                                    &data ); /*This is a pointer that will be passed to
                                                    your callback*/
        if( err != paNoError ) printf(  "PortAudio error: %s\n", Pa_GetErrorText( err ) );

        err = Pa_StartStream( stream );
        if( err != paNoError ) printf(  "PortAudio error: %s\n", Pa_GetErrorText( err ) );

        /* Sleep for several seconds. */
        Pa_Sleep(5*1000);

        err = Pa_StopStream( stream );
        if( err != paNoError ) printf(  "PortAudio error: %s\n", Pa_GetErrorText( err ) );

        err = Pa_CloseStream( stream );
        if( err != paNoError ) printf(  "PortAudio error: %s\n", Pa_GetErrorText( err ) );
    }

    err = Pa_Terminate();
    
}