
#include "Buzzer.h"

Buzzer::Buzzer(uint8_t speaker_pin) {
	_speaker_pin = speaker_pin;
	pinMode(_speaker_pin, OUTPUT);
	
}

void Buzzer::Init(int SPEAKER) {
	
}

void Buzzer::buzz(unsigned int delayTime, unsigned long duration) {
	bool speak = false;

	for (int i = 0; i < duration / delayTime; i++) {
		speak = !speak;
		digitalWrite(_speaker_pin, speak);
		delay(delayTime);
	}
	digitalWrite(_speaker_pin, LOW);
}

// EXAMPLE playSong(bond_tempo, bond_melody, sizeof(bond_melody) / sizeof(int), 750);
void Buzzer::playSong(int _tempo[], int _Kelody[], int size, int slowness) {
  // LCD PRINT out to dispay should be in here
  for (int thisNote = 0; thisNote < size; thisNote++) {
    int noteDuration = slowness / _tempo[thisNote];
    buzz(_Kelody[thisNote], noteDuration);
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
  }
}
//#define SPEAKER _speaker_pin
//extern Buzzer buzzer(SPEAKER);

