import RPi.GPIO as GPIO
import time

class Button:
    button_fell = False
    def __init__(self, pin):
        self.pin = pin
        self.pressed = False
        # setup pin...
        GPIO.setup(pin, GPIO.IN, pull_up_down=GPIO.PUD_UP)
        GPIO.add_event_detect(pin, GPIO.FALLING, callback=self.button_callback, bouncetime=300)

    def tick(self):
        # button_state = GPIO.input(self.pin)
        # if not button_state:
        #     self.button_fell = True
        #     print("Button pressed")
        #     time.sleep(0.4) # This should be debounced...
        pass # do nothing for now...

    def fell(self):
        if self.button_fell:
            self.button_fell = False
            print("returning true")
            return True
        else:
            return False

    def button_callback(self, channel): # make sure this function works!!
        self.button_fell = True
        print("Button pressed")