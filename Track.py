import Globals

class Track:

    playing = False
    recording = False
    frames = []

    def __init__(self, audio_interface):
        self.audio_interface = audio_interface

    def start_playing(self):
        # start playing
        self.playing = True
        print ("start playing")
        # self.audio_interface.start_playing(self.frames)

    def start_recording(self):
        self.recording = True
        print ("start recording")
        # self.audio_interface.start_recording(self.frames)

    def stop_playing(self):
        self.playing = False
        print ("stop playing")
        # self.audio_interface.stop_playing()

    def stop_recording(self):
        self.recording = False
        if Globals.waiting_to_start == 1:  # If this was the first recording
            self.audio_interface.set_global_track_length()
            self.audio_interface.reset_current_position()
        print ("stop recording")
        # self.audio_interface.stop_recording()

    def abort_record(self):
        self.recording = False
        print ("stop recording")
        # self.audio_interface.stop_recording()

    def continue_recording(self):
        # do nothing for now...
        pass

    # def check_audio_interface(self):
    #     self.audio_interface.keep_running()