/**
 * @file AudioInterface.h
 *
 * @brief Audio interface
 *
 * This file provides the API to send and receives data from the Fe-Pi audio board.
 *
 * @author Bryan Cisneros
 */

#pragma once

#include "Track.h"

// Initialize the audio interface and start the audio thread
void audio_init(void);

// Add a track to the audio interface. Safe to call before audio_init()
void audio_add_track(Track* track);

// Set the track length to the current position
void audio_set_track_length(void);

// Set the audio position to 'position'
void audio_set_track_position(int position);

