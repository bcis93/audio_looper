#include "Looper.h"
#include "Button.h"
#include "TrackController.h"
#include "Track.h"
#include "AudioInterface.h"
#include "Led.h"
#include "LedInterface.h"
#include <stdio.h>
#include <bcm2835.h>
#include <time.h>
#include <sched.h>
//#include <pthread.h>

#define REC_PLAY_BUTTON     16
#define RESET_BUTTON        12
#define START_STOP_BUTTON   4
#define TRACK_1_BUTTON      25
#define TRACK_2_BUTTON      17
#define TRACK_3_BUTTON      24
#define TRACK_4_BUTTON      23

bool recordingMode; //true if recording mode, false if playing mode
bool masterDone; //true every time the master track starts over. will stay true only for one tick cycle
int waitingToStart; //0 until the first track starts recording, 1 while the first track is recording, 2 once the first track finishes
volatile bool rollover; //set by the audio interface every time the track wraps around

//Buttons
Button recPlayButton(REC_PLAY_BUTTON);
Button startStopButton(START_STOP_BUTTON);
Button resetButton(RESET_BUTTON);
Button track1Button(TRACK_1_BUTTON);
Button track2Button(TRACK_2_BUTTON);
Button track3Button(TRACK_3_BUTTON);
Button track4Button(TRACK_4_BUTTON);

//Tracks
Track track1(1);
Track track2(2);
Track track3(3);
Track track4(4);

//LEDs
Led red1(6);
Led green1(7);
Led red2(5);
Led green2(4);
Led red3(3);
Led green3(2);
Led red4(1);
Led green4(0);
Led record_led(8);
Led play_led(9);

//TrackControllers
TrackController track1Controller(&track1, &track1Button, &red1, &green1);
TrackController track2Controller(&track2, &track2Button, &red2, &green2);
TrackController track3Controller(&track3, &track3Button, &red3, &green3);
TrackController track4Controller(&track4, &track4Button, &red4, &green4);


Looper looper(&recPlayButton, &startStopButton, &resetButton, &record_led, &play_led);

int main() {
  printf("Starting setup...\n");

//  if (!bcm2835_init())
//  {
//      printf("bcm2835 init failed!\n");
//      return 1;
//  }

//  const struct sched_param priority = {70};
//  sched_setscheduler(0, SCHED_FIFO, &priority);


  looper.addTrack(&track1Controller);
  looper.addTrack(&track2Controller);
  looper.addTrack(&track3Controller);
  looper.addTrack(&track4Controller);

  audio_init();

  LedInterface_init();

  struct timespec sleep_time, time2;
  sleep_time.tv_sec = 0;
  sleep_time.tv_nsec = 2000000L;

  rollover = false;

  record_led.turnOn(); //turn on the record LED to signify that setup is complete!
  printf("Setup complete!\n");

  while (1)
  {
	looper.tick();
	//printf("tick\n");
	nanosleep(&sleep_time, &time2);
  }

  return 0;
}
