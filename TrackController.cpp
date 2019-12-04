#include "TrackController.h"
#include "Globals.h"
#include "AudioInterface.h"
#include <stdio.h>

TrackController::TrackController()
{
}

TrackController::TrackController(Track* track, Button* button, Led* red, Led* green) {
	this->track = track;
	this->button = button;
	led_red = red;
	led_green = green;
	state = idle;
  buttonPressed = false;
  waiting_to_play = false;
  waiting_to_stop = false;
  audio_add_track(track);
}


TrackController::~TrackController()
{
}

void TrackController::tick()
{
	button->tick();
	buttonPressed = button->fell();

	led_red->tick();
	led_green->tick();

	//state action
	switch (state)
	{
	case TrackController::idle:
		break;
	case TrackController::recording:
		break;
	case TrackController::playing:
		break;
	case TrackController::waiting:
		break;
	default:
		break;
	}
 

	//state update
	switch (state)
	{
	case TrackController::idle:
		if (buttonPressed && (waitingToStart == 0)) {
      audio_set_track_position(0);
			//start recording
      track->startRecording();
      track->startPlaying();
	led_red->turnOn();
	led_green->turnOn();
			waitingToStart++;
			state = recording;
//			Serial.println("state: recording");
      //buttonPressed = false;
		}
		else if(buttonPressed && recordingMode) {
			//start recording
      track->startRecording();
      track->startPlaying();
	led_red->turnOn();
//	sleep(1);
	led_green->turnOn();
			state = recording;
//			Serial.println("state: recording");
      //buttonPressed = false;
		}
		break;
	case TrackController::recording:
		if (buttonPressed) {
			//stop recording
      track->stopRecording();
	led_red->turnOff();

      if (waitingToStart == 1)
      {
        audio_set_track_length();
        waitingToStart++;
      }

			state = playing;
//			Serial.println("state: playing");
      //buttonPressed = false;
		}
		break;
	case TrackController::playing:
		if (waiting_to_stop && masterDone)
		{
			track->stopPlaying();
			led_green->turnOff();
			state = waiting;
			waiting_to_stop = false;
		}
		if (buttonPressed && !recordingMode) {
			led_green->flash();
			waiting_to_stop = true;
		}
		if (buttonPressed && recordingMode) {
      track->startPlaying(); // I think this can be removed...
      track->startRecording();
	led_green->turnOn();
//	sleep(1);
	led_red->turnOn();
			state = recording;
//			Serial.println("state: overdub");
      //buttonPressed = false;
		}
		break;
	case TrackController::waiting:
		if (waiting_to_play && masterDone)
		{
			track->startPlaying();
			led_green->turnOn();
			state = playing;
			waiting_to_play = false;
		}
		if (buttonPressed && !recordingMode) {
			led_green->flash();
			waiting_to_play = true;
		}
		if (buttonPressed && recordingMode) {
      track->startPlaying();
      track->startRecording();
	led_red->turnOn();
//	sleep(1);
	led_green->turnOn();
			state = recording;
//			Serial.println("state: recording");
      //buttonPressed = false;
		}
		break;
	default:
		break;
	}
}

TrackController::State TrackController::getState()
{
	return state;
}

void TrackController::stopButton(){
  lastState = state;
  switch (state){
    case idle:
      break;
    case recording:
      track->stopRecording();
      track->stopPlaying();
	led_red->turnOff();
	led_green->turnOff();
      break;
    case playing:
      track->stopPlaying();
	led_green->turnOff();
      break;
    case waiting:
      break;
    default:
      break;
  }
  state = stopped;
}

void TrackController::startButton(){
  switch (lastState){
    case idle:
      state = idle;
      break;
    case recording:
      state = idle;
      break;
    case playing:
      track->startPlaying();
	led_green->turnOn();
      state = playing;
      break;
    case waiting:
      state = waiting;
      break;
    default:
      state = idle;
      printf("Hit default case in TrackController::startButton()\n");
      break;
  }
}

void TrackController::resetButton(){
  switch (state){
    case idle:
      break;
    case recording:
      track->stopRecording();
      track->stopPlaying();
	led_red->turnOff();
	led_green->turnOff();
      break;
    case playing:
      track->stopPlaying();
	led_green->turnOff();
      break;
    case waiting:
      break;
    default:
      break;
  }
  state = idle;
}

