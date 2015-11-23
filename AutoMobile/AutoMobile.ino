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
#include "NewPing.h"               // Ultrasonic
#include "EEPROM.h"                // Access to internal memory
#include "Wire.h"                  // Comm
#include "I2Cdev.h"                // Comm
#include "artofcircuits_l298n.h"   // Drive commands
//#include "MPU6050.h"             // Accel and Gyro Commands
//#include "MPU6050_6Axis_MotionApps20.h"
#include "SPI.h"                   // Cant say yet
#include <Wtv020sd16p.h>           // Audio chip (wtv020m01 1.0)
#include <LiquidCrystal.h>         // LCD screen (LCM1602c v2.1)
#include <FastLED.h>               // Neo Pixel 1x8

FASTLED_USING_NAMESPACE

// --------------------------------- NEW MPU LIBRARY
// Define registers per MPU6050, Register Map and Descriptions, Rev 4.2, 08/19/2013 6 DOF Motion sensor fusion device
// Invensense Inc., www.invensense.com
// See also MPU-6050 Register Map and Descriptions, Revision 4.0, RM-MPU-6050A-00, 9/12/2012 for registers not listed in 
// above document; the MPU6050 and MPU-9150 are virtually identical but the latter has an on-board magnetic sensor
//
#define XGOFFS_TC        0x00 // Bit 7 PWR_MODE, bits 6:1 XG_OFFS_TC, bit 0 OTP_BNK_VLD                 
#define YGOFFS_TC        0x01                                                                          
#define ZGOFFS_TC        0x02
#define X_FINE_GAIN      0x03 // [7:0] fine gain
#define Y_FINE_GAIN      0x04
#define Z_FINE_GAIN      0x05
#define XA_OFFSET_H      0x06 // User-defined trim values for accelerometer
#define XA_OFFSET_L_TC   0x07
#define YA_OFFSET_H      0x08
#define YA_OFFSET_L_TC   0x09
#define ZA_OFFSET_H      0x0A
#define ZA_OFFSET_L_TC   0x0B
#define SELF_TEST_X      0x0D
#define SELF_TEST_Y      0x0E    
#define SELF_TEST_Z      0x0F
#define SELF_TEST_A      0x10
#define XG_OFFS_USRH     0x13  // User-defined trim values for gyroscope; supported in MPU-6050?
#define XG_OFFS_USRL     0x14
#define YG_OFFS_USRH     0x15
#define YG_OFFS_USRL     0x16
#define ZG_OFFS_USRH     0x17
#define ZG_OFFS_USRL     0x18
#define SMPLRT_DIV       0x19
#define CONFIG           0x1A
#define GYRO_CONFIG      0x1B
#define ACCEL_CONFIG     0x1C
#define FF_THR           0x1D  // Free-fall
#define FF_DUR           0x1E  // Free-fall
#define MOT_THR          0x1F  // Motion detection threshold bits [7:0]
#define MOT_DUR          0x20  // Duration counter threshold for motion interrupt generation, 1 kHz rate, LSB = 1 ms
#define ZMOT_THR         0x21  // Zero-motion detection threshold bits [7:0]
#define ZRMOT_DUR        0x22  // Duration counter threshold for zero motion interrupt generation, 16 Hz rate, LSB = 64 ms
#define FIFO_EN          0x23
#define I2C_MST_CTRL     0x24   
#define I2C_SLV0_ADDR    0x25
#define I2C_SLV0_REG     0x26
#define I2C_SLV0_CTRL    0x27
#define I2C_SLV1_ADDR    0x28
#define I2C_SLV1_REG     0x29
#define I2C_SLV1_CTRL    0x2A
#define I2C_SLV2_ADDR    0x2B
#define I2C_SLV2_REG     0x2C
#define I2C_SLV2_CTRL    0x2D
#define I2C_SLV3_ADDR    0x2E
#define I2C_SLV3_REG     0x2F
#define I2C_SLV3_CTRL    0x30
#define I2C_SLV4_ADDR    0x31
#define I2C_SLV4_REG     0x32
#define I2C_SLV4_DO      0x33
#define I2C_SLV4_CTRL    0x34
#define I2C_SLV4_DI      0x35
#define I2C_MST_STATUS   0x36
#define INT_PIN_CFG      0x37
#define INT_ENABLE       0x38
#define DMP_INT_STATUS   0x39  // Check DMP interrupt
#define INT_STATUS       0x3A
#define ACCEL_XOUT_H     0x3B
#define ACCEL_XOUT_L     0x3C
#define ACCEL_YOUT_H     0x3D
#define ACCEL_YOUT_L     0x3E
#define ACCEL_ZOUT_H     0x3F
#define ACCEL_ZOUT_L     0x40
#define TEMP_OUT_H       0x41
#define TEMP_OUT_L       0x42
#define GYRO_XOUT_H      0x43
#define GYRO_XOUT_L      0x44
#define GYRO_YOUT_H      0x45
#define GYRO_YOUT_L      0x46
#define GYRO_ZOUT_H      0x47
#define GYRO_ZOUT_L      0x48
#define EXT_SENS_DATA_00 0x49
#define EXT_SENS_DATA_01 0x4A
#define EXT_SENS_DATA_02 0x4B
#define EXT_SENS_DATA_03 0x4C
#define EXT_SENS_DATA_04 0x4D
#define EXT_SENS_DATA_05 0x4E
#define EXT_SENS_DATA_06 0x4F
#define EXT_SENS_DATA_07 0x50
#define EXT_SENS_DATA_08 0x51
#define EXT_SENS_DATA_09 0x52
#define EXT_SENS_DATA_10 0x53
#define EXT_SENS_DATA_11 0x54
#define EXT_SENS_DATA_12 0x55
#define EXT_SENS_DATA_13 0x56
#define EXT_SENS_DATA_14 0x57
#define EXT_SENS_DATA_15 0x58
#define EXT_SENS_DATA_16 0x59
#define EXT_SENS_DATA_17 0x5A
#define EXT_SENS_DATA_18 0x5B
#define EXT_SENS_DATA_19 0x5C
#define EXT_SENS_DATA_20 0x5D
#define EXT_SENS_DATA_21 0x5E
#define EXT_SENS_DATA_22 0x5F
#define EXT_SENS_DATA_23 0x60
#define MOT_DETECT_STATUS 0x61
#define I2C_SLV0_DO      0x63
#define I2C_SLV1_DO      0x64
#define I2C_SLV2_DO      0x65
#define I2C_SLV3_DO      0x66
#define I2C_MST_DELAY_CTRL 0x67
#define SIGNAL_PATH_RESET  0x68
#define MOT_DETECT_CTRL   0x69
#define USER_CTRL        0x6A  // Bit 7 enable DMP, bit 3 reset DMP
#define PWR_MGMT_1       0x6B // Device defaults to the SLEEP mode
#define PWR_MGMT_2       0x6C
#define DMP_BANK         0x6D  // Activates a specific bank in the DMP
#define DMP_RW_PNT       0x6E  // Set read/write pointer to a specific start address in specified DMP bank
#define DMP_REG          0x6F  // Register in DMP from which to read or to which to write
#define DMP_REG_1        0x70
#define DMP_REG_2        0x71
#define FIFO_COUNTH      0x72
#define FIFO_COUNTL      0x73
#define FIFO_R_W         0x74
#define WHO_AM_I_MPU6050 0x75 // Should return 0x68

// Using the GY-521 breakout board, I set ADO to 0 by grounding through a 4k7 resistor
// Seven-bit device address is 110100 for ADO = 0 and 110101 for ADO = 1
#define ADO 0
#if ADO
#define MPU6050_ADDRESS 0x69  // Device address when ADO = 1
#else
#define MPU6050_ADDRESS 0x68  // Device address when ADO = 0
#endif

// Set initial input parameters
enum Ascale {
  AFS_2G = 0,
  AFS_4G,
  AFS_8G,
  AFS_16G
};

enum Gscale {
  GFS_250DPS = 0,
  GFS_500DPS,
  GFS_1000DPS,
  GFS_2000DPS
};

// Specify sensor full scale
int Gscale = GFS_250DPS;
int Ascale = AFS_2G;
float aRes, gRes; // scale resolutions per LSB for the sensors
  
// Pin definitions
int intPin = 2;  // This can be changed, 2 and 3 are the Arduinos ext int pins // changed from 12 to 2

