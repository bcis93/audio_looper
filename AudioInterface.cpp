/**
 * @file AudioInterface.cpp
 *
 * @brief Audio interface
 *
 * This file sends and receives data from the Fe-Pi audio board. It also records
 * audio and plays it back, depending on which tracks have flags set.
 *
 * @author Bryan Cisneros
 */

#include "AudioInterface.h"
#include "portaudio.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <sched.h>
#include <pthread.h>
#include "Globals.h"
#include <string.h>

#define SAMPLE_RATE (44100)
#define AUDIO_LENGTH (441000) // The max length of audio we can record is 10 seconds
#define CHUNK_SIZE (512) // Handle audio in 512 sample chunks
#define MAX_NUMBER_OF_TRACKS (4) // Maximum number of tracks that are supported
#define CHANNELS (1) // Mono audio

#define AUDIO_THREAD_PRIORITY (80)
#define DEVICE_INDEX (2)

// Variables related to audio tracks
static Track* tracks[MAX_NUMBER_OF_TRACKS] = {};
static int number_of_tracks = 0;

// Array and variables to store and access audio
static int16_t audio[MAX_NUMBER_OF_TRACKS][AUDIO_LENGTH] = {};
static volatile long current_position = 0;
static volatile long write_position = 0;
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

    // write position should be one chunk before current, but make sure it's
    // not negative
    write_position = current_position - CHUNK_SIZE;
    if (write_position < 0)
    {
        write_position += track_length;
    }
}

void audio_init(void)
{
    // Create and start the audio thread!
    pthread_t audio_thread_id;
    pthread_create(&audio_thread_id, NULL, audio_thread, NULL);
}

void audio_reset(void)
{
    // Completely clear out the audio buffers
//    memset(audio, 0, MAX_NUMBER_OF_TRACKS * AUDIO_LENGTH * 2);
    for (int i = 0; i < MAX_NUMBER_OF_TRACKS; i++)
    {
        for (int j = 0; j < AUDIO_LENGTH; j++)
        {
            audio[i][j] = 0;
        }
    }
    track_length = AUDIO_LENGTH;
}


void* audio_thread(void *arg)
{
    // Set this thread as a high priority real time thread
    const struct sched_param priority = {AUDIO_THREAD_PRIORITY};
    sched_setscheduler(0, SCHED_FIFO, &priority);

    // Initialize PortAudio
    PaError err = Pa_Initialize();
    if( err != paNoError )
    {
        printf(  "PortAudio error: %s\n", Pa_GetErrorText( err ) );
    }
    else
    {
        // Create a PortAudio stream
        PaStream *stream;
        PaError err;
        PaDeviceIndex index = DEVICE_INDEX;
        PaStreamParameters input = {index, CHANNELS, paInt16, (Pa_GetDeviceInfo(index))->defaultLowInputLatency, NULL};
        PaStreamParameters output = {index, CHANNELS, paInt16, (Pa_GetDeviceInfo(index))->defaultLowOutputLatency, NULL};

        // Open the stream
        err = Pa_OpenStream	(&stream, &input, &output, SAMPLE_RATE, CHUNK_SIZE, paNoFlag, paCallback, NULL);
        if( err != paNoError )
        {
            printf(  "PortAudio error: %s\n", Pa_GetErrorText( err ) );
        }

        // Start the stream
        err = Pa_StartStream( stream );
        if( err != paNoError )
        {
            printf(  "PortAudio error: %s\n", Pa_GetErrorText( err ) );
        }

        // now that the stream is started, we don't need to do anything else in
        // this thread, other than keep the thread alive, so we'll just sit in a
        // while(1) and just yield anytime this thread may become active. All of
        // the audio processing will be handled in the callback function.
        while(1)
        {
            sched_yield();
        }

        err = Pa_StopStream( stream );
        if( err != paNoError )
        {
            printf(  "PortAudio error: %s\n", Pa_GetErrorText( err ) );
        }

        err = Pa_CloseStream( stream );
        if( err != paNoError )
        {
            printf(  "PortAudio error: %s\n", Pa_GetErrorText( err ) );
        }
    }

    err = Pa_Terminate();
    return NULL;
}

// This routine will be called by the PortAudio engine when audio is needed.
static int paCallback( const void *inputBuffer, void *outputBuffer,
                       unsigned long framesPerBuffer,
                       const PaStreamCallbackTimeInfo* timeInfo,
                       PaStreamCallbackFlags statusFlags,
                       void *userData )
{
    int16_t* in = (int16_t*) inputBuffer;
    int16_t* out = (int16_t*) outputBuffer;
    unsigned int i;

    // For each frame in the input buffer
    for( i=0; i<framesPerBuffer; i++ )
    {
        // Always pass the incoming audio through to the output
        *out = *in;

        // check each track. If the track is recording, add the current audio
        // to its audio buffer. If the track is playing, add the audio in its
        // buffer to the output
        for (int i = 0; i < number_of_tracks; i++)
        {
            if (tracks[i]->isRecording())
            {
                audio[i][write_position] += *in;
            }

            if (tracks[i]->isPlaying())
            {
                *out += audio[i][current_position];
            }
        }

        // Update the current position. If it has wrapped around, reset to zero
        // and set the 'rollover' flag (used by the main state machine)
        current_position++;
        if (current_position >= track_length || current_position >= AUDIO_LENGTH)
        {
            current_position = 0;
            rollover = true;
        }

        // Update the write position, and reset to 0 if necessary
        write_position++;
        if (write_position >= track_length || write_position >= AUDIO_LENGTH)
        {
            write_position = 0;
        }

        // Advance the in and out pointers to the next location
        out++;
        in++;
    }
    return 0;
}

