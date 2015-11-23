// AMLCD.h
#ifndef _AMLCD_h
#define _AMLCD_h
#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include <LiquidCrystal.h>
class AMLCD {

protected:
	
	String lcdMsg = "";   // for the second row (incoming message)
						  // Letters
	const byte test[8] = { 0b11111, 0b00000, 0b01010, 0b01011, 0b11011, 0b11011, 0b10001, 0b11111 };
	const byte type_R[8] = { 0b11111, 0b10011, 0b10101, 0b10101, 0b10011, 0b10101, 0b10101, 0b11111 };
	const byte type_U[8] = { 0b11111, 0b10101, 0b10101, 0b10101, 0b10101, 0b10101, 0b10001, 0b11111 };
	const byte type_C[8] = { 0b11111, 0b10001, 0b10111, 0b10111, 0b10111, 0b10111, 0b10001, 0b11111 };
	const byte typeT[8] = { 0b11111, 0b00000, 0b01010, 0b01011, 0b11011, 0b11011, 0b10001, 0b11111 };
	const byte type_K[8] = { 0b11111, 0b10111, 0b10110, 0b10101, 0b10011, 0b10101, 0b10110, 0b11111 };
	const byte type_S[8] = { 0b00000, 0b00110, 0b01001, 0b01000, 0b00110, 0b00001, 0b01001, 0b00110 };
	const byte type_SB[8] = { 0b11111, 0b11001, 0b10110, 0b10111, 0b11001, 0b11110, 0b10110, 0b11001 };
	const byte type_I[8] = { 0b11111, 0b10001, 0b11011, 0b11011, 0b11011, 0b11011, 0b10001, 0b11111 };
	//const byte Spacer[8] = { 0b10101, 0b01110, 0b10101, 0b01110, 0b10101, 0b01110, 0b10101, 0b01110 };
public:
	LiquidCrystal *_lcd = NULL;
	void init();
	void startupAnimation();
	void _TEST();
	void clear();
	void print(String text);
};
#endif