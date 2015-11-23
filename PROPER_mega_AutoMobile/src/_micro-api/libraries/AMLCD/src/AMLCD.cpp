#include "AMLCD.h"
#include "DEFINITIONS.h"
#include <LiquidCrystal.h>

//LiquidCrystal __am_lcd(LCD_PIN_4, LCD_PIN_6, LCD_PIN_11, LCD_PIN_12, LCD_PIN_13, LCD_PIN_14);

//// Letters
//const byte test[8] = { 0b11111, 0b00000, 0b01010, 0b01011, 0b11011, 0b11011, 0b10001, 0b11111 };
//const byte type_R[8] = { 0b11111, 0b10011, 0b10101, 0b10101, 0b10011, 0b10101, 0b10101, 0b11111 };
//const byte type_U[8] = { 0b11111, 0b10101, 0b10101, 0b10101, 0b10101, 0b10101, 0b10001, 0b11111 };
//const byte type_C[8] = { 0b11111, 0b10001, 0b10111, 0b10111, 0b10111, 0b10111, 0b10001, 0b11111 };
//const byte typeT[8] = { 0b11111, 0b00000, 0b01010, 0b01011, 0b11011, 0b11011, 0b10001, 0b11111 };
//const byte type_K[8] = { 0b11111, 0b10111, 0b10110, 0b10101, 0b10011, 0b10101, 0b10110, 0b11111 };
//const byte type_S[8] = { 0b00000, 0b00110, 0b01001, 0b01000, 0b00110, 0b00001, 0b01001, 0b00110 };
//const byte type_SB[8] = { 0b11111, 0b11001, 0b10110, 0b10111, 0b11001, 0b11110, 0b10110, 0b11001 };
//const byte type_I[8] = { 0b11111, 0b10001, 0b11011, 0b11011, 0b11011, 0b11011, 0b10001, 0b11111 };
//const byte Spacer[8] = { 0b10101, 0b01110, 0b10101, 0b01110, 0b10101, 0b01110, 0b10101, 0b01110 };


void AMLCD::init() {
	//this->_lcd = &__am_lcd;
	_lcd = new LiquidCrystal(LCD_PIN_4, LCD_PIN_6, LCD_PIN_11, LCD_PIN_12, LCD_PIN_13, LCD_PIN_14);
	delay(100);

	//_lcd.createChar(1, typeT); delay(10);
	//_lcd->createChar(12, type_SB); delay(10);
	//__am_lcd.createChar(2, type_R); delay(10);
	//__am_lcd.createChar(4, type_C); delay(10);
	//__am_lcd.createChar(5, type_K); delay(10);
	//__am_lcd.createChar(6, type_U); delay(10);
	//__am_lcd.createChar(99, Spacer); delay(10);
	//__am_lcd.createChar(7, type_SB); delay(10);
	//__am_lcd.createChar(8, type_I); delay(10);
	//__am_lcd.createChar(9, type_S); delay(10);
	//__am_lcd.createChar(1, test); delay(10);
	//const byte Spacer[8] = { 0b10101, 0b01110, 0b10101, 0b01110, 0b10101, 0b01110, 0b10101, 0b01110 };
	//_lcd->createChar(100, Spacer);
	delay(100);

	_lcd->begin(16, 2); // 16 x 2 display
	_TEST();
	delay(100);

	//_lcd->println("LCD READY");

	delay(100);
}


void AMLCD::startupAnimation() {
	// set the cursor to (0,0):
	_lcd->setCursor(0, 0);
	// print from 0 to 9:
	for (int thisChar = 0; thisChar < 10; thisChar++) {
		_lcd->print(thisChar);
		delay(500);
	}

	// set the cursor to (16,1):
	_lcd->setCursor(16, 1);
	// set the display to automatically scroll:
	_lcd->autoscroll();
	// print from 0 to 9:
	for (int thisChar = 0; thisChar < 10; thisChar++) {
		_lcd->print(thisChar);
		delay(500);
	}
	// turn off automatic scrolling
	_lcd->noAutoscroll();

	// clear screen for the next loop:
	_lcd->clear();


	//__am_lcd.clear();

	/*
	delay(275);
	_lcd->setCursor(3, 0);
	_lcd->write(2);
	delay(275);
	_lcd->setCursor(4, 0);
	_lcd->write(6);
	delay(275);
	_lcd->setCursor(5, 0);
	_lcd->write(4);
	delay(275);
	_lcd->setCursor(6, 0);
	_lcd->write(5);
	delay(275);
	_lcd->setCursor(7, 0);
	_lcd->write(6);
	delay(275);
	_lcd->setCursor(8, 0);
	_lcd->write(7);
	delay(275);
	_lcd->setCursor(9, 0);
	_lcd->write(8);
	delay(275);
	_lcd->setCursor(10, 0);
	_lcd->write(7);
	delay(275);
	_lcd->setCursor(11, 0);
	_lcd->write(1);
	//  delay(2500);
	//  _lcd.autoscroll();
	//  // scroll out the top line
	//  for (int thisChar = 0; thisChar <= 15; thisChar++) {
	//    _lcd.write(99);  // 99 is space
	//    delay(175);
	//  }
	//  _lcd.noAutoscroll();
	delay(755);
	*/
}

void AMLCD::_TEST() {
	_lcd->cursor();
	_lcd->setCursor(0, 8);
	_lcd->write("TEST");
	_lcd->scrollDisplayLeft();
	delay(2000);
	_lcd->setCursor(1, 8);
	_lcd->println("TEST");
	_lcd->scrollDisplayLeft();
	delay(2000);
	_lcd->clear();
	_lcd->home();
	_lcd->print("Hello Michael...");
}

void AMLCD::clear() {
	//	_lcd->begin(16, 2);
	_lcd->clear();
	//	_lcd->home();
	//	_lcd->setCursor(1, 1);
	//	_lcd->clear();
}

void AMLCD::print(String text) {
	_lcd->print(text);
}