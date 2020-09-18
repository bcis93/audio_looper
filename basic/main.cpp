#include "portaudio.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

int main()
{
    // Initialize PortAudio
    printf("initialize PortAudio");
    PaError err = Pa_Initialize();
    if( err != paNoError )
    {
        printf(  "PortAudio error: %s\n", Pa_GetErrorText( err ) );
    }
    else
    {
        printf("Get number of devices");
        int numOfDevices = Pa_GetDeviceCount();
        if(numOfDevices < 0) {
            fprintf(stderr, "Error: portaudio was unable to find a audio device! Code: 0x%x\n", numOfDevices);
            quit(-1);
        }
        for(int i = 0; i < numOfDevices; i++) {
            const PaDeviceInfo *deviceInfo = Pa_GetDeviceInfo(i);

            // skip devices don't have input
            if(deviceInfo->maxInputChannels==0) continue;
            
            printf("Device #%d: %s\n\
    \tInput Channels: %d\n\
    \tDefault SampleRate: %lf\n",\
            i+1, deviceInfo->name, deviceInfo->maxInputChannels,\
            deviceInfo->defaultSampleRate);
        }
    }
    return 0;
}
