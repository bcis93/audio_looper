/**
 * @file main.cpp
 * 
 * @brief main for looper program
 * 
 * This file sets up the looper, tracks, audio interface, buttons, etc, then
 * enters a while(1) loop to periodically run state machines and check for
 * button presses
 * 
 * @author Bryan Cisneros
 */

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

// Button gpio pin assignements
#define REC_PLAY_BUTTON     (16)
#define RESET_BUTTON        (12)
#define START_STOP_BUTTON   (4)
#define TRACK_1_BUTTON      (25)
#define TRACK_2_BUTTON      (17)
#define TRACK_3_BUTTON      (24)
#define TRACK_4_BUTTON      (23)

#define TWO_MS (2000000L)

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
Track track1;
Track track2;
Track track3;
Track track4;

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

// Looper
Looper looper(&recPlayButton, &startStopButton, &resetButton, &record_led, &play_led);

int main() {
  printf("Starting setup...\n");

  // Add tracks to the looper
  looper.addTrack(&track1Controller);
  looper.addTrack(&track2Controller);
  looper.addTrack(&track3Controller);
  looper.addTrack(&track4Controller);

  // Initialize the audio and LED interfaces
  audio_init();
  LedInterface_init();

  // Set up a struct to sleep between state machine ticks
  struct timespec sleep_time, time2;
  sleep_time.tv_sec = 0;
  sleep_time.tv_nsec = TWO_MS;

  rollover = false; // make sure rollover is initialized (to false)

  //turn on the record LED to signify that setup is complete!
  record_led.turnOn();
  printf("Setup complete!\n");

  while (1)
  {
    // Run the state machine, then sleep for ~2ms. The timing here is very soft.
    // As long as we're running the state machine every few ms (or somewhat
    // close to it), we'll be able to detect button presses and update our state
    // machines fast enough for acceptable performance. The audio handling is
    // all done in a separate, high-priority, real-time thread, so there is
    // nothing critical depending on a tight 2ms timing. Extra delays will occur
    // occasionaly as the OS is running other tasks, but this is not noticable
    // to the user.
    looper.tick();
    nanosleep(&sleep_time, &time2);
  }

  return 0;
}
