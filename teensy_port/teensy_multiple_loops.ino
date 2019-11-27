#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <Bounce2.h>

#include "Looper.h"
#include "Button.h"
#include "TrackController.h"
#include "Track.h"

//This commented out code is for 4 tracks:
//// GUItool: begin automatically generated code
//AudioPlaySdRaw           playRaw1;       //xy=103,173
//AudioPlaySdRaw           playRaw4;     //xy=103,350
//AudioPlaySdRaw           playRaw3;       //xy=104,294
//AudioPlaySdRaw           playRaw2;       //xy=105,233
//AudioInputI2S            i2sInput;       //xy=114,68.00000190734863
//AudioMixer4              sdRecordMixer;         //xy=311.0000457763672,299.00000762939453
//AudioMixer4              sdOutputMixer;         //xy=313.0000457763672,202.0000457763672
//AudioMixer4              outputMixer;    //xy=479.0000114440918,89.00000190734863
//AudioMixer4              recordMixer;     //xy=523.0000152587891,234.00000762939453
//AudioRecordQueue         queue1;         //xy=676.0000190734863,248.00000762939453
//AudioOutputI2S           i2sOutput;      //xy=677.0000190734863,90.00000381469727
//AudioConnection          patchCord1(playRaw1, 0, sdOutputMixer, 0);
//AudioConnection          patchCord2(playRaw1, 0, sdRecordMixer, 0);
//AudioConnection          patchCord3(playRaw4, 0, sdOutputMixer, 3);
//AudioConnection          patchCord4(playRaw4, 0, sdRecordMixer, 3);
//AudioConnection          patchCord5(playRaw3, 0, sdOutputMixer, 2);
//AudioConnection          patchCord6(playRaw3, 0, sdRecordMixer, 2);
//AudioConnection          patchCord7(playRaw2, 0, sdOutputMixer, 1);
//AudioConnection          patchCord8(playRaw2, 0, sdRecordMixer, 1);
//AudioConnection          patchCord9(i2sInput, 0, outputMixer, 0);
//AudioConnection          patchCord10(i2sInput, 0, recordMixer, 0);
//AudioConnection          patchCord11(sdRecordMixer, 0, recordMixer, 1);
//AudioConnection          patchCord12(sdOutputMixer, 0, outputMixer, 1);
//AudioConnection          patchCord13(outputMixer, 0, i2sOutput, 0);
//AudioConnection          patchCord14(outputMixer, 0, i2sOutput, 1);
//AudioConnection          patchCord15(recordMixer, queue1);
//AudioControlSGTL5000     sgtl5000_1;     //xy=101,434.0000114440918
//// GUItool: end automatically generated code


// GUItool: begin automatically generated code
AudioPlaySdRaw           playRaw1;       //xy=103,173
AudioPlaySdRaw           playRaw3;       //xy=104,294
AudioPlaySdRaw           playRaw2;       //xy=105,233
AudioInputI2S            i2sInput;       //xy=114,68.00000190734863
AudioMixer4              outputMixer;    //xy=479.0000114440918,89.00000190734863
AudioMixer4              recordMixer;     //xy=523.0000152587891,234.00000762939453
AudioRecordQueue         queue1;         //xy=676.0000190734863,248.00000762939453
AudioOutputI2S           i2sOutput;      //xy=677.0000190734863,90.00000381469727
AudioConnection          patchCord1(playRaw1, 0, outputMixer, 1);
AudioConnection          patchCord2(playRaw1, 0, recordMixer, 1);
AudioConnection          patchCord3(playRaw3, 0, recordMixer, 3);
AudioConnection          patchCord4(playRaw3, 0, outputMixer, 3);
AudioConnection          patchCord5(playRaw2, 0, outputMixer, 2);
AudioConnection          patchCord6(playRaw2, 0, recordMixer, 2);
AudioConnection          patchCord7(i2sInput, 0, outputMixer, 0);
AudioConnection          patchCord8(i2sInput, 0, recordMixer, 0);
AudioConnection          patchCord9(outputMixer, 0, i2sOutput, 0);
AudioConnection          patchCord10(outputMixer, 0, i2sOutput, 1);
AudioConnection          patchCord11(recordMixer, queue1);
AudioControlSGTL5000     sgtl5000_1;     //xy=101,434.0000114440918
// GUItool: end automatically generated code




