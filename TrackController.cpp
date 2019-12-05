/**
 * @file TrackController.cpp
 *
 * @brief TrackController class
 *
 * This file implements the TrackController class. This class handles
 * everything related to the track, meaning the track itself, the button associated
 * with the track, and the LEDs related to the track.
 *
 * @author Bryan Cisneros
 */

#include "TrackController.h"
#include "Globals.h"
#include "AudioInterface.h"
#include <stdio.h>

TrackController::TrackController(Track* track, Button* button, Led* red, Led* green)
{
    // Initialize member variables with inputs
    this->track = track;
    this->button = button;
    led_red = red;
    led_green = green;

    // Initialize to idle
    state = idle;
    waiting_to_play = false;
    waiting_to_stop = false;

    // Add the track to the audio interface
    audio_add_track(track);
}


TrackController::~TrackController()
{
}

void TrackController::tick()
{
    // Run the button state machine
    button->tick();
    bool buttonPressed = button->fell();

    // Run the LED state machines
    led_red->tick();
    led_green->tick();

    //state action
    switch (state)
    {
    case TrackController::idle:
        break;
    case TrackController::recording:
        break;
    case TrackController::playing:
        if (buttonPressed && !recordingMode)
        {
            // If we've pressed the button and we're in playing mode, we want to
            // stop playing the track at the next wrap around. Start flashing
            // the LED and set the flag
            led_green->flash();
            waiting_to_stop = true;
        }
        break;
    case TrackController::waiting:
        if (buttonPressed && !recordingMode)
        {
            // If we've pressed the button and we're in playing mode, we want to
            // start playing the track at the next wrap around. Start flashing
            // the LED and set the flag
            led_green->flash();
            waiting_to_play = true;
        }
        break;
    default:
        break;
    }


    //state update
    switch (state)
    {
    case TrackController::idle:
        if (buttonPressed && (waitingToStart == 0))
        {
            // start recording (and make sure the audio starts at position 0)
            audio_set_track_position(0);
            track->startRecording();
            track->startPlaying(); // this will be empty audio for now...

            // Turn on both LEDs. Technically we are playing audio too, but it's empty
            led_red->turnOn();
            led_green->turnOn();

            // Increment waiting to start variable. This will let everyone know
            // that we are in the first recording state
            waitingToStart++;

            // move to recording state
            state = recording;
        }
        else if(buttonPressed && recordingMode)
        {
            track->startRecording(); //start recording
            track->startPlaying(); // if we weren't playing, start playing too

            // Turn both LEDs on
            led_red->turnOn();
            led_green->turnOn();

            // move to recording state
            state = recording;
        }
        break;
    case TrackController::recording:
        if (buttonPressed)
        {
            //stop recording and turn off the red LED
            track->stopRecording();
            led_red->turnOff();

            // If this was the first recording, set the length of the track
            // and increment waiting to start variable
            if (waitingToStart == 1)
            {
                audio_set_track_length();
                waitingToStart++;
            }

            // move to playing state
            state = playing;
        }
        break;
    case TrackController::playing:
        if (waiting_to_stop && masterDone)
        {
            // stop playing and turn off green LED
            track->stopPlaying();
            led_green->turnOff();

            // move to waiting state and clear flag
            state = waiting;
            waiting_to_stop = false;
        }
        if (buttonPressed && recordingMode)
        {
            // If we've pressed the button and are in recording mode, we don't
            // need to wait for a wrap around. Start recording immediately
            track->startRecording();
            led_red->turnOn();

            // move to recording state
            state = recording;
        }
        break;
    case TrackController::waiting:
        if (waiting_to_play && masterDone)
        {
            // If we were waiting to start playing and the wrap around happened,
            // start playing and turn on the green LED
            track->startPlaying();
            led_green->turnOn();

            // move to playing state and clear flag
            state = playing;
            waiting_to_play = false;
        }
        if (buttonPressed && recordingMode)
        {
            // If we've pressed the button and are in recording mode, we don't
            // need to wait for a wrap around. Start recording (and also playing)
            // immediately. Also turn on both LEDs
            track->startPlaying();
            track->startRecording();
            led_red->turnOn();
            led_green->turnOn();

            // move to recording state
            state = recording;
        }
        break;
    default:
        break;
    }
}

TrackController::State TrackController::getState()
{
    return state;
}

void TrackController::stopButton()
{
    // store the last state so we can come back to it
    lastState = state;

    // based on what the state was, stop recording/playing and turn off the LEDs
    switch (state)
    {
    case idle:
        break;
    case recording:
        track->stopRecording();
        track->stopPlaying();
        led_red->turnOff();
        led_green->turnOff();
        break;
    case playing:
        track->stopPlaying();
        led_green->turnOff();
        break;
    case waiting:
        break;
    default:
        break;
    }

    // clear flags
    waiting_to_play = false;
    waiting_to_stop = false;

    // move to stopped state
    state = stopped;
}

void TrackController::startButton()
{
    // Restore the saved state, and start playing again if necessary. Note that
    // if we were previously recording, we don't keep recording after a stop
    switch (lastState)
    {
    case idle:
        state = idle;
        break;
    case recording:
        state = idle;
        break;
    case playing:
        track->startPlaying();
        led_green->turnOn();
        state = playing;
        break;
    case waiting:
        state = waiting;
        break;
    default:
        state = idle;
        printf("Hit default case in TrackController::startButton()\n");
        break;
    }
}

void TrackController::resetButton()
{
    // based on what state we were in, stop playing/recording and turn off LEDs
    switch (state)
    {
    case idle:
        break;
    case recording:
        track->stopRecording();
        track->stopPlaying();
        led_red->turnOff();
        led_green->turnOff();
        break;
    case playing:
        track->stopPlaying();
        led_green->turnOff();
        break;
    case waiting:
        break;
    default:
        break;
    }

    // clear flags
    waiting_to_play = false;
    waiting_to_stop = false;

    // move to the idle state
    state = idle;
}

