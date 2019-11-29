extern bool recordingMode; //true if recording mode, false if playing mode
extern bool masterDone; //true every time the master track starts over. will stay true only for one tick cycle
extern int waitingToStart; //0 until the first track starts recording, 1 while the first track is recording, 2 once the first track finishes
extern bool rollover; //set by the audio interface every time the track wraps around
