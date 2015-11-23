//#include "NewPing.h"             // Ultrasonic
//#include "EEPROM.h"              // Access to internal memory
//#include "Wire.h"                // Comm
//#include "I2Cdev.h"              // Comm
//#include "SoftwareSerial.h"      // Bluetooth setup
//#include "artofcircuits_l298n.h" // Drive commands
//#include "MPU6050.h"             // Acel and Gyro Commands
//#include "SPI.h"                 // Cant say yet
//#include "nRF24L01.h"            // havent looked
//#include "RF24.h"                // This is what i think im using for the RF COMM
////#include "tests.h"               // Backup of Clutter from old function tests... DO NOT DELETE
//
//RF24 radio(CE_PIN, CSN_PIN);                        // Create a Radio
//int joystick[5];  // 2 element array holding Joystick readings
////CHAT ELEMENTS
//boolean stringComplete = false;  // whether the string is complete
//static int dataBufferIndex = 0;
//boolean stringOverflow = false;
//char charOverflow = 0;
//
//char SendPayload[31] = "";
//char RecvPayload[31] = "";
//char serialBuffer[31] = "";
//
//void testRF() {
//  joystick[0] = analogRead(JOYSTICK_X);
//  joystick[1] = analogRead(JOYSTICK_Y);
//  joystick[2] = 8;
//  joystick[3] = 7;
//  joystick[4] = 42;
//
//  radio.write( joystick, sizeof(joystick) );
//  blinkState = !blinkState;
//  digitalWrite(b_led, blinkState);
//  delay(10);
//  if ( radio.available() ) {
//    blinkState = !blinkState;
//    digitalWrite(y_led, blinkState);
//  }
//}
//
//void RFCOMM() {
//  nRF_receive();
//  serial_receive();
//}
//
//void GatherDataString() {
// accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz); // going to be sending Telemetry of the MPU6050
// int Sonarping = sonar.ping_cm();                    // need that sonar data
// int DBping = AnalogRead(A0);                        // This will be the Microphone DB tester 
//}
//
//void serialEvent() {
//  while (Serial.available() > 0 ) {
//      char incomingByte = Serial.read();
//     
//      if (stringOverflow) {
//         serialBuffer[dataBufferIndex++] = charOverflow;  // Place saved overflow byte into buffer
//         serialBuffer[dataBufferIndex++] = incomingByte;  // saved next byte into next buffer
//         stringOverflow = false;                          // turn overflow flag off
//      } 
//      else if (dataBufferIndex > 31) {
//         stringComplete = true;        // Send this buffer out to radio
//         stringOverflow = true;        // trigger the overflow flag
//         charOverflow = incomingByte;  // Saved the overflow byte for next loop
//         dataBufferIndex = 0;          // reset the bufferindex
//         break;
//      }
//      else if(incomingByte=='\n'){
//          serialBuffer[dataBufferIndex] = 0;
//          stringComplete = true;
//      } 
//      else {
//          serialBuffer[dataBufferIndex++] = incomingByte;
//          serialBuffer[dataBufferIndex] = 0;
//      }         
//  } // end while()
//} // end serialEvent()
//
//void nRF_receive(void) {
//  int len = 0;
//  if ( radio.available() ) {
//      bool done = false;
//      while ( !done ) {
//        len = radio.getDynamicPayloadSize();
//        done = radio.read(&RecvPayload,len);
//        delay(5);  }
//    RecvPayload[len] = 0; // null terminate string
//    Serial.print("R:");
//    Serial.print(RecvPayload);
//    Serial.println();
//    RecvPayload[0] = 0;  // Clear the buffers
//  } 
//} // end nRF_receive()
//
//void serial_receive(void){
// 
//  if (stringComplete) {
//        strcat(SendPayload,serialBuffer);     
//        // swap TX & Rx addr for writing
//        radio.openWritingPipe(pipes[1]);
//        radio.openReadingPipe(0,pipes[0]); 
//        radio.stopListening();
//        bool ok = radio.write(&SendPayload,strlen(SendPayload));
//        Serial.print("S:");
//        Serial.print(SendPayload);         
//        Serial.println();
//        stringComplete = false;
//        // restore TX & Rx addr for reading       
//        radio.openWritingPipe(pipes[0]);
//        radio.openReadingPipe(1,pipes[1]);
//        radio.startListening(); 
//        SendPayload[0] = 0;
//        dataBufferIndex = 0;
//  } // endif
//} // end serial_receive() 
