// input.c
//
// Example program for bcm2835 library
// Reads and prints the state of an input pin
//
// After installing bcm2835, you can build this 
// with something like:
// gcc -o input input.c -l bcm2835
// sudo ./input
//
// Or you can test it before installing with:
// gcc -o input -I ../../src ../../src/bcm2835.c input.c
// sudo ./input
//
// Author: Mike McCauley
// Copyright (C) 2011 Mike McCauley
// $Id: RF22.h,v 1.21 2012/05/30 01:51:25 mikem Exp $
#include <bcm2835.h>
#include <stdio.h>
// Input on RPi pin GPIO 15
#define PIN 12

void delay_loop_ms(int ms)
{
    int i, j;
    for (i = 0; i < ms; i++)
    {
        for(j=0;j<100000;j++){};
    }
}

int main(int argc, char **argv)
{
    // If you call this, it will not actually access the GPIO
    // Use for testing
//    bcm2835_set_debug(1);
    if (!bcm2835_init())
        return 1;
    // Set RPI pin P1-15 to be an input
    bcm2835_gpio_fsel(PIN, BCM2835_GPIO_FSEL_INPT);
    //  with a pullup
    bcm2835_gpio_set_pud(PIN, BCM2835_GPIO_PUD_UP);
    //bcm2835_gpio_fen(PIN);
    // Blink
    while (1)
    {
        int pin = bcm2835_gpio_lev(PIN);
        if (!pin)
        {
            printf("press!\n");
            delay_loop_ms(500);
        }
        
    }
    bcm2835_close();
    return 0;
}