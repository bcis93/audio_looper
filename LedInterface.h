/**
 * @file LedInterface.h
 * 
 * @brief LedInterface module
 * 
 * This file implements the API for the LedInterface module, which acts as a bridge
 * between the Led class and the LedDriver of the other running process. This module
 * updates the shared memory used by both processes with the desired LED states
 * 
 * @author Bryan Cisneros
 */

#pragma once

/**
 * @brief Initialize the LED interface
 * 
 * @return void
 */
void LedInterface_init(void);

/**
 * @brief Turn on an LED
 * 
 * This function sets the appropriate value in shared memory so that the LED
 * driver will turn on an LED
 * 
 * @param channel LED channel to turn on
 * 
 * @return void
 */
void LedInterface_turnOnLed(int channel);

/**
 * @brief Turn off an LED
 * 
 * This function sets the appropriate value in shared memory so that the LED
 * driver will turn off an LED
 * 
 * @param channel LED channel to turn off
 * 
 * @return void
 */
void LedInterface_turnOffLed(int channel);

