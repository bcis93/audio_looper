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

/**
 * @brief Audio init
 * 
 * Initialize the audio interface and start the audio thread
 * 
 * @return void
 */
void audio_init(void);

/**
 * @brief Add track to the audio interface
 * 
 * @note Safe to call before audio_init()
 * 
 * @param track track to add to audio interface
 * 
 * @return void
 */
void audio_add_track(Track* track);

/**
 * @brief Set the track length to the current position
 * 
 * @return void
 */
void audio_set_track_length(void);

/**
 * @brief Set the audio position
 * 
 * @param position position to set the audio to
 * 
 * @return void
 */
void audio_set_track_position(int position);

/**
 * @brief Reset audio
 * 
 * Reset the audio interface back to default (and erase all tracks)
 * 
 * @return void
 */
void audio_reset(void);

