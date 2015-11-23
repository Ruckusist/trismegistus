/*
-----------Ruckusist - RoboCar 2015-------------
Written using Eclipse Juno - transfer from proteus 8 - july 15
Finally ported the script to Visual Studio Free Edition - Sept 15

This is the main command line for the operation of the car
At this point thef Car has equipped:
LED 4 light function board  - working
SR-04 ultrasonic sensor x 2 - working
L298d DC motor driver       - working
HC-06 bluetooth module      - working
MPU 6050 Accel and Gyro     - working
nRF24 wireless comm         - working (disconnected)
HOT UK05 dB sensor          - not working (wont change readings)
NodeMPU 8266 ESP-12E Wifi = working

Oct 15 recap notes...
A lot has happened since the previous update... Arduino since the last update
change many of its libraries around... there is not softserial anymore so the
old problems we were having with the bluetooth might have gone away that is under
development right now. A lot of work as been put in to unity and it show a lot of
potential. We are now using the NodeMPU 8266 ESP-12E DevKit for wifi and that is
working quite well. It is a whole nother arduino that sits with this one and handles
commz that would slow this guy down. it seems to be a nice combo. we are on a new
MPU library and that is giving us  better real world accel data. we are in the process
or cleaning the code to present this material in billings before intrested parties.
Also the LCD screen on the car is nice for debugging to see what it see without it
taking off away from you.

Sept 15 recap notes...
A lot has happened since the previous update... The motor Driver works good,
Credit goes to Art of Circuits for their Libz, The HC-06 seems faulty it never
recieved an input, although the output was solid. Currently the nRF24 is carrying
the signal to another arduino paired with this one called "HomeBase". The MPU6050
works well... although it needs to be gone through again for a better understanding
of its process and outputs. Now a new effort to clean the code up... and build in an
enum switch for ease of use and debugging stuff... Plus there is a Unity Program
that combines all the output of this sketch with an output feed of motor controls,
the hope is that all the rest of the operational programming will sit in Unity.
*/

// --------------------------------- added Libraries
//#include "RoBoCar.h"             // Extra line for Eclipse Build keep out for otherwise
#include "NewPing.h"               // Ultrasonic
#include "EEPROM.h"                // Access to internal memory
#include "Wire.h"                  // Comm
#include "I2Cdev.h"                // Comm
#include "artofcircuits_l298n.h"   // Drive commands
//#include "MPU6050.h"             // Accel and Gyro Commands
#include "MPU6050_6Axis_MotionApps20.h"
#include "printf.h"                // print handling for memories sake
#include "SPI.h"                   // Cant say yet
#include "nRF24L01.h"              // havent looked
#include "RF24.h"                  // This is what i think im using for the RF COMM
//#include "SoftwareSerial.h"      //  THis doesnt Exist
//#include "tests.h"               // Backup of Clutter from old function tests... DO NOT DELETE
//#include "RFCOMMS.h"             // This is a test of building Voids outside of this script
#include <Wtv020sd16p.h>           // Audio chip (wtv020m01 1.0)
#include <LiquidCrystal.h>         // LCD screen (LCM1602c v2.1)





// --------------------------------- VARIABLES FOR ALL THINGS!
//SR04 ultrasonic variables
#define MAX_DISTANCE 200           // Maximum sensor distance is rated at 400-500cm.
const int sensorMin = 10;          // sensor minimum, discovered through experiment
const int sensorMax = 190;         // sensor maximum, discovered through experiment
const int TOO_CLOSE = 10;          // Min distance

#define SENSORS_COUNT 9
int sensors[SENSORS_COUNT];        // 8 element array holding Sensor readings

int Move[2];                       // 2 element array holding recieved data for Motor_Movement

bool blinkState = false;           // Activity for Sending
bool BTblinkState = false;         // Activity for Recieving
bool yblinkState = false;



#define TXMIT_DELAY  100


#define NODE_QUIET 2000        // Max time between node receives before we consider it quiet (in ms)
long lastNodeRec = 0;

bool lcdOn = false;



