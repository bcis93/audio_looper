#include "TrackController.h"
#include "Button.h"
#include "Led.h"
#include <vector>

class Looper
{
public:
	Looper();
	Looper(Button* recPlay, Button* startStop, Button* resetButton, Led* red_led, Led* green_led);
	~Looper();
	void tick();
	void addTrack(TrackController* track);
private:
	std::vector<TrackController*> trackControllers;
	Button* recPlayButton;
	Button* startStopButton;
	Button* resetButton;
	Led* red_led;
	Led* green_led;
	TrackController* masterTrack;
	enum State { idle, firstRecording, normalOperation, stopped };
	State state;
	void stopButton();
	void startButton();
	void resetPressed();
};

