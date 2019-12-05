/**
 * @file Looper.cpp
 * 
 * @brief Looper class
 * 
 * This file implements the Looper class
 * 
 * @author Bryan Cisneros
 */

#include "Looper.h"
#include "Globals.h"
#include "AudioInterface.h"
#include <stdio.h>

Looper::Looper(Button* recPlay, Button* startStop, Button* resetButton, Led* red_led, Led* green_led)
{
	// Point the member variables to the buttons and LEDs
	recPlayButton = recPlay;
	startStopButton = startStop;
	this->resetButton = resetButton;
	this->red_led = red_led;
	this->green_led = green_led;

	// Initialize to idle state, recording mode
	state = idle;
	recordingMode = true;
  	masterTrack = NULL;
}

void Looper::tick()
{
	// masterDone is set to true when a rollover (wrap-around) occurs. Rollover
	// is set by the audio interface, so we want to clear it as soon as possible,
	// but we want to make sure that masterDone is true for one (and only one)
	// whole tick process.
	masterDone = rollover;
	if (rollover)
	{
		rollover = false;
	}

	// Run the state machine for each track controller
	for (unsigned i = 0; i < trackControllers.size(); i++) {
		trackControllers[i]->tick();
	}

	// Run the state machine for each button
	recPlayButton->tick();
	startStopButton->tick();
	resetButton->tick();
  
	// Check if any of the buttons have been pressed
	bool recPlayButtonPressed = recPlayButton->fell();
	bool startStopButtonPressed = startStopButton->fell();
	bool resetButtonPressed = resetButton->fell();

	if (resetButtonPressed){
		resetPressed();
	}


	if (startStopButtonPressed){
		printf("Start/stop button pressed!\n");

		// If we were in the stopped state, call the startButton function.
		// If we were in any other state, call the stopButton function
		switch (state){
		case Looper::stopped:
			//start playing again
			startButton();

			//move to normal operation
			state = normalOperation;
			audio_set_track_position(0);

			break;
		default:
			stopButton();
			state = stopped;
			break;
		}
	}
  

	//state action
	switch (state){
  	case Looper::idle:
  		break;
  	case Looper::firstRecording:
		break;
  	case Looper::normalOperation:
		if (recPlayButtonPressed) {
			printf("recPlay button pressed!\n");
			recordingMode = !recordingMode;
			if (recordingMode)
			{
				// In recording mode, red should be on and green off
				green_led->turnOff();
				red_led->turnOn();
			}
			else
			{
				// In playing mode, red should be off and green on
				red_led->turnOff();
				green_led->turnOn();
			}
		}
  		break;
    case Looper::stopped:
      	break;
  	default:
  		break;
	}

	//state update
	switch (state)
	{
	case Looper::idle:
		// Check each track to see if any of them have moved into the recording
		// state. If they have, we have started our first recording. That track
		// becomes our master track, and we move into the first recording state
		for (unsigned i = 0; i < trackControllers.size(); i++) {
			if (trackControllers[i]->getState() == TrackController::recording) {
				masterTrack = trackControllers[i];
				state = firstRecording;
				printf("looper state: firstRecording\n");
				break;
			}
		}
		break;
	case Looper::firstRecording:
		if (masterTrack->getState() == TrackController::playing) {
			// If the master track has moved to the playing state, then our first
			// recording is complete. move to the normal operation state
			rollover = false; // make sure rollover is false before normal operation
			state = normalOperation;
			printf("looper state: normalOperation\n");
		}
		break;
	case Looper::normalOperation:
		break;
  	case Looper::stopped:
	  	break;
	default:
		break;
	}	
}

void Looper::addTrack(TrackController* track)
{
	// Just add the ttrack controller to the vector
	trackControllers.push_back(track);
}

void Looper::stopButton(){
  // Call the stop button function on each of the track controllers
  for (unsigned i = 0; i < trackControllers.size(); i++) {
    trackControllers[i]->stopButton();
  }
}

void Looper::startButton(){
	// Call the start button function on each of the track controllers
	for (unsigned i = 0; i < trackControllers.size(); i++) {
		trackControllers[i]->startButton();
	}
}

void Looper::resetPressed(){
	// Reset everything back to default!
	printf("Reset button pressed!\n");
	state = idle;
	recordingMode = true;
	masterTrack = NULL;
	waitingToStart = 0;

	// Call the reset button function on each of the track controllers
	for (unsigned i = 0; i < trackControllers.size(); i++) {
    	trackControllers[i]->resetButton();
  	}
}

