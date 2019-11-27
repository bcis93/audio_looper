#include "Arduino.h"
#include "Button.h"
#include "Globals.h"

Button::Button() {
}

Button::Button(int pin)
{
	buttonPin = pin;
	pinMode(buttonPin, INPUT_PULLUP);
	button.attach(buttonPin);
	button.interval(DEBOUNCE_TIME);
	pressed = false;
}


Button::~Button()
{
}

void Button::tick() {
	button.update();
}

bool Button::fell() {
	return button.fell();
}
