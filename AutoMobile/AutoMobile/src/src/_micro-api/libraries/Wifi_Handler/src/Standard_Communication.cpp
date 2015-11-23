
#include "Wifi_Handler.h"
using namespace NODEMCU;

Com::Com() {
	// THIS is what i want... A@B|Function,data~
	// THAT IS AWEFUL...
	//...
	/*
	Build A mode Selector
	
	1. Handshake -- If Comm is availiable ... find out who...
		Communicate with them...
		-- DisConnect
		-- Return Mode

	2. WebServer
		- Always On
	3. Unity Connect
		- Always Available
	4

	*/
}


bool Com::ParseIncData(String data) {

	int _BEGIN = 0;
	int _TO = data.indexOf('@');
	int _BREAK = data.indexOf('|');
	int _SPLIT = data.indexOf(',');
	int _END = data.indexOf('~');

	String TO_ = (status == WL_CONNECTED) ? data.substring(_BEGIN, _TO) : data;     // ERROR IF POOR FORMAT
	String FROM_ = (status == WL_CONNECTED) ? data.substring(_TO, _BREAK) : "";
	String FUNCTION_ = (status == WL_CONNECTED) ? data.substring(_BREAK, _SPLIT) : data.substring(_BEGIN, _SPLIT);
	String DATA_ = (status == WL_CONNECTED) ? data.substring(_SPLIT, _END) : "";

	// BACK UP THE COMMAND AT THIS POINT TO THE SD CARD
	String _Backup = "To: " + TO_ + "From: "+ FROM_ + "Function: " + FUNCTION_ + "data: " + DATA_;
	//BackupSD(_Backup);                         // TO BE DONE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	// SET INPUT - Global
	int I = TO_.toInt();
	switch (I) {
	case 'A': 
		Input = _bluetooth;
		break;
	case 'B':
		Input = _mega;
		break;
	case 'C':
		Input = _node;
		break;
	case 'D':
		Input = _UDP;
		break;
	case 'E':
		Input = _web;
		break;
	default:
		Input = _web;
		break;
	}
	// SET OUTPUT - Global
	int O = FROM_.toInt();
	switch (O) {
	case 'A':
		Input = _bluetooth;
		break;
	case 'B':
		Input = _mega;
		break;
	case 'C':
		Input = _node;
		break;
	case 'D':
		Input = _UDP;
		break;
	case 'E':
		Input = _web;
		break;
	default:
		Input = _node;
		break;
	}
	// CALL FUNCTION - Global
	int F = FUNCTION_.toInt();
	switch (F) {
	case 1: // List networks
		Node.listNetworks();
		break;

	case 2: // Set SSID
	  // should clear this data first.. and then pass it into a known ssid database
		//ssid = data;
		break;

	case 3: // Set Password
		//pass = data;
		break;

	case 4: // Connect/Disconnect
		if (status == WL_CONNECTED) {
			//disconnect();
		}
		else {
			//connect(true);
		}
		break;

	case 5: // Set Unity IP
		//setUnityIP(data);
		break;

	case 6: // SEND DATA TO UNITY
		sendToUnity(data);
		break;

	case 7: // SEND DATA TO ARDUINO
		//sendToArduino(data);
		break;

	case 0: // Show Menu -OR- Not a valid integer -- fall through to default
	default: // Unknown command

		if (FUNCTION_.equals("0")) { // Actually show menu
			//showMenuUDP();
		}
	}

	//int comma = data.indexOf(',');
	//String cmdStr = (comma > 0) ? data.substring(0, comma) : data;
	//int cmd = cmdStr.toInt();
	//String data = (comma > 0) ? data.substring(comma + 1) : "";
	
	// IN and OUT have been set to global preset...


	return true;
}

void Com::MENU(String output) {
	// MENU
#define QQ + "\n"
	String _menu = ("|------######|||| AutoMobile Main MENU ||||######------|" QQ);
	_menu += "" QQ;

	switch (output) {
	case 1: // is bluetooth
		Serial.print("1, " + _menu);
		break;
	case 2: // is Mega
		Serial.print("2, " + _menu);
		break;
	case 3: // is Node
		Serial.print("3, " + _menu);
		break;
	case 4:

	}
}


void Com::sendToUnity(String data) {
	WiFiUDP send;
	  if (status == WL_CONNECTED) {
		if (send.beginPacket(remotePort, remotePort) == 1) {  // BEGIN TRANSMISSION!

		  // Attach the received data line
		  //send.write("just a test\n");
		  //data = String("testing again");
		  char buffer[data.length() + 1];                    // WTF is the red but not erroring out... not gonna worry anymore...
		  data.toCharArray(buffer, data.length() + 1);
		  send.write(buffer);

		  // End UDP packet
		  send.endPacket();                              // END THE PACKET AND START AGAIN!!!!!!
		}
	  }
}

