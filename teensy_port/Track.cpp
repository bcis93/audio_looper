#include "Arduino.h"
#include "Track.h"

#define FILE_EXTENSION (".RAW")

Track::Track() {
	playing = false;
	recording = false;
}

Track::Track(int id, AudioRecordQueue* q, AudioPlaySdRaw* pr, AudioMixer4* mix) {
  playing = false;
  recording = false;
  this->id = id;
  c = 68 + id;
  filenameCurrent = c;
  filenameNext = c;
  Serial.println(filenameCurrent);
  const char* cstr = (filenameCurrent + FILE_EXTENSION).c_str();
  mixer = mix;
  
  AudioNoInterrupts();
  if (SD.exists(cstr)) {
    SD.remove(cstr);
  }
  AudioInterrupts();
  queue = q;
  playRaw = pr;
  
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
  String tmp = filenameCurrent + FILE_EXTENSION;
  const char* cstr = tmp.c_str();
  playRaw->play(cstr);
  
//	Serial.println("startPlaying");
	playing = true;
}

void Track::startRecording()
{
  for (int i = 1; i < 4; i++){
//    if (i == id-1){ // For 4 track mixer
    if (i == id){
      mixer->gain(i, 1);
    }
    else{
      mixer->gain(i, 0);
    }
  }
  String tmp = filenameNext + FILE_EXTENSION;
  const char* cstr = tmp.c_str();
  AudioNoInterrupts();
  if (SD.exists(cstr)) {
    SD.remove(cstr);
  }
  file = SD.open(cstr, FILE_WRITE);
  AudioInterrupts();

  if (file) {
  queue->begin();
  }
  else {
    Serial.println(filenameNext + " failed to open!");
  }
  
	Serial.println("startRecording");
	recording = true;
}

void Track::stopPlaying()
{
  playRaw->stop();
//	Serial.println("stopPlaying");
	playing = false;
}

void Track::stopRecording()
{
  queue->end();

  AudioNoInterrupts();
  while (queue->available() > 0) {
    file.write((byte*)queue->readBuffer(), 256);
    queue->freeBuffer();
  }
  file.close();
  AudioInterrupts();
  
//	Serial.println("stopRecording");
	recording = false;

  updateFilename();
}

//void Track::continuePlaying()
//{
//  if (!playRaw->isPlaying()) {
//    playRaw->stop();
//    AudioNoInterrupts();
//    const char* cstr = (filenameCurrent + FILE_EXTENSION).c_str();
//    if (!playRaw->play(cstr)){
//      //Serial.println("failed to open " + track1Current + FILE_EXTENSION);
//    }
//    AudioInterrupts();
//  }
//}

void Track::continueRecording()
{
  if (queue->available() >= 2) {
    byte buffer[512];
    memcpy(buffer, queue->readBuffer(), 256);
    queue->freeBuffer();
    memcpy(buffer+256, queue->readBuffer(), 256);
    queue->freeBuffer();

    // write all 512 bytes to the SD card
    AudioNoInterrupts();
    file.write(buffer, 512);
    AudioInterrupts();
  }
}

void Track::updateFilename(){
  filenameCurrent = filenameNext;
  filenameNext += c;
  if (filenameNext.length() > 8){
    filenameNext = c;
  }
//  Serial.println("filenameNext: " + filenameNext);
  //check length!!!
}

void Track::abortRecord(){
  String tmp = filenameNext + FILE_EXTENSION;
  const char* cstr = tmp.c_str();
  
  queue->end();

  AudioNoInterrupts();
  file.close();
  SD.remove(cstr);
  AudioInterrupts();
  
//  Serial.println("abortRecording");
  recording = false;
}

