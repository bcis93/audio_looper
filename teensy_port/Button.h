#pragma once
#include <Bounce2.h>

class Button
{
public:
	Button();
	Button(int pin);
	~Button();
	virtual void tick(); //does this really need to be virtual?
	bool fell();
private:
	Bounce button;
	int buttonPin;
	bool pressed;
};

