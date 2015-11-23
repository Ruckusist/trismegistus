#ifndef _AMLED_H
#define _AMLED_H


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
