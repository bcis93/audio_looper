/**
 * @file main.cpp
 * 
 * @brief main for LED driver program
 * 
 * This file initializes the LED driver, then enters a while(1) loop to 
 * periodically check/update the LEDs
 * 
 * @author Bryan Cisneros
 */

#include "LedDriver.h"
#include <time.h>
#include <stdio.h>

#define FIFTEEN_MS (15000000)

int main(void)
{
	// Set up a struct to be able to sleep for 15 ms
	struct timespec sleep_time, time2;
	sleep_time.tv_sec = 0;
	sleep_time.tv_nsec = FIFTEEN_MS;

	printf("Starting initialization...\n");

	LedDriver_init();

	printf("Done!\n");

	while(1)
	{
		// Check for updates, then sleep for ~15 ms
		LedDriver_checkForUpdates();
		nanosleep(&sleep_time, &time2);
	}
}
