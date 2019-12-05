/**
 * @file Button.h
 * 
 * @brief Button class
 * 
 * This file implements the API for the Button class
 * 
 * @author Bryan Cisneros
 */

#pragma once

class Button
{
public:
	/**
	 * @brief Button constructor
	 * 
	 * @param pin pin to initialize
	 * @return void
	 */
	Button(int pin);

	/**
	 * @brief destructor
	 */
	~Button(void);

	/**
	 * @brief Tick
	 * 
	 * This function checks the state of the pin to detect a press. It should be
	 * called frequently for proper function.
	 * 
	 * @return void
	 */
	void tick();

	/**
	 * @brief reports if press was detected
	 * 
	 * @return true if press detected, false otherwise
	 */
	bool fell();

private:
	int buttonPin; // pin number
	bool pressed; // bool to record a press
	int timeout; // timeout for debouncing
	static bool bcm2835_initialized; // bool to track bcm2835 initialization
};

