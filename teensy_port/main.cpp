#include "Looper.h"
#include "Button.h"
#include "TrackController.h"
#include "Track.h"
#include "AudioInterface.h"
#include <stdio.h>
#include <bcm2835.h>

#define REC_PLAY_BUTTON     16
#define RESET_BUTTON        12
#define START_STOP_BUTTON   8
#define TRACK_1_BUTTON      23
#define TRACK_2_BUTTON      18
#define TRACK_3_BUTTON      17
#define TRACK_4_BUTTON      14

bool recordingMode; //true if recording mode, false if playing mode
bool masterDone; //true every time the master track starts over. will stay true only for one tick cycle
int waitingToStart; //0 until the first track starts recording, 1 while the first track is recording, 2 once the first track finishes

//Buttons
//Button recPlayButton(REC_PLAY_BUTTON);
//Button startStopButton(START_STOP_BUTTON);
//Button resetButton(RESET_BUTTON);
//Button track1Button(TRACK_1_BUTTON);
//Button track2Button(TRACK_2_BUTTON);
//Button track3Button(TRACK_3_BUTTON);
//Button track4Button(TRACK_4_BUTTON);

//Tracks
//Track track1(1);
//Track track2(2);
//Track track3(3);
//Track track4(4);

//TrackControllers
//TrackController track1Controller(&track1, &track1Button);
//TrackController track2Controller(&track2, &track2Button);
//TrackController track3Controller(&track3, &track3Button);
//TrackController track4Controller(&track4, &track4Button);


//Looper looper(&recPlayButton, &startStopButton, &resetButton);

int main() {
  printf("Starting setup...\n");

//  if (!bcm2835_init())
//  {
//      printf("bcm2835 init failed!\n");
//      return 1;
//  }

  
  //looper.addTrack(&track1Controller);
  //looper.addTrack(&track2Controller);
  //looper.addTrack(&track3Controller);
  //looper.addTrack(&track4Controller);

  audio_init();

  printf("Setup complete!\n");

  while (1)
  {
    //looper.tick();
  }

  return 0;
}
