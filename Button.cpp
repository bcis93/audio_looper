/**
 * @file Button.cpp
 *
 * @brief Button class
 *
 * This file implements the Button class, including initialization, button
 * presses, and debouncing
 *
 * @author Bryan Cisneros
 */

#include <bcm2835.h>
#include "Button.h"
#include "Globals.h"
#include <stdio.h>

#define TIMEOUT_COUNT (250)

bool Button::bcm2835_initialized = false;

Button::Button(int pin)
{
    // Make sure the bcm2835 is initialized!
    if(!bcm2835_initialized)
    {
        if (!bcm2835_init())
        {
            printf("bcm2835 init failed!/n");
        }
        bcm2835_initialized = true;
    }

    buttonPin = pin;

    // Set pin as an input
    bcm2835_gpio_fsel(buttonPin, BCM2835_GPIO_FSEL_INPT);

    // Enable pullup on pin
    bcm2835_gpio_set_pud(buttonPin, BCM2835_GPIO_PUD_UP);

    pressed = false;
}


Button::~Button()
{
}

void Button::tick()
{
    // Check for a press, and check that the previous press has already timed out
    if (!bcm2835_gpio_lev(buttonPin) && timeout == 0)
    {
        // Press detected! set the flag, and also set the timeout for debouncing
        pressed = true;
        timeout = TIMEOUT_COUNT;
    }

    // If the timeout is running, decrement it
    if (timeout > 0)
    {
        timeout--;
    }
}

bool Button::fell()
{
    // If the button has been pressed, reset the flag and return true. Otherwise
    // return false;
    if (pressed)
    {
        pressed = false;
        return true;
    }
    else
    {
        return false;
    }

}