int16_t accelCount[3];           // Stores the 16-bit signed accelerometer sensor output
float ax, ay, az;                // Stores the real accel value in g's
int16_t gyroCount[3];            // Stores the 16-bit signed gyro sensor output
float gx, gy, gz;                // Stores the real gyro value in degrees per seconds
float gyroBias[3], accelBias[3]; // Bias corrections for gyro and accelerometer
int16_t tempCount;               // Stores the internal chip temperature sensor output 
float temperature;               // Scaled temperature in degrees Celsius
float SelfTest[6];               // Gyro and accelerometer self-test sensor output
uint32_t count = 0;

// --------------------------------- VARIABLES FOR ALL THINGS!
// Letters
byte test[8] = { 0b11111, 0b00000, 0b01010, 0b01011, 0b11011, 0b11011, 0b10001, 0b11111 };
byte type_R[8] = { 0b11111, 0b10011, 0b10101, 0b10101, 0b10011, 0b10101, 0b10101, 0b11111 };
byte type_U[8] = { 0b11111, 0b10101, 0b10101, 0b10101, 0b10101, 0b10101, 0b10001, 0b11111 };
byte type_C[8] = { 0b11111, 0b10001, 0b10111, 0b10111, 0b10111, 0b10111, 0b10001, 0b11111 };
byte typeT[8] = { 0b11111, 0b00000, 0b01010, 0b01011, 0b11011, 0b11011, 0b10001, 0b11111 };
byte type_K[8] = { 0b11111, 0b10111, 0b10110, 0b10101, 0b10011, 0b10101, 0b10110, 0b11111 };
byte type_S[8] = { 0b00000, 0b00110, 0b01001, 0b01000, 0b00110, 0b00001, 0b01001, 0b00110 };
byte type_SB[8] = { 0b11111, 0b11001, 0b10110, 0b10111, 0b11001, 0b11110, 0b10110, 0b11001 };
byte type_I[8] = { 0b11111, 0b10001, 0b11011, 0b11011, 0b11011, 0b11011, 0b10001, 0b11111 };
byte Spacer[8] = { 0b10101, 0b01110, 0b10101, 0b01110, 0b10101, 0b01110, 0b10101, 0b01110 };

const unsigned int SERIAL_BAUD = 9600;    // Same serial baud for all 3 serial ports

										  //SR04 ultrasonic variables
#define MAX_DISTANCE 200           // Maximum sensor distance is rated at 400-500cm.
const int sensorMin = 10;          // sensor minimum, discovered through experiment
const int sensorMax = 190;         // sensor maximum, discovered through experiment
const int TOO_CLOSE = 10;          // Min distance

#define SENSORS_COUNT 9
int sensors[SENSORS_COUNT];        // 9 element array holding Sensor readings
bool sensorsEnabled = false;

bool blinkState = false;           // Activity for Sending
bool BTblinkState = false;         // Activity for Recieving
bool yblinkState = false;
bool gblinkState = false;
#define TXMIT_DELAY  100
#define NODE_QUIET 2000        // Max time between node receives before we consider it quiet (in ms)
long lastNodeRec = 0;
bool lcdOn = false;

//DEFINE NAME      PINHOLE
// --------------------------------- PINOUTs
#define y_led        32            // these lights should maybe go somewhere else
#define b_led        33            // and i should get a multi color light for debug purpose
#define r_led        30
#define g_led        31
#define LED_PIN      13            // there are no calls for this... and thats okay too
#define TRIGGER_PIN   6            // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN      7            // Arduino pin tied to echo pin on the ultrasonic sensor.
#define Pix_LED       9
//#define TRIGGER_PIN_REAR  9        // Arduino pin tied to trigger pin on the ultrasonic sensor.
//#define ECHO_PIN_REAR     8        // Arduino pin tied to echo pin on the ultrasonic sensor.
//--------------------------------- NodeMPU 8266 ESP-12E Wifi (Connected)
//#define RxD        19          // Software Serial Wifi recieve -- no more softserial... i guess
//#define TxD        18          // Software Serial Wifi Transmit
// --------------------------------- Pinouts for MPU6050      (Connected)
//#20 SDA -- #21 SDL -- AH HA!!! you just are supposed to know they to into 20 and 21 and the not set them up at all... go figure
// --------------------------------- Pinouts for artofcircuits_l298n
#define Ldir_1       48
#define Ldir_2       50
#define L_enb        49
#define Rdir_1       51
#define Rdir_2       53
#define R_enb        52
#define Relay        44
bool RelaySwitch;

/*
uint8_t mpuIntStatus;    // holds actual interrupt status byte from MPU
uint8_t devStatus;       // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;     // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;      // count of all bytes currently in FIFO
uint8_t fifoBuffer[256]; // FIFO storage buffer
						
						 // orientation/motion vars
Quaternion qq;           // [w, x, y, z]         quaternion container
VectorInt16 aa;          // [x, y, z]            accel sensor measurements
VectorInt16 aaReal;      // [x, y, z]            gravity-free accel sensor measurements
VectorInt16 aaWorld;     // [x, y, z]            world-frame accel sensor measurements
VectorFloat gravity;     // [x, y, z]            gravity vector
// --------------------------------- MPU Variables
int16_t ax, ay, az;
int16_t gx, gy, gz;
*/

// --------------------------------- LCD
// initialize the library with the numbers of the interface pins
#define LCD_PIN_14      22
#define LCD_PIN_13       3
#define LCD_PIN_12       4
#define LCD_PIN_11       5
#define LCD_PIN_6       11
#define LCD_PIN_4       12
LiquidCrystal lcd(LCD_PIN_4, LCD_PIN_6, LCD_PIN_11, LCD_PIN_12, LCD_PIN_13, LCD_PIN_14);
String lcdMsg = "";   // for the second row (incoming message)

// --------------------------------- NeoPixel Setup
#define COLOR_ORDER             GRB
#define CHIPSET              WS2811
#define NUM_LEDS                  8
#define BRIGHTNESS              100   // comes stock at 200 that is really bright... 
#define FRAMES_PER_SECOND        60
// COOLING: How much does the air cool as it rises?
// Less cooling = taller flames.  More cooling = shorter flames.
// Default 50, suggested range 20-100 
#define COOLING  85
// SPARKING: What chance (out of 255) is there that a new spark will be lit?
// Higher chance = more roaring fire.  Lower chance = more flickery fire.
// Default 120, suggested range 50-200.
#define SPARKING 25
#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))
bool gReverseDirection = false;
CRGB leds[NUM_LEDS];
bool Demo_Reel100;                 // Demo Functions that come with the Deal
bool Pix_LEDz;                     // Other Demo Functions -- Fire
bool Nightrider;                   // Night Rider Lights... Sweeeeeeeeet...
bool Signal_LEDz;                  // Trying to write my own thing... will be different colors for stats of the machine

bool Stage1;
bool Stage2;

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns

// --------------------------------- SETUP devices
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
//NewPing sonarRear(TRIGGER_PIN_REAR, ECHO_PIN_REAR, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
L298n dcm1(L_enb, Ldir_1, Ldir_2);                  // Motor Left
L298n dcm2(R_enb, Rdir_1, Rdir_2);                  // Motor Right
//MPU6050 accelgyro;                                  // Accel Controls

