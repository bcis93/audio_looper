recording_mode = True # true if recording mode, false if playing mode
master_done = False # true every time the master track starts over. will stay true only for one tick cycle
waiting_to_start = 0 # 0 until the first track starts recording, 1 while the first track is recording, 2 once the first track finishes
track_length = 176400
audio_wrap_around = False