//DEFINE NAME      PINHOLE
// --------------------------------- PINOUTs
#define y_led        34            // these lights should maybe go somewhere else
#define b_led        35            // and i should get a multi color light for debug purpose
#define r_led        31
#define g_led        29
#define LED_PIN      13            // there are no calls for this... and thats okay too
#define TRIGGER_PIN  12            // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     11            // Arduino pin tied to echo pin on the ultrasonic sensor.
#define TRIGGER_PIN_REAR  9        // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN_REAR     8        // Arduino pin tied to echo pin on the ultrasonic sensor.
// FIXME: not using multi-led just yet...
#define MULTI_LED_1  a8
#define MULTI_LED_2  a9
#define MULTI_LED_3  a10

//--------------------------------- NodeMPU 8266 ESP-12E Wifi (Connected)
//#define RxD        19          // Software Serial Wifi recieve -- no more softserial... i guess
//#define TxD        18          // Software Serial Wifi Transmit
// --------------------------------- Pinouts for MPU6050      (Connected)
//#20 SDA -- #21 SDL -- AH HA!!! you just are supposed to know they to into 20 and 21 and the not set them up at all... go figure
// --------------------------------- Pinouts for artofcircuits_l298n
#define Ldir_1       48
#define Ldir_2       49
#define L_enb        46
#define Rdir_1       43
#define Rdir_2       42
#define R_enb        44

bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[256]; // FIFO storage buffer

						 // orientation/motion vars
Quaternion q;           // [w, x, y, z]         quaternion container
VectorInt16 aa;         // [x, y, z]            accel sensor measurements
VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements
VectorInt16 aaWorld;    // [x, y, z]            world-frame accel sensor measurements
VectorFloat gravity;    // [x, y, z]            gravity vector



						// --------------------------------- Pinouts for NRF24L01  (disconnected)
#define CE_PIN       47
#define CSN_PIN      53

						// --------------------------------- MPU Variables
int16_t ax, ay, az;
int16_t gx, gy, gz;

// --------------------------------- NRF24 Variables
const uint64_t pipes[2] = {        // Define the transmit pipes
	0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL
};





// --------------------------------- Audio player
int resetPin = 24; // The pin number of the reset pin.
				   //Connect to RST pin of evaluation board. This connects to Pin 1 in wtv020sd-16P module.

int clockPin = 25; // The pin number of the clock pin.
				   //Connect to P04 pin of evaluation board. This connects to Pin 7 in wtv020sd-16P module

int dataPin = 26; // The pin number of the data pin.
				  //Connect to P05 pin of evaluation board. This connects to Pin 10 in wtv020sd-16P module.

int busyPin = 27; // The pin number of the busy pin.
				  //Connect to P06 pin of evaluation board. This connects to Pin 15 in wtv020sd-16P module.
				  /*
				  Create an instance of the Wtv020sd16p class.
				  1st parameter: Reset pin number.
				  2nd parameter: Clock pin number.
				  3rd parameter: Data pin number.
				  4th parameter: Busy pin number.
				  */
Wtv020sd16p audio(resetPin, clockPin, dataPin, busyPin);


// --------------------------------- LCD
// initialize the library with the numbers of the interface pins
#define LCD_PIN_14      22
#define LCD_PIN_13      3
#define LCD_PIN_12      4
#define LCD_PIN_11      5
#define LCD_PIN_6       6
#define LCD_PIN_4       7
LiquidCrystal lcd(LCD_PIN_4, LCD_PIN_6, LCD_PIN_11, LCD_PIN_12, LCD_PIN_13, LCD_PIN_14);
String lcdText;


