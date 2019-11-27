#pragma once
#include "Arduino.h"
#include "TrackController.h"
#include "Button.h"
#include <vector>

class Looper
{
public:
	Looper();
	Looper(Button* recPlay, Button* startStop, Button* resetButton);
	~Looper();
//	bool getWaitingToStart();
	void tick();
	void addTrack(TrackController* track);
private:
//	bool waitingToStart;
	std::vector<TrackController*> trackControllers;
	Button* recPlayButton;
  Button* startStopButton;
  Button* resetButton;
	TrackController* masterTrack;
	unsigned trackLength;
	elapsedMillis currentPosition;
	enum State { idle, firstRecording, normalOperation, stopped };
	State state;
  void stopButton();
  void startButton();
  void resetPressed();
};

