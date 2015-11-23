#include "AMDrive.h"
#include "artofcircuits_l298n.h"   // Drive commands
#include "DEFINITIONS.h"

AMDrive::AMDrive() {
	pinMode(L_enb, OUTPUT);
	pinMode(R_enb, OUTPUT);
	pinMode(Ldir_1, OUTPUT);
	pinMode(Ldir_2, OUTPUT);
	pinMode(Rdir_1, OUTPUT);
	pinMode(Rdir_2, OUTPUT);

	Init();
}

void AMDrive::Init() {
	Left = new L298n(L_enb, Ldir_1, Ldir_2);
	Right = new L298n(R_enb, Rdir_1, Rdir_2);
}

void AMDrive::Drive(String data) {
	String line = Serial1.readStringUntil('\n');           // Input will be "-###,-###,XXX" where ### is drive values and XXX is LCD display text
	if (line != NULL) { // if line isnt null...
		isDriveing = true;
		int comma = line.indexOf(',');                       // comma seperator
		if (comma > 0) {
			int comma2 = line.indexOf(',', comma + 1);

			// First line is nothing to comma will be LEFT_Motors
			int left = line.substring(0, comma).toInt();

			// Second line is comma to comma2/end will be RIGHT_Motors
			int right = comma2>comma ? line.substring(comma + 1, comma2).toInt() : line.substring(comma + 1).toInt();
			if (left > 1) {
				Left->backward(left);  // if LMotor speed is positive turn forward
			}
			else if (left < 1) {
				Left->forward(-1 * left);  // if LMotor speed is Neg, Turn backward
			}
			else {
				Left->breaking();  // If left is changing from front to back Break first
			}
			if (right > 1) {
				Right->backward(right);  // if RMotor speed is positive turn forward
			}
			else if (right < 1) {
				Right->forward(-1 * right);  // if RMotor speed is positive turn backward
			}
			else {
				Right->breaking();  // If right is changing from front to back Break first
			}
		}
	}
	isDriveing = false;
}

void AMDrive::Catch(String data) {
	String line = Serial1.readStringUntil('\n');           // Input will be "-###,-###,XXX" where ### is drive values and XXX is LCD display text
	if (line != NULL) { // if line isnt null...
		isDriveing = true;
		int comma = line.indexOf(',');                       // comma seperator
		if (comma > 0) {
			int comma2 = line.indexOf(',', comma + 1);

			// First line is nothing to comma will be LEFT_Motors
			int left = line.substring(0, comma).toInt();

			// Second line is comma to comma2/end will be RIGHT_Motors
			int right = comma2 > comma ? line.substring(comma + 1, comma2).toInt() : line.substring(comma + 1).toInt();
		
			Ramping(left, right);
		}
	}
}

void AMDrive::GOGO(int data[2]) {
	if (data[1] > 1) {
		Left->backward(data[1]);  // if LMotor speed is positive turn forward
	}
	else if (data[1] < 1) {
		Left->forward(-1 * data[1]);  // if LMotor speed is Neg, Turn backward
	}
	else {
		Left->breaking();  // If left is changing from front to back Break first
	}
	if (data[0] > 1) {
		Right->backward(data[0]);  // if RMotor speed is positive turn forward
	}
	else if (data[0] < 1) {
		Right->forward(-1 * data[0]);  // if RMotor speed is positive turn backward
	}
	else {
		Right->breaking();  // If right is changing from front to back Break first
	}
}

void AMDrive::Ramping(int TargetMotorSpeedL, int TargetMotorSpeedR) {
	
	// Left Side Speed Ramping
	if (RealMotorSpeedL != TargetMotorSpeedL) {
		if (TargetMotorSpeedL > RealMotorSpeedL) {
			RealMotorSpeedL++;
			if (RealMotorSpeedL == 1) {
				RealMotorSpeedL += 100;
			}
			if (RealMotorSpeedL == -100) {
				RealMotorSpeedL += 100;
			}
		}
		if (TargetMotorSpeedL < RealMotorSpeedL) {
			RealMotorSpeedL--;
			if (RealMotorSpeedL == -1) {
				RealMotorSpeedL -= 101;
			}
			if (RealMotorSpeedL == 100) {
				RealMotorSpeedL -= 100;
			}
		}
		if (TargetMotorSpeedL == 0) {
			RealMotorSpeedL = 0;
		}
	}
	// Right Side Motor Ramping
	if (RealMotorSpeedR != TargetMotorSpeedR) {
		if (TargetMotorSpeedR > RealMotorSpeedR) {
			RealMotorSpeedR++;
			if (RealMotorSpeedR == 1) {
				RealMotorSpeedR += 100;
			}
			if (RealMotorSpeedR == -100) {
				RealMotorSpeedR += 100;
			}

		}
		if (TargetMotorSpeedR < RealMotorSpeedR) {
			RealMotorSpeedR--;
			if (RealMotorSpeedR == -1) {
				RealMotorSpeedR -= 101;
			}
			if (RealMotorSpeedR == 100) {
				RealMotorSpeedR -= 100;
			}
		}
		if (TargetMotorSpeedR == 0) {
			RealMotorSpeedR = 0;
		}
	}

	int _Ramp_it[2];
	//final collect and Communicate
	_Ramp_it[1] = (int)RealMotorSpeedL;
	_Ramp_it[0] = (int)RealMotorSpeedR;
	GOGO(_Ramp_it);
}