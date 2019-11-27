#ifndef AUDIO_INTERFACE_H
#define AUDIO_INTERFACE_H

#include "Track.h"

void audio_init(void);

void audio_add_track(void);

void audio_set_track_length(Track* track);

void audio_set_track_position(int position);

#endif // AUDIO_INTERFACE_H