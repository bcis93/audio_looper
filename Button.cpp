#include <bcm2835.h>
#include "Button.h"
#include "Globals.h"
#include <stdio.h>

#define TIMEOUT_COUNT (250)

bool Button::bcm2835_initialized = false;

Button::Button() {
}

Button::Button(int pin)
{
	if(!bcm2835_initialized)
	{
		if (!bcm2835_init())
		{
			printf("bcm2835 init failed!/n");
		}
		bcm2835_initialized = true;
	}
	buttonPin = pin;
	// Set RPI pin to be an input
    bcm2835_gpio_fsel(buttonPin, BCM2835_GPIO_FSEL_INPT);
    //  with a pullup
    bcm2835_gpio_set_pud(buttonPin, BCM2835_GPIO_PUD_UP);

	pressed = false;
}


Button::~Button()
{
}

void Button::tick() {
	if (!bcm2835_gpio_lev(buttonPin) && timeout == 0)
	{
		pressed = true;
		timeout = TIMEOUT_COUNT;
	}
	if (timeout > 0)
	{
		timeout--;
	}
}

bool Button::fell() {
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
