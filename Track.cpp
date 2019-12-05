/**
 * @file Track.cpp
 * 
 * @brief Track class
 * 
 * This file implements the Track class. The Track class simply keeps track of
 * if the track is currently playing, recording, or both (or neither).
 * 
 * @author Bryan Cisneros
 */

#include "Track.h"

Track::Track() {
	// Initialize both playing and recording to false.
	playing = false;
	recording = false;
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
