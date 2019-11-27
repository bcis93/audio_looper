#include "TrackController.h"
#include "Button.h"
#include <vector>

class Looper
{
public:
	Looper();
	Looper(Button* recPlay, Button* startStop, Button* resetButton);
	~Looper();
	void tick();
	void addTrack(TrackController* track);
private:
	std::vector<TrackController*> trackControllers;
	Button* recPlayButton;
	Button* startStopButton;
	Button* resetButton;
	TrackController* masterTrack;
	enum State { idle, firstRecording, normalOperation, stopped };
	State state;
	void stopButton();
	void startButton();
	void resetPressed();
};

