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


//DEFINE NAME      PINHOLE
// --------------------------------- PINOUTs
#define ledG          14
#define ledR          12
// LOL! all this with just an LED...

// --------------------------------- VARIABLES FOR ALL THINGS!
bool blinkStateG = false;
bool blinkStateR = false;
char packetBuffer[255];                     //buffer to hold incoming packet
char  ReplyBuffer[] = "acknowledged";       // a string to send back

											// -- Serial (arduino) --
unsigned int SERIAL_BAUD = 9600;


// -- Wifi (unity) --
//WiFiUDP sender;              // so... we dont need this
WiFiUDP receiver;              // this is to set up a 2nd port for listening
WiFiUDP Udp;                   // for test_receiver *** Now being used for all sending ***

							   //char ssid[] = "yourNetwork";       //  your network SSID (name)
							   //char pass[] = "secretPassword";    // your network password (use for WPA, or use as key for WEP)
unsigned int localPort = 7799;       // local port to listen on

IPAddress remoteAddr(192, 168, 2, 137); // address of the server (Unity)
unsigned int remotePort1 = 9977;
int status = WL_IDLE_STATUS;

// --------------------------------- Devices

ESP8266WiFiMulti wifiMulti;                            // Autoconnect for Wifi to known addresses... using addAP

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

	receiver.begin(localPort);

	// setup complete!!!
	digitalWrite(ledR, LOW);
}

void loop() {
	WORKING();                // is not working...
}

void WORKING() {
	// Blink for Status...
	blinkStateG = !blinkStateG;
	digitalWrite(ledG, blinkStateG);
	// ---
	// SERIAL RECEIVE (Arduino) -> WIFI SEND to Unity
	// ---

	String line = Serial.readStringUntil('\n');

	if (line != NULL) {                              // if line isnt null...
		Udp.beginPacket(remoteAddr, remotePort1);      // BEGIN THE PACKET TRANSMISSION!!!!!
													   //sender.write ();                             // Blank spaceing
		IPAddress ip = WiFi.localIP();                 // Gather the Local Ip of the NodeMCU

		for (int i = 0; i < 4; i++) {                  // this does not appear to be getting through
			String str = String(ip[i]);
			for (int c = 0; c < str.length(); c++) {
				Udp.write(str.charAt(c));
			}
			if (i < 3) {
				Udp.write('.');
			}
		}
		Udp.write(',');                               // Denotes a single data Element in Unity... The IP will be one thing

		for (int i = 0; i < line.length(); i++) {     // this is working... but not enough elements are getting through...
			Udp.write(line.charAt(i));
		}
		//    sender.write ('\n');                    // new line ending
		Udp.endPacket();                              // END THE PACKET AND START AGAIN!!!!!!
	}

	// ---
	// WIFI RECEIVE (Unity) -> SERIAL SEND TO ARDUINO
	// ---
	int size = receiver.parsePacket();
	if (size > 0) {                                 // this is not working...
		blinkStateR = !blinkStateR;
		digitalWrite(ledR, blinkStateR);
		for (int i = 0; i < size; i++) {              // Fixed this... cant say WTF happened... good work...
			char c = receiver.read();
			Serial.print(c);
		}
		Serial.println();                             // arduino is expecting a newline between "packets"
		delay(3);
	}
}

void test_reciever() {                            // this is a more generic serial port test ... so you can see it...

	Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
	Udp.write(ReplyBuffer);
	blinkStateG = !blinkStateG;
	digitalWrite(ledG, blinkStateG);
	Udp.endPacket();

	int len = Udp.read(packetBuffer, 255);
	if (len > 0) {
		packetBuffer[len] = 0;
		blinkStateR = !blinkStateR;
		digitalWrite(ledR, blinkStateR);
	}
	Serial.println("Contents:");
	Serial.println(packetBuffer);
	// if there's data available, read a packet
	int packetSize = Udp.parsePacket();
	if (packetSize) {

		Serial.print("Received packet of size ");
		Serial.println(packetSize);
		Serial.print("From ");
		IPAddress remoteIp = Udp.remoteIP();
		Serial.print(remoteIp);
		Serial.print(", port ");
		Serial.println(Udp.remotePort());
	}
	delay(3);
}