// this goes before the setup...
// ================================================================
// ===               INTERRUPT DETECTION ROUTINE                ===
// ================================================================
/*
volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
void dmpDataReady() {
	mpuInterrupt = true;
}
*/
/******************

SETUP

*******************/
void setup() {
// --------------------------------- DEFINE PINMODES FOR ALL PINS
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
	pinMode(Relay, OUTPUT);
    pinMode(intPin, INPUT);
//---------------------------------#20 and 21 know what they are doing

// -------------------------- LCD Custom Chars
	lcd.createChar(1, typeT);
	lcd.createChar(12, type_SB);
	lcd.createChar(2, type_R);
	lcd.createChar(4, type_C);
	lcd.createChar(5, type_K);
	lcd.createChar(6, type_U);
	lcd.createChar(99, Spacer);
	lcd.createChar(7, type_SB);
	lcd.createChar(8, type_I);
	lcd.createChar(9, type_S);
	lcd.createChar(1, test);

// --------------------------------- START UP OPTIONS
	digitalWrite(Relay, HIGH);       // turn the relay off for battery life
	digitalWrite(y_led, LOW);        // super lame dim test light
	digitalWrite(b_led, LOW);        // b for bright test light
	digitalWrite(r_led, LOW);        // red should always be off by nature bad things with red
	digitalWrite(g_led, HIGH);       // green is for on... and powered up
	digitalWrite(LED_PIN, LOW);      // onboard led off
	digitalWrite(intPin, LOW);       // Set interupt pin... could be any PWM pin though 

// --------------------------------- Serial and Wifi
	Wire.begin();
	TWBR = 24; // 400kHz I2C clock (200kHz if CPU is 8MHz)

			   // USB Serial (console)
	Serial.begin(SERIAL_BAUD);                              // Open serial monitor at 115200 baud to see ping results
	delay(50);
	//printf_begin();                                         // This needs to be open for proper debugging
	Serial.println(F("::   AutoMobile: The Ruckusist     ::"));
	Serial.println(F("::     Development Robot ::: 2015  ::"));
	Serial.println(F("::          Debug Mode             ::"));

	// NodeMCU Serial - (Connected)
	Serial1.begin(SERIAL_BAUD);                              // Wifi Commz are good to 921000 baudz
	delay(50);

	// Bluetooth - (Connected)
	Serial2.begin(SERIAL_BAUD);                              // Bluetooth Commz are good to 115200 baudz
	delay(50);
	Serial2.println("BLUETOOTH READY");
	Serial.println(F("::       Bluetooth Enabled         ::"));
	Serial.println(F("::        Wireless Enabled         ::"));
	Serial.println(F("::          LCD Enabled            ::"));
	lcd.begin(16, 2); // 16 x 2 display
	lcdOn = true;
	lcd.print("SERIAL READY");
/* ------------ OLD  MPU SETUP and START UP
	// --------------------------------- MPU 6050 setup (FIX THESE OFFSETS!!)
	Serial.println(F("::         Starting MPU             ::"));
	lcd.clear();
	lcd.print("STARTING MPU");
	accelgyro.initialize();
	if (accelgyro.testConnection()) {
		Serial.println(F("::          MPU Enabled            ::"));
	}
	else
	{
		Serial.println(F("::          MPU Failed             ::"));
	}
	Serial.println(F("::          DMP initilizing            ::"));
	lcd.clear();
	lcd.print("DMP initialize...");
	devStatus = accelgyro.dmpInitialize();
	accelgyro.setXGyroOffset(220);     // all these settings are FUCKED -- came at 220
	accelgyro.setYGyroOffset(76);      // -- came at 76
	accelgyro.setZGyroOffset(-85);     // -- came at -85
	accelgyro.setZAccelOffset(1788);   // 1688 factory default for my test chip

	if (devStatus == 0) {
		Serial.println(F("::          DMP initilized            ::"));
		lcd.clear();
		lcd.print("Enabling DMP...");
		accelgyro.setDMPEnabled(true);
		lcd.clear();
		lcd.print("DMP int attach");
		attachInterrupt(0, dmpDataReady, RISING);
		mpuIntStatus = accelgyro.getIntStatus();
		lcd.clear();
		lcd.print("DMP READY");

		// get expected DMP packet size for later comparison
		packetSize = accelgyro.dmpGetFIFOPacketSize();
	}
	else
	{
		Serial.println(F("::          DMP initilized FAILED           ::"));
		lcd.clear();
		lcd.print("DMP init failed!");
		Serial.print(devStatus);

		while (1);
	}
*/

	// -------------------------------- NEW MPU SETUP
	uint8_t c = readByte(MPU6050_ADDRESS, WHO_AM_I_MPU6050);  // Read WHO_AM_I register for MPU-6050
	Serial.println("::   MPU 6050 I AM : " + (c, HEX));
	Serial.println("I should Be : ");
	Serial.print(0x68, HEX);

	if (c == 0x68) {// WHO_AM_I should always be 0x68
    
    Serial.println("MPU6050 is online...");
    
    MPU6050SelfTest(SelfTest); // Start by performing self test and reporting values
    Serial.print("x-axis self test: acceleration trim within : "); Serial.print(SelfTest[0],1); Serial.println("% of factory value");
    Serial.print("y-axis self test: acceleration trim within : "); Serial.print(SelfTest[1],1); Serial.println("% of factory value");
    Serial.print("z-axis self test: acceleration trim within : "); Serial.print(SelfTest[2],1); Serial.println("% of factory value");
    Serial.print("x-axis self test: gyration trim within : "); Serial.print(SelfTest[3],1); Serial.println("% of factory value");
    Serial.print("y-axis self test: gyration trim within : "); Serial.print(SelfTest[4],1); Serial.println("% of factory value");
    Serial.print("z-axis self test: gyration trim within : "); Serial.print(SelfTest[5],1); Serial.println("% of factory value");

    if(SelfTest[0] < 1.0f && SelfTest[1] < 1.0f && SelfTest[2] < 1.0f && SelfTest[3] < 1.0f && SelfTest[4] < 1.0f && SelfTest[5] < 1.0f) {
    //display.clearDisplay();
    //display.setCursor(0, 30); display.print("Pass Selftest!");  
    //display.display();
    delay(1000);
  
    calibrateMPU6050(gyroBias, accelBias); // Calibrate gyro and accelerometers, load biases in bias registers  
    initMPU6050(); Serial.println("MPU6050 initialized for active data mode...."); // Initialize device for active mode read of acclerometer, gyroscope, and temperature

   }
   else {
    Serial.print("Could not connect to MPU6050: 0x");
    Serial.println(c, HEX);
    while(1) ; // Loop forever if communication doesn't happen
   }

  }
	// -------------------------------- NEW MPU END SETUP


	lcd.clear();
	lcd.print("...");
	ledSequence();

	Serial.println(F("::          LED SETUP             ::"));
	FastLED.addLeds<CHIPSET, Pix_LED, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
	FastLED.setBrightness( BRIGHTNESS );
	Pix_LEDz = false;              //  Startup with the TEST animation
	Signal_LEDz = false;           // this will override 
	Demo_Reel100 = true;           // This will start the demo test with regular bootup

	Serial.println(F("::        Setup Finished           ::"));
	Serial.println(F("::       Press 0 for Menu          ::"));

	lcd.clear();
	StartupAnimaitonLCD();             // NEW start up screen
									   //lcd.print("Hello Michael...");   // classic startup screen

									   // discard any initial NodeMCU data as line/startup noise
	while (Serial1.available() > 0) {
		Serial1.read();
	}
}
// ENd OF seTup

// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = { rainbow, rainbowWithGlitter, confetti, sinelon, juggle, bpm };

/******************

LOOP

*******************/
void loop() {
	//WORKING();
}

/******************

WORKING

*******************/

