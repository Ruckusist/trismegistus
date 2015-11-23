// AMNode.h

#ifndef _AMNODE_h
#define _AMNODE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "AMMenu.h"
#include "AMLED.h"

#include "AMComponent.h"


#define SERIAL_BAUD 9600


class System : AMComponent {
public:
	System();
};


class Console : AMComponent {
public:
	Console();
	void print(String text);
	void println();
};




class Buzz : AMMenuOption {
public:
	Buzz();
	void trigger(String source, String data);
};



class PlaySong : AMMenuOption {
public:
	PlaySong();
	void trigger(String source, String data);
};



class AMNode : AMMenu {
public:
	AMNode();
	void loop();

protected:
	const Console CONSOLE = Console();

	AMLED red = AMLED();
	AMLED green = AMLED();

};


#endif

