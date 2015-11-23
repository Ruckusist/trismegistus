#include "AMAccelGyro.h"
#include "AMLCD.h"
#include "AMDrive.h"
#include "AMLED.h"
#include "DEFINITIONS.h"
#include "AutoMobile.h"
#include "Wire.h"
#include "LiquidCrystal.h"
#include "artofcircuits_l298n.h"   // Drive commands

AutoMobile Automobile;

void setup() {
	
  /* add setup code here */

}

void loop() {
	Automobile.loop();
  /* add main program code here */

}
