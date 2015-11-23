/*
 * UNO TEST script
 * 
 * Testing out components on the Uno before putting to use.
 */
#include <Wtv020sd16p.h>
#include <EEPROM.h>

const unsigned int SERIAL_BAUD = 9600;

#define r_led        9
#define g_led        10
#define b_led        11
bool rblink = false;
bool gblink = false;
bool bblink = false;


#define SOUND_SENSOR A1

#define SPEAKER      8


#define AUDIO_RESET  2
#define AUDIO_CLOCK  3
#define AUDIO_DATA   4
#define AUDIO_BUSY   5
Wtv020sd16p audio(AUDIO_RESET, AUDIO_CLOCK, AUDIO_DATA, AUDIO_BUSY);


void setup() {
	pinMode(r_led, OUTPUT);
	pinMode(g_led, OUTPUT);
	pinMode(b_led, OUTPUT);
	
	digitalWrite(r_led, HIGH);
	digitalWrite(g_led, HIGH);
	digitalWrite(b_led, HIGH);

	Serial.begin(SERIAL_BAUD);
	while (!Serial) { }

	// clear the memory just once
	//clearMemory();
	
	int runs;
	EEPROM.get(0, runs);
	runs++;
	EEPROM.put(0, runs);
	
	Serial.println(String("This board has been booted ")+runs+" time"+((runs!=1 ? "s" : "")));
		
	/*
	pinMode(AUDIO_RESET, OUTPUT);
	pinMode(AUDIO_CLOCK, OUTPUT);
	pinMode(AUDIO_DATA, OUTPUT);
	pinMode(AUDIO_BUSY, OUTPUT);	
	*/
	

	for (int i=0; i<3000; i++) {
		tone(SPEAKER, i, 50);
	}
	for (int i=3000; i>=0; i--) {
		tone(SPEAKER, i, 50);
	}
	
	/*
	audio.reset();
	audio.playVoice(9);
	audio.reset();
	audio.playVoice(1);
	audio.reset();
	audio.playVoice(2);
	audio.reset();
	audio.playVoice(3);
	audio.reset();
	audio.playVoice(4);
//	audio.playVoice(5);
//	audio.playVoice(6);
	*/

}

void loop() {
	gblink = !gblink;
	digitalWrite(g_led, gblink);
	
//	Serial.println(analogRead(SOUND_SENSOR));
	
	delay(150);
}

void clearMemory() {
	for (int i = 0 ; i < EEPROM.length() ; i++) {
		EEPROM.write(i, 0);
	}
}