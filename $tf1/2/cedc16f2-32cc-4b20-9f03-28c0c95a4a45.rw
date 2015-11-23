// --------------------------------- added Libraries
#include "NewPing.h"             // Ultrasonic
#include "EEPROM.h"              // Access to internal memory
#include "Wire.h"                // Comm
#include "I2Cdev.h"              // Comm
#include "SoftwareSerial.h"      // Bluetooth setup
#include "artofcircuits_l298n.h" // Drive commands
#include "MPU6050.h"             // Acel and Gyro Commands
#include "SPI.h"                 // Cant say yet
#include "nRF24L01.h"            // havent looked
#include "RF24.h"                // This is what i think im using for the RF COMM


//// --------------------------------- SETUP devices
//NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
//SoftwareSerial btSerial(RxD, TxD);                  // Bluetooth pins
//L298n dcm1 (L_enb, Ldir_1, Ldir_2);                 // Motor Left
//L298n dcm2 (R_enb, Rdir_1, Rdir_2);                 // Motor Right
//MPU6050 accelgyro;                                  // Accel Controls
//RF24 radio(CE_PIN, CSN_PIN);                        // Create a Radio
//
//// --------------------------------- Void testSR04()
//void testSR04() { // THIS WORKS
//  //  delay(50);                     // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
//  Serial.print("Ping: ");
//  Serial.print(sonar.ping_cm()); // Send ping, get distance in cm and print result (0 = outside set distance range)
//  Serial.println("cm");
//  btSerial.print("Ping: ");
//  btSerial.print(sonar.ping_cm()); // Send ping, get distance in cm and print result (0 = outside set distance range)
//  btSerial.println("cm");
//}
//

