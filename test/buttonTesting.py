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

print("waiting...")
while GPIO.input(BUTTON_PIN):
	pass
        # do nothing...
print('Recording')
# create pyaudio stream
start_time = time.time()
stream = audio.open(format = form_1,rate = samp_rate,channels = chans, \
                    input_device_index = dev_index,input = True, \
                    frames_per_buffer=chunk)

while time.time() < start_time + 1 or GPIO.input(BUTTON_PIN):
	data = stream.read(chunk)
	frames.append(data)

print("finished recording")
time.sleep(0.5)

output_stream = audio.open(format=form_1,
				channels=chans,
				rate=samp_rate,
				output_device_index = dev_index,
				output=True,
				frames_per_buffer=chunk)
				
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
stream.stop_stream()
stream.close()
output_stream.stop_stream()
output_stream.close()
audio.terminate()

# save the audio frames as .wav file
wavefile = wave.open(wav_output_filename,'wb')
wavefile.setnchannels(chans)
wavefile.setsampwidth(audio.get_sample_size(form_1))
wavefile.setframerate(samp_rate)
wavefile.writeframes(b''.join(frames))
wavefile.close()
