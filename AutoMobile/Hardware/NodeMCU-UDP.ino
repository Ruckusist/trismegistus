/*
-----------Ruckusist - NodeMCU 2015-------------

NodeMCU is the arduino knock off with built in Wifi capabilities, its job it to pass data
back and for from Unity to the Arduino Platform. Its a pretty cool system with another
6 digital I/O ports left open, A lot more could be done here. Unity could do call back commands
to this board and use it to fire other functionality.

2015 AutoMobile -- "Kit" Project --

its possible that this is the wrong copy of the file because there is no Reciever on the UDP...
could be my fault...

okay i found it... it will be in the changesets now... so ... i dont know how good that is...
FILE UPDATED : 9-13-15
*/

// --------------------------------- added Libraries
#include "ESP8266WiFi.h"           // Replaces Stock wifi with module features
#include "ESP8266WiFiMulti.h"      // Creates the AutoConnect list
#include "ESP8266mDNS.h"           // This is for the DNS 
#include "WiFiClient.h"            // this is the Stock Wifi Client with Refs on the Arduino site
#include "SPI.h"                   // SPI is a comm like Wire
#include "EEPROM.h"                // Access to internal memory
#include "Wire.h"                  // Comm for condensed comms between sensors
#include "WiFiUdp.h"               // Wifi Transmit fuctionality
//#include "pitches.h"               // predefined pitches for speaker


//DEFINE NAME      PINHOLE
// --------------------------------- PINOUTs
#define ledG          14
#define ledR          12
#define mled1          4
#define mled2          5
#define mled3          0 // this is being used for something else
#define SPEAKER_PIN   13
// LOL! all this with just an LED...

// --------------------------------- VARIABLES FOR ALL THINGS!
bool blinkStateG = false;
bool blinkStateR = false;
char packetBuffer[255];                     //buffer to hold incoming packet
char  ReplyBuffer[] = "acknowledged";       // a string to send back

											// -- Serial (arduino) --
unsigned int SERIAL_BAUD = 9600;

// -- Speaker --
// notes in the melody:
int melody[] = {
	NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
	4, 8, 8, 4, 4, 4, 4, 4
};


// -- Wifi (unity) --
ESP8266WiFiMulti wifiMulti;     // Autoconnect for Wifi to known addresses... using addAP
WiFiUDP udpRec;					// UDP receiver
WiFiUDP udpSend;                // UDP sender

							   //char ssid[] = "yourNetwork";       //  your network SSID (name)
							   //char pass[] = "secretPassword";    // your network password (use for WPA, or use as key for WEP)
unsigned int localPort = 7799; // local port to listen on

IPAddress remoteAddr(192, 168, 2, 137); // address of the server (Unity)
unsigned int remotePort1 = 9977;
int status = WL_IDLE_STATUS;



/**************************

			SETUP

***************************/
void setup() {

	//DEFINE PINMODES FOR ALL PINS
	pinMode(ledG, OUTPUT);
	pinMode(ledR, OUTPUT);


	// --------------------------------- START UP OPTIONS
	digitalWrite(ledG, LOW);
	digitalWrite(ledR, HIGH);

	Serial.begin(SERIAL_BAUD);
	delay(100);

	if (WiFi.status() == WL_NO_SHIELD) {
		// do nothing forever
		while (true);
	}

	// attempt to connect to Known Wifi networks:
	digitalWrite(ledR, blinkStateR);
	wifiMulti.addAP("Ruckusist", "eric2344");
	wifiMulti.addAP("Ruckus", "eric2344");
	//wifiMulti.addAP("ssid_from_AP_3", "your_password_for_AP_3");   // add more known wifis here...

	status = WiFi.status();

	// wait for connection  -- Manual Connection Protocol
	while (status != WL_CONNECTED) {
		delay(50);
		blinkStateR = !blinkStateR;
		digitalWrite(ledR, blinkStateR);
		status = WiFi.status();
	}
	digitalWrite(ledR, HIGH);

	udpRec.begin(localPort);

	// setup complete!!!
	digitalWrite(ledR, LOW);

	// iterate over the notes of the melody:
	for (int thisNote = 0; thisNote < 8; thisNote++) {

		// to calculate the note duration, take one second
		// divided by the note type.
		//e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
		int noteDuration = 1000 / noteDurations[thisNote];
		tone(SPEAKER_PIN, melody[thisNote], noteDuration);

		// to distinguish the notes, set a minimum time between them.
		// the note's duration + 30% seems to work well:
		int pauseBetweenNotes = noteDuration * 1.30;
		delay(pauseBetweenNotes);
		// stop the tone playing:
		noTone(SPEAKER_PIN);
	}

}



/**************************

			LOOP

***************************/
void loop() {
	WORKING();
}

bool unityReceived = false;




