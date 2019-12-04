#include <bcm2835.h>
#include "Led.h"
#include "LedInterface.h"
#include <stdio.h>


#define FLASH_TIMEOUT (150)

bool Led::bcm2835_initialized = false;

Led::Led(void) {
}

Led::Led(int channel)
{
	if(!bcm2835_initialized)
	{
		if (!bcm2835_init())
		{
			printf("bcm2835 init failed!/n");
		}
		bcm2835_initialized = true;
	}
    this->channel = channel;
    count = 0;
    flashing = false;
    led_on = false;
}

Led::~Led(void)
{
}


void Led::tick(void)
{
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
    LedInterface_turnOnLed(channel);
    led_on = true;
    flashing = false;
}

void Led::turnOff(void)
{
    LedInterface_turnOffLed(channel);
    led_on = false;
    flashing = false;
}

void Led::flash(void)
{
    LedInterface_turnOnLed(channel);
    flashing = true;
    count = 0;
}
