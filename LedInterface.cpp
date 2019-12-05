/**
 * @file LedInterface.cpp
 *
 * @brief LedInterface module
 *
 * This file implements the LedInterface module, which acts as a bridge between
 * the Led class and the LedDriver of the other running process. This module
 * updates the shared memory used by both processes with the desired LED states
 *
 * @author Bryan Cisneros
 */

#include "LedInterface.h"
#include <stdint.h>
#include  <stdio.h>
#include <sys/shm.h>

#define ON ((uint8_t)0xFF)
#define OFF ((uint8_t)0x00)

typedef struct
{
    bool update;
    char led_values[12];
} shared_leds_t;

void* shared_pointer = NULL;
shared_leds_t* shared_leds;
int shared_leds_id;


void LedInterface_init(void)
{
    // get shared memory
    shared_leds_id = shmget((key_t)1234, sizeof(shared_leds_t), 0);
    if (shared_leds_id == -1)
    {
        printf("failed to get shared memory\n");
    }
    shared_pointer = shmat(shared_leds_id, (void*)0, 0);
    if (shared_pointer == (void*)-1)
    {
        printf("shmat() failed!\n");
    }
    shared_leds = (shared_leds_t*)shared_pointer;

    // Initialize all LEDs to off
    for (int i = 0; i < 12; i++)
    {
        shared_leds->led_values[i] = OFF;
    }
    shared_leds->update = true;
}

void LedInterface_turnOnLed(int channel)
{
    // Update value in shared memory and set update flag.
    // The LED driver will then update the LED accordingly
    shared_leds->led_values[channel] = ON;
    shared_leds->update = true;
}

void LedInterface_turnOffLed(int channel)
{
    // Update value in shared memory and set update flag.
    // The LED driver will then update the LED accordingly
    shared_leds->led_values[channel] = OFF;
    shared_leds->update = true;
}
