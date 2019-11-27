#include "Arduino.h"
#include "TrackController.h"
#include "Globals.h"


TrackController::TrackController()
{
}

TrackController::TrackController(Track* track, Button* button) {
	this->track = track;
	this->button = button;
	state = idle;
  buttonPressed = false;
}


TrackController::~TrackController()
{
}

void TrackController::tick()
{
	button->tick();
//	bool buttonPressed = button.fell();
  if (button->fell()){
    buttonPressed = true; //maybe just have this toggle buttonPressed? so that you can cancel if you didn't mean to press it?
//    Serial.println("button pressed!");
  }
//	if (buttonPressed) {
//		Serial.println("button pressed!");
//	}


	//state action
	switch (state)
	{
	case TrackController::idle:
		break;
	case TrackController::recording:
		track->continueRecording();
		break;
	case TrackController::playing:
//		track->continuePlaying();
    if (masterDone){
      track->stopPlaying();
      track->startPlaying();
    }
		break;
	case TrackController::waiting:
		break;
	case TrackController::overdub:
		track->continueRecording();
//		track->continuePlaying();
		break;
	default:
		break;
	}
 

	//state update
	switch (state)
	{
	case TrackController::idle:
		if (buttonPressed && (waitingToStart == 0)) {
			//start recording
      track->startRecording();
			waitingToStart++;
			state = recording;
//			Serial.println("state: recording");
      buttonPressed = false;
		}
		else if(buttonPressed && recordingMode && masterDone) {
			//start recording
      track->startRecording();
			state = recording;
//			Serial.println("state: recording");
      buttonPressed = false;
		}
		break;
	case TrackController::recording:
		if (buttonPressed && (waitingToStart == 1)) {
			//stop recording
      track->stopRecording();
			//start playing
      track->startPlaying();
			waitingToStart++;
			state = playing;
//			Serial.println("state: playing");
      buttonPressed = false;
		}
		else if (masterDone) {
			//stop recording
      track->stopRecording();
			//start playing
      track->startPlaying();
			state = playing;
//			Serial.println("state: playing");
		}
		break;
	case TrackController::playing:
		if (buttonPressed && !recordingMode && masterDone) {
      track->stopPlaying();
			state = waiting;
//			Serial.println("state: waiting");
      buttonPressed = false;
		}
		if (buttonPressed && recordingMode && masterDone) {
      track->stopPlaying();
      track->startPlaying();
      track->startRecording();
			state = overdub;
//			Serial.println("state: overdub");
      buttonPressed = false;
		}
		break;
	case TrackController::waiting:
		if (buttonPressed && !recordingMode && masterDone) {
      //should I put in a track->stopPlaying() here just in case?
      track->startPlaying();
			state = playing;
//			Serial.println("state: playing");
      buttonPressed = false;
		}
		if (buttonPressed && recordingMode && masterDone) {
      //should I put in a track->stopPlaying() here just in case?
      track->startPlaying();
      track->startRecording();
			state = overdub;
//			Serial.println("state: overdub");
      buttonPressed = false;
		}
		break;
	case TrackController::overdub:
		if (masterDone) { //I could add a check here to potentially keep recording until the button is pressed again
      track->stopRecording();
      track->startPlaying();
			state = playing;
//			Serial.println("state: playing");
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
      track->abortRecord();
      break;
    case playing:
      track->stopPlaying();
      break;
    case waiting:
      break;
    case overdub:
      track->stopPlaying();
      track->abortRecord();
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
    case overdub:
      track->startPlaying();
      state = playing;
      break;
    default:
      state = idle;
      Serial.println("Hit default case in TrackController::startButton()");
      break;
  }
}

void TrackController::resetButton(){
  switch (state){
    case idle:
      break;
    case recording:
      track->abortRecord();
      break;
    case playing:
      track->stopPlaying();
      break;
    case waiting:
      break;
    case overdub:
      track->stopPlaying();
      track->abortRecord();
      break;
    default:
      break;
  }
  state = idle;
}

