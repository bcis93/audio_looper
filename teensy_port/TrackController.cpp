#include "TrackController.h"
#include "Globals.h"
#include "AudioInterface.h"
#include <stdio.h>

TrackController::TrackController()
{
}

TrackController::TrackController(Track* track, Button* button) {
	this->track = track;
	this->button = button;
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
			waitingToStart++;
			state = recording;
//			Serial.println("state: recording");
      //buttonPressed = false;
		}
		else if(buttonPressed && recordingMode) {
			//start recording
      track->startRecording();
      track->startPlaying();
			state = recording;
//			Serial.println("state: recording");
      //buttonPressed = false;
		}
		break;
	case TrackController::recording:
		if (buttonPressed) {
			//stop recording
      track->stopRecording();

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
			state = waiting;
			waiting_to_stop = false;
		}
		if (buttonPressed && !recordingMode) {
			waiting_to_stop = true;
		}
		if (buttonPressed && recordingMode) {
      track->startPlaying();
      track->startRecording();
			state = recording;
//			Serial.println("state: overdub");
      //buttonPressed = false;
		}
		break;
	case TrackController::waiting:
		if (waiting_to_play && masterDone)
		{
			track->startPlaying();
			state = playing;
			waiting_to_play = false;
		}
		if (buttonPressed && !recordingMode) {
			waiting_to_play = true;
		}
		if (buttonPressed && recordingMode) {
      track->startPlaying();
      track->startRecording();
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
      break;
    case playing:
      track->stopPlaying();
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
      break;
    case playing:
      track->stopPlaying();
      break;
    case waiting:
      break;
    default:
      break;
  }
  state = idle;
}

