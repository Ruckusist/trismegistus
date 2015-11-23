/*
-----------Ruckusist - NodeMCU 2015-------------

NodeMCU is the arduino knock off with built in Wifi capabilities, its job it to pass data
back and for from Unity to the Arduino Platform. Its a pretty cool system with another
6 digital I/O ports left open, A lot more could be done here. Unity could do call back commands
to this board and use it to fire other functionality.

2015 AutoMobile -- "Kit" Project --

NEW PROPER NODEMCU BUILDING PROCESS
-Nov15 ::: Finishing code porting and build process to VS... should be a better process.

****11-3-15: New functionality added. The Speaker playing out tones from a library we had to build from
example code online. SD card reader is now functional but no file system or functionality has been
added because of it, but its there and hooked up. Because of the SD card and such all the pinouts have
been changed... and this update set will include all the new pinouts and most of the functionality.
but there is a lot that can still be done with both the speaker and the SD card.
Everything in the bluetooth commstack seems to work well... ive just now switch the UDP back to the
home settings... so hopefully that works again.
****

its possible that this is the wrong copy of the file because there is no Reciever on the UDP...
could be my fault...

okay i found it... it will be in the changesets now... so ... i dont know how good that is...
FILE UPDATED : 9-13-15
*/

//===========================================
// LIBRARIES
//===========================================
#include "AMComponent.h"
#include "AMLED.h"
#include "AMNode.h"
#include "AMMenu.h"
/*
#include "SD.h"                    // SD card basics... maybe there is a better lib outthere
#include <ESP8266httpUpdate.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WiFi.h>
#include "SPI.h"                   // Need this
#include "Wifi_Handler.h" // *RUCKUSLIB - Wifi / WebServer / UDP Capabilities
#include "Buzzer.h"       // *RUCKUSLIB - basic speaker functionality
//#include "songs.h"        // *RUCKUSLIB - included songs (takein a break? look for more songs! add them here...)
//=END LIBRARIES=============================
*/
//using namespace NODEMCU;
//===========================================
// DEVICE SETUP
//===========================================
#define _NODE Serial
#define _COM Serial.print
#define _COMLN Serial.println
#define _BAUD 9600

//Buzzer buzzer(15);    // LIB SETUP for Buzzer.h
//Node node;
//Com com;


AMNode node;

//=END DEVICE SETUP==========================

//===========================================
// STARTUP FUNCTION (called once on startup)
//===========================================
void setup() {
	/*
	// HELLO!... this machine only has one Comm port so far as i can tell... we are gonna call it NODE... for fun... 
	_NODE.begin(_BAUD);                              // none of this is working 
	_COMLN("NodeMCU_PROPER startup");                //    **
	_COMLN("Attempting to connect @ a Baud of: ");   //    **
	_COMLN(_BAUD);
	buzzer.buzz(3, 50);    // this is the basic buzz call... 3 is the tone, 50 is the duration
	//node.Init(true);             // Startup Wifi
	delay(250);            // this is a pause in Millis for basic speration of events
	//buzzer.playSong(bond_tempo, bond_melody, sizeof(bond_melody) / sizeof(int), 750);  // Although this is called it does nothing
	*/
}
//=END STARTUP===============================

//===========================================
// MAIN FUNCTION (called as often as possible 80Mhz )
//===========================================
void loop() {
	// This handles basic Web calls to this nodes IP address
	//node.HandleBasicWifi(true);          // This is now working as expected
//	node.Responder();                          // this should handle whatever SERVER is going

	node.loop();
}
//=END MAIN LOOP=============================
