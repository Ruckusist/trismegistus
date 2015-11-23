
#include <arduino.h>
#include "AutoMobile.h"
#include "lib/DEFINITIONS.h"


int sensors[SENSORS_COUNT];        // 9 element array holding Sensor readings
bool sensorsEnabled = false;
bool sensors2Enabled = false;

bool blinkState = false;           // Activity for Sending
bool BTblinkState = false;         // Activity for Recieving
bool yblinkState = false;
bool gblinkState = false;

long lastNodeRec = 0;
bool lcdOn = false;


bool dmpReady = false;   // set true if DMP init was successful
uint8_t mpuIntStatus;    // holds actual interrupt status byte from MPU
uint8_t devStatus;       // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;     // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;      // count of all bytes currently in FIFO
uint8_t fifoBuffer[256]; // FIFO storage buffer
						 //volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high

// orientation/motion vars
/*
Quaternion q;            // [w, x, y, z]         quaternion container
VectorInt16 aa;          // [x, y, z]            accel sensor measurements
VectorInt16 aaReal;      // [x, y, z]            gravity-free accel sensor measurements
VectorInt16 aaWorld;     // [x, y, z]            world-frame accel sensor measurements
VectorFloat gravity;     // [x, y, z]            gravity vector
float euler[3];          // [psi, theta, phi]    Euler angle container
float ypr[3];            // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector
// --------------------------------- MPU Variables
int16_t ax, ay, az;
int16_t gx, gy, gz;
*/

void AutoMobile::setup() {

}

void AutoMobile::loop() {

}