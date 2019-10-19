from Looper import Looper
from Button import Button
from TrackController import TrackController
from Track import Track
from AudioInterface import AudioInterface
import RPi.GPIO as GPIO

REC_PLAY_BUTTON     = 16
RESET_BUTTON        = 12
START_STOP_BUTTON   = 8
TRACK_1_BUTTON      = 23
TRACK_2_BUTTON      = 18
TRACK_3_BUTTON      = 17
TRACK_4_BUTTON      = 14

print ("setting up...")

# Buttons
GPIO.setmode(GPIO.BCM)
recPlayButton = Button(REC_PLAY_BUTTON)
startStopButton = Button(START_STOP_BUTTON)
resetButton = Button(RESET_BUTTON)
track1Button = Button(TRACK_1_BUTTON)
track2Button = Button(TRACK_2_BUTTON)
track3Button = Button(TRACK_3_BUTTON)
track4Button = Button(TRACK_4_BUTTON)

audio_interface = AudioInterface()
audio_interface.init()

# Tracks
track1 = Track(audio_interface)
track2 = Track(audio_interface)
track3 = Track(audio_interface)
track4 = Track(audio_interface)

# TrackControllers
track1Controller = TrackController(track1, track1Button)
track2Controller = TrackController(track2, track2Button)
track3Controller = TrackController(track3, track3Button)
track4Controller = TrackController(track4, track4Button)

track_controllers = [track1Controller] #, track2Controller, track3Controller, track4Controller]

looper = Looper(recPlayButton, startStopButton, resetButton, track_controllers)
# looper = Looper(1,2,3)


#start main program
print("Setup complete!")
print("Starting normal operation...")

while True:
    looper.tick()