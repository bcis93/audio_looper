/**
 * @file Led.h
 * 
 * @brief Led class
 * 
 * This file implements the API for the Led class
 * 
 * @author Bryan Cisneros
 */

#pragma once

class Led
{
public:
	/**
	 * @brief Led constructor
	 * 
	 * @param channel LED channel to initialize
	 * @return void
	 */
	Led(int channel);

	/**
	 * @brief destructor
	 */
	~Led(void);

	/**
	 * @brief Tick
	 * 
	 * This function checks if it is time for the LED to flash. It should be
	 * called frequently for proper function.
	 * 
	 * @return void
	 */
	void tick(void);

	/**
	 * @brief Turn on the LED
	 * @return void
	 */
	void turnOn(void);

	/**
	 * @brief Turn off the LED
	 * @return void
	 */
    void turnOff(void);

	/**
	 * @brief Start flashing the LED
	 * @return void
	 */
    void flash(void);

private:
    int channel; // LED channel
	int count; // count used for timing the flashing
    bool flashing; // true if flashing, false otherwise
    bool led_on; // records state of LED
};