// --------------------------------- SETUP devices
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
NewPing sonarRear(TRIGGER_PIN_REAR, ECHO_PIN_REAR, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
L298n dcm1(L_enb, Ldir_1, Ldir_2);                  // Motor Left
L298n dcm2(R_enb, Rdir_1, Rdir_2);                  // Motor Right
MPU6050 accelgyro;                                  // Accel Controls
RF24 radio(CE_PIN, CSN_PIN);                        // Create a Radio
													//SoftwareSerial Serial1(RxD, TxD);                 // Wifi pins know what the are doing



													/******************

													SETUP

													*******************/
void setup() {
	//DEFINE PINMODES FOR ALL PINS
	pinMode(y_led, OUTPUT);
	pinMode(b_led, OUTPUT);
	pinMode(r_led, OUTPUT);
	pinMode(g_led, OUTPUT);
	pinMode(LED_PIN, OUTPUT);
	pinMode(Ldir_1, OUTPUT);
	pinMode(Ldir_2, OUTPUT);
	pinMode(L_enb, OUTPUT);
	pinMode(Rdir_1, OUTPUT);
	pinMode(Rdir_2, OUTPUT);
	pinMode(R_enb, OUTPUT);
	//---------------------------------#20 and 21 know what they are doing
	//pinMode(RxD, INPUT);           // these pins know what the are doing
	//pinMode(TxD, OUTPUT);          // these pins know what the are doing

	// --------------------------------- START UP OPTIONS
	digitalWrite(y_led, LOW);        // super lame dim test light
	digitalWrite(b_led, LOW);        // b for bright test light
	digitalWrite(r_led, LOW);        // red should always be off by nature bad things with red
	digitalWrite(g_led, HIGH);       // green is for on... and powered up
	digitalWrite(LED_PIN, LOW);      // onboard led off

									 // --------------------------------- Serial and Wifi
	Wire.begin();
	TWBR = 24; // 400kHz I2C clock (200kHz if CPU is 8MHz)
	Serial.begin(9600);                                   // Open serial monitor at 115200 baud to see ping results
	printf_begin();                                         // This needs to be open for proper debugging
	printf("\n\r::   AutoMobile: The Ruckusist     ::\n\r");
	printf("\n\r::     Development Robot ::: 2015  ::\n\r");
	printf("\n\r::          Debug Mode             ::\n\r");
	// NodeMCU Serial - (Connected)
	Serial1.begin(9600);                                  // Wifi Commz are good to 921000 baudz
														  // Bluetooth - (Connected)
	Serial2.begin(9600);                                  // Bluetooth Commz are good to 115200 baudz
	Serial2.println("BLUETOOTH READY");

	printf("\n\r\::   LCD DISPLAY   ::\n\r");
	lcd.begin(16, 2); // 16 x 2 display
	lcdOn = true;
	lcd.print("SERIAL READY");

	// --------------------------------- nRf24L01 setup (Disconnected)
	printf("\n\r::   STARTING RADIO  ::\n\r");
	lcd.clear();
	lcd.print("STARTING RADIO");
	radio.begin();
	radio.enableDynamicPayloads();
	radio.setRetries(15, 15);
	radio.openWritingPipe(pipes[0]);
	radio.openReadingPipe(1, pipes[1]);
	radio.startListening();
	radio.printDetails();

	// --------------------------------- MPU 6050 setup (FIX THESE OFFSETS!!)
	printf("\n\r::   STARTING MPU  ::\n\r");
	lcd.clear();
	lcd.print("STARTING MCU");
	accelgyro.initialize();
	if (accelgyro.testConnection()) {
		printf("MPU test connection successful\n");
	}
	else
	{
		printf("MPU test connection FAILED\n");
	}
	printf("DMP initialize...\n");
	lcd.clear();
	lcd.print("DMP initialize...");
	devStatus = accelgyro.dmpInitialize();
	accelgyro.setXGyroOffset(220);     // all these settings are FUCKED -- came at 220
	accelgyro.setYGyroOffset(76);      // -- came at 76
	accelgyro.setZGyroOffset(-85);     // -- came at -85
	accelgyro.setZAccelOffset(1788);   // 1688 factory default for my test chip

	if (devStatus == 0) {
		printf("Enabling DMP...\n");
		lcd.clear();
		lcd.print("Enabling DMP...");
		accelgyro.setDMPEnabled(true);


		printf("Attaching DMP interrupt...\n");
		lcd.clear();
		lcd.print("DMP int attach");

		attachInterrupt(0, dmpDataReady, RISING);

		mpuIntStatus = accelgyro.getIntStatus();

		printf("DMP ready!\n");
		lcd.clear();
		lcd.print("DMP READY");

		// get expected DMP packet size for later comparison
		packetSize = accelgyro.dmpGetFIFOPacketSize();
	}
	else
	{
		printf("DMP initialization failed: ");
		lcd.clear();
		lcd.print("DMP init failed!");
		Serial.print(devStatus);
		printf("\n");
	}

	// FIXME: not working yet (need 512MB or 1GB SD card max)
	/*
	printf("\n\r\::   STARTUP SOUND: 0000.AD4   ::\n\r");
	lcd.clear();
	lcd.print("STARTUP SOUND");
	audio.asyncPlayVoice(0); // plays song 0, 0000.AD4
	*/


	printf("\n\r::   BEGIN  ::\n\r");
	lcd.clear();
	lcd.print("HELLO MICHAEL");
	delay(2500);
	//startMenu();
}
// ENd OF seTup



/******************

LOOP

*******************/
void loop() {
	WORKING();
}







/******************

WORKING

*******************/

void WORKING() {
	// Startup Mpu
	mpuIntStatus = accelgyro.getIntStatus();

	// get current FIFO count
	fifoCount = accelgyro.getFIFOCount();

	// check for overflow (this should never happen unless our code is too inefficient)
	if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
		// reset so we can continue cleanly
		accelgyro.resetFIFO();
		printf("FIFO overflow!\n");

		// otherwise, check for DMP data ready interrupt (this should happen frequently)
	}
	else if (mpuIntStatus & 0x02) {
		//printf("accel gyro data good\n");
		// wait for correct available data length, should be a VERY short wait
		while (fifoCount < packetSize) fifoCount = accelgyro.getFIFOCount();

		// read a packet from FIFO
		accelgyro.getFIFOBytes(fifoBuffer, packetSize);

		accelgyro.dmpGetQuaternion(&q, fifoBuffer);
		accelgyro.dmpGetAccel(&aa, fifoBuffer);
		accelgyro.dmpGetGravity(&gravity, &q);
		accelgyro.dmpGetLinearAccel(&aaReal, &aa, &gravity);
		accelgyro.dmpGetLinearAccelInWorld(&aaWorld, &aaReal, &q);

		sensors[0] = analogRead(A0);
		sensors[1] = sonar.ping_cm();
		sensors[2] = aaReal.x;
		sensors[3] = aaReal.y;
		sensors[4] = aaReal.z;
		sensors[5] = aaWorld.x;
		sensors[6] = aaWorld.y;
		sensors[7] = aaWorld.z;
		sensors[8] = sonarRear.ping_cm();

		// --------- Set Node Command to "Send to Unity"
		Serial1.print("5,");

		// --------- Send The Sensor Data
		Serial1.print(sensors[0]);
		for (int i = 1; i < SENSORS_COUNT; i++) {
			Serial1.print(F(","));
			Serial1.print(sensors[i]);
		}
		Serial1.println();

		/* -- TESTING --
		for (int i = 0; i < SENSORS_COUNT; i++) {                           // this is unnecessary spewing into the Serial Port
		Serial.print(F(", "));                                // for debuging reasons i suppose... but it works...
		Serial.print(sensors[i]);                             // turn it off...
		}
		Serial.println();
		*/

		blinkState = !blinkState;
		digitalWrite(b_led, blinkState);
	}


	// RECEIVE DATA FROM NodeMCU

	if (Serial1.available() > 0) {
		String line = Serial1.readStringUntil('\n');

		int comma = line.indexOf(',');
		int cmd = (comma > 0) ? line.substring(0, comma).toInt() : line.toInt();

		switch (cmd) {
		case 0:	// show menu
			showMenuSerial();
			break;

		case 1: // drive 
		{
			String driveData = line.substring(comma + 1);
			//Serial1.println("driving: " + driveData);
			drive(driveData);
			break;
		}

		case 2: // LCD on/off
			lcdToggle();
			break;

		case 3: // LCD text
			lcdText((comma > 0) ? line.substring(comma + 1) : "");
			break;

		case 4: // Node (menu)
			//Serial1.println("Sending data to Node...");
			sendToNode((comma > 0) ? line.substring(comma + 1) : "");
			break;

		default:
			//Serial1.println("UNKNOWN SELECTION. Send 0 for menu.");
			break;
		}
	}



	/*

	if (Serial1.available() > 0) {
		lastNodeRec = millis();
		String line = Serial1.readStringUntil('\n');           // Input will be "-###,-###"

		if (line != NULL) { // if line isnt null...
			Serial.println(line);                                // DEBUG THE RECIVER.. now testing
																 //motors_running(line);                              // more cleanly pass the input to another program
			yblinkState = !yblinkState;                          // love this... totally worked first try...
			digitalWrite(y_led, yblinkState);
			// THis needs a new home...
			int comma = line.indexOf(',');                       // comma seperator
			int comma2 = line.indexOf(',', comma + 1);
			String newText = line.substring(comma2 + 1, line.length() - 1);
			if (!(lcdText && newText.equals(lcdText))) {
				lcdText = newText;
				lcd.clear();
				lcd.home();
				lcd.print("I HEAR THE NODE");
				lcd.setCursor(0, 1); // second line
				lcd.print(lcdText);
			}

			if (millis() - lastNodeRec > NODE_QUIET) {          // this is not happening... all is well
				lcd.clear();
				lcd.home();
				lcd.print("NODE IS QUIET");
			}

			if (comma > 0) {
				//        drive(line);

				int left = line.substring(0, comma).toInt();       // First line is nothing to comma will be LEFT_Motors
				int right = line.substring(comma + 1).toInt();     // Second line is comma to the end will be RIGHT_Motors
				if (left > 1) {
					dcm1.backward(left);  // if LMotor speed is positive turn forward
				}
				else if (left < 1) {
					dcm1.forward(-1 * left);  // if LMotor speed is Neg, Turn backward
				}
				else {
					dcm1.breaking();  // If left is changing from front to back Break first
				}
				if (right > 1) {
					dcm2.backward(right);  // if RMotor speed is positive turn forward
				}
				else if (right < 1) {
					dcm2.forward(-1 * right);  // if RMotor speed is positive turn backward
				}
				else {
					dcm2.breaking();  // If right is changing from front to back Break first
				}

			}

		}
	}

	*/


	// READ BLUETOOTH
	if (Serial2.available() > 0) {
		BTblinkState = !BTblinkState;
		digitalWrite(r_led, BTblinkState);

		String line = Serial2.readStringUntil('\n');

		int comma = line.indexOf(',');
		int cmd = (comma>0) ? line.substring(0, comma).toInt() : line.toInt();

		switch (cmd) {
		case 0:	// show menu
			showMenuBT();
			break;

		case 1: // drive 
		{
			String driveData = line.substring(comma + 1);
			Serial2.println("driving: " + driveData);
			drive(driveData);
			break;
		}

		case 2: // LCD on/off
			lcdToggle();
			break;

		case 3: // LCD text
			Serial2.println("Setting LCD text...");
			lcdText((comma > 0) ? line.substring(comma + 1) : "");
			break;

		case 4: // Node (menu)
			Serial2.println("Sending data to Node...");
			sendToNode((comma > 0) ? line.substring(comma + 1) : "");
			break;

		default:
			Serial2.println("UNKNOWN SELECTION. Send 0 for menu.");
			break;
		}
	}


}



