/**
 * @file pwmled.cpp
 *
 */
/* Copyright (C) 2018-2019 by Arjan van Vught mailto:info@raspberrypi-dmx.nl
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

#include "bcm2835.h"

#include "tlc59711.h"

#define MAKE16BITS(x)	((uint16_t) ((uint16_t)(x) & 0xFF) << 8 | ((x) & 0xFF))


int main(int argc, char **argv) {
	if (getuid() != 0) {
		fprintf(stderr, "Program is not started as \'root\' (sudo)\n");
		return -1;
	}

	if (bcm2835_init() == 0) {
		fprintf(stderr, "Function bcm2835_init() failed\n");
		return -2;
	}

	TLC59711 pwmled;

	const uint8_t gbc = 0x30;

	pwmled.SetGbcRed(gbc);
	pwmled.SetGbcGreen(gbc);
	pwmled.SetGbcBlue(gbc);

	pwmled.Dump();


	for (;;) {

		pwmled.SetRgb(0, (uint8_t)0xFF, 0xFF, 0);
		pwmled.Update();
		sleep(1);
		pwmled.SetRgb(1, (uint8_t)0xFF, 0xFF, 0);
		pwmled.Update();
		sleep(1);
		pwmled.SetRgb(0, (uint8_t)0, 0, 0);
		pwmled.Update();
		sleep(1);
		pwmled.SetRgb(1, (uint8_t)0, 0, 0);
		pwmled.Update();
		sleep(1);


//		for (int i = 0; i < 5; i++)
//		{
//			pwmled.Set(i, (uint8_t)0xFF);
//			pwmled.Update();
//			sleep(1);
//		}
//
//		for (int i = 0; i < 5; i++)
//		{
//			pwmled.Set(i, (uint8_t)0);
//			pwmled.Update();
//			sleep(1);
//		}
	}
}

