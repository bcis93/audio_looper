led_driver: main.cpp LedDriver.cpp tlc59711.cpp
	g++ main.cpp LedDriver.cpp tlc59711.cpp -DNDEBUG -Wall -o led_driver -lbcm2835

led: *.cpp
	g++ *.cpp -DNDEBUG -Wall -o led -lbcm2835

clean:
	rm led
	rm led_driver