void WORKING() {

	gblinkState = !gblinkState;
	digitalWrite(g_led, gblinkState);
	digitalWrite(Relay, RelaySwitch);

	if (Pix_LEDz && !Demo_Reel100){
		Fire();
		FastLED.show();                          // display this frame
		FastLED.delay(1000 / FRAMES_PER_SECOND);
	}

	if (Demo_Reel100){
		Demo_Reel();
	}
/*
	if (sensorsEnabled) {


		// ---
		// SENSOR DATA (Send to NodeMCU -> Unity)
		// ---

		
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
		// THIS CALL SLOWS DOWN OUR LOOP CONSIDERABLY
		accelgyro.getFIFOBytes(fifoBuffer, packetSize);

		// MORE CALLS SLOW US DOWN... we start getting FIFO overflow now

		accelgyro.dmpGetQuaternion(&qq, fifoBuffer);
		accelgyro.dmpGetAccel(&aa, fifoBuffer);
		accelgyro.dmpGetGravity(&gravity, &qq);
		accelgyro.dmpGetLinearAccel(&aaReal, &aa, &gravity);
		accelgyro.dmpGetLinearAccelInWorld(&aaWorld, &aaReal, &qq);

		// why wait? reset now to avoid overflows since we took so long
		accelgyro.resetFIFO();

		//      sensors[0] = analogRead(A1);
		sensors[0] = -1;
		//    sensors[1] = sonar.ping_cm();
		sensors[1] = -1;
		sensors[2] = aaReal.x;
		sensors[3] = aaReal.y;
		sensors[4] = aaReal.z;
		sensors[5] = aaWorld.x;
		sensors[6] = aaWorld.y;
		sensors[7] = aaWorld.z;
		//    sensors[8] = sonarRear.ping_cm();
		sensors[8] = -1;

		// why wait? reset now to avoid overflows since we took so long
		accelgyro.resetFIFO();
		
/*
		// FIXME: TESTING
		accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
		sensors[0] = -1;
		sensors[1] = -1;
		sensors[2] = ax;
		sensors[3] = ay;
		sensors[4] = az;
		sensors[5] = gx;
		sensors[6] = gy;
		sensors[7] = gz;
		sensors[8] = -1;


		// --------- Set Node Command to "Send to Unity"
		Serial1.print("6,");

		// --------- Send The Sensor Data
		Serial1.print(sensors[0]);
		for (int i = 1; i < SENSORS_COUNT; i++) {
			Serial1.print(F(","));
			Serial1.print(sensors[i]);
		}
		Serial1.println();

		/* -- TESTING --
		for (int i = 0; i < SENSORS_COUNT; i++) {             // this is unnecessary spewing into the Serial Port
		Serial.print(F(", "));                                // for debuging reasons i suppose... but it works...
		Serial.print(sensors[i]);                             // turn it off...
		}
		Serial.println();
		

		blinkState = !blinkState;
		digitalWrite(b_led, blinkState);
		    }
	}
*/
	// ---
	// SERIAL RECEIVE (USB / console)
	// ---

	// ---------------------------------------- GET NEW MPU SENSOR READINGS -- STAGE 1
	if (Stage1){
		Get_New_MPU();
	}

	if (Serial.available() > 0) {
		String line = Serial.readStringUntil('\n');

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
			{
				Serial.println("driving: " + data);
				drive(data);
				break;
			}

			case 2: // LCD on/off
				lcdToggle(Serial, true);
				break;

			case 3: // LCD text
				lcdText(data);
				break;

			case 4: // Node send
				Serial.println("Sending data to Node...");
				sendToNode(data);
				break;

			case 5: // Bluetooth send
				sendToBluetooth(data);
				break;

			case 6: // Print sensors
				//printSensors(Serial);
				break;

			case 7: // Sensor blast enable/disable
				sensorsEnabled = !sensorsEnabled;
				break;

			case 8: // Sensor blast enable/disable
				RelaySwitch = !RelaySwitch;
				break;

			case 9: // Sensor blast enable/disable
				Demo_Reel100 = !Demo_Reel100;
				Pix_LEDz = !Pix_LEDz;
				break;

			case 10: // Sensor blast enable/disable
				Stage1 = !Stage1;
				break;

			case 11: // Sensor blast enable/disable
				Stage2 = !Stage2;
				break;
			case 0: // Show Menu -OR- Not a valid integer -- fall through to default
			default: // Unknown command

				if (cmdStr == "0") { // Actually show menu
					showMenu(Serial);
				}
				else {
					Serial.println("UNKNOWN SELECTION. Send 0 for menu.");
				}

				break;
			}
		}
	}


	// ---
	// SERIAL1 RECEIVE (NodeMCU)
	// ---

	if (Serial1.available() > 0) {
		yblinkState = !yblinkState;
		digitalWrite(y_led, yblinkState);
		String line = Serial1.readStringUntil('\n');

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
					//Serial1.println("driving: " + data);
				drive(data);
				break;

			case 2: // LCD on/off
				lcdToggle(Serial1, false);
				break;

			case 3: // LCD text
				lcdText(data);
				break;

			case 4: // Node send
					//Serial1.println("Sending data to Node...");
				sendToNode(data);
				break;

			case 5: // Bluetooth send
					//lcdText(data); // debug (local echo) -- temporary
				sendToBluetooth(data);
				break;

			case 6: // Print sensors
				//printSensors(Serial1);
				break;

			case 7: // Sensor blast enable/disable
				sensorsEnabled = !sensorsEnabled;
				break;

			case 8: // Sensor blast enable/disable
				RelaySwitch = !RelaySwitch;
				break;

			case 9: // Sensor blast enable/disable
				Demo_Reel100 = !Demo_Reel100;
				Pix_LEDz = !Pix_LEDz;
				break;

			case 10: // Sensor blast enable/disable
				Stage1 = !Stage1;
				break;

			case 11: // Sensor blast enable/disable
				Stage2 = !Stage2;
				break;

			case 0: // Show Menu -OR- Not a valid integer -- fall through to default
			default: // Unknown command

				if (cmdStr == "0") { // Actually show menu
					showMenu(Serial1);
				}
				else {
					//Serial1.println("UNKNOWN SELECTION. Send 0 for menu.");
				}

				break;
			}
		}
	}


	// ---
	// SERIAL2 RECEIVE (Bluetooth)
	// ---

	if (Serial2.available() > 0) {
		BTblinkState = !BTblinkState;
		digitalWrite(r_led, BTblinkState);

		String line = Serial2.readStringUntil('\n');
		if (line != NULL && line.length() > 0) {

			// Sometimes we get carriage-returns, grr... ditch them
			if (line.charAt(line.length() - 1) == '\r') {
				line = line.substring(0, line.length() - 1);
			}

			int comma = line.indexOf(',');
			String cmdStr = (comma > 0) ? line.substring(0, comma) : line;
			int cmd = cmdStr.toInt();
			String data = (comma > 0) ? line.substring(comma + 1) : "";

			String driveData;

			switch (cmd) {
			case 1: // drive
				Serial2.println("driving: " + data);
				drive(data);
				break;

			case 2: // LCD on/off
				lcdToggle(Serial2, true);
				break;

			case 3: // LCD text
				Serial2.println("Setting LCD text...");
				lcdText(data);
				break;

			case 4: // Node send
					//Serial2.println("Sending data to Node...");
				sendToNode(data);
				break;

			case 5: // Bluetooth send
				sendToBluetooth(data);
				break;

			case 6: // Print sensors
				//printSensors(Serial2);
				break;

			case 7: // Sensor blast enable/disable
				sensorsEnabled = !sensorsEnabled;
				break;

			case 8: // Sensor blast enable/disable
				RelaySwitch = !RelaySwitch;
				break;

			case 9: // Sensor blast enable/disable
				Demo_Reel100 = !Demo_Reel100;
				Pix_LEDz = !Pix_LEDz;
				break;

			case 10: // Sensor blast enable/disable
				Stage1 = !Stage1;
				break;

			case 11: // Sensor blast enable/disable
				Stage2 = !Stage2;
				break;

			case 0: // Show Menu -OR- Not a valid integer -- fall through to default
			default: // Unknown command

				if (cmdStr == "0") { // Actually show menu
					showMenu(Serial2);
				}
				else {
					Serial2.println("UNKNOWN SELECTION. Send 0 for menu.");
				}

				break;
			}
		}
	}


}

void showMenu(HardwareSerial& serial) {
	serial.println();
	serial.println("AutoMobile Serial Menu");
	serial.println();
	serial.println("   0. Show Menu");
	serial.println("   1. Drive <L,R>");
	serial.println("   2. LCD on/off");
	serial.println("   3. LCD print <text>");
	serial.println("   4. Send to Node <DATA>");
	serial.println("   5. Send to Bluetooth <DATA>");
	serial.println("   6. Print sensor readings");
	serial.println(String("   7. Sensor blast ") + (sensorsEnabled ? "disable" : "enable"));
	serial.println(String("   8. Relay Switch ") + (RelaySwitch ? "disable" : "enable"));
	serial.println(String("   9. Light Show ") + (Demo_Reel100 ? "disable" : "enable"));
	serial.println(String("   10. Stage1 MPU ") + (Stage1 ? "disable" : "enable"));
	serial.println(String("   11. Stage2 MPU ") + (Stage2 ? "disable" : "enable"));
	
	serial.println();
}


void lcdToggle(HardwareSerial serial, boolean verbose) {
	if (lcdOn) {
		if (verbose) serial.println("Turning LCD display OFF");
		lcd.noDisplay();
	}
	else {
		if (verbose) serial.println("Turning ON LCD display");
		lcd.display();
	}
	lcdOn = !lcdOn;
}


void lcdText(String text) {
	lcdMsg = text;
	lcd.clear();
	lcd.home();
	//lcd.print("TEXT RECEIVED");
	lcd.setCursor(0, 1); // second line
	lcd.print(lcdMsg + " ");
}


