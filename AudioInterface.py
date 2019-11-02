import pyaudio
import time
from operator import add
import Globals
import threading

class AudioInterface:
    form_1 = pyaudio.paInt16 # 16-bit resolution
    bytes_per_sample = 2 # 16-bit resolution
    chans = 2 # 2 channels - I really only need 1, but the audio playback was all choppy with just 1...
    samp_rate = 44100 # 44.1kHz sampling rate
    chunk = 4096 # 2^12 samples for buffer
    # record_secs = 5 # seconds to record
    dev_index = 2 # device index found by p.get_device_info_by_index(ii)
    # wav_output_filename = 'test1.wav' # name of .wav file

    
    current_position = 0
    thread_lock = threading.Lock()
    stream = None
    output_stream = None

    track_count = 0
    tracks = []

    first_recording = True

    


    def init(self):
        self.audio = pyaudio.PyAudio() # create pyaudio instantiation

        # create pyaudio stream
        self.stream = self.audio.open(format=self.form_1,
                                    rate=self.samp_rate,
                                    channels=self.chans,
                                    input_device_index=self.dev_index,
                                    output_device_index=self.dev_index,
                                    input=True,
                                    output=True,
                                    frames_per_buffer=self.chunk,
                                    stream_callback=self.audio_callback)
        time.sleep(0.01)
        self.stream.start_stream()

    def audio_callback(self, in_data, frame_count, time_info, status):
        print(self.current_position)
        print("status: {}".format(status))
        print("in_data[0]: {}".format(in_data[0]))
        self.thread_lock.acquire()
        ############ record audio ###################
        
        samples_to_write = len(in_data)

        if Globals.waiting_to_start == 1: # first recording
            for track in self.tracks:
                if track.recording:
                    track.frames.extend(in_data)
                    print("adding data")
                else:
                    track.frames.extend(b'x\00'*samples_to_write)
            self.current_position += samples_to_write
        elif Globals.waiting_to_start >= 1: # not first recording
            space_left = Globals.track_length - self.current_position
            if space_left < samples_to_write:
                for track in self.tracks:
                    if track.recording:
                        track.frames[self.current_position:self.current_position + space_left] = list(map(add, track.frames[self.current_position:self.current_position + space_left], in_data[0:space_left]))
                        track.frames[0:samples_to_write-space_left] = list(map(add, track.frames[0:samples_to_write-space_left], in_data[space_left:]))
            else:
                for track in self.tracks:
                    if track.recording:
                        track.frames[self.current_position:self.current_position + samples_to_write] = list(map(add, track.frames[self.current_position:self.current_position + samples_to_write], in_data))
        # current_position should be updated, but I'll need to either combine into a single callback or create separate positions first
        
        ############ play audio ###################
        samples_to_write = frame_count * self.chans * self.bytes_per_sample
        
        if Globals.waiting_to_start <= 1:  # If first recording (meaning we don't have anything to play back yet)
            data = bytearray(in_data)
        else:
            data_left = Globals.track_length - self.current_position

            if data_left < samples_to_write:
                #data = bytearray(data_left)
                for track in self.tracks:
                    if track.playing:
                        print("playing data")
                        #data = list(map(add, data, track.frames[self.current_position:]))
                        data = track.frames[self.current_position:]
                
                #extra = bytearray(samples_to_write - data_left)
                for track in self.tracks:
                    if track.playing:
                        #extra = list(map(add, extra, track.frames[0:samples_to_write - data_left]))
                        extra = track.frames[0:samples_to_write - data_left]

                data.extend(extra)
                self.current_position = samples_to_write - data_left
                # TODO: also set master_done if master track??
                Globals.audio_wrap_around = True # set a flag so the main loop knows to set master_done to true for one cycle
            else:
                data = bytearray(samples_to_write)
                for track in self.tracks:
                    if track.playing:
                        pass
                        #data = list(map(add, data, track.frames[self.current_position:self.current_position + samples_to_write]))
                        #data = track.frames[self.current_position:self.current_position + samples_to_write]
                self.current_position += samples_to_write
        
        self.thread_lock.release()
        print("data: {}".format(data[0]))
        print("data len: {}".format(len(data)))
        return (bytes(data), pyaudio.paContinue)

    # def recording_callback(self, in_data, frame_count, time_info, status):
    #     samples_to_write = len(in_data)
    #     space_left = Globals.track_length - self.current_position
    #     if Globals.waiting_to_start <= 1: # first recording
    #         for track in self.tracks:
    #             if track.recording:
    #                 self.frames.extend(in_data)
    #     else: # not first recording
    #         if space_left < samples_to_write:
    #             for track in self.tracks:
    #                 if track.recording:
    #                     self.frames[self.current_position:self.current_position + space_left] = list(map(add, self.frames[self.current_position:self.current_position + space_left], in_data[0:space_left]))
    #                     self.frames[0:samples_to_write-space_left] = list(map(add, self.frames[0:samples_to_write-space_left], in_data[space_left:]))
    #         else:
    #             for track in self.tracks:
    #                 if track.recording:
    #                     self.frames[self.current_position:self.current_position + samples_to_write] = list(map(add, self.frames[self.current_position:self.current_position + samples_to_write], in_data))
    #     # current_position should be updated, but I'll need to either combine into a single callback or create separate positions first
    #     return (in_data, pyaudio.paContinue)

    # def playing_callback(self, in_data, frame_count, time_info, status):
    #     samples_to_write = frame_count * self.chans * self.bytes_per_sample
        

    #     data_left = Globals.track_length - self.current_position

    #     if data_left < samples_to_write:
    #         data = bytearray(data_left)
    #         for track in self.tracks:
    #             if track.playing:
    #                 data = list(map(add, data, track.frames[self.current_position:]))
            
    #         extra = bytearray(samples_to_write - data_left)
    #         for track in self.tracks:
    #             if track.playing:
    #                 extra = list(map(add, extra, track.frames[0:samples_to_write - data_left]))

    #         data.extend(extra)
    #         self.current_position = samples_to_write - data_left
    #         # TODO: also set master_done if master track??
    #     else:
    #         data = bytearray(samples_to_write)
    #         for track in self.tracks:
    #             if track.playing:
    #                 data = list(map(add, data, track.frames[self.current_position:self.current_position + samples_to_write]))
    #         self.current_position += samples_to_write
        
    #     return (bytes(data), pyaudio.paContinue)

    # def start_first_recording(self):
    #     # create pyaudio stream
    #     self.stream = self.audio.open(format=self.form_1,
    #                                 rate=self.samp_rate,
    #                                 channels=self.chans,
    #                                 input_device_index=self.dev_index,
    #                                 output_device_index=self.dev_index,
    #                                 input=True,
    #                                 output=True,
    #                                 frames_per_buffer=self.chunk,
    #                                 stream_callback=self.recording_callback)
    #     time.sleep(0.01)
    #     self.stream.start_stream()

    # def start_recording(self):
    #     if self.first_recording:
    #         self.start_first_recording()
        
    #     print('Stream recording')

    # def stop_recording(self):
    #     # stop stream
    #     # self.stream.stop_stream()
    #     if self.first_recording:
    #         Globals.track_length = self.current_position # I need to fix the current_position issue...
    #     print('Stream finished recording')

    # def start_playing(self, frames):
    #     self.frames = frames
    #     print('Stream starting playback')
    #     self.output_stream = self.audio.open(format=self.form_1,
    #                                         channels=self.chans,
    #                                         rate=self.samp_rate,
    #                                         output_device_index=self.dev_index,
    #                                         output=True,
    #                                         frames_per_buffer=self.chunk,
    #                                         stream_callback=self.playing_callback)
    #     self.output_stream.start_stream()

    # def stop_playing(self):
    #     self.output_stream.stop_stream()
    #     self.current_position = 0

    def add_track(self, track):
        track_id = self.track_count
        self.track_count += 1
        self.tracks.append(track)
        # return track_id # I probably don't need this

    def set_global_track_length(self):
        self.thread_lock.acquire()
        Globals.track_length = self.current_position
        self.thread_lock.release()

    def reset_current_position(self):
        self.thread_lock.acquire()
        self.current_position = 0
        self.thread_lock.release()

    # def keep_running(self):
    #     if not self.stream.is_active():
    #         self.stream.start_stream()

