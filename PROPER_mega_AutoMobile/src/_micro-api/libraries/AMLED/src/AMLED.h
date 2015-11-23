// AMLED.h

#ifndef _AMLED_h
#define _AMLED_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class AMLED {

protected:
	int pin;
	bool on = false;

public:
	void init(int pin);
	bool blink();
	void light();
	void dark();
};
#endif

