
#include "LiquidCrystal.h"
#include "customChar.h"


bool WifiOn;
bool bluetoothOn;

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() {
	// initialize LCD and set up the number of columns and rows: 
	lcd.begin(16, 2);

	lcd.createChar(11, test);
	lcd.createChar(12, type_SB);
	lcd.createChar(2, type_R);
	lcd.createChar(3, type_U);
	lcd.createChar(4, type_C);
	lcd.createChar(5, type_K);
	lcd.createChar(6, type_U);

	lcd.createChar(99, Spacer);
	lcd.createChar(81, wifiOnL);
	lcd.createChar(82, wifiOnR);
	lcd.createChar(83, wifiDisL);
	lcd.createChar(84, wifiDisR);
	lcd.createChar(85, bluetooth_on);
	lcd.createChar(86, bluetooth_off);

	lcd.createChar(7, type_SB);
	lcd.createChar(8, type_I);
	lcd.createChar(9, type_S);
	lcd.createChar(1, test);
	delay(2500);
	lcd.setCursor(2, 0);
	StartupAnimaitonLCD();
}

//void loop() {
//  //lcd.autoscroll();
//  
//  lcd.setCursor(4, 0);
//  // draw the little man, arms down:
//  lcd.write(11);
//  lcd.write(12);
//  lcd.write(13);
//  lcd.write(14);
//  lcd.write(15);
//  delay(1000);
//  lcd.autoscroll();
//  delay(100);
//  lcd.autoscroll();
//}
void loop() {
	// set the cursor to (0,0):
	//  lcd.setCursor(0, 0);
	//  // print from 0 to 9:
	//  for (int thisChar = 1; thisChar < 9; thisChar++) {
	//    lcd.print(thisChar);
	//    delay(500);
	//  }
	//  // set the cursor to (16,1):
	//  lcd.setCursor(16, 1);
	//  // set the display to automatically scroll:
	//  lcd.autoscroll();
	//  // print from 0 to 9:
	//  for (int thisChar = 1; thisChar <= 9; thisChar++) {
	//    lcd.write(thisChar);
	//    delay(500);
	//  }
	//  // turn off automatic scrolling
	//  lcd.noAutoscroll();
	//  delay(5000);
	//  // clear screen for the next loop:
	//  lcd.clear();
}

void StartupAnimaitonLCD() {
	delay(275);
	lcd.setCursor(3, 0);
	lcd.write(2);
	delay(275);
	lcd.setCursor(4, 0);
	lcd.write(3);
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
	delay(275);
	lcd.autoscroll();
	// scroll out the top line
	for (int thisChar = 0; thisChar <= 8; thisChar++) {
		//lcd.setCursor(thisChar, 0);
		lcd.write(99);  // 99 is space 
		delay(175);
	}
	// run bottom line program 
	// print from 0 to 9:
	//  for (int thisChar = 2; thisChar <= 10; thisChar++) {
	//    //lcd.setCursor(thisChar, 0);
	//    lcd.write(thisChar); 
	//    delay(175);
	//  }
	delay(2500);
}

void LCD_Main() {
	// Top Line will be for text and existing input

	// Bottom Line |-16 digit-| should be |wwSbbSmode#ST###|

	// first 2 will be wifi on or off
	lcd.setCursor(0, 1); // element zero line 1 is second line

	if (WifiOn = true) {
		lcd.setCursor(0, 1);
		lcd.write(81);
		lcd.setCursor(1, 1);
		lcd.write(82);
	}
	else {
		lcd.setCursor(0, 1);
		lcd.write(83);
		lcd.setCursor(1, 1);
		lcd.write(84);
	}
	// Spacer
	lcd.setCursor(2, 1);
	lcd.write(99);
	// Elements 3
	if (bluetoothOn = true) {
		lcd.setCursor(3, 1);
		lcd.write(85);
	}
	else {
		lcd.setCursor(3, 1);
		lcd.write(86);
	}
	// Spacer
	lcd.setCursor(4, 1);
	lcd.write(99);
	// Elements 4
	lcd.setCursor(5, 1);
	lcd.write(82);
}









/* --- List Of Custom Symobols for the Screen --- */
#include "arduino.h"
#include "LiquidCrystal.h"

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

// Symbols
byte singleLineArrow1[8] = { 0b00100, 0b10110, 0b01011, 0b01101, 0b01101, 0b01011, 0b10110, 0b00100 };
byte singleLineArrow2[8] = { 0b11011, 0b01101, 0b10110, 0b10010, 0b10010, 0b10110, 0b01101, 0b11011 };
byte wifiDisL[8] = { 0b10000, 0b10100, 0b10010, 0b10001, 0b10010, 0b10100, 0b10000, 0b11111 };
byte wifiDisR[8] = { 0b00001, 0b01001, 0b10001, 0b00001, 0b10001, 0b01001, 0b00001, 0b11111 };
byte wifiOnL[8] = { 0b00000, 0b10010, 0b10001, 0b10010, 0b11011, 0b10111, 0b00000, 0b11111 };
byte wifiOnR[8] = { 0b00001, 0b11001, 0b10101, 0b11001, 0b10101, 0b10101, 0b00001, 0b11111 };
byte bluetooth_off[8] = { 0b00110, 0b00101, 0b10110, 0b01100, 0b10110, 0b00101, 0b00110, 0b00100 };
byte bluetooth_on[8] = { 0b11001, 0b11010, 0b01001, 0b10011, 0b01001, 0b11010, 0b11001, 0b11011 };
byte WaveUp[8] = { 0b10100, 0b11110, 0b10100, 0b01110, 0b00101, 0b00100, 0b01110, 0b01010 };
byte WaveDn[8] = { 0b00100, 0b01110, 0b00100, 0b01110, 0b10101, 0b00100, 0b01110, 0b01010 };
byte Spacer[8] = { 0b10101, 0b01110, 0b10101, 0b01110, 0b10101, 0b01110, 0b10101, 0b01110 };







