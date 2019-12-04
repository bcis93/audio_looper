looper: *.cpp lib-tlc59711/tlc59711.cpp
	g++ *.cpp lib-tlc59711/tlc59711.cpp -I lib-tlc59711 -Wall -lrt -lasound -lpthread -lportaudio -lbcm2835 -o looper -O3

no_leds: *.cpp
	g++ *.cpp -Wall -lrt -lasound -lpthread -lportaudio -lbcm2835 -o no_leds -O3 -DNO_LEDS

clean:
	rm looper
