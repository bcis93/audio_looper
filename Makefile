ifeq ($(CC),)
	CC=$(CROSS_COMPILE)g++
endif
ifeq ($(CFLAGS),)
	CFLAGS = -g -Wall -Werror -O3
endif
ifeq ($(LDFLAGS),)
	LDFLAGS = -lrt -lasound -lpthread -lportaudio -lbcm2835
endif

all: looper basic

looper: *.cpp
	$(CC) $(CFLAGS) $^ -o $@ $(INCLUDES) $(LDFLAGS)

basic: basic/main.cpp
	$(CC) $(CFLAGS) $^ -o $@ $(INCLUDES) $(LDFLAGS)

clean:
	rm looper
	rm basic
