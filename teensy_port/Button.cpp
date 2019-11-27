#include <bcm2835.h>
#include "Button.h"
#include "Globals.h"

#define TIMEOUT_COUNT (10)

Button::Button() {
}

Button::Button(int pin)
{
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
