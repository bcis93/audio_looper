import pyaudio
import time

class AudioInterface:
    form_1 = pyaudio.paInt16 # 16-bit resolution
    bytes_per_sample = 2 # 16-bit resolution
    chans = 2 # 2 channels - I really only need 1, but the audio playback was all choppy with just 1...
    samp_rate = 44100 # 44.1kHz sampling rate
    chunk = 4096 # 2^12 samples for buffer
    # record_secs = 5 # seconds to record
    dev_index = 2 # device index found by p.get_device_info_by_index(ii)
    # wav_output_filename = 'test1.wav' # name of .wav file


    def init(self):
        self.audio = pyaudio.PyAudio() # create pyaudio instantiation
        self.frames = []
        self.current_position = 0
        self.stream = None
        self.output_stream = None

    def recording_callback(self, in_data, frame_count, time_info, status):
        # data = stream.read(chunk)
        self.frames.extend(in_data)
        return (in_data, pyaudio.paContinue)

    def playing_callback(self, in_data, frame_count, time_info, status):
        # data_left = len(self.frames) - self.current_position
        # if data_left > frame_count:
        #     samples_to_read = frame_count
        # else:
        #     samples_to_read = data_left
        # data = self.frames[self.current_position: self.current_position + samples_to_read]
        # self.current_position += samples_to_read
        # if self.current_position >= len(self.frames):
        #     self.current_position = 0
        # return (data, pyaudio.paContinue)

        data = bytearray()

        samples_to_write = frame_count * self.chans * self.bytes_per_sample

        data_left = len(self.frames) - self.current_position
        if data_left < samples_to_write:
            data = self.frames[self.current_position:]
            data.extend(self.frames[0:samples_to_write - data_left])
            self.current_position = samples_to_write - data_left
            # TODO: also set master_done if master track??
        else:
            data = self.frames[self.current_position:self.current_position + samples_to_write]
            self.current_position += samples_to_write
        
        return (bytes(data), pyaudio.paContinue)

    def start_recording(self, frames):
        self.frames = frames
        # create pyaudio stream
        self.stream = self.audio.open(format=self.form_1,
                                    rate=self.samp_rate,
                                    channels=self.chans,
                                    input_device_index=self.dev_index,
                                    input=True,
                                    frames_per_buffer=self.chunk,
                                    stream_callback=self.recording_callback)
        time.sleep(0.01)
        # start recording
        self.stream.start_stream()
        print('Stream recording')

    def stop_recording(self):
        # stop stream
        self.stream.stop_stream()
        print('Stream finished recording')

    def start_playing(self, frames):
        self.frames = frames
        print('Stream starting playback')
        self.output_stream = self.audio.open(format=self.form_1,
                                            channels=self.chans,
                                            rate=self.samp_rate,
                                            output_device_index=self.dev_index,
                                            output=True,
                                            frames_per_buffer=self.chunk,
                                            stream_callback=self.playing_callback)
        self.output_stream.start_stream()

    def stop_playing(self):
        self.output_stream.stop_stream()
        self.current_position = 0