void drive(String data) {
	lcd.clear(); // would rather not clear here
	lcd.home();
	//lcd.print("                "); // clear the top line
	//lcd.home();
	lcd.print("GO: " + data);
	lcd.setCursor(0, 1); // second line
	lcd.print(lcdMsg + " ");

	
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


void sendToBluetooth(String data) {
	Serial2.println(data);
}


void ledSequence() {
	digitalWrite(g_led, LOW);
	digitalWrite(r_led, LOW);
	digitalWrite(b_led, LOW);
	digitalWrite(y_led, LOW);

	int seq[] = { y_led, b_led, r_led, g_led, r_led, b_led, y_led };
	int len = sizeof(seq) / sizeof(int);

	for (int i = 0; i <= len; i++) {
		if (i < len) {
			digitalWrite(seq[i], HIGH);
		}
		delay(25);
		if (i > 0) {
			digitalWrite(seq[i - 1], LOW);
		}
		delay(50);
	}
}

/*
void printSensors(HardwareSerial& serial) {
	accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
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
	blinkState = !blinkState;
	digitalWrite(b_led, blinkState);
	delay(10);
}
*/

void StartupAnimaitonLCD() {
	delay(275);
	lcd.setCursor(3, 0);
	lcd.write(2);
	delay(275);
	lcd.setCursor(4, 0);
	lcd.write(6);
	delay(275);
	lcd.setCursor(5, 0);
	lcd.write(4);
	delay(275);
	lcd.setCursor(6, 0);
	lcd.write(5);
	delay(275);
	lcd.setCursor(7, 0);
	lcd.write(6);
	delay(275);
	lcd.setCursor(8, 0);
	lcd.write(7);
	delay(275);
	lcd.setCursor(9, 0);
	lcd.write(8);
	delay(275);
	lcd.setCursor(10, 0);
	lcd.write(7);
	delay(275);
	lcd.setCursor(11, 0);
	lcd.write(1);
	//  delay(2500);
	//  lcd.autoscroll();
	//  // scroll out the top line
	//  for (int thisChar = 0; thisChar <= 15; thisChar++) {
	//    lcd.write(99);  // 99 is space
	//    delay(175);
	//  }
	//  lcd.noAutoscroll();
	delay(755);
}

void LED_switcher(String data) {
	int choice = data.toInt();

	switch (choice) {
	case 1: // Fire Test setup
		Fire();		
		break;

	case 2: // Night rider basic back and forth
		NightRider();
		break;
	
	case 3: // BOND
		Signal_LED();
		break;

	case 0:
	default:
		Serial.println("5,Light Show Selection:");
		Serial.println("5,   1. Fire");
		Serial.println("5,   2. Night Rider");
		Serial.println("5,   3. Signal Feedback");
		break;
	}
}

void Fire() {   // this works
	// Array of temperature readings at each simulation cell
  static byte heat[NUM_LEDS];

    // Step 1.  Cool down every cell a little
  for( int i = 0; i < NUM_LEDS; i++) {
      heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
    }
  
    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
  for( int k= NUM_LEDS - 1; k >= 2; k--) {
      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
    }
    
    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
  if( random8() < SPARKING ) {
      int y = random8(7);
      heat[y] = qadd8( heat[y], random8(160,255) );
    }

    // Step 4.  Map from heat cells to LED colors
  for( int j = 0; j < NUM_LEDS; j++) {
      CRGB color = HeatColor( heat[j]);
      int pixelnumber;
    if( gReverseDirection ) {
        pixelnumber = (NUM_LEDS-1) - j;
    } else {
        pixelnumber = j;
      }
      leds[pixelnumber] = color;
    }
	FastLED.show();
}

void NightRider() {
	// Move a single white led 
   for(int whiteLed = 0; whiteLed < NUM_LEDS; whiteLed = whiteLed + 1) {
      // Turn our current led on to white, then show the leds
      leds[whiteLed] = CRGB::Green;

      // Show the leds (only one of which is set to white, from above)
      FastLED.show();

      // Wait a little bit
      delay(75);

      // Turn our current led back to black for the next loop around
      leds[whiteLed] = CRGB::Black;
   }
   for(int whiteLed = NUM_LEDS - 1; whiteLed > 0; whiteLed = whiteLed - 1) {
      // Turn our current led on to white, then show the leds
      leds[whiteLed] = CRGB::Green;

      // Show the leds (only one of which is set to white, from above)
      FastLED.show();

      // Wait a little bit
      delay(75);

      // Turn our current led back to black for the next loop around
      leds[whiteLed] = CRGB::Black;
   }
}

void Signal_LED() {
	
}

void Demo_Reel() {
	// Call the current pattern function once, updating the 'leds' array
  gPatterns[gCurrentPatternNumber]();

  // send the 'leds' array out to the actual LED strip
  FastLED.show();  
  // insert a delay to keep the framerate modest
  FastLED.delay(1000/FRAMES_PER_SECOND); 

  // do some periodic updates
  EVERY_N_MILLISECONDS( 10 ) { gHue++; }   // slowly cycle the "base color" through the rainbow
  EVERY_N_SECONDS( 25 ) { nextPattern(); } // change patterns periodically
}

void nextPattern() {
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
}

void rainbow() 
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS, gHue, 7);
}

void rainbowWithGlitter() 
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(80);
}

void addGlitter( fract8 chanceOfGlitter) 
{
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}

void confetti() 
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(64), 200, 255);
}

void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = beatsin16(13,0,NUM_LEDS);
  leds[pos] += CHSV( gHue, 255, 192);
}

void bpm()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
}

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  byte dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds[beatsin16(i+7,0,NUM_LEDS)] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}


void Get_New_MPU() {  
  // If data ready bit set, all data registers have new data
  if(readByte(MPU6050_ADDRESS, INT_STATUS) & 0x01) {  // check if data ready interrupt

    readAccelData(accelCount);  // Read the x/y/z adc values
    getAres();
    
    // Now we'll calculate the accleration value into actual g's
    ax = (float)accelCount[0]*aRes - accelBias[0];  // get actual g value, this depends on scale being set
    ay = (float)accelCount[1]*aRes - accelBias[1];   
    az = (float)accelCount[2]*aRes - accelBias[2];  
   
    readGyroData(gyroCount);  // Read the x/y/z adc values
    getGres();
 
    // Calculate the gyro value into actual degrees per second
    gx = (float)gyroCount[0]*gRes - gyroBias[0];  // get actual gyro value, this depends on scale being set
    gy = (float)gyroCount[1]*gRes - gyroBias[1];  
    gz = (float)gyroCount[2]*gRes - gyroBias[2];   

    tempCount = readTempData();  // Read the x/y/z adc values
    temperature = ((float) tempCount) / 340. + 36.53; // Temperature in degrees Centigrade
   }  
   
    uint32_t deltat = millis() - count;
    if(deltat > 500) {
 
    // Print acceleration values in milligs!
    Serial.print("X-acceleration: "); Serial.print(1000*ax); Serial.print(" mg "); 
    Serial.print("Y-acceleration: "); Serial.print(1000*ay); Serial.print(" mg "); 
    Serial.print("Z-acceleration: "); Serial.print(1000*az); Serial.println(" mg"); 
 
    // Print gyro values in degree/sec
    Serial.print("X-gyro rate: "); Serial.print(gx, 1); Serial.print(" degrees/sec "); 
    Serial.print("Y-gyro rate: "); Serial.print(gy, 1); Serial.print(" degrees/sec "); 
    Serial.print("Z-gyro rate: "); Serial.print(gz, 1); Serial.println(" degrees/sec"); 
    
   // Print temperature in degrees Centigrade      
    Serial.print("Temperature is ");  Serial.print(temperature, 2);  Serial.println(" degrees C"); // Print T values to tenths of s degree C
    Serial.println("");
        
    count = millis();
    }

}


//===================================================================================================================
//====== Set of useful function to access acceleration, gyroscope, and temperature data
//===================================================================================================================

void getGres() {
  switch (Gscale)
  {
 	// Possible gyro scales (and their register bit settings) are:
	// 250 DPS (00), 500 DPS (01), 1000 DPS (10), and 2000 DPS  (11). 
    case GFS_250DPS:
          gRes = 250.0/32768.0;
          break;
    case GFS_500DPS:
          gRes = 500.0/32768.0;
          break;
    case GFS_1000DPS:
          gRes = 1000.0/32768.0;
          break;
    case GFS_2000DPS:
          gRes = 2000.0/32768.0;
          break;
  }
}

void getAres() {
  switch (Ascale)
  {
 	// Possible accelerometer scales (and their register bit settings) are:
	// 2 Gs (00), 4 Gs (01), 8 Gs (10), and 16 Gs  (11). 
    case AFS_2G:
          aRes = 2.0/32768.0;
          break;
    case AFS_4G:
          aRes = 4.0/32768.0;
          break;
    case AFS_8G:
          aRes = 8.0/32768.0;
          break;
    case AFS_16G:
          aRes = 16.0/32768.0;
          break;
  }
}


