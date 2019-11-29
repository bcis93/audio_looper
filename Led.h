#pragma once

class Led
{
public:
	Led(void);
	Led(int channel);
	~Led(void);
	void tick(void);
	void turnOn(void);
    void turnOff(void);
    void flash(void);
private:
    int channel;
	int count;
	static bool bcm2835_initialized;
    bool flashing;
    bool led_on;
};