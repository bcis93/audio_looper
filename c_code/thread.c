#include "portaudio.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <bcm2835.h>
#include <sched.h>
#include <pthread.h>

// gcc test.c -lrt -lasound -lpthread -lportaudio -o test

typedef struct
{
    float left_phase;
    float right_phase;
}   
paTestData;

#define PIN (23)
#define RESET_PIN (12)


#define SAMPLE_RATE (44100)
#define SECONDS (20)
#define AUDIO_LENGTH (441000)
#define CHUNK_SIZE (256)
static paTestData data;

static int32_t audio[AUDIO_LENGTH] = {};
static volatile long long current_position = 0;
static volatile long long write_position = 0;
static volatile int count = 0;
static volatile bool flag = false;
static bool recording = false;
static volatile int first_recording = 0;
static volatile long long track_length = AUDIO_LENGTH;
static bool reset_flag = false;

void* audio_thread(void *arg);

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
    int32_t* in = (int32_t*) inputBuffer;
    int32_t* out = (int32_t*) outputBuffer;
    unsigned int i;
    //outputBuffer = (void*)&(audio[current_position]);
	//printf("tick\r\n");

    if (recording && first_recording == 0)
    {
        first_recording++;
        current_position = 0;
        write_position = 0;
    }
    else
    {
        write_position = current_position - framesPerBuffer;// * 2;
        if (write_position < 0)
        {
            write_position += track_length;
        }
    }
    

    for( i=0; i<framesPerBuffer; i++ )
    {
        if (recording)
        {
            audio[write_position] += *in;
            *out = audio[current_position] + *in;
        }
        else
        {
            *out = audio[current_position] +  *in;
        }

        // *out = *in;

        // audio[current_position] += *in;
        // *out = audio[current_position];

        current_position++;
        if (current_position >= track_length || current_position >= AUDIO_LENGTH)
        {
            current_position = 0;
            //count++;
            //flag = true;
        }
        write_position++;
        if (write_position >= track_length || write_position >= AUDIO_LENGTH)
        {
            write_position = 0;
            //count++;
            //flag = true;
        }

        out++;
        in++;
    }
    return 0;
}

void delay_loop_ms(int ms)
{
    static volatile int i;
    static volatile int j;
    for (i = 0; i < ms; i++)
    {
        for(j=0;j<100000;j++){};
    }
}

void wait_for_press(void)
{
    int track1 = 1;
    int reset = 1;
    while(track1 && reset)
    {
        track1 = bcm2835_gpio_lev(PIN);
        reset = bcm2835_gpio_lev(RESET_PIN);
    }
    printf("press!\n");
    if (!reset)
    {
        reset_flag = true;
    }
}

int main()
{

    if (!bcm2835_init())
    {
        printf("bcm2835 init failed!\n");
        return 1;
    }
        
    // Set RPI pin 23 to be an input
    bcm2835_gpio_fsel(PIN, BCM2835_GPIO_FSEL_INPT);
    //  with a pullup
    bcm2835_gpio_set_pud(PIN, BCM2835_GPIO_PUD_UP);

    // Set RPI pin 23 to be an input
    bcm2835_gpio_fsel(RESET_PIN, BCM2835_GPIO_FSEL_INPT);
    //  with a pullup
    bcm2835_gpio_set_pud(RESET_PIN, BCM2835_GPIO_PUD_UP);

    pthread_t audio_thread_id; 
    printf("Before Thread\n"); 
    pthread_create(&audio_thread_id, NULL, audio_thread, NULL);

    

        /* Sleep for several seconds. */
        //Pa_Sleep(5*1000);
        while (!reset_flag)
        {
            wait_for_press();
            if (first_recording == 1)
            {
                track_length = current_position;
                first_recording++;
            }
            recording = !recording;
            delay_loop_ms(500);
        }
    
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
        PaStreamParameters input = {2, 1, paInt16, (Pa_GetDeviceInfo(2))->defaultLowInputLatency, NULL};
        PaStreamParameters output = {2, 1, paInt16, (Pa_GetDeviceInfo(2))->defaultLowOutputLatency, NULL};

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
                                    CHUNK_SIZE,
                                    paNoFlag,
                                    patestCallback,
                                    &data );	
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
}