void readAccelData(int16_t * destination)
{
  uint8_t rawData[6];  // x/y/z accel register data stored here
  readBytes(MPU6050_ADDRESS, ACCEL_XOUT_H, 6, &rawData[0]);  // Read the six raw data registers into data array
  destination[0] = (int16_t)((rawData[0] << 8) | rawData[1]) ;  // Turn the MSB and LSB into a signed 16-bit value
  destination[1] = (int16_t)((rawData[2] << 8) | rawData[3]) ;  
  destination[2] = (int16_t)((rawData[4] << 8) | rawData[5]) ; 
}

void readGyroData(int16_t * destination)
{
  uint8_t rawData[6];  // x/y/z gyro register data stored here
  readBytes(MPU6050_ADDRESS, GYRO_XOUT_H, 6, &rawData[0]);  // Read the six raw data registers sequentially into data array
  destination[0] = (int16_t)((rawData[0] << 8) | rawData[1]) ;  // Turn the MSB and LSB into a signed 16-bit value
  destination[1] = (int16_t)((rawData[2] << 8) | rawData[3]) ;  
  destination[2] = (int16_t)((rawData[4] << 8) | rawData[5]) ; 
}

int16_t readTempData()
{
  uint8_t rawData[2];  // x/y/z gyro register data stored here
  readBytes(MPU6050_ADDRESS, TEMP_OUT_H, 2, &rawData[0]);  // Read the two raw data registers sequentially into data array 
  return ((int16_t)rawData[0]) << 8 | rawData[1] ;  // Turn the MSB and LSB into a 16-bit value
}



// Configure the motion detection control for low power accelerometer mode
void LowPowerAccelOnlyMPU6050()
{

// The sensor has a high-pass filter necessary to invoke to allow the sensor motion detection algorithms work properly
// Motion detection occurs on free-fall (acceleration below a threshold for some time for all axes), motion (acceleration
// above a threshold for some time on at least one axis), and zero-motion toggle (acceleration on each axis less than a 
// threshold for some time sets this flag, motion above the threshold turns it off). The high-pass filter takes gravity out
// consideration for these threshold evaluations; otherwise, the flags would be set all the time!
  
  uint8_t c = readByte(MPU6050_ADDRESS, PWR_MGMT_1);
  writeByte(MPU6050_ADDRESS, PWR_MGMT_1, c & ~0x30); // Clear sleep and cycle bits [5:6]
  writeByte(MPU6050_ADDRESS, PWR_MGMT_1, c |  0x30); // Set sleep and cycle bits [5:6] to zero to make sure accelerometer is running

  c = readByte(MPU6050_ADDRESS, PWR_MGMT_2);
  writeByte(MPU6050_ADDRESS, PWR_MGMT_2, c & ~0x38); // Clear standby XA, YA, and ZA bits [3:5]
  writeByte(MPU6050_ADDRESS, PWR_MGMT_2, c |  0x00); // Set XA, YA, and ZA bits [3:5] to zero to make sure accelerometer is running
    
  c = readByte(MPU6050_ADDRESS, ACCEL_CONFIG);
  writeByte(MPU6050_ADDRESS, ACCEL_CONFIG, c & ~0x07); // Clear high-pass filter bits [2:0]
// Set high-pass filter to 0) reset (disable), 1) 5 Hz, 2) 2.5 Hz, 3) 1.25 Hz, 4) 0.63 Hz, or 7) Hold
  writeByte(MPU6050_ADDRESS, ACCEL_CONFIG,  c | 0x00);  // Set ACCEL_HPF to 0; reset mode disbaling high-pass filter

  c = readByte(MPU6050_ADDRESS, CONFIG);
  writeByte(MPU6050_ADDRESS, CONFIG, c & ~0x07); // Clear low-pass filter bits [2:0]
  writeByte(MPU6050_ADDRESS, CONFIG, c |  0x00);  // Set DLPD_CFG to 0; 260 Hz bandwidth, 1 kHz rate
    
  c = readByte(MPU6050_ADDRESS, INT_ENABLE);
  writeByte(MPU6050_ADDRESS, INT_ENABLE, c & ~0xFF);  // Clear all interrupts
  writeByte(MPU6050_ADDRESS, INT_ENABLE, 0x40);  // Enable motion threshold (bits 5) interrupt only
  
// Motion detection interrupt requires the absolute value of any axis to lie above the detection threshold
// for at least the counter duration
  writeByte(MPU6050_ADDRESS, MOT_THR, 0x80); // Set motion detection to 0.256 g; LSB = 2 mg
  writeByte(MPU6050_ADDRESS, MOT_DUR, 0x01); // Set motion detect duration to 1  ms; LSB is 1 ms @ 1 kHz rate
  
  delay (100);  // Add delay for accumulation of samples
  
  c = readByte(MPU6050_ADDRESS, ACCEL_CONFIG);
  writeByte(MPU6050_ADDRESS, ACCEL_CONFIG, c & ~0x07); // Clear high-pass filter bits [2:0]
  writeByte(MPU6050_ADDRESS, ACCEL_CONFIG, c |  0x07);  // Set ACCEL_HPF to 7; hold the initial accleration value as a referance
   
  c = readByte(MPU6050_ADDRESS, PWR_MGMT_2);
  writeByte(MPU6050_ADDRESS, PWR_MGMT_2, c & ~0xC7); // Clear standby XA, YA, and ZA bits [3:5] and LP_WAKE_CTRL bits [6:7]
  writeByte(MPU6050_ADDRESS, PWR_MGMT_2, c |  0x47); // Set wakeup frequency to 5 Hz, and disable XG, YG, and ZG gyros (bits [0:2])  

  c = readByte(MPU6050_ADDRESS, PWR_MGMT_1);
  writeByte(MPU6050_ADDRESS, PWR_MGMT_1, c & ~0x20); // Clear sleep and cycle bit 5
  writeByte(MPU6050_ADDRESS, PWR_MGMT_1, c |  0x20); // Set cycle bit 5 to begin low power accelerometer motion interrupts

}


void initMPU6050()
{  
 // Initialize MPU6050 device

//  wake up device-don't need this here if using calibration function below
//  writeByte(MPU6050_ADDRESS, PWR_MGMT_1, 0x00); // Clear sleep mode bit (6), enable all sensors 
//  delay(100); // Delay 100 ms for PLL to get established on x-axis gyro; should check for PLL ready interrupt  

  // get stable time source
  writeByte(MPU6050_ADDRESS, PWR_MGMT_1, 0x01);  // Set clock source to be PLL with x-axis gyroscope reference, bits 2:0 = 001

 // Configure Gyro and Accelerometer
 // Disable FSYNC and set accelerometer and gyro bandwidth to 44 and 42 Hz, respectively; 
 // DLPF_CFG = bits 2:0 = 010; this sets the sample rate at 1 kHz for both
  writeByte(MPU6050_ADDRESS, CONFIG, 0x03);  
 
 // Set sample rate = gyroscope output rate/(1 + SMPLRT_DIV)
  writeByte(MPU6050_ADDRESS, SMPLRT_DIV, 0x04);  // Use a 200 Hz sample rate 
 
 // Set gyroscope full scale range
 // Range selects FS_SEL and AFS_SEL are 0 - 3, so 2-bit values are left-shifted into positions 4:3
  uint8_t c =  readByte(MPU6050_ADDRESS, GYRO_CONFIG);
  writeByte(MPU6050_ADDRESS, GYRO_CONFIG, c & ~0xE0); // Clear self-test bits [7:5] 
  writeByte(MPU6050_ADDRESS, GYRO_CONFIG, c & ~0x18); // Clear AFS bits [4:3]
  writeByte(MPU6050_ADDRESS, GYRO_CONFIG, c | Gscale << 3); // Set full scale range for the gyro
   
 // Set accelerometer configuration
  c =  readByte(MPU6050_ADDRESS, ACCEL_CONFIG);
  writeByte(MPU6050_ADDRESS, ACCEL_CONFIG, c & ~0xE0); // Clear self-test bits [7:5] 
  writeByte(MPU6050_ADDRESS, ACCEL_CONFIG, c & ~0x18); // Clear AFS bits [4:3]
  writeByte(MPU6050_ADDRESS, ACCEL_CONFIG, c | Ascale << 3); // Set full scale range for the accelerometer 

  // Configure Interrupts and Bypass Enable
  // Set interrupt pin active high, push-pull, and clear on read of INT_STATUS, enable I2C_BYPASS_EN so additional chips 
  // can join the I2C bus and all can be controlled by the Arduino as master
   writeByte(MPU6050_ADDRESS, INT_PIN_CFG, 0x02);    
   writeByte(MPU6050_ADDRESS, INT_ENABLE, 0x01);  // Enable data ready (bit 0) interrupt
}


