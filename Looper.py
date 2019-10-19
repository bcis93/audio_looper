from enum import Enum
import Globals
import time

class Looper:
    class State(Enum):
        idle = 0
        first_recording = 1
        normal_operation = 2
        stopped = 3

    state = State.idle
    start_time = 0

    def __init__(self, rec_play_button, start_stop_button, reset_button, track_controllers):
        self.rec_play_button = rec_play_button
        self.start_stop_button = start_stop_button
        self.reset_button = reset_button
        self.buttons = [self.rec_play_button, self.start_stop_button, self.reset_button]
        self.track_controllers = []
        self.master_track = None
        self.track_length = 0
        self.track_controllers = track_controllers
        # self.state = 

    def tick(self):

        for track_controller in self.track_controllers:
            track_controller.tick()

        for button in self.buttons:
            button.tick()

        rec_play_button_pressed = self.rec_play_button.fell()
        start_stop_button_pressed = self.start_stop_button.fell()
        reset_button_pressed = self.reset_button.fell()
        
        if reset_button_pressed:
            print("Reset button pressed!")
            self.reset_pressed()

        if start_stop_button_pressed:
            if self.state is self.State.stopped:
                self.start_button()
                self.state = self.State.normal_operation
                self.start_time = time.time()
            else:
                self.stop_button()
                self.state = self.State.stopped

        #state action
        if self.state is self.State.idle:
            self.start_time = time.time()
        elif self.state is self.State.first_recording:
            pass
        elif self.state is self.State.normal_operation:
            # if Globals.master_done:
            #     Globals.master_done = False
            # if Globals.audio_wrap_around:
            #     Globals.audio_wrap_around = False
            #     Globals.master_done = True
                # self.start_time = time.time()
            if rec_play_button_pressed:
                print("record/play button pressed!")
                Globals.recording_mode = not Globals.recording_mode
        elif self.state is self.State.stopped:
            pass
        else:
            print("hit default state in looper state action")

        #state update
        if self.state is self.State.idle:
            for track_controller in self.track_controllers:
                if track_controller.is_recording():
                    self.master_track = track_controller
                    self.state = self.State.first_recording
                    print("first recording")
        elif self.state is self.State.first_recording:
            if self.master_track.is_playing():
                self.track_length = time.time() - self.start_time
                self.start_time = time.time()
                self.state = self.State.normal_operation
                print("normal operation")
        elif self.state is self.State.normal_operation:
            pass
        elif self.state is self.State.stopped:
            pass
        else:
            print("hit default state in looper state update")
                


    
    def addTrack(self, track):
        pass

    def test(self):
        return 2

    def reset_pressed(self):
        for track_controller in self.track_controllers:
            track_controller.reset_button()
        
        self.state = self.State.idle
        Globals.recording_mode = True
        self.master_track = None
        Globals.waiting_to_start = 0

    def start_button(self):
        for track_controller in self.track_controllers:
            track_controller.start_button()

    def stop_button(self):
        for track_controller in self.track_controllers:
            track_controller.stop_button()

