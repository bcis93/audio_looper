#include "LedDriver.h"
#include <time.h>
#include <stdio.h>

int main(void)
{
	struct timespec sleep_time, time2;
	sleep_time.tv_sec = 0;
	sleep_time.tv_nsec = 15000000;

	printf("Starting initialization...\n");

	LedDriver_init();

	printf("Done!\n");

	while(1)
	{
		LedDriver_checkForUpdates();
		nanosleep(&sleep_time, &time2);
	}
}
