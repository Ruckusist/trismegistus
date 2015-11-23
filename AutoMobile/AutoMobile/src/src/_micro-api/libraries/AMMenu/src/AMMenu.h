// AMMenu.h

#ifndef _AMMENU_h
#define _AMMENU_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "AMComponent.h"


#define MAX_MENU_OPTIONS 10



class AMMenuOption {
public:
	AMMenuOption(String text);
	void display(HardwareSerial& serial);
	virtual void trigger(String source, String data) = 0;

protected:
	String text;

};



class ShowMenu : AMMenuOption {
public:
	ShowMenu(AMMenu* menu);
	void trigger(String source, String data);

protected:
	AMMenu* menu;
};




class AMMenu {
public:
	AMMenu();
	void display(AMComponent& src, AMComponent& dest);
	void handle(String source, HardwareSerial& serial);


protected:
	AMMenuOption* options[MAX_MENU_OPTIONS];
	int optionIdx = -1;

	void addOption(AMMenuOption* option);

};



#endif
