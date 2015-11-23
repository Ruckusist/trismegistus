// Buzzer.h

#ifndef _BUZZER_h
#define _BUZZER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
//#include "songs.h"

class Buzzer {
public:
	Buzzer(uint8_t speaker_pin);
	void Init(int SPEAKER);
	void buzz(unsigned int delayTime, unsigned long duration);
	void playSong(int _tempo[], int _Kelody[], int size, int slowness);
	uint8_t _speaker_pin;
private:

};

#endif