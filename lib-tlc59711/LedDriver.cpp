#include "LedDriver.h"
#include "tlc59711.h"
#include "bcm2835.h"
#include <sys/shm.h>
#include <stdio.h>
#include <string.h>

#define ON (0xFF)
#define OFF (0x00)
#define BRIGHTNESS (0x30)

typedef struct
{
	bool update;
	uint8_t led_values[12];
} shared_leds_t;

void* shared_pointer = NULL;
shared_leds_t* shared_leds;
int shared_leds_id;

uint8_t local_led_values[12];

TLC59711 leds;

void LedDriver_init(void)
{
	bcm2835_init();

	printf("setting brightness\n");
	leds.SetGbcRed(BRIGHTNESS);
	leds.SetGbcGreen(BRIGHTNESS);
	leds.SetGbcBlue(BRIGHTNESS);
	for (int i = 0; i < TLC59711_OUT_CHANNELS; i++)
	{
		leds.Set(i, (uint8_t)OFF);
	}
	leds.Update();

	shared_leds_id= shmget((key_t)1234, sizeof(shared_leds_t), 0666 | IPC_CREAT);
	if (shared_leds_id == -1)
	{
		printf("shmget() failed!\n");
	}

	shared_pointer = shmat(shared_leds_id, (void*)0, 0);
	if (shared_pointer == (void*)-1)
	{
		printf("shmat() failed!\n");
	}

	shared_leds = (shared_leds_t*)shared_pointer;
	shared_leds->update = false;
	for (int i = 0; i < TLC59711_OUT_CHANNELS; i++)
	{
		shared_leds->led_values[i] = 0;
	}
}

void LedDriver_checkForUpdates(void)
{
	if (shared_leds->update)
	{
		memcpy(local_led_values, shared_leds->led_values, 12);
		shared_leds->update = false;
		for (int i = 0; i < TLC59711_OUT_CHANNELS; i++)
		{
			leds.Set(i, local_led_values[i]);
//		 	printf("%d, ", local_led_values[i]);
		}
//		printf("\n");
		leds.Update();
	}
}
