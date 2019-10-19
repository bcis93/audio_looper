import RPi.GPIO as GPIO
import pyaudio
import wave
import sys
import time

BUTTON_PIN = 23
GPIO.setmode(GPIO.BCM)
GPIO.setup(BUTTON_PIN, GPIO.IN, pull_up_down=GPIO.PUD_UP)

form_1 = pyaudio.paInt16 # 16-bit resolution
chans = 2 # 1 channel
samp_rate = 44100 # 44.1kHz sampling rate
chunk = 1024 # 2^12 samples for buffer
record_secs = 5 # seconds to record
dev_index = 2 # device index found by p.get_device_info_by_index(ii)
wav_output_filename = 'test1.wav' # name of .wav file

audio = pyaudio.PyAudio() # create pyaudio instantiation

frames = bytearray()
current_position = 0

stream = None

def wait_for_button_press():
	while GPIO.input(BUTTON_PIN):
		pass
        # do nothing...

def callback(in_data, frame_count, time_info, status):
	# frames = bytearray()
	# frames.extend(in_data)
	# frame_count *= ( 2 * chans)
	# print("frame_count: {}".format(frame_count))
	# print("in_data: {}".format(len(in_data)))
	return (in_data, pyaudio.paContinue)


print("waiting to start...")
wait_for_button_press()

stream = audio.open(format=form_1,
						rate=samp_rate,
						channels=chans,
						input_device_index=dev_index,
						output_device_index=dev_index,
						input=True,
                        output=True,
						frames_per_buffer=chunk,
						stream_callback=callback)

time.sleep(0.01)
stream.start_stream()

time.sleep(0.2) # so we don't see another button press

print("waiting to stop...")
wait_for_button_press()
stream.stop_stream()

# stop the stream, close it, and terminate the pyaudio instantiation
stream.close()
audio.terminate()