looper: *.cpp lib-tlc59711/tlc59711.cpp
	g++ *.cpp -Wall -lrt -lasound -lpthread -lportaudio -lbcm2835 -o looper -O3

clean:
	rm looper
