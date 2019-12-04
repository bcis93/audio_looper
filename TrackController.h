#pragma once
#include "Track.h"
#include "Button.h"
#include "Led.h"

class TrackController
{
public:
	TrackController();
	TrackController(Track* track, Button* button, Led* red, Led* green);
	~TrackController();
	enum State { idle, recording, playing, waiting, stopped };
	void tick();
	State getState();
	void stopButton();
	void startButton();
	void resetButton();
private:
	State state;
  	State lastState;
	Track* track;
	Button* button;
	Led* led_red;
	Led* led_green;
  	bool buttonPressed;
	bool waiting_to_play;
	bool waiting_to_stop;
};

