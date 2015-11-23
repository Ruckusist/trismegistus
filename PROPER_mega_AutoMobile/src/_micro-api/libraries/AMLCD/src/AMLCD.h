// AMLCD.h

#ifndef _AMLCD_h
#define _AMLCD_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


#include <LiquidCrystal.h>


class AMLCD {

protected:
	//	LiquidCrystal *_lcd;
	String lcdMsg = "";   // for the second row (incoming message)

public:
	void init();
	void startupAnimation();

	void clear();
	void print(String text);
};

#endif

