from enum import Enum
from Track import Track
from Button import Button
import Globals

class TrackController:
    class State(Enum):
        idle = 0
        recording = 1
        playing = 2
        waiting = 3
        overdub = 4
        stopped = 5

    state = State.idle
    last_state = State.idle
    button_pressed = False

    def __init__(self, track, button):
        self.track = track
        self.button = button

    def tick(self):
        # self.track.check_audio_interface()

        self.button.tick()

        if self.button.fell():
            self.button_pressed = True # maybe just toggle this?
            print("setting button_pressed to True")

        # print(self.button_pressed)
        # print(self.state)
        # print(Globals.waiting_to_start)
        
        # state action
        if self.state == self.State.idle:
            pass
        elif self.state == self.State.recording:
            self.track.continue_recording()
        elif self.state == self.State.playing:
            # if Globals.master_done:
            #     self.track.stop_playing()
            #     self.track.start_playing()
            pass
        elif self.state == self.State.waiting:
            pass
        elif self.state == self.State.overdub:
            self.track.continue_recording()
        else:
            print("Hit default in TrackController state action")

        # state update
        if self.state == self.State.idle:
            if self.button_pressed and Globals.waiting_to_start == 0:
                self.track.start_recording()
                Globals.waiting_to_start += 1
                self.state = self.State.recording
                self.button_pressed = False
            elif self.button_pressed and Globals.recording_mode and Globals.master_done:
                self.track.start_recording()
                self.state = self.State.recording
                self.button_pressed = False
        elif self.state == self.State.recording:
            if self.button_pressed and Globals.waiting_to_start == 1:
                self.track.stop_recording()
                self.track.start_playing()
                Globals.waiting_to_start += 1
                self.state = self.State.playing
                self.button_pressed = False
            # elif Globals.master_done:
            #     self.track.stop_recording()
            #     self.track.start_playing()
            #     self.state = self.State.playing
        elif self.state == self.State.playing:
            if self.button_pressed and not Globals.recording_mode and Globals.master_done:
                self.track.stop_playing()
                self.state = self.State.waiting
                self.button_pressed = False
            elif self.button_pressed and Globals.recording_mode and Globals.master_done:
                self.track.stop_playing()
                self.track.start_playing()
                self.track.start_recording()
                self.state = self.State.overdub
                self.button_pressed = False
        elif self.state == self.State.waiting:
            if self.button_pressed and not Globals.recording_mode and Globals.master_done:
                # should I put in a track.stop_playing() here just in case?
                self.track.start_playing()
                self.state = self.State.playing
                self.button_pressed = False
            elif self.button_pressed and Globals.recording_mode and Globals.master_done:
                # should I put in a track.stop_playing() here just in case?
                self.track.start_playing()
                self.track.start_recording()
                self.state = self.State.overdub
                self.button_pressed = False
        elif self.state == self.State.overdub:
            if Globals.master_done:
                self.track.stop_recording()
                self.track.start_playing()
                self.state = self.State.playing  
        else:
            print("Hit default in TrackController state update")

    def reset_button(self):
        if self.state == self.State.idle:
            pass
        elif self.state == self.State.recording:
            self.track.abort_record()
        elif self.state == self.State.playing:
            self.track.stop_playing()
        elif self.state == self.State.waiting:
            pass
        elif self.state == self.State.overdub:
            self.track.stop_playing()
            self.track.abort_record()
        else:
            print("Hit default in TrackController reset_button")

    def start_button(self):
        if self.last_state == self.State.idle:
            self.state = self.State.idle
        elif self.last_state == self.State.recording:
            self.state = self.State.idle
        elif self.last_state == self.State.playing:
            self.track.start_playing()
            self.state = self.State.playing
        elif self.last_state == self.State.waiting:
            self.state = self.State.waiting
        elif self.last_state == self.State.overdub:
            self.track.start_playing()
            self.state = self.State.playing
        else:
            self.state = self.State.idle
            print("Hit default in TrackController start_button")

    def stop_button(self):
        self.last_state = self.state # Do I need to copy this somehow?
        if self.last_state == self.State.idle:
            pass
        elif self.last_state == self.State.recording:
            self.track.abort_record()
        elif self.last_state == self.State.playing:
            self.track.stop_playing()
        elif self.last_state == self.State.waiting:
            pass
        elif self.last_state == self.State.overdub:
            self.track.stop_playing()
            self.track.abort_record()
        else:
            print("Hit default in TrackController stop_button")

    def is_recording(self):
        return self.state == self.State.recording

    def is_playing(self):
        return self.state == self.State.playing