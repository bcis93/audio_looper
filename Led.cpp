/**
 * @file Led.cpp
 * 
 * @brief Led clas
 * 
 * This file implements the Led class
 * 
 * @author Bryan Cisneros
 */

#include "Led.h"
#include "LedInterface.h"
#include <stdio.h>


#define FLASH_TIMEOUT (150)

Led::Led(int channel)
{
    this->channel = channel;
    count = 0;
    
    // initialize to off and not flashing
    flashing = false;
    led_on = false;
}

Led::~Led(void)
{
}


void Led::tick(void)
{
    // If the LED is flashing, increment the counter. If the timeout is reached,
    // toggle the LED and reset the counter
    if (flashing)
    {
        count++;
        if (count >= FLASH_TIMEOUT)
        {
            if (led_on)
            {
                LedInterface_turnOffLed(channel);
                led_on = false;
            }
            else
            {
                LedInterface_turnOnLed(channel);
                led_on = true;
            }
            count = 0;
        }
    }
}

void Led::turnOn(void)
{
    // Turn on the LED, and update internal status variables
    LedInterface_turnOnLed(channel);
    led_on = true;
    flashing = false;
}

void Led::turnOff(void)
{
    // Turn off the LED, and update internal status variables
    LedInterface_turnOffLed(channel);
    led_on = false;
    flashing = false;
}

void Led::flash(void)
{
    // Start with the LED on, and update internal status variables
    LedInterface_turnOnLed(channel);
    led_on = true;
    flashing = true;
    count = 0;
}
