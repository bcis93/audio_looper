#pragma once

class Button
{
public:
	Button();
	Button(int pin);
	~Button();
	virtual void tick(); //does this really need to be virtual?
	bool fell();
private:
	int buttonPin;
	bool pressed;
	int timeout;
};

