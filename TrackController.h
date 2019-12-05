/**
 * @file TrackController.h
 *
 * @brief TrackController class
 *
 * This file implements the API for the TrackController class. This class handles
 * everything related to the track, meaning the track itself, the button associated
 * with the track, and the LEDs related to the track.
 *
 * @author Bryan Cisneros
 */

#pragma once
#include "Track.h"
#include "Button.h"
#include "Led.h"

class TrackController
{
public:
    /**
     * @brief TrackController constructor
     *
     * @param track track object
     * @param button button associated with track
     * @param red red LED (used when recording)
     * @param green green LED (used when playing)
     *
     * @return void
     */
    TrackController(Track* track, Button* button, Led* red, Led* green);

    /**
     * @brief TrackController destructor
     *
     * @return void
     */
    ~TrackController();

    // States for the state machine
    enum State { idle, recording, playing, waiting, stopped };

    /**
     * @brief Tick
     *
     * This function runs the state machine, as well as all of the sub state
     * machines (for the button, and leds). It should be called frequently for
     * proper function.
     *
     * @return void
     */
    void tick();

    /**
     * @brief get the current state of the track controller
     *
     * @return the state of the track controller
     */
    State getState();

    /**
     * @brief handle the main stop button being pressed
     *
     * @return void
     */
    void stopButton();

    /**
     * @brief handle the main start button being pressed
     *
     * @return void
     */
    void startButton();

    /**
     * @brief handle the main reset button being pressed
     *
     * @return void
     */
    void resetButton();

private:
    State state; // Current state
    State lastState; // Remembers the last state on a stop
    Track* track; // keeps track of recording/playing
    Button* button; // input to the state machine
    Led* led_red; // turns on when recording
    Led* led_green; // turns on when playing

    // These variables are used when we are going to start or stop playing
    // the track, but we're waiting for a rollover to occur
    bool waiting_to_play;
    bool waiting_to_stop;
};