//void RFCOMM() {
//  nRF_receive();
//  serial_receive();
//}
//
//void GatherDataString() {
//  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz); // going to be sending Telemetry of the MPU6050
//  int Sonarping = sonar.ping_cm();                    // need that sonar data
//  int DBping = analogRead(A0);                        // This will be the Microphone DB tester
//}
//
//void serialEvent() {
//  while (Serial.available() > 0 ) {
//    char incomingByte = Serial.read();
//
//    if (stringOverflow) {
//      serialBuffer[dataBufferIndex++] = charOverflow;  // Place saved overflow byte into buffer
//      serialBuffer[dataBufferIndex++] = incomingByte;  // saved next byte into next buffer
//      stringOverflow = false;                          // turn overflow flag off
//    }
//    else if (dataBufferIndex > 31) {
//      stringComplete = true;        // Send this buffer out to radio
//      stringOverflow = true;        // trigger the overflow flag
//      charOverflow = incomingByte;  // Saved the overflow byte for next loop
//      dataBufferIndex = 0;          // reset the bufferindex
//      break;
//    }
//    else if (incomingByte == '\n') {
//      serialBuffer[dataBufferIndex] = 0;
//      stringComplete = true;
//    }
//    else {
//      serialBuffer[dataBufferIndex++] = incomingByte;
//      serialBuffer[dataBufferIndex] = 0;
//    }
//  } // end while()
//} // end serialEvent()
//
//void nRF_receive(void) {
//  int len = 0;
//  if ( radio.available() ) {
//    bool done = false;
//    while ( !done ) {
//      len = radio.getDynamicPayloadSize();
//      done = radio.read(&RecvPayload, len);
//      delay(5);
//    }
//    RecvPayload[len] = 0; // null terminate string
//    Serial.print("R:");
//    Serial.print(RecvPayload);
//    Serial.println();
//    RecvPayload[0] = 0;  // Clear the buffers
//  }
//} // end nRF_receive()
//
//void serial_receive(void) {
//
//  if (stringComplete) {
//    strcat(SendPayload, serialBuffer);
//    // swap TX & Rx addr for writing
//    radio.openWritingPipe(pipes[1]);
//    radio.openReadingPipe(0, pipes[0]);
//    radio.stopListening();
//    bool ok = radio.write(&SendPayload, strlen(SendPayload));
//    Serial.print("S:");
//    Serial.print(SendPayload);
//    Serial.println();
//    stringComplete = false;
//    // restore TX & Rx addr for reading
//    radio.openWritingPipe(pipes[0]);
//    radio.openReadingPipe(1, pipes[1]);
//    radio.startListening();
//    SendPayload[0] = 0;
//    dataBufferIndex = 0;
//  } // endif
//} // end serial_receive()
//
//void COMMTEST() {
//  // The payload will always be the same, what will change is how much of it we send.
//  static char send_payload[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ789012";
//
//  // First, stop listening so we can talk.
//  radio.stopListening();
//
//  // Take the time, and send it.  This will block until complete
//  printf("Now sending length %i...", next_payload_size);
//  radio.write( send_payload, next_payload_size );
//
//  // Now, continue listening
//  radio.startListening();
//
//  // Wait here until we get a response, or timeout
//  unsigned long started_waiting_at = millis();
//  bool timeout = false;
//  while ( ! radio.available() && ! timeout )
//    if (millis() - started_waiting_at > 500 )
//      timeout = true;
//
//  // Describe the results
//  if ( timeout )
//  {
//    printf("Failed, response timed out.\n\r");
//  }
//  else
//  {
//    // Grab the response, compare, and send to debugging spew
//    uint8_t len = radio.getDynamicPayloadSize();
//    radio.read( receive_payload, len );
//
//    // Put a zero at the end for easy printing
//    receive_payload[len] = 0;
//
//    // Spew it
//    printf("Got response size=%i value=%s\n\r", len, receive_payload);
//  }
//
//  // Update size for next time.
//  next_payload_size += payload_size_increments_by;
//  if ( next_payload_size > max_payload_size )
//    next_payload_size = min_payload_size;
//
//  // Try again 1s later
//  delay(1000);
//}
//// --------------------------------- Void testdrives()
//void testdrives() { // THIS WORKS
//  dcm1.forward(255);    // cc/forward motion possible value of pwm can be 0 - 255
//  dcm2.forward(255);    // cc/forward motion possible value of pwm can be 0 - 255
//  delay (2000);
//  dcm1.breaking();
//  dcm2.breaking();
//  delay (100);
//  //dcm1.disable();
//  dcm1.backward(170);    // ccw/backward motion possible value of pwm can be 0 - 255
//  dcm2.backward(170);    // ccw/backward motion possible value of pwm can be 0 - 255
//  delay (2000);
//  dcm1.breaking();
//  dcm2.breaking();
//  delay (100);
//}
//
//// THIS IS IT!!! the MAIN LINE... Unity Communicato! The send out and the recieve in the same command.
//void BTcommunicate() {
//
//  //This is communcations out.
//
//  // Gather the data... all of it...
//  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz); // going to be sending Telemetry of the MPU6050
//  int Sonarping = sonar.ping_cm();                    // need that sonar data
//  // int DBping = AnalogRead(A0);                       // This will be the Microphone DB tester
//
//  // SENDER OF DATA...
//  btSerial.print("");
//  btSerial.print(ax);            // Unity data [0]
//  btSerial.print(", ");
//  btSerial.print(ay);            // Unity data [1]
//  btSerial.print(", ");
//  btSerial.print(az);            // Unity data [2]
//  btSerial.print(", ");
//  btSerial.print(gx);            // Unity data [3]
//  btSerial.print(", ");
//  btSerial.print(gy);            // Unity data [4]
//  btSerial.print(", ");
//  btSerial.print(gz);            // Unity data [5]
//  btSerial.print(", ");
//  btSerial.println(Sonarping);   // Unity data [6] Sonar ping
//  //Serial.println(F("Sent data bluetooth"));  // DEBUG Sent data
//
//  BTblinkState = !BTblinkState;    // Acitivity Monitor on the output
//  digitalWrite(y_led, BTblinkState);
//
//  // RECIEVER OF DATA...
//  if (btSerial.available() > 0) {
//    String line = btSerial.readStringUntil('\n');  // Input will be "-###,-###"
//    String testStr = "";
//    //Serial.print(F("Recieved data on Bluetooth: "));
//
//    if (line != NULL) { // if line isnt null...
//      blinkState = !blinkState;    // Acitivity Monitor on the input
//      digitalWrite(b_led, blinkState);
//
//      int btcomma = line.indexOf(','); // comma seperator
//      if (btcomma > 0) {
//        int left = line.substring(0, btcomma).toInt();  // First line is nothing to comma will be LEFT_Motors
//        int right = line.substring(btcomma + 1).toInt(); // Second line is comma to the end will be RIGHT_Motors
//        testStr = left + (" : ") + right;  // this is Return the passed speeds back to unity for debug
//        Serial.println(testStr);
//
//        if (left > 1) {               // if Motor speed is positive turn forward
//          //    dcm1.forward (left);
//          dcm1.backward(left);
//        }
//        else if (left < 1) {          // if Motor speed is Neg, Turn backward
//          dcm1.forward(-1 * left);
//          //dcm1.backward(-1*left);
//        }
//        else {
//          dcm1.breaking ();
//        }
//        if (right > 1) {              // if Motor speed is positive turn forward
//          dcm2.backward (right);
//        }
//        else if (right < 1) {         // if Motor speed is Neg, Turn backward
//          dcm2.forward(-1 * right);
//        }
//        else {                        // else ... ya know... stop
//          dcm2.breaking ();
//        }
//      }
//    }
//  }
//  delay(TXMIT_DELAY);
//}
//
//// --------------------------------- Void BTrecieve()
//void BTrecieve() {
//  if (btSerial.isListening()) {
//    Serial.println("BTSerial is listening!");
//  } else {
//    Serial.println("BTSerial is not listening!");
//  }
//  if (btSerial.available() > 0) {
//    blinkState = !blinkState;    // Acitivity Monitor on the input
//    digitalWrite(b_led, blinkState);
//    Serial.println("HIT !!!!!!");
//  }
//  delay(105);
//}
//
//// ------------- FOR BLUETOOTH -----------------------------------
//// THIS IS IT!!! the MAIN LINE... Unity Communicato! The send out and the recieve in the same command.
//void communicate() {
//  //This is communcations out.
//
//  // Gather the data... all of it...
//  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz); // going to be sending Telemetry of the MPU6050
//  int Sonarping = sonar.ping_cm();                    // need that sonar data
//  // int DBping = AnalogRead(A0);                       // This will be the Microphone DB tester
//
//  // SENDER OF DATA...
//  Serial.print("");
//  Serial.print(ax);            // Unity data [0]
//  Serial.print(", ");
//  Serial.print(ay);            // Unity data [1]
//  Serial.print(", ");
//  Serial.print(az);            // Unity data [2]
//  Serial.print(", ");
//  Serial.print(gx);            // Unity data [3]
//  Serial.print(", ");
//  Serial.print(gy);            // Unity data [4]
//  Serial.print(", ");
//  Serial.print(gz);            // Unity data [5]
//  Serial.print(", ");
//  Serial.println(Sonarping);   // Unity data [6] Sonar ping
//
//
//  blinkState = !blinkState;    // Acitivity Monitor on the output
//  digitalWrite(b_led, blinkState);
//
//  // RECIVER OF DATA...
//  if (Serial.available() > 0) {
//    String line = Serial.readStringUntil('\n');  // Input will be "-###,-###"
//
//    if (line != NULL) { // if line isnt null...
//      int comma = line.indexOf(','); // comma seperator
//      if (comma > 0) {
//        int left = line.substring(0, comma).toInt();  // First line is nothing to comma will be LEFT_Motors
//        int right = line.substring(comma + 1).toInt(); // Second line is comma to the end will be RIGHT_Motors
//        //testStr = new String("")+left+new String(" / ")+right;  // this is Return the passed speeds back to unity for debug
//
//        if (left > 1) {               // if Motor speed is positive turn forward
//          //    dcm1.forward (left);
//          dcm1.backward(left);
//        }
//        else if (left < 1) {          // if Motor speed is Neg, Turn backward
//          dcm1.forward(-1 * left);
//          //dcm1.backward(-1*left);
//        }
//        else {
//          dcm1.breaking ();
//        }
//        if (right > 1) {              // if Motor speed is positive turn forward
//          dcm2.backward (right);
//        }
//        else if (right < 1) {         // if Motor speed is positive turn forward
//          dcm2.forward(-1 * right);
//        }
//        else {                        // else ... ya know... stop
//          dcm2.breaking ();
//        }
//      }
//    }
//  }
//  delay(TXMIT_DELAY);
//}
//
//// --------------------------------- Void testMPU()
//void testMPU() { // THiS WORKS
//  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
//  Serial.print("");
//  Serial.print(ax);
//  Serial.print(", ");
//  Serial.print(ay);
//  Serial.print(", ");
//  Serial.print(az);
//  Serial.print(", ");
//  Serial.print(gx);
//  Serial.print(", ");
//  Serial.print(gy);
//  Serial.print(", ");
//  Serial.print(gz);
//  Serial.print(", ");
//  Serial.println(sonar.ping_cm());
//  blinkState = !blinkState;
//  digitalWrite(b_led, blinkState);
//  delay(45);
//}
//
//// --------------------------------- Void testavoidance()
//void testavoidance() {  // this is not turning and backing up as expected
//  int p = sonar.ping_cm();       // the ping command is now p
//  unsigned long currentMillis = millis(); // the time is now set
//  if (p < 50) {
//    dcm1.breaking();
//    dcm2.breaking();
//    if (currentMillis - previousMillis > backtime) {
//      previousMillis = currentMillis;
//      dcm1.forward(100);
//      dcm2.forward(100);
//    }
//    currentMillis = millis();   // reset the timer
//    if (currentMillis - previousMillis > turntime) {
//      if (turnstate == true) {
//        previousMillis = currentMillis;
//        dcm1.forward(150);
//        dcm2.backward(150);
//        turnstate = false;
//      }
//      else {
//        previousMillis = currentMillis;
//        dcm1.backward(150);
//        dcm2.forward(150);
//        turnstate = true;
//      }
//    }
//    else {
//      dcm1.forward(150);
//      dcm2.backward(150);
//    }
//  }
//  if (p > 51 && p < 120) {
//    dcm1.backward(150);
//    dcm2.backward(150);
//  }
//  if (p > 121) {
//    dcm1.backward(250);
//    dcm2.backward(250);
//  }
//}
//
//// --------------------------------- Void decider()
//void decider() { // this did not do what was expected... but thats cool too
//  if (Serial.available() > 0) {
//    int inByte = Serial.read();
//    switch (inByte) {
//      case '1':
//        Millistimer();
//        break;
//      case '2':
//        testavoidance();
//        break;
//      case '3':
//        testSR04();
//        break;
//      case '4':
//        testdrives();
//        break;
//      case '5':
//        testMPU();
//        break;
//      default:
//        // turn all the LEDs off:
//        for (int thisPin = 2; thisPin < 7; thisPin++) {
//          digitalWrite(thisPin, LOW);
//        }
//    }
//  }
//}
//
//// --------------------------------- Void decider2()
//void decider2() { // AMAZINGLY this fucking WORKS!... lots of crashes...
//  // read the sensor:
//  int sensorReading = sonar.ping_cm();
//  // map the sensor range to a range of four options:
//  int range = map(sensorReading, sensorMin, sensorMax, 0, 4);
//
//  // do something different depending on the
//  // range value:
//  switch (range) {
//    case 0:    // your hand is on the sensor
//      Serial.println("Wide Open");
//      digitalWrite(b_led, turnstate);
//      dcm1.forward(200);
//      dcm2.forward(200);
//      turnstate = !turnstate;
//      break;
//    case 1:    // your hand is close to the sensor
//      Serial.println("close");
//      //    if (turnstate == true) {
//      //      digitalWrite(y_led, HIGH);
//      //      digitalWrite(b_led, LOW);
//      //      dcm1.backward(225);
//      //      dcm2.forward(225);
//      //    }
//      //    else {
//      //      digitalWrite(y_led, LOW);
//      //      digitalWrite(b_led, HIGH);
//      //      dcm1.forward(225);
//      //      dcm2.backward(225);
//      //    }
//      dcm1.forward(225);
//      dcm2.backward(225);
//      break;
//    case 2:    // your hand is a few inches from the sensor
//      Serial.println("getting close");
//      dcm1.backward(200);
//      dcm2.backward(200);
//      break;
//    case 3:    // your hand is nowhere near the sensor
//      Serial.println("clear path");
//      dcm1.backward(255);
//      dcm2.backward(255);
//      break;
//  }
//  delay(45);        // delay in between reads for stability
//}
//
//// --------------------------------- Void Millistimer()
//void Millistimer() {  // this did as expected but its hard to translate to other things
//  unsigned long currentMillis = millis();
//
//  if (currentMillis - previousMillis > interval) {
//    // save the last time you blinked the LED
//    previousMillis = currentMillis;
//
//    // if the LED is off turn it on and vice-versa:
//    if (ledState == LOW)
//      ledState = HIGH;
//    else
//      ledState = LOW;
//
//    // set the LED with the ledState of the variable:
//    digitalWrite(b_led, ledState);
//  }
//}

