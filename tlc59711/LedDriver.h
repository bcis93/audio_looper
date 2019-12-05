/**
 * @file LedDriver.h
 *
 * @brief LED driver
 *
 * This file contains the API to update the LEDs
 *
 * @author Bryan Cisneros
 */

#pragma once

/**
 * @brief Initialize the LED driver
 *
 * @return void
 */
void LedDriver_init(void);

/**
 * @brief Check for updates
 *
 * This function checks the shared memory for any updates. If there is an update,
 * the LED driver then updates the LEDs with the new values.
 *
 * @return void
 */
void LedDriver_checkForUpdates(void);


