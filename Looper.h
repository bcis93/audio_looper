/**
 * @file Looper.h
 * 
 * @brief Looper class
 * 
 * This file implements the API for the Looper class
 * 
 * @author Bryan Cisneros
 */

#include "TrackController.h"
#include "Button.h"
#include "Led.h"
#include <vector>

class Looper
{
public:
	/**
	 * @brief Looper constructor
	 * 
	 * @param recPlay record/play button
	 * @param startStop start/stop button
	 * @param resetButton reset button
	 * @param red_led Red LED (for recording mode)
	 * @param green_led Green LED (for playing mode)
	 * 
	 * @return void
	 */
	Looper(Button* recPlay, Button* startStop, Button* resetButton, Led* red_led, Led* green_led);

	/**
	 * @brief destructor
	 */
	~Looper();

	/**
	 * @brief Tick
	 * 
	 * This function runs the state machines, as well as all of the sub state
	 * machines (for buttons, leds, track controllers, etc). It should be called
	 * frequently for proper function.
	 * 
	 * @return void
	 */
	void tick();

	/**
	 * @brief add a track to the looper
	 * 
	 * Adding a track will enable the looper to start recording/playing audio on
	 * the track.
	 * 
	 * @param track track to add
	 * 
	 * @return void
	 */
	void addTrack(TrackController* track);

private:
	std::vector<TrackController*> trackControllers; // vector of all the track controllers

	// Buttons
	Button* recPlayButton;
	Button* startStopButton;
	Button* resetButton;

	// LEDs
	Led* red_led;
	Led* green_led;

	// The master track is the first one recorded. It determines the track length, etc
	TrackController* masterTrack;

	// States for the state machine
	enum State { idle, firstRecording, normalOperation, stopped };
	State state;

	// Functions called when the associated buttons are pressed
	void stopButton();
	void startButton();
	void resetPressed();
};