#define REC_PLAY_BUTTON     26
#define RESET_BUTTON        27
#define START_STOP_BUTTON   28
#define TRACK_1_BUTTON      31
#define TRACK_2_BUTTON      32
#define TRACK_3_BUTTON      33
#define TRACK_4_BUTTON      34

#define SDCARD_CS_PIN       BUILTIN_SDCARD
#define SDCARD_MOSI_PIN     11  // not actually used
#define SDCARD_SCK_PIN      13  // not actually used

// which input on the audio shield will be used?
const int myInput = AUDIO_INPUT_LINEIN;
//const int myInput = AUDIO_INPUT_MIC;

bool recordingMode; //true if recording mode, false if playing mode
bool masterDone; //true every time the master track starts over. will stay true only for one tick cycle
int waitingToStart; //0 until the first track starts recording, 1 while the first track is recording, 2 once the first track finishes

//Buttons
Button recPlayButton(REC_PLAY_BUTTON);
Button startStopButton(START_STOP_BUTTON);
Button resetButton(RESET_BUTTON);
Button track1Button(TRACK_1_BUTTON);
Button track2Button(TRACK_2_BUTTON);
Button track3Button(TRACK_3_BUTTON);
Button track4Button(TRACK_4_BUTTON);

//Tracks
Track track1(1, &queue1, &playRaw1, &recordMixer);
Track track2(2, &queue1, &playRaw2, &recordMixer);
Track track3(3, &queue1, &playRaw3, &recordMixer);
//Track track4(4, &queue1, &playRaw4, &recordMixer);

//TrackControllers
TrackController track1Controller(&track1, &track1Button);
TrackController track2Controller(&track2, &track2Button);
TrackController track3Controller(&track3, &track3Button);
//TrackController track4Controller(&track4, &track4Button);


Looper looper(&recPlayButton, &startStopButton, &resetButton);

void setup() {
  Serial.begin(9600);
  Serial.println("Starting setup...");

  // Audio connections require memory, and the record queue
  // uses this memory to buffer incoming audio.
  AudioMemory(160);

  // Enable the audio shield, select input, and enable output
  sgtl5000_1.enable();
  sgtl5000_1.inputSelect(myInput);
  sgtl5000_1.volume(.8);

  // Initialize the SD card
  SPI.setMOSI(SDCARD_MOSI_PIN);
  SPI.setSCK(SDCARD_SCK_PIN);
  if (!(SD.begin(SDCARD_CS_PIN))) {
    // stop here if no SD card, but print a message
    while (1) {
      Serial.println("Unable to access the SD card");
      delay(500);
    }
  }

  if (SD.exists("TEST.RAW")) {
    SD.remove("TEST.RAW");
  }
  File file = SD.open("TEST.RAW", FILE_WRITE);
  if (!file){
    while (1) {
      Serial.println("Failed to open...");
      delay(500);
    }
  }


  outputMixer.gain(0, .8);
  outputMixer.gain(1, .8);
  outputMixer.gain(2, .8);
  outputMixer.gain(3, .8);

// For 4 track mixer
//  recordMixer.gain(0, .8);
//  recordMixer.gain(1, 1);
  
  
  looper.addTrack(&track1Controller);
  looper.addTrack(&track2Controller);
  looper.addTrack(&track3Controller);
//  looper.addTrack(&track4Controller);
  Serial.println("Setup complete!");
}

void loop() {
  looper.tick();
}