void showMenuSerial() {
	Serial1.println("AutoMobile Serial Menu");
	Serial1.println();
	Serial1.println("   0. Show Menu");
	Serial1.println("   1. Drive <L,R>");
	Serial1.println("   2. LCD on/off");
	Serial1.println("   3. LCD text");
	Serial1.println("   4. Send to Node <DATA>");
	Serial1.println();
}

void showMenuBT() {
	Serial2.println("AutoMobile Bluetooth Menu");
	Serial2.println();
	Serial2.println("   0. Show Menu");
	Serial2.println("   1. Drive <L,R>");
	Serial2.println("   2. LCD on/off");
	Serial2.println("   3. LCD text");
	Serial2.println("   4. Send to Node <DATA>");
	Serial2.println();
}


void lcdToggle() {
	if (lcdOn) {
		Serial2.println("Turning LCD display OFF");
		lcd.noDisplay();
	}
	else {
		Serial2.println("Turning ON LCD display");
		lcd.display();
	}
	lcdOn = !lcdOn;
}


void lcdText(String text) {
	lcd.clear();
	lcd.home();
	lcd.print("TEXT RECEIVED");
	lcd.setCursor(0, 1); // second line
	lcd.print(text);
}


void drive(String data) {
	int comma = data.indexOf(',');
	if (comma > 0) {
		int left = data.substring(0, comma).toInt();       // First line is nothing to comma will be LEFT_Motors
		int right = data.substring(comma + 1).toInt();     // Second line is comma to the end will be RIGHT_Motors

		if (left > 1) {
			dcm1.backward(left);  // if LMotor speed is positive turn forward
		}
		else if (left < 1) {
			dcm1.forward(-1 * left);  // if LMotor speed is Neg, Turn backward
		}
		else {
			dcm1.breaking();  // If left is changing from front to back Break first
		}
		if (right > 1) {
			dcm2.backward(right);  // if RMotor speed is positive turn forward
		}
		else if (right < 1) {
			dcm2.forward(-1 * right);  // if RMotor speed is positive turn backward
		}
		else {
			dcm2.breaking();  // If right is changing from front to back Break first
		}

	}
}