// Function which accumulates gyro and accelerometer data after device initialization. It calculates the average
// of the at-rest readings and then loads the resulting offsets into accelerometer and gyro bias registers.
void calibrateMPU6050(float * dest1, float * dest2)
{  
  uint8_t data[12]; // data array to hold accelerometer and gyro x, y, z, data
  uint16_t ii, packet_count, fifo_count;
  int32_t gyro_bias[3] = {0, 0, 0}, accel_bias[3] = {0, 0, 0};
  
// reset device, reset all registers, clear gyro and accelerometer bias registers
  writeByte(MPU6050_ADDRESS, PWR_MGMT_1, 0x80); // Write a one to bit 7 reset bit; toggle reset device
  delay(100);  
   
// get stable time source
// Set clock source to be PLL with x-axis gyroscope reference, bits 2:0 = 001
  writeByte(MPU6050_ADDRESS, PWR_MGMT_1, 0x01);  
  writeByte(MPU6050_ADDRESS, PWR_MGMT_2, 0x00); 
  delay(200);
  
// Configure device for bias calculation
  writeByte(MPU6050_ADDRESS, INT_ENABLE, 0x00);   // Disable all interrupts
  writeByte(MPU6050_ADDRESS, FIFO_EN, 0x00);      // Disable FIFO
  writeByte(MPU6050_ADDRESS, PWR_MGMT_1, 0x00);   // Turn on internal clock source
  writeByte(MPU6050_ADDRESS, I2C_MST_CTRL, 0x00); // Disable I2C master
  writeByte(MPU6050_ADDRESS, USER_CTRL, 0x00);    // Disable FIFO and I2C master modes
  writeByte(MPU6050_ADDRESS, USER_CTRL, 0x0C);    // Reset FIFO and DMP
  delay(15);
  
// Configure MPU6050 gyro and accelerometer for bias calculation
  writeByte(MPU6050_ADDRESS, CONFIG, 0x01);      // Set low-pass filter to 188 Hz
  writeByte(MPU6050_ADDRESS, SMPLRT_DIV, 0x00);  // Set sample rate to 1 kHz
  writeByte(MPU6050_ADDRESS, GYRO_CONFIG, 0x00);  // Set gyro full-scale to 250 degrees per second, maximum sensitivity
  writeByte(MPU6050_ADDRESS, ACCEL_CONFIG, 0x00); // Set accelerometer full-scale to 2 g, maximum sensitivity
 
  uint16_t  gyrosensitivity  = 131;   // = 131 LSB/degrees/sec
  uint16_t  accelsensitivity = 16384;  // = 16384 LSB/g

// Configure FIFO to capture accelerometer and gyro data for bias calculation
  writeByte(MPU6050_ADDRESS, USER_CTRL, 0x40);   // Enable FIFO  
  writeByte(MPU6050_ADDRESS, FIFO_EN, 0x78);     // Enable gyro and accelerometer sensors for FIFO  (max size 1024 bytes in MPU-6050)
  delay(80); // accumulate 80 samples in 80 milliseconds = 960 bytes

// At end of sample accumulation, turn off FIFO sensor read
  writeByte(MPU6050_ADDRESS, FIFO_EN, 0x00);        // Disable gyro and accelerometer sensors for FIFO
  readBytes(MPU6050_ADDRESS, FIFO_COUNTH, 2, &data[0]); // read FIFO sample count
  fifo_count = ((uint16_t)data[0] << 8) | data[1];
  packet_count = fifo_count/12;// How many sets of full gyro and accelerometer data for averaging

  for (ii = 0; ii < packet_count; ii++) {
    int16_t accel_temp[3] = {0, 0, 0}, gyro_temp[3] = {0, 0, 0};
    readBytes(MPU6050_ADDRESS, FIFO_R_W, 12, &data[0]); // read data for averaging
    accel_temp[0] = (int16_t) (((int16_t)data[0] << 8) | data[1]  ) ;  // Form signed 16-bit integer for each sample in FIFO
    accel_temp[1] = (int16_t) (((int16_t)data[2] << 8) | data[3]  ) ;
    accel_temp[2] = (int16_t) (((int16_t)data[4] << 8) | data[5]  ) ;    
    gyro_temp[0]  = (int16_t) (((int16_t)data[6] << 8) | data[7]  ) ;
    gyro_temp[1]  = (int16_t) (((int16_t)data[8] << 8) | data[9]  ) ;
    gyro_temp[2]  = (int16_t) (((int16_t)data[10] << 8) | data[11]) ;
    
    accel_bias[0] += (int32_t) accel_temp[0]; // Sum individual signed 16-bit biases to get accumulated signed 32-bit biases
    accel_bias[1] += (int32_t) accel_temp[1];
    accel_bias[2] += (int32_t) accel_temp[2];
    gyro_bias[0]  += (int32_t) gyro_temp[0];
    gyro_bias[1]  += (int32_t) gyro_temp[1];
    gyro_bias[2]  += (int32_t) gyro_temp[2];
            
}
    accel_bias[0] /= (int32_t) packet_count; // Normalize sums to get average count biases
    accel_bias[1] /= (int32_t) packet_count;
    accel_bias[2] /= (int32_t) packet_count;
    gyro_bias[0]  /= (int32_t) packet_count;
    gyro_bias[1]  /= (int32_t) packet_count;
    gyro_bias[2]  /= (int32_t) packet_count;
    
  if(accel_bias[2] > 0L) {accel_bias[2] -= (int32_t) accelsensitivity;}  // Remove gravity from the z-axis accelerometer bias calculation
  else {accel_bias[2] += (int32_t) accelsensitivity;}
 
// Construct the gyro biases for push to the hardware gyro bias registers, which are reset to zero upon device startup
  data[0] = (-gyro_bias[0]/4  >> 8) & 0xFF; // Divide by 4 to get 32.9 LSB per deg/s to conform to expected bias input format
  data[1] = (-gyro_bias[0]/4)       & 0xFF; // Biases are additive, so change sign on calculated average gyro biases
  data[2] = (-gyro_bias[1]/4  >> 8) & 0xFF;
  data[3] = (-gyro_bias[1]/4)       & 0xFF;
  data[4] = (-gyro_bias[2]/4  >> 8) & 0xFF;
  data[5] = (-gyro_bias[2]/4)       & 0xFF;

// Push gyro biases to hardware registers; works well for gyro but not for accelerometer
//  writeByte(MPU6050_ADDRESS, XG_OFFS_USRH, data[0]); 
//  writeByte(MPU6050_ADDRESS, XG_OFFS_USRL, data[1]);
//  writeByte(MPU6050_ADDRESS, YG_OFFS_USRH, data[2]);
//  writeByte(MPU6050_ADDRESS, YG_OFFS_USRL, data[3]);
//  writeByte(MPU6050_ADDRESS, ZG_OFFS_USRH, data[4]);
//  writeByte(MPU6050_ADDRESS, ZG_OFFS_USRL, data[5]);

  dest1[0] = (float) gyro_bias[0]/(float) gyrosensitivity; // construct gyro bias in deg/s for later manual subtraction
  dest1[1] = (float) gyro_bias[1]/(float) gyrosensitivity;
  dest1[2] = (float) gyro_bias[2]/(float) gyrosensitivity;

// Construct the accelerometer biases for push to the hardware accelerometer bias registers. These registers contain
// factory trim values which must be added to the calculated accelerometer biases; on boot up these registers will hold
// non-zero values. In addition, bit 0 of the lower byte must be preserved since it is used for temperature
// compensation calculations. Accelerometer bias registers expect bias input as 2048 LSB per g, so that
// the accelerometer biases calculated above must be divided by 8.

  int32_t accel_bias_reg[3] = {0, 0, 0}; // A place to hold the factory accelerometer trim biases
  readBytes(MPU6050_ADDRESS, XA_OFFSET_H, 2, &data[0]); // Read factory accelerometer trim values
  accel_bias_reg[0] = (int16_t) ((int16_t)data[0] << 8) | data[1];
  readBytes(MPU6050_ADDRESS, YA_OFFSET_H, 2, &data[0]);
  accel_bias_reg[1] = (int16_t) ((int16_t)data[0] << 8) | data[1];
  readBytes(MPU6050_ADDRESS, ZA_OFFSET_H, 2, &data[0]);
  accel_bias_reg[2] = (int16_t) ((int16_t)data[0] << 8) | data[1];
  
  uint32_t mask = 1uL; // Define mask for temperature compensation bit 0 of lower byte of accelerometer bias registers
  uint8_t mask_bit[3] = {0, 0, 0}; // Define array to hold mask bit for each accelerometer bias axis
  
  for(ii = 0; ii < 3; ii++) {
    if(accel_bias_reg[ii] & mask) mask_bit[ii] = 0x01; // If temperature compensation bit is set, record that fact in mask_bit
  }

  // Construct total accelerometer bias, including calculated average accelerometer bias from above
  accel_bias_reg[0] -= (accel_bias[0]/8); // Subtract calculated averaged accelerometer bias scaled to 2048 LSB/g (16 g full scale)
  accel_bias_reg[1] -= (accel_bias[1]/8);
  accel_bias_reg[2] -= (accel_bias[2]/8);
 
  data[0] = (accel_bias_reg[0] >> 8) & 0xFF;
  data[1] = (accel_bias_reg[0])      & 0xFF;
  data[1] = data[1] | mask_bit[0]; // preserve temperature compensation bit when writing back to accelerometer bias registers
  data[2] = (accel_bias_reg[1] >> 8) & 0xFF;
  data[3] = (accel_bias_reg[1])      & 0xFF;
  data[3] = data[3] | mask_bit[1]; // preserve temperature compensation bit when writing back to accelerometer bias registers
  data[4] = (accel_bias_reg[2] >> 8) & 0xFF;
  data[5] = (accel_bias_reg[2])      & 0xFF;
  data[5] = data[5] | mask_bit[2]; // preserve temperature compensation bit when writing back to accelerometer bias registers

  // Push accelerometer biases to hardware registers; doesn't work well for accelerometer
  // Are we handling the temperature compensation bit correctly?
//  writeByte(MPU6050_ADDRESS, XA_OFFSET_H, data[0]);  
//  writeByte(MPU6050_ADDRESS, XA_OFFSET_L_TC, data[1]);
//  writeByte(MPU6050_ADDRESS, YA_OFFSET_H, data[2]);
//  writeByte(MPU6050_ADDRESS, YA_OFFSET_L_TC, data[3]);  
//  writeByte(MPU6050_ADDRESS, ZA_OFFSET_H, data[4]);
//  writeByte(MPU6050_ADDRESS, ZA_OFFSET_L_TC, data[5]);

// Output scaled accelerometer biases for manual subtraction in the main program
   dest2[0] = (float)accel_bias[0]/(float)accelsensitivity; 
   dest2[1] = (float)accel_bias[1]/(float)accelsensitivity;
   dest2[2] = (float)accel_bias[2]/(float)accelsensitivity;
}


