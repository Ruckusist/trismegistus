#ifndef _AMLCD_H
#define _AMLCD_H

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