// --------------------------------- Void testrun1
//void testrun1() { // This dodges Nothing... but responds... does sucks
//  int p = sonar.ping_cm();       // the ping command is now p
//  bool turnstate = true;         // true turns one way false the other
//  long toss = random(250, 1000); // turn increments for a look around
//  long PrevTime = 0;
//  long Time = 0;
//  unsigned long CurTime = millis();
//  if (p < 10) {
//    dcm1.breaking();
//    dcm2.breaking();
//    if (metro0.check() == 1) { // check if the metro has passed its interval .
//      if (state0==HIGH)  {
//        state0=LOW;
//      }
//      else {
//        state0=HIGH;
//      }
//      dcm1.forward(155);
//      dcm2.backward(155);
//      //turnstate = !turnstate;
//    }
//    else {
//      //unsigned long CurTime = millis();
//      //while (CurTime - PrevTime <= toss)
//      dcm1.backward(155);
//      dcm2.forward(155);
//      //turnstate = !turnstate;
//    }
//    turnstate = !turnstate;
//  }
//  else if (p > 10 && p < 50) {
//    dcm1.backward(100);
//    dcm2.backward(100);
//  }
//  else {
//    dcm1.backward(150);
//    dcm2.backward(150);
//  }
//
//}

// THIS is the Obsticle avoidance Program... as copied from Miguel Grinberg
// hopefully this can be moved to its own file for clutter reasons

// --------------------------------- Void Ruckusavoid()
//namespace Ruckusavoid {
//	class Ruckus {
//	public:
//		Ruckus()
//				: leftMotor(LEFT_MOTOR_INIT), rightMotor(RIGHT_MOTOR_INIT),
//	              distanceSensor(DISTANCE_SENSOR_INIT),
//	              distanceAverage(TOO_CLOSE * 10)
//	{
//			Gogo();
//	}
//
//	};
//}