// Accelerometer and gyroscope self test; check calibration wrt factory settings
void MPU6050SelfTest(float * destination) // Should return percent deviation from factory trim values, +/- 14 or less deviation is a pass
{
   uint8_t rawData[4];
   uint8_t selfTest[6];
   float factoryTrim[6];
   
   // Configure the accelerometer for self-test
   writeByte(MPU6050_ADDRESS, ACCEL_CONFIG, 0xF0); // Enable self test on all three axes and set accelerometer range to +/- 8 g
   writeByte(MPU6050_ADDRESS, GYRO_CONFIG,  0xE0); // Enable self test on all three axes and set gyro range to +/- 250 degrees/s
   delay(250);  // Delay a while to let the device execute the self-test
   rawData[0] = readByte(MPU6050_ADDRESS, SELF_TEST_X); // X-axis self-test results
   rawData[1] = readByte(MPU6050_ADDRESS, SELF_TEST_Y); // Y-axis self-test results
   rawData[2] = readByte(MPU6050_ADDRESS, SELF_TEST_Z); // Z-axis self-test results
   rawData[3] = readByte(MPU6050_ADDRESS, SELF_TEST_A); // Mixed-axis self-test results
   // Extract the acceleration test results first
   selfTest[0] = (rawData[0] >> 3) | (rawData[3] & 0x30) >> 4 ; // XA_TEST result is a five-bit unsigned integer
   selfTest[1] = (rawData[1] >> 3) | (rawData[3] & 0x0C) >> 2 ; // YA_TEST result is a five-bit unsigned integer
   selfTest[2] = (rawData[2] >> 3) | (rawData[3] & 0x03) >> 0 ; // ZA_TEST result is a five-bit unsigned integer
   // Extract the gyration test results first
   selfTest[3] = rawData[0]  & 0x1F ; // XG_TEST result is a five-bit unsigned integer
   selfTest[4] = rawData[1]  & 0x1F ; // YG_TEST result is a five-bit unsigned integer
   selfTest[5] = rawData[2]  & 0x1F ; // ZG_TEST result is a five-bit unsigned integer   
   // Process results to allow final comparison with factory set values
   factoryTrim[0] = (4096.0*0.34)*(pow( (0.92/0.34) , (((float)selfTest[0] - 1.0)/30.0))); // FT[Xa] factory trim calculation
   factoryTrim[1] = (4096.0*0.34)*(pow( (0.92/0.34) , (((float)selfTest[1] - 1.0)/30.0))); // FT[Ya] factory trim calculation
   factoryTrim[2] = (4096.0*0.34)*(pow( (0.92/0.34) , (((float)selfTest[2] - 1.0)/30.0))); // FT[Za] factory trim calculation
   factoryTrim[3] =  ( 25.0*131.0)*(pow( 1.046 , ((float)selfTest[3] - 1.0) ));             // FT[Xg] factory trim calculation
   factoryTrim[4] =  (-25.0*131.0)*(pow( 1.046 , ((float)selfTest[4] - 1.0) ));             // FT[Yg] factory trim calculation
   factoryTrim[5] =  ( 25.0*131.0)*(pow( 1.046 , ((float)selfTest[5] - 1.0) ));             // FT[Zg] factory trim calculation
   
 //  Output self-test results and factory trim calculation if desired
 //  Serial.println(selfTest[0]); Serial.println(selfTest[1]); Serial.println(selfTest[2]);
 //  Serial.println(selfTest[3]); Serial.println(selfTest[4]); Serial.println(selfTest[5]);
 //  Serial.println(factoryTrim[0]); Serial.println(factoryTrim[1]); Serial.println(factoryTrim[2]);
 //  Serial.println(factoryTrim[3]); Serial.println(factoryTrim[4]); Serial.println(factoryTrim[5]);

 // Report results as a ratio of (STR - FT)/FT; the change from Factory Trim of the Self-Test Response
 // To get to percent, must multiply by 100 and subtract result from 100
   for (int i = 0; i < 6; i++) {
     destination[i] = 100.0 + 100.0*((float)selfTest[i] - factoryTrim[i])/factoryTrim[i]; // Report percent differences
   }
   
}

  void writeByte(uint8_t address, uint8_t subAddress, uint8_t data)
{
	Wire.beginTransmission(address);  // Initialize the Tx buffer
	Wire.write(subAddress);           // Put slave register address in Tx buffer
	Wire.write(data);                 // Put data in Tx buffer
	Wire.endTransmission();           // Send the Tx buffer
}

  uint8_t readByte(uint8_t address, uint8_t subAddress)
{
	uint8_t data; // `data` will store the register data	 
	Wire.beginTransmission(address);         // Initialize the Tx buffer
	Wire.write(subAddress);	                 // Put slave register address in Tx buffer
	Wire.endTransmission(false);             // Send the Tx buffer, but send a restart to keep connection alive
	Wire.requestFrom(address, (uint8_t) 1);  // Read one byte from slave register address 
	data = Wire.read();                      // Fill Rx buffer with result
	return data;                             // Return data read from slave register
}

  void readBytes(uint8_t address, uint8_t subAddress, uint8_t count, uint8_t * dest)
{  
	Wire.beginTransmission(address);   // Initialize the Tx buffer
	Wire.write(subAddress);            // Put slave register address in Tx buffer
	Wire.endTransmission(false);       // Send the Tx buffer, but send a restart to keep connection alive
	uint8_t i = 0;
        Wire.requestFrom(address, count);  // Read bytes from slave register address 
	while (Wire.available()) {
        dest[i++] = Wire.read(); }         // Put read results in the Rx buffer
}
// --------------------------------- ENd of the GAME, SON!!!!