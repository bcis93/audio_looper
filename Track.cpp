#include "Track.h"


Track::Track() {
	playing = false;
	recording = false;
}

Track::Track(int id) {
  playing = false;
  recording = false;
  this->id = id;  
}

Track::~Track()
{
}

bool Track::isPlaying() {
	return playing;
}

bool Track::isRecording() {
	return recording;
}

void Track::startPlaying()
{
	playing = true;
}

void Track::startRecording()
{
	recording = true;
}

void Track::stopPlaying()
{
	playing = false;
}

void Track::stopRecording()
{
	recording = false;
}
