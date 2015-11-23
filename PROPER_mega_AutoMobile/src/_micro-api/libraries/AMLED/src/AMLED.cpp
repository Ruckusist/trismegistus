#include "AMLED.h"

void AMLED::init(int pin) {
	this->pin = pin;

	pinMode(pin, OUTPUT);
	digitalWrite(pin, LOW);
}

bool AMLED::blink() {
	on = !on;
	digitalWrite(pin, on);
}

void AMLED::light() {
	on = true;
	digitalWrite(pin, on);
}

void AMLED::dark() {
	on = false;
	digitalWrite(pin, on);
}    
