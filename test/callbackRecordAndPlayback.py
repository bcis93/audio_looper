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
chunk = 4096 # 2^12 samples for buffer
record_secs = 5 # seconds to record
dev_index = 2 # device index found by p.get_device_info_by_index(ii)
wav_output_filename = 'test1.wav' # name of .wav file

audio = pyaudio.PyAudio() # create pyaudio instantiation

frames = bytearray()
current_position = 0

stream = None
output_stream = None

recording = True
playing = False

first = True


def wait_for_button_press():
	while GPIO.input(BUTTON_PIN):
		pass
        # do nothing...

def recording_callback(in_data, frame_count, time_info, status):
	# data = stream.read(chunk)
	# if first:
	# 	in_data.extend(b'x\00'*frame_count)
	tmp = bytearray(in_data)
	tmp.extend(b'x\00'*frame_count)
	print(status)
	print("recording: len(in_data): {}, frame_count: {}".format(len(in_data), frame_count))
	frames.extend(in_data)
	return (bytes(tmp), pyaudio.paContinue)

def rec_play_callback(in_data, frame_count, time_info, status):
	global current_position
	global frames
	global recording
	global playing

	frame_count *= ( 2 * chans)

	if recording:
		data = bytearray(frame_count)
		frames.extend(in_data)
	elif playing:
		data = bytearray()
		data_left = len(frames) - current_position
		if data_left < frame_count:
			data = frames[current_position:]
			data.extend(frames[0:frame_count - data_left])
			current_position = frame_count - data_left
			# TODO: also set master_done if master track??
		else:
			data = frames[current_position:current_position + frame_count]
			current_position += frame_count
	else:
		data = bytearray(frame_count)
	return (bytes(data), pyaudio.paContinue)

def start_recording():
	# create pyaudio stream
	global stream
	stream = audio.open(format=form_1,
						rate=samp_rate,
						channels=chans,
						input_device_index=dev_index,
						output_device_index=dev_index,
						input=True,
						output=True,
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
	stream.close()
	print('Finished recording')
	global recording
	recording = False
	
def playing_callback(in_data, frame_count, time_info, status):
	print("playing: frame_count: {}".format(frame_count))
	global current_position
	global frames

	data = bytearray()

	frame_count *= ( 2 * chans)

	data_left = len(frames) - current_position
	if data_left < frame_count:
		data = frames[current_position:]
		data.extend(frames[0:frame_count - data_left])
		current_position = frame_count - data_left
		# TODO: also set master_done if master track??
	else:
		data = frames[current_position:current_position + frame_count]
		current_position += frame_count
	#data = frames[current_position: current_position + samples_to_read]
	#current_position += samples_to_read
	
	# for pos in range(samples_to_read):
	# 	data.extend(frames[current_position + pos])
	# current_position += samples_to_read
	# if current_position >= l
	# for _ in range(frame_count):
	# 	data.extend(frames[current_position])
	# 	current_position += 1
	# 	if current_position >= len(frames):
	# 		current_position = 0
	# 		# TODO: also set master_done if master track??
	return (bytes(data), pyaudio.paContinue)
	
def start_playing():
	# global output_stream
	print('Starting playback')
	output_stream = audio.open(format=form_1,
				channels=chans,
				rate=samp_rate,
				output_device_index=dev_index,
				output=True,
				frames_per_buffer=chunk,
				stream_callback=playing_callback)
	output_stream.start_stream()
	global playing
	playing = True

def stop_playing():
	output_stream.stop_stream()
	global playing
	playing = False
	# stream.stop_stream()
	

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
# output_stream = audio.open(format=form_1,
# 				channels=chans,
# 				rate=samp_rate,
# 				output_device_index = dev_index,
# 				output=True,
# 				frames_per_buffer=chunk)
# for frame in frames:
#     output_stream.write(frame)

print('Waiting to stop playback')
wait_for_button_press()
stop_playing()
time.sleep(0.2) # so we don't see another button press

print("frames length: {}".format(len(frames)))
print(frames[0:100])

	

# stop the stream, close it, and terminate the pyaudio instantiation
stream.close()
audio.terminate()