void sendToNode(String data) {
	Serial1.println(data);
}



void motors_running(int Move[]) {                      // this is a cleaner way to move... untested though
	int left = Move[0];                                // First line is nothing to comma will be LEFT_Motors
	int right = Move[1];                               // Second line is comma to the end will be RIGHT_Motors
	if (left > 1) {
		dcm1.backward(left);  // if LMotor speed is positive turn forward
	}
	else if (left < 1) {
		dcm1.forward(-1 * left);  // if LMotor speed is Neg, Turn backward
	}
	else {
		dcm1.breaking();  // If left is changing from front to back Break first
	}
	if (right > 1) {
		dcm2.backward(right);  // if RMotor speed is positive turn forward
	}
	else if (right < 1) {
		dcm2.forward(-1 * right);  // if RMotor speed is positive turn backward
	}
	else {
		dcm2.breaking();  // If right is changing from front to back Break first
	}
}

void startMenu() {
	Serial.println(F(":::Select Mode:::"));
	Serial.println(F("::: Press 1 for a SensorSweep :::"));
	Serial.println(F("::: Press 2 for a Radio Check :::"));
	Serial.println(F("::: Press 3 to Return to Menu :::"));
	Serial.println(F("::: Press 4 to RFCOMMCHAT :::"));
}

