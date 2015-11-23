// DEFINITIONS.h

#ifndef _DEFINITIONS_h
#define _DEFINITIONS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


const unsigned int SERIAL_USB_BAUD = 38400;
const unsigned int SERIAL_NODE_BAUD = 9600;
const unsigned int SERIAL_BT_BAUD = 9600;




//DEFINE NAME      PINHOLE
// --------------------------------- PINOUTs
#define y_led        32            // these lights should maybe go somewhere else
#define b_led        33            // and i should get a multi color light for debug purpose
#define r_led        30
#define g_led        31
#define LED_PIN      13            // there are no calls for this... and thats okay too
#define TRIGGER_PIN   6            // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN      7            // Arduino pin tied to echo pin on the ultrasonic sensor.
#define TRIGGER_PIN_REAR  9        // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN_REAR     8        // Arduino pin tied to echo pin on the ultrasonic sensor.
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

// --------------------------------- LCD
// initialize the library with the numbers of the interface pins
#define LCD_PIN_14      22
#define LCD_PIN_13      3
#define LCD_PIN_12      4
#define LCD_PIN_11      5
#define LCD_PIN_6       11
#define LCD_PIN_4       12


#define MAX_DISTANCE 200           // Maximum sensor distance is rated at 400-500cm.
const int sensorMin = 10;          // sensor minimum, discovered through experiment
const int sensorMax = 190;         // sensor maximum, discovered through experiment
const int TOO_CLOSE = 10;          // Min distance

#define SENSORS_COUNT 9

#define TXMIT_DELAY  100
#define NODE_QUIET 2000        // Max time between node receives before we consider it quiet (in ms)


#endif

