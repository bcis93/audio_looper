#pragma once

#include "Track.h"

void audio_init(void);

void audio_add_track(Track* track);

void audio_set_track_length(void);

void audio_set_track_position(int position);

