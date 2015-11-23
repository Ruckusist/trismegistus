// AMDrive.h

#ifndef _AMDRIVE_h
#define _AMDRIVE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include "artofcircuits_l298n.h"   // Drive commands

class AMDrive {

public:
	AMDrive(); 
	void Drive(String data);
	void Catch(String data);

protected:
	void Init();
	void GOGO(int data[2]);
	void Ramping(int TargetMotorSpeedL, int TargetMotorSpeedR);
	L298n *Left;                  // Motor Left
	L298n *Right;                  // Motor Right
	int counter1;
	int counter2;
	int RealMotorSpeedL;
	int RealMotorSpeedR;
	bool isDriveing;
	

};
#endif