/**************************

			WORKING

***************************/
void WORKING() {
	// Blink for Status...
	blinkStateG = !blinkStateG;
	digitalWrite(ledG, blinkStateG);
	// ---
	// SERIAL RECEIVE (Arduino) -> WIFI SEND to Unity
	// ---

		
	IPAddress ip = WiFi.localIP();                 // Gather the Local Ip of the NodeMCU

	String line = Serial.readStringUntil('\n');

	if (line != NULL) {                              // if line isnt null...

		int comma = line.indexOf(',');
		int cmd = (comma>0) ? line.substring(0, comma).toInt() : line.toInt();

		switch (cmd) {
		case 0: // Show menu
			showMenu();
			break;

		case 5:	// SEND DATA TO UNITY
			sendToUnity(ip, (comma>0) ? line.substring(comma+1) : "");
			break;

		default:
			Serial.println("UNKOWN COMMAND: "+cmd);
			break;
		}
	}

	// ---
	// WIFI RECEIVE (Unity) -> SERIAL SEND TO ARDUINO
	// ---
	int size = udpRec.parsePacket();
	if (size > 0) {
		unityReceived = true;
		blinkStateR = !blinkStateR;
		digitalWrite(ledR, blinkStateR);

		bool doneSending = false;

		// for now we only take drive data...
		Serial.print("1,");

		for (int i = 0; i < size; i++) {
			char c = udpRec.read();

			if (c == 0) // our NULL-separator... what follows we don't send, it's our audio data
			{
				doneSending = true;
			}
			else if (doneSending) // this is our own data
			{
				String str = "";
				int tone = 0;
				int freq = 0;

				for (; i < size; i++) // read some more (increment same index)
				{
					c = udpRec.read();

					if (c == ',')
					{
						tone = str.toInt();
						str = "";
					}
					else
					{
						str = str + c;
					}
				}

				freq = str.toInt();
			}
			else // standard data, send it to arduino
			{
				Serial.print(c);
			}
		}
		Serial.println();                             // arduino is expecting a newline between "packets"
		delay(3);
	}
	else if (!unityReceived) {    // Unity has yet to send us anything (just started up?)
								  // send full stop with ip address
		Serial.print("0,0,");
		for (int i = 0; i < 4; i++) {
			String str = String(ip[i]);
			Serial.print(str);
			if (i < 3) {
				Serial.print('.');
			}
		}
		Serial.println();
	}
}


void showMenu() {
	Serial.println("NodeMCU Serial Menu");
	Serial.println();
	Serial.println("   0. Show Menu");
	Serial.println("   1. List WiFi Networks");
	Serial.println("   2. Set SSID <name>");
	Serial.println("   3. Set Password <password>");
	Serial.println("   4. Connect / Disconnect");
	Serial.println("   5. Send to Unity <DATA>");
	Serial.println();
}



void sendToUnity(IPAddress ip, String data) {

	// Begin UDP packet
	udpSend.beginPacket(remoteAddr, remotePort1);      // BEGIN THE PACKET TRANSMISSION!!!!!
													//sender.write ();                             // Blank spaceing

	// Insert IP address as first element to send
	for (int i = 0; i < 4; i++) {
		String str = String(ip[i]);
		for (int c = 0; c < str.length(); c++) {
			udpSend.write(str.charAt(c));
		}
		if (i < 3) {
			udpSend.write('.');
		}
	}

	// field separator
	udpSend.write(',');                               // Denotes a single data Element in Unity... The IP will be one thing

	// Attach the received data line
	for (int i = 0; i < data.length(); i++) {     
		udpSend.write(data.charAt(i));
	}

	// End UDP packet
	udpSend.endPacket();                              // END THE PACKET AND START AGAIN!!!!!!
}





void test_reciever() {                            // this is a more generic serial port test ... so you can see it...

	udpSend.beginPacket(udpSend.remoteIP(), udpSend.remotePort());
	udpSend.write(ReplyBuffer);
	blinkStateG = !blinkStateG;
	digitalWrite(ledG, blinkStateG);
	udpSend.endPacket();

	int len = udpSend.read(packetBuffer, 255);
	if (len > 0) {
		packetBuffer[len] = 0;
		blinkStateR = !blinkStateR;
		digitalWrite(ledR, blinkStateR);
	}
	Serial.println("Contents:");
	Serial.println(packetBuffer);
	// if there's data available, read a packet
	int packetSize = udpSend.parsePacket();
	if (packetSize) {

		Serial.print("Received packet of size ");
		Serial.println(packetSize);
		Serial.print("From ");
		IPAddress remoteIp = udpSend.remoteIP();
		Serial.print(remoteIp);
		Serial.print(", port ");
		Serial.println(udpSend.remotePort());
	}
	delay(3);
}