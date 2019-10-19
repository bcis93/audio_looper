import pyaudio
import wave

# create an audio object
print('opening test1.wav')
wf = wave.open('test1.wav', 'rb')
p = pyaudio.PyAudio()
chunk = 4096
dev_index = 2

# open stream based on the wave object which has been input.
print('opening stream')
print(wf.getnchannels())
print(wf.getframerate())
stream = p.open(format=pyaudio.paInt16,
				channels=wf.getnchannels(),
				rate=wf.getframerate(),
				output_device_index = dev_index,
				output=True)

# read data (based on the chunk size)
print('reading data')
data = wf.readframes(chunk)

# play stream (looping from beginning of file to the end)
print('playing data')
while data != '':
	# writing to the stream is what *actually* plays the sound.
	stream.write(data)
	data = wf.readframes(chunk)

	# cleanup stuff.
stream.close()
p.terminate() 