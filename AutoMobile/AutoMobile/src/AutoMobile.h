
#ifndef _AUTOMOBILE_H
#define _AUTOMOBILE_H

#include <arduino.h>
#include "lib/DEFINITIONS.h"
#include "lib/AMLED.h"
#include "lib/AMAccelGyro.h"
#include "lib/AMLCD.h"
#include "lib/AMDrive.h"

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
