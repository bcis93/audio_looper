#pragma once
#include <Audio.h>
#include "Button.h"
class Track
{
public:
	Track();
  Track(int id, AudioRecordQueue* q, AudioPlaySdRaw* pr, AudioMixer4* mix);
	~Track();
	bool isPlaying();
	bool isRecording();
	void startPlaying();
	void startRecording();
	void stopPlaying();
	void stopRecording();
	void continuePlaying();
	void continueRecording();
  void abortRecord();
private:
  void updateFilename();
	bool playing;
	bool recording;
  int id;
  char c;
  File file;
  String filenameCurrent;
  String filenameNext;
  AudioRecordQueue* queue;
  AudioPlaySdRaw* playRaw;
  AudioMixer4* mixer;
};

