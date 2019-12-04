#include "LedInterface.h"
#include <stdint.h>

#define ON ((uint8_t)0xFF)
#define OFF ((uint8_t)0x00)

#ifndef NO_LEDS
#include "lib-tlc59711/tlc59711.h"

#define BRIGHTNESS (0x30)

TLC59711 leds;

void LedInterface_init(void)
{
    leds.SetGbcRed(BRIGHTNESS);
	leds.SetGbcGreen(BRIGHTNESS);
	leds.SetGbcBlue(BRIGHTNESS);
	for (int i = 0; i < TLC59711_OUT_CHANNELS; i++)
	{
		leds.Set(i, OFF);
	}
	leds.Update();
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

#else //NO_LEDS

#include  <stdio.h>
#include <sys/shm.h>

typedef struct
{
	bool update;
	char led_values[12];
} shared_leds_t;

void* shared_pointer = NULL;
shared_leds_t* shared_leds;
int shared_leds_id;

//uint8_t local_led_values[12] = {};

void LedInterface_init(void)
{
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

	for (int i = 0; i < 12; i++)
	{
		shared_leds->led_values[i] = OFF;
	}
	shared_leds->update = true;
}

void LedInterface_turnOnLed(int channel)
{
	printf("%d on\n", channel);
	shared_leds->led_values[channel] = ON;
	shared_leds->update = true;
}

void LedInterface_turnOffLed(int channel)
{
	printf("%d off\n", channel);
	shared_leds->led_values[channel] = OFF;
	shared_leds->update = true;
}

#endif //NO_LEDS
