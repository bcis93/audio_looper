import RPi.GPIO as GPIO
import pyaudio
import wave
import sys
import time

BUTTON_PIN = 21
GPIO.setmode(GPIO.BCM)
GPIO.setup(BUTTON_PIN, GPIO.IN, pull_up_down=GPIO.PUD_UP)

form_1 = pyaudio.paInt16 # 16-bit resolution
chans = 1 # 1 channel
samp_rate = 44100 # 44.1kHz sampling rate
chunk = 4096 # 2^12 samples for buffer
record_secs = 5 # seconds to record
dev_index = 2 # device index found by p.get_device_info_by_index(ii)
wav_output_filename = 'test1.wav' # name of .wav file

audio = pyaudio.PyAudio() # create pyaudio instantiation

frames = []
current_position = 0

stream = None
output_stream = None


def wait_for_button_press():
	while GPIO.input(BUTTON_PIN):
		pass
        # do nothing...

def recording_callback(in_data, frame_count, time_info, status):
	# data = stream.read(chunk)
	frames.append(in_data)
	return (in_data, pyaudio.paContinue)

def start_recording():
	# create pyaudio stream
	global stream
	stream = audio.open(format=form_1,
						rate=samp_rate,
						channels=chans,
						input_device_index=dev_index,
						input=True,
						frames_per_buffer=chunk,
						stream_callback=recording_callback)
	time.sleep(0.01)
	# start recording
	stream.start_stream()
	print('Recording')

def stop_recording():
	# stop stream
	global stream
	stream.stop_stream()
	print('Finished recording')
	
def playing_callback(in_data, frame_count, time_info, status):
	data_left = len(frames) - current_position
	if data_left > frame_count:
		samples_to_read = frame_count
	else:
		samples_to_read = data_left
	data = frames[current_position, current_position + samples_to_read]
	current_position += samples_to_read
	if current_position >= len(frames):
		current_position = 0
	return (data, pyaudio.paContinue)
	
def start_playing():
	global output_stream
	print('Starting playback')
	output_stream = audio.open(format=form_1,
				channels=chans,
				rate=samp_rate,
				output_device_index=dev_index,
				output=True,
				frames_per_buffer=chunk,
				stream_callback=playing_callback)
	output_stream.start_stream()

def stop_playing():
	output_stream.stop_stream()
	

############### start of main program ##################

print("waiting to start...")
wait_for_button_press()

start_recording()
time.sleep(0.2) # so we don't see another button press

# wait for button to stop recording
wait_for_button_press()
stop_recording()
time.sleep(0.2) # so we don't see another button press

print("waiting to playback...")
wait_for_button_press()
start_playing()
time.sleep(0.2) # so we don't see another button press

print('Waiting to stop playback')
wait_for_button_press()
stop_playing()
time.sleep(0.2) # so we don't see another button press
				
print("waiting to playback...")
while GPIO.input(BUTTON_PIN):
	pass
        # do nothing...

# play stream (looping from beginning of file to the end)
print('playing data')
for data in frames:
	# writing to the stream is what *actually* plays the sound.
	output_stream.write(data)
	

# stop the stream, close it, and terminate the pyaudio instantiation
stream.close()
output_stream.close()
audio.terminate()

# save the audio frames as .wav file
wavefile = wave.open(wav_output_filename,'wb')
wavefile.setnchannels(chans)
wavefile.setsampwidth(audio.get_sample_size(form_1))
wavefile.setframerate(samp_rate)
wavefile.writeframes(b''.join(frames))
wavefile.close()