void decider() {                     // this did not do what was expected... but thats cool too
	digitalWrite(r_led, blinkState);
	if (Serial.available()) {
		char inByte = Serial.read();
		if (inByte == '1') {
			SensorReadings();
		}
		if (inByte == '3') {
			startMenu();
		}
		if (inByte == '4') {
			WORKING();
		}
		else if (inByte == '2') {
			radio.printDetails();
		}
	}
	blinkState = !blinkState;
}

void SensorReadings() {
	accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
	sensors[0] = analogRead(A0);
	sensors[1] = sonar.ping_cm();
	sensors[2] = ax;
	sensors[3] = ay;
	sensors[4] = az;
	sensors[5] = gx;
	sensors[6] = gy;
	sensors[7] = gz;

	Serial1.print(sensors[0]);
	for (int i = 1; i < 7; i++) {
		Serial1.print(F(","));
		Serial1.print(sensors[i]);
	}
	Serial1.println();
	Serial.println(F("::"));

	//radio.write(sensors, sizeof(sensors));
	blinkState = !blinkState;
	digitalWrite(b_led, blinkState);
	delay(10);
}
// ================================================================
// ===               INTERRUPT DETECTION ROUTINE                ===
// ================================================================

volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
void dmpDataReady() {
	mpuInterrupt = true;
}
// --------------------------------- ENd of the GAME, SON!!!!



