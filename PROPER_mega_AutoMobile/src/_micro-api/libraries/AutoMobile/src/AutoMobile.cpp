
#include <arduino.h>
#include "AutoMobile.h"
#include "DEFINITIONS.h"
#include "AMLED.h"
#include "AMAccelGyro.h"
#include "AMLCD.h"
#include "AMDrive.h"


AutoMobile::AutoMobile() {
	setup();
}

void AutoMobile::setup() {
	Serial.begin(SERIAL_USB_BAUD); delay(100);    // Open serial monitor at 115200 baud to see ping results
	Serial1.begin(SERIAL_NODE_BAUD); delay(100);   // NodeMCU - Connection
	Serial2.begin(SERIAL_BT_BAUD); delay(100);   // Bluetooth Commz are good to 115200 baudz (after programming)

	green = AMLED();
	green.init(g_led);
	red = AMLED();
	red.init(r_led);
	blue = AMLED();
	blue.init(b_led);
	yellow = AMLED();
	yellow.init(y_led);

	lcd.init();

	accelGyro.newMPUsetup(Serial);
	sensor3Serial = &Serial;

	red.light();
	blue.light();
	yellow.light();

	delay(300);

	lcd.clear();
	lcd.print("BEGIN");

	lcd.startupAnimation();

	delay(5000);

	lcd.clear();
	lcd.print("END");
}


void AutoMobile::loop() {
	green.blink();

	// USB - DEBUG CONSOLE
	select(Serial);
	red.blink();

	// NODE-MCU
	select(Serial1);
	yellow.blink();

	// BLUETOOTH
	select(Serial2);
	blue.blink();


	accelGyro.sensors3(*sensor3Serial);

	delay(30);


//	accelGyro.sensors3(Serial2);
}

bool AutoMobile::select(HardwareSerial& serial) {
	if (serial.available() > 0) {
		String line = serial.readStringUntil('\n');

		if (line != NULL && line.length() > 0) {

			// Sometimes we get carriage-returns, grr... ditch them
			if (line.charAt(line.length() - 1) == '\r') {
				line = line.substring(0, line.length() - 1);
			}

			int comma = line.indexOf(',');
			String cmdStr = (comma > 0) ? line.substring(0, comma) : line;
			int cmd = cmdStr.toInt();
			String data = (comma > 0) ? line.substring(comma + 1) : "";

			switch (cmd) {
				
			case 1: // drive
				Serial.println("driving: " + data);
				lcd.print("GOGO: ");
				lcd.print(data);
				drive.Drive(data);
				break;
				
			case 2: // Drive Ramping 
				Serial.println("driving: " + data);
				lcd.print("GOGO: ");
				lcd.print(data);
				drive.Catch(data);
				break;

			case 3: // LCD text
				lcd.print(data);
				break;

			case 4: // Node send
				Serial.println("Sending data to Node...");
				Serial2.println(data);
				break;

			case 5: // Bluetooth send
				sendToBluetooth(serial, data);
				break;

				/*
			case 6: // Print sensors
				printSensors(Serial);
				break;

			case 7: // Sensor blast enable/disable
				sensorsEnabled = !sensorsEnabled;
				break;
				*/

			case 8: // Sensor blast enable/disable
				sensor3Serial = &serial;
				accelGyro.sensors3Enabled = !accelGyro.sensors3Enabled;
				//accelGyro.serial = serial;
				break;

			case 0: // Show Menu -OR- Not a valid integer -- fall through to default
			default: // Unknown command

				if (cmdStr == "0") { // Actually show menu
					showMenu(serial);
				}
				else {
					serial.println("UNKNOWN SELECTION. Send 0 for menu.");
				}

				break;
			}
		}
	}
}


void AutoMobile::showMenu(HardwareSerial& serial) {
	serial.println("AutoMobile Serial Menu");
	serial.println();
	serial.println("   0. Show Menu");
	serial.println("   1. Drive <L,R>");
	serial.println("   2. LCD on/off");
	serial.println("   3. LCD print <text>");
	serial.println("   4. Send to Node <DATA>");
	serial.println("   5. Send to Bluetooth <DATA>");
	serial.println("   6. Print sensor readings");
	serial.println(String("   7. Sensor blast ") + (sensorsEnabled ? "!: click to disable" : ": click to enable"));
	serial.println(String("   8. Sensor3 blast ") + (accelGyro.sensors3Enabled ? "!: click to disable" : ": click to enable"));
	serial.println();
}



void AutoMobile::printSensors(HardwareSerial& serial) {

//	accelGyro.sensors3(serial);

	/*
	accelGyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
	sensors[0] = analogRead(A1);
	sensors[1] = sonar.ping_cm();
	sensors[2] = ax;
	sensors[3] = ay;
	sensors[4] = az;
	sensors[5] = gx;
	sensors[6] = gy;
	sensors[7] = gz;

	serial.print(sensors[0]);
	for (int i = 1; i < 7; i++) {
		serial.print(F(","));
		serial.print(sensors[i]);
	}

	serial.println();
	*/
}

void AutoMobile::sendToBluetooth(HardwareSerial& serial, String data) {
	Serial2.println(data);
}
