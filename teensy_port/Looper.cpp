#include "Looper.h"
#include "Globals.h"


Looper::Looper()
{
	state = idle;
	recordingMode = true;
}

Looper::Looper(Button* recPlay, Button* startStop, Button* resetButton)
{
	recPlayButton = recPlay;
	startStopButton = startStop;
	this->resetButton = resetButton;
	state = idle;
	recordingMode = true;
  	masterTrack = NULL;
}


Looper::~Looper()
{
}

void Looper::tick()
{
	bool recPlayButtonPressed;
	bool startStopButtonPressed;
	bool resetButtonPressed;
  
	for (int i = 0; i < trackControllers.size(); i++) {
		trackControllers[i]->tick();
	}

	recPlayButton->tick();
	startStopButton->tick();
	resetButton->tick();
  
	recPlayButtonPressed = recPlayButton->fell();
	startStopButtonPressed = startStopButton->fell();
	resetButtonPressed = resetButton->fell();

	if (resetButtonPressed){
		printf("Reset button pressed!\n");
		resetPressed();
		state = idle;
		recordingMode = true;
		masterTrack = NULL;
		waitingToStart = 0;
	}


	if (startStopButtonPressed){
		printf("Start/stop button pressed!\n");
		switch (state){
		case Looper::stopped:
			//start playing again
			startButton();
			//move to normal operation
			state = normalOperation;
			//currentPosition = 0;
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
  		//currentPosition = 0;
  		break;
  	case Looper::firstRecording:
  		break;
  	case Looper::normalOperation:
  		if (masterDone) { //masterDone flag should only be true for one tick
  			masterDone = false;
  		}
  		// if (currentPosition >= trackLength) {
  		// 	masterDone = true;
  		// 	currentPosition = 0;
  		// }
		if (recPlayButtonPressed) {
			printf("recPlay button pressed!\n");
			recordingMode = !recordingMode;
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
		for (int i = 0; i < trackControllers.size(); i++) {
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
			//trackLength = currentPosition;
			//currentPosition = 0;
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


//	if (waitingToStart == 0) {
//		currentPosition = 0;
//	}
//	if (waitingToStart == 1) {
//		for (int i = 0; i < trackControllers.size(); i++) {
//			if (trackControllers[i].getState == TrackController::recording) {
//				masterTrack = &trackControllers[i];
//			}
//		}
//	}

	
}

void Looper::addTrack(TrackController* track)
{
	trackControllers.push_back(track);
}

void Looper::stopButton(){
  for (int i = 0; i < trackControllers.size(); i++) {
    trackControllers[i]->stopButton();
  }
}

void Looper::startButton(){
  for (int i = 0; i < trackControllers.size(); i++) {
    trackControllers[i]->startButton();
  }
}

void Looper::resetPressed(){
  for (int i = 0; i < trackControllers.size(); i++) {
    trackControllers[i]->resetButton();
  }
}

