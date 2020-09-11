ifeq ($(CC),)
	CC=$(CROSS_COMPILE)g++
endif
ifeq ($(CFLAGS),)
	CFLAGS = -Wall -Werror -DNDEBUG
endif
ifeq ($(LDFLAGS),)
	LDFLAGS = -lbcm2835
endif

led_driver: main.cpp LedDriver.cpp tlc59711.cpp
	$(CC) $(CFLAGS) $^ -o $@ $(INCLUDES) $(LDFLAGS)

clean:
	rm led_driver
