// AMComponent.h

#ifndef _AMCOMPONENT_h
#define _AMCOMPONENT_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


class AMComponent {
public:
	AMComponent(String label, String desc);
	virtual void print(String text);
	virtual void println();
	void tag(AMComponent& dest);

protected:
	String label;
	String desc;

};


#endif

