
#ifndef _AUTOMOBILE_H
#define _AUTOMOBILE_H

#include <arduino.h>
#include "DEFINITIONS.h"
#include "AMLED.h"
#include "AMAccelGyro.h"
#include "AMLCD.h"
#include "AMDrive.h"

class AutoMobile {

protected:

	AMLED green;
	AMLED red;
	AMLED blue;
	AMLED yellow;

	AMLCD lcd;


	AMAccelGyro accelGyro;
	HardwareSerial* sensor3Serial;
	bool sensorsEnabled = false;
//	bool sensors2Enabled = false;

	bool select(HardwareSerial& serial);
	void printSensors(HardwareSerial& serial);
	void showMenu(HardwareSerial& serial);
	void sendToBluetooth(HardwareSerial& serial, String data);

public:
	void setup();
	void loop();
};


#endif
