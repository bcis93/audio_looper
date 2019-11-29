#include "LedInterface.h"
#include "lib-tlc59711/tlc59711.h"

#define ON ((uint8_t)0xFF)
#define OFF ((uint8_t)0x00)
#define BRIGHTNESS (0x30)

TLC59711 leds;

void LedInterface_init(void)
{
    leds.SetGbcRed(BRIGHTNESS);
	leds.SetGbcGreen(BRIGHTNESS);
	leds.SetGbcBlue(BRIGHTNESS);
}

void LedInterface_turnOnLed(int channel)
{
    leds.Set(channel, ON);
    leds.Update();
}

void LedInterface_turnOffLed(int channel)
{
    leds.Set(channel, OFF);
    leds.Update();
}