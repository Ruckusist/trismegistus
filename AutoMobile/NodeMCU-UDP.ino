/*
-----------Ruckusist - NodeMCU 2015-------------

NodeMCU is the arduino knock off with built in Wifi capabilities, its job it to pass data
back and for from Unity to the Arduino Platform. Its a pretty cool system with another
6 digital I/O ports left open, A lot more could be done here. Unity could do call back commands
to this board and use it to fire other functionality.

2015 AutoMobile -- "Kit" Project --

****11-3-15: New functionality added. The Speaker playing out tones from a library we had to build from
example code online. SD card reader is now functional but no file system or functionality has been
added because of it, but its there and hooked up. Because of the SD card and such all the pinouts have
been changed... and this update set will include all the new pinouts and most of the functionality.
but there is a lot that can still be done with both the speaker and the SD card.
Everything in the bluetooth commstack seems to work well... ive just now switch the UDP back to the
home settings... so hopefully that works again.
****

its possible that this is the wrong copy of the file because there is no Reciever on the UDP...
could be my fault...

okay i found it... it will be in the changesets now... so ... i dont know how good that is...
FILE UPDATED : 9-13-15
*/
//
//// --------------------------------- added Libraries
//#include "ESP8266WiFi.h"           // Replaces Stock wifi with module features
//#include "ESP8266WiFiMulti.h"      // Creates the AutoConnect list
//#include "ESP8266mDNS.h"           // This is for the DNS 
//#include "WiFiClient.h"            // this is the Stock Wifi Client with Refs on the Arduino site
//#include "ESP8266WebServer.h"      // THis will run the web server starter
//#include "SPI.h"                   // SPI is a comm like Wire
//#include "SD.h"                    // SD card library *STOCK* im sure there has to be libraries with more stuff
//#include "EEPROM.h"                // Access to internal memory
//#include "Wire.h"                  // Comm for condensed comms between sensors
//#include "WiFiUdp.h"               // Wifi Transmit fuctionality
//#include "pitches.h"               // predefined pitches for speaker
//#include "mario.h"                 // Mario theme
//#include "jamesbond.h"             // Bond theme
//
//
////DEFINE NAME      PINHOLE
//// --------------------------------- PINOUTs
////static const uint8_t D0 = 16;             // Pinouts for MCU translated to proper Arduino index...
//static const uint8_t ledG = 5;
//static const uint8_t ledR = 4;
////static const uint8_t D3 = 0;
//static const uint8_t SD_PIN = 2;                // Cable Select for SD... could put more SD cards in on more pins then keep the same SPI bus...
////static const uint8_t D5 = 14;             // SPI bus SCK
////static const uint8_t D6 = 12;             // SPI bus MISO
////static const uint8_t D7 = 13;             // SPI bus MOSI
//static const uint8_t SPEAKER_PIN = 15;      // Speaker Pin
////static const uint8_t D9 = 3;
////static const uint8_t D10 = 1;
//
//// --------------------------------- -- VARIABLES FOR ALL THINGS!
//// This is for the WebServer Example
//#define DBG_OUTPUT_PORT Serial
//
//bool blinkStateG = false;
//bool blinkStateR = false;
//static bool hasSD = false;
//bool WS_CONNECTED;
//
//// --------------------------------- -- SDCARD SETUP --
//Sd2Card card;
//SdVolume volume;
//SdFile root;
//
//// --------------------------------- -- Serial (arduino) --
//
//const unsigned int SERIAL_BAUD = 115200;
//const unsigned int ARDUINO_STARTUP_DELAY = 250;  // Extra time to wait for Arduino to fully boot before we send LCD text
//
////-------------------------------- -- Wifi (unity) --
//
//int status = WL_IDLE_STATUS;
//unsigned int remotePort = 9977;       //   Remote Send UDP Port
//unsigned int localPort = 7799;        //   Local Listening UDP Port
//#define WEB_PORT 80
//ESP8266WiFiMulti wifiMulti;           //   Autoconnect for Wifi to known addresses... using addAP
//WiFiUDP udp;
//
////------------------------------------ Web Server
//ESP8266WebServer server(WEB_PORT);
//File uploadFile;
//
////char ssid[] = "yourNetwork";                   //  your network SSID (name)
////char pass[] = "secretPassword";                //  your network password (use for WPA, or use as key for WEP)
//
//IPAddress remoteAddr(192, 168, 2, 59);           //  address of the server (Unity)     (HOME)
////IPAddress remoteAddr(10, 9, 0, 107);           //  address of the server (Unity)     (MTSPACE)
//
//// Wifi credentials
//#define WIFI_CONNECT_TIMEOUT 10000
//String ssid = "";
//String pass = "";
//String ssid_Home = "Ruckus";
//String pass_Home = "eric2344";
//String ssid_MTS = "MTSpace";
//String pass_MTS = "makeitso";
//const char* host = "esp8266sd";
////-------------------------------- SD WEB SERVER SETTINGS
//void returnOK() {
//  server.sendHeader("Connection", "close");
//  server.sendHeader("Access-Control-Allow-Origin", "*");
//  server.send(200, "text/plain", "");
//}
//
//void returnFail(String msg) {
//  server.sendHeader("Connection", "close");
//  server.sendHeader("Access-Control-Allow-Origin", "*");
//  server.send(500, "text/plain", msg + "\r\n");
//}
//
//bool loadFromSdCard(String path) {
//  String dataType = "text/plain";
//  if (path.endsWith("/")) path += "index.htm";
//
//  if (path.endsWith(".src")) path = path.substring(0, path.lastIndexOf("."));
//  else if (path.endsWith(".htm")) dataType = "text/html";
//  else if (path.endsWith(".css")) dataType = "text/css";
//  else if (path.endsWith(".js")) dataType = "application/javascript";
//  else if (path.endsWith(".png")) dataType = "image/png";
//  else if (path.endsWith(".gif")) dataType = "image/gif";
//  else if (path.endsWith(".jpg")) dataType = "image/jpeg";
//  else if (path.endsWith(".ico")) dataType = "image/x-icon";
//  else if (path.endsWith(".xml")) dataType = "text/xml";
//  else if (path.endsWith(".pdf")) dataType = "application/pdf";
//  else if (path.endsWith(".zip")) dataType = "application/zip";
//
//  Serial.println(String("loading from SD: ")+path);
//
//  File dataFile = SD.open(path.c_str());
//  if (dataFile.isDirectory()) {
//    path += "/index.htm";
//    dataType = "text/html";
//    dataFile = SD.open(path.c_str());
//  }
//
//  if (!dataFile)
//    return false;
//
//  if (server.hasArg("download")) dataType = "application/octet-stream";
//
//  if (server.streamFile(dataFile, dataType) != dataFile.size()) {
//    DBG_OUTPUT_PORT.println("Sent less data than expected!");
//  }
//
//  dataFile.close();
//  return true;
//}
//
//void handleFileUpload() {
//  if (server.uri() != "/edit") return;
//  HTTPUpload& upload = server.upload();
//  if (upload.status == UPLOAD_FILE_START) {
//    if (SD.exists((char *)upload.filename.c_str())) SD.remove((char *)upload.filename.c_str());
//    uploadFile = SD.open(upload.filename.c_str(), FILE_WRITE);
//    DBG_OUTPUT_PORT.print("Upload: START, filename: "); DBG_OUTPUT_PORT.println(upload.filename);
//  } else if (upload.status == UPLOAD_FILE_WRITE) {
//    if (uploadFile) uploadFile.write(upload.buf, upload.currentSize);
//    DBG_OUTPUT_PORT.print("Upload: WRITE, Bytes: "); DBG_OUTPUT_PORT.println(upload.currentSize);
//  } else if (upload.status == UPLOAD_FILE_END) {
//    if (uploadFile) uploadFile.close();
//    DBG_OUTPUT_PORT.print("Upload: END, Size: "); DBG_OUTPUT_PORT.println(upload.totalSize);
//  }
//}
//
//void deleteRecursive(String path) {
//  File file = SD.open((char *)path.c_str());
//  if (!file.isDirectory()) {
//    file.close();
//    SD.remove((char *)path.c_str());
//    return;
//  }
//
//  file.rewindDirectory();
//  while (true) {
//    File entry = file.openNextFile();
//    if (!entry) break;
//    String entryPath = path + "/" + entry.name();
//    if (entry.isDirectory()) {
//      entry.close();
//      deleteRecursive(entryPath);
//    } else {
//      entry.close();
//      SD.remove((char *)entryPath.c_str());
//    }
//    yield();
//  }
//
//  SD.rmdir((char *)path.c_str());
//  file.close();
//}
//
//void handleDelete() {
//  if (server.args() == 0) return returnFail("BAD ARGS");
//  String path = server.arg(0);
//  if (path == "/" || !SD.exists((char *)path.c_str())) {
//    returnFail("BAD PATH");
//    return;
//  }
//  deleteRecursive(path);
//  returnOK();
//}
//
//void handleCreate() {
//  if (server.args() == 0) return returnFail("BAD ARGS");
//  String path = server.arg(0);
//  if (path == "/" || SD.exists((char *)path.c_str())) {
//    returnFail("BAD PATH");
//    return;
//  }
//
//  if (path.indexOf('.') > 0) {
//    File file = SD.open((char *)path.c_str(), FILE_WRITE);
//    if (file) {
//      file.write((const char *)0);
//      file.close();
//    }
//  } else {
//    SD.mkdir((char *)path.c_str());
//  }
//  returnOK();
//}
//
//void printDirectory() {
//  if (!server.hasArg("dir")) return returnFail("BAD ARGS");
//  String path = server.arg("dir");
//  if (path != "/" && !SD.exists((char *)path.c_str())) return returnFail("BAD PATH");
//  File dir = SD.open((char *)path.c_str());
//  path = String();
//  if (!dir.isDirectory()) {
//    dir.close();
//    return returnFail("NOT DIR");
//  }
//  dir.rewindDirectory();
//  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
//  server.send(200, "text/json", "");
//  WiFiClient client = server.client();
//
//  server.sendContent("[");
//  for (int cnt = 0; true; ++cnt) {
//    File entry = dir.openNextFile();
//    if (!entry)
//      break;
//
//    String output;
//    if (cnt > 0)
//      output = ',';
//
//    output += "{\"type\":\"";
//    output += (entry.isDirectory()) ? "dir" : "file";
//    output += "\",\"name\":\"";
//    output += entry.name();
//    output += "\"";
//    output += "}";
//    server.sendContent(output);
//    entry.close();
//  }
//  server.sendContent("]");
//  dir.close();
//}
//
//void handleNotFound() {
//  if (hasSD && loadFromSdCard(server.uri())) return;
//  String message = "SDCARD Not Detected or FILE NOT FOUND\n\n";
//  message += "URI: ";
//  message += server.uri();
//  message += "\nMethod: ";
//  message += (server.method() == HTTP_GET) ? "GET" : "POST";
//  message += "\nArguments: ";
//  message += server.args();
//  message += "\n";
//  for (uint8_t i = 0; i < server.args(); i++) {
//    message += " NAME:" + server.argName(i) + "\n VALUE:" + server.arg(i) + "\n";
//  }
//  server.send(404, "text/plain", message);
//  DBG_OUTPUT_PORT.print(message);
//}
////-------------------------------- SD WEB SERVER SETTINGS ** END
//
///**************************
//
//SETUP
//
//***************************/
//void setup() {
//
//  //DEFINE PINMODES FOR ALL PINS
//  pinMode(ledG, OUTPUT);
//  pinMode(ledR, OUTPUT);
//  pinMode(SPEAKER_PIN, OUTPUT);
//
//
//  // --------------------------------- START UP INDICATORS
//  digitalWrite(ledG, HIGH);
//  digitalWrite(ledR, HIGH);
//
//  //  buzz(5000, 3000);
//  beep(1, 50);
//
//
//  Serial.begin(SERIAL_BAUD);
//  Serial.setDebugOutput(true);
//  Serial.print("\n");
//  delay(100);
//
//  // ---------------------------------- SD Card init  
//  Serial.print("\nInitializing SD card...");
//  hasSD = false;
//
//  // we'll use the initialization code from the utility libraries
//  // since we're just testing if the card is working!
//  if (!card.init(SPI_HALF_SPEED, SD_PIN)) {
//    Serial.println("initialization failed. Things to check:");
//    Serial.println("* is a card inserted?");
//    Serial.println("* is your wiring correct?");
//    Serial.println("* did you change the SD_PIN pin to match your shield or module?");
//
//    //return;
//  } else {
//    Serial.println("Wiring is correct and a card is present.");
//	hasSD = true;
//
//    beep(5, 50);
//	beep(4, 50);
//  }
//
//  if (hasSD) {
//	  // print the type of card
//	  Serial.print("\nCard type: ");
//	  switch (card.type()) {
//		case SD_CARD_TYPE_SD1:
//		  Serial.println("SD1");
//		  break;
//		case SD_CARD_TYPE_SD2:
//		  Serial.println("SD2");
//		  break;
//		case SD_CARD_TYPE_SDHC:
//		  Serial.println("SDHC");
//		  break;
//		default:
//		  Serial.println("Unknown");
//	  }
//
//	  // Now we will try to open the 'volume'/'partition' - it should be FAT16 or FAT32
//	  if (!volume.init(card)) {
//		Serial.println("Could not find FAT16/FAT32 partition.\nMake sure you've formatted the card");
//		return;
//	  }
//
//	  // print the type and size of the first FAT-type volume
//	  uint32_t volumesize;
//	  Serial.print("\nVolume type is FAT");
//	  Serial.println(volume.fatType(), DEC);
//	  Serial.println();
//
//	  volumesize = volume.blocksPerCluster();    // clusters are collections of blocks
//	  volumesize *= volume.clusterCount();       // we'll have a lot of clusters
//	  volumesize *= 512;                            // SD card blocks are always 512 bytes
//	  Serial.print("Volume size (bytes): ");
//	  Serial.println(volumesize);
//	  Serial.print("Volume size (Kbytes): ");
//	  volumesize /= 1024;
//	  Serial.println(volumesize);
//	  Serial.print("Volume size (Mbytes): ");
//	  volumesize /= 1024;
//	  Serial.println(volumesize);
//
//	  root.openRoot(volume);
//
//	  /*
//	  // list all files in the card with date and size
//	  Serial.println("\nFiles found on the card (name, date and size in bytes): ");
//	  root.ls(LS_R | LS_DATE | LS_SIZE);
//	  */
//
//	  root.close();
//
//	  SD.begin(SD_PIN);
//  }
//
//
//  // ---------------------------------- WiFi init
//  if (WiFi.status() == WL_NO_SHIELD) {
//    // do nothing (flash red) forever
//    while (true) {
//      blinkStateR = !blinkStateR;
//      digitalWrite(ledR, blinkStateR);
//      delay(750);
//    }
//  }
//
//  // DEFAULT: attempt to connect to Known Wifi network
//  ssid = ssid_Home;
//  pass = pass_Home;
//  connect(false);
//  DBG_OUTPUT_PORT.print("Connecting to ");
//  DBG_OUTPUT_PORT.println(ssid);
//
//
//  digitalWrite(ledR, HIGH);
//
//  //  udpRec.begin(localPort);
//
//  // wait long enough for Arduino to finish booting...
//  delay(ARDUINO_STARTUP_DELAY);
//
//  // Set LCD text to our IP address
//  if (status == WL_CONNECTED) {
//    IPAddress ip = WiFi.localIP();                 // Gather the Local Ip of the NodeMCU
//    Serial.println(String("3,") + ip[0] + "." + ip[1] + "." + ip[2] + "." + ip[3]);
//  }
//  else {
//    Serial.println("3,No Network");
//  }
//
//  // discard any initial packets as line/startup noise
//  delay(50);
//  while (Serial.available() > 0) {
//    Serial.read();
//  }
//
//  // Wait for connection
//  uint8_t i = 0;
//  while (WiFi.status() != WL_CONNECTED && i++ < 20) {//wait 10 seconds
//    delay(500);
//  }
//  if (i == 21) {
//    DBG_OUTPUT_PORT.print("Could not connect to");
//    DBG_OUTPUT_PORT.println(ssid);
//    while (1) delay(500);
//  }
//  DBG_OUTPUT_PORT.print("Connected! IP address: ");
//  DBG_OUTPUT_PORT.println(WiFi.localIP());
//
//  if (MDNS.begin(host)) {
//    MDNS.addService("http", "tcp", 80);
//    DBG_OUTPUT_PORT.println("MDNS responder started");
//    DBG_OUTPUT_PORT.print("You can now connect to http://");
//    DBG_OUTPUT_PORT.print(host);
//    DBG_OUTPUT_PORT.println(".local");
//  }
//
//
//  server.on("/list", HTTP_GET, printDirectory);
//  server.on("/edit", HTTP_DELETE, handleDelete);
//  server.on("/edit", HTTP_PUT, handleCreate);
//  server.on("/edit", HTTP_POST, []() {
//    returnOK();
//  });
//  server.onNotFound(handleNotFound);
//  server.onFileUpload(handleFileUpload);
//
//  server.begin();
//  DBG_OUTPUT_PORT.println("HTTP server started");
//
//  if (SD.begin(SD_PIN)) {
//    DBG_OUTPUT_PORT.println("SD Card initialized.");
//    hasSD = true;
//  }
//
//
//  // completion tone
//  beep(1, 150);
//
//  // enable Arduino sensor blast
//  Serial.println("7");
//
//  // setup complete!!!
//  digitalWrite(ledR, (status == WL_CONNECTED) ? LOW : HIGH);
//
//  //  playSong(mario_tempo, mario_melody, sizeof(mario_melody) / sizeof(int), 1000);
//  //  playSong(bond_tempo, bond_melody, sizeof(bond_melody) / sizeof(int), 750);
//
//  digitalWrite(ledG, LOW);
//}
///**************************
//
//LOOP
//
//***************************/
//void loop() {
//
//  // Blink for Status...
//  digitalWrite(ledG, blinkStateG);
//  // If Web Server Then Dont Run The Working Code
////  if (WS_CONNECTED) {}
////  else {
//  WORKING();
////}
//  if (status == WL_CONNECTED) {
//    blinkStateG = !blinkStateG;
////    beep(37, 75);
//  }
//}
//
///**************************
//
//WORKING
//
//***************************/
//void WORKING() {
//  // ---
//  // SERIAL RECEIVE (Arduino)
//  // ---
//
//  if (Serial.available() > 0) {
//    blinkStateG = !blinkStateG;
//    digitalWrite(ledG, blinkStateG);
//    String line = Serial.readStringUntil('\n');
//
//    //    Serial.println("3,got:"+line);  // DEBUG
//
//    if (line != NULL && line.length() > 0) {
//
//      // Sometimes we get carriage-returns, grr... ditch them
//      if (line.charAt(line.length() - 1) == '\r') {
//        line = line.substring(0, line.length() - 1);
//      }
//
//      int comma = line.indexOf(',');
//      String cmdStr = (comma > 0) ? line.substring(0, comma) : line;
//      int cmd = cmdStr.toInt();
//      String data = (comma > 0) ? line.substring(comma + 1) : "";
//
//      switch (cmd) {
//
//        case 1: // List networks
//          listNetworks("5,"); // Echo the return lines back to bluetooth
//          break;
//
//        case 2: // Set SSID
//          // should clear the existing ssid first
//          ssid = data;
//          Serial.println("5,SSID set to: " + ssid);
//          break;
//
//        case 3: // Set Password
//          // should clear existing pass first
//          pass = data;
//          Serial.println("5,Password set to: " + pass);
//          break;
//
//        case 4: // Connect/Disconnect
//          if (status == WL_CONNECTED) {
//            disconnect();
//          }
//          else {
//            connect(true);
//          }
//          break;
//
//        case 5: // Set Unity IP
//          if (setUnityIP(data)) {
//            Serial.println(String("5,IP set to: ") + remoteAddr[0] + "." + remoteAddr[1] + "." + remoteAddr[2] + "." + remoteAddr[3]);
//          }
//          else {
//            Serial.println("5,Failed to set IP");
//          }
//          break;
//
//        case 6: // SEND DATA TO UNITY
//          sendToUnity(data);
//          break;
//
//        case 7: // SEND DATA TO ARDUINO
//          sendToArduino(data);
//          break;
//
//        case 8: // PLAY MELODY
//          playMelody(data);
//          break;
//
///*
//		case 9: // Connect/Disconnect
//          if (status == WS_CONNECTED) {
//            disconnect();
//          }
//          else {
//            Web_Server(true);
//          }
//          break;
//*/
//
///*
//		case 10: // Regenerate index.htm
//			writeIndexHTML();
//			break;
//*/
//
//        case 0: // Show Menu -OR- Not a valid integer -- fall through to default
//        default: // Unknown command
//
//          if (cmdStr.equals("0")) { // Actually show menu
//            showMenu("5,"); // Menu output lines will begin with "5," (to Arduino) which means send to bluetooth
//          }
//          else {
//            Serial.println(String("5,UNKOWN COMMAND: ") + cmdStr); // "5, ..." (Arduino send to bluetooth)
//            /*
//            Serial.println(String("5,UNKOWN COMMAND: ")+cmd+","+cmdStr.length()); // "5, ..." (Arduino send to bluetooth)
//            Serial.println(String("5,cmdStr: ")+cmdStr);
//            Serial.println(String("5,[0] val: ")+((int) cmdStr.charAt(0)));
//            Serial.println(String("5,[1] val: ")+((int) cmdStr.charAt(1)));
//            */
//          }
//          break;
//      }
//    }
//  }
//
//
//  // ---
//  // WIFI RECEIVE (Unity)
//  // ---
//
//  if (status == WL_CONNECTED) {
//
//	server.handleClient();
//
//    int size = udp.parsePacket();
//    if (size > 0) {
//      blinkStateR = !blinkStateR;
//      digitalWrite(ledR, blinkStateR);
//
//      char buffer[size + 1];  //buffer to hold incoming packet
//      udp.read(buffer, size + 1);
//      String line = buffer;
//
//      if (line != NULL && line.length() > 0) {
//
//        // Find a newline and trim string there if it has one.
//        // This is ugly... shouldn't need it, but getting extra characters while debugging!
//        int newline = line.indexOf('\n');
//        if (newline >= 0) {
//          line = line.substring(0, newline);
//        }
//
//        // Sometimes we get carriage-returns, grr... ditch them
//        if (line.charAt(line.length() - 1) == '\r') {
//          line = line.substring(0, line.length() - 1);
//        }
//
//        int comma = line.indexOf(',');
//        String cmdStr = (comma > 0) ? line.substring(0, comma) : line;
//        int cmd = cmdStr.toInt();
//        String data = (comma > 0) ? line.substring(comma + 1) : "";
//
//        switch (cmd) {
//
//          case 1: // List networks
//            listNetworksUDP();
//            break;
//
//          case 2: // Set SSID
//            // should clear this data first.. and then pass it into a known ssid database
//            ssid = data;
//            break;
//
//          case 3: // Set Password
//            pass = data;
//            break;
//
//          case 4: // Connect/Disconnect
//            if (status == WL_CONNECTED) {
//              disconnect();
//            }
//            else {
//              connect(true);
//            }
//            break;
//
//          case 5: // Set Unity IP
//            setUnityIP(data);
//            break;
//
//          case 6: // SEND DATA TO UNITY
//            sendToUnity(data);
//            break;
//
//          case 7: // SEND DATA TO ARDUINO
//            sendToArduino(data);
//            break;
//
//          case 0: // Show Menu -OR- Not a valid integer -- fall through to default
//          default: // Unknown command
//
//            if (cmdStr.equals("0")) { // Actually show menu
//              showMenuUDP();
//            }
//            else {
//              sendToUnity(String("UNKNOWN COMMAND: ") + cmd + " (length=" + cmdStr.length() + ")" + "\n");
//              for (int i = 0; i < cmdStr.length(); i++) {
//                sendToUnity(String("char[") + i + "] as int: " + ((int)cmdStr.charAt(i)) + "\n");
//              }
//            }
//            break;
//        }
//      }
//    }
//  }
//}
//
//void showMenu(String pre) {
//
//  Serial.println("5,NodeMCU Serial Menu");
//  Serial.println("5, ");
//  Serial.println("5,   0. Show Menu");
//  Serial.println("5,   1. List WiFi Networks");
//  Serial.println("5,   2. Set SSID <name>");
//  Serial.println("5,   3. Set Password <password>");
//  if (status == WL_CONNECTED) {
//    Serial.println("5,   4. Disconnect");
//  }
//  else {
//    Serial.println("5,   4. Connect");
//  }
//  Serial.println("5,   5. Set Unity IP <address>");
//  Serial.println("5,   6. Send to Unity <DATA>");
//  Serial.println("5,   7. Send to Arduino <DATA>");
//  Serial.println("5,   8. Play Melody <MELODY>");
//  Serial.println("5,   9. Run Web Server");
//  if (status == WS_CONNECTED) {
//    Serial.print("5,   9. Disconnect");
//  }
//  else {
//    Serial.print("5,   9. Connect");
//  }
//  Serial.println("5,  10. Regenerate /index.htm on SD card");
//  Serial.println("5, ");
//
//  /*
//  Serial.println("5,Something do a lot more and then a big line");
//  //  delay(10);
//  //  Serial.println("5,here i am with a bunch more 2 and some");
//  //  delay(10);
//  //  Serial.println("5,here i am with a bunch more 3 and some");
//  //  delay(10);
//  //  Serial.println("5,here i am with a bunch more 4 and some");
//  //  delay(10);
//  Serial.println("5,here i am with a bunch more 5 and some");
//  //  delay(10);
//  */
//}
//
//void showMenuUDP() {
//  String menu = String("NodeMCU UDP Menu\n")
//                + "\n"
//                + "   0. Show Menu\n"
//                + "   1. List WiFi Networks\n"
//                + "   2. Set SSID <name>\n"
//                + "   3. Set Password <password>\n"
//                + ((status == WL_CONNECTED) ? "   4. Disconnect\n"
//                   : "   4. Connect\n")
//                + "   5. Set Unity IP <address>\n"
//                + "   6. Send to Unity <DATA>\n"
//                + "   7. Send to Arduino <DATA>\n"
//                + "\n";
//
//  sendToUnity(menu);
//}
//
//void listNetworks(String pre) {
//  int count = WiFi.scanNetworks();
//
//  if (count < 0) {
//    Serial.println(pre + "--NO CONNECTION--");
//  }
//  else if (count > 0) {
//    for (int i = 0; i < count; i++) {
//      Serial.println(pre + WiFi.SSID(i));
//    }
//  }
//  else {
//    Serial.println(pre + "--No Networks Found--");
//  }
//}
//
//void listNetworksUDP() {
//  int count = WiFi.scanNetworks();
//  String list;
//
//  if (count < 0) {
//    list = "--NO CONNECTION--";
//  }
//  else if (count > 0) {
//    list = "";
//    for (int i = 0; i < count; i++) {
//      list += String(WiFi.SSID(i)) + "\n";
//    }
//  }
//  else {
//    list = "--No Networks Found--";
//  }
//
//  sendToUnity(list);
//}
//
//
//
//bool connect(bool verbose) {
//  digitalWrite(ledR, HIGH);
//  beep(2, 50);
//  //beep(GamOver_melody, GameOver_tempo);
//  if (verbose) {
//    Serial.println("3,Net Connect...");
//  }
//
//  char _ssid[ssid.length() + 1], _pass[pass.length() + 1];
//  ssid.toCharArray(_ssid, ssid.length() + 1);
//  pass.toCharArray(_pass, pass.length() + 1);
//  //  wifiMulti.addAP(_ssid, _pass);
//  WiFi.begin(_ssid, _pass);
//  //  WiFi.begin("MTSpace", "makeitso");
//
//  status = WiFi.status();
//
//  // wait for connection
//  long begin = millis();
//
//  while (status != WL_CONNECTED) {
//    long now = millis();
//
//    if (now - begin > WIFI_CONNECT_TIMEOUT)
//    {
//      if (verbose) {
//        Serial.println("3,Connect timeout");
//      }
//      beep(4, 250);
//      beep(5, 250);
//      digitalWrite(ledR, HIGH);
//      return false;
//    }
//
//    delay(50);
//    blinkStateR = !blinkStateR;
//    digitalWrite(ledR, blinkStateR);
//    status = WiFi.status();
//  }
//
//  // Set LCD text to our IP address
//  if (verbose) {
//    IPAddress ip = WiFi.localIP();                 // Gather the Local Ip of the NodeMCU
//    Serial.println(String("3,") + ip[0] + "." + ip[1] + "." + ip[2] + "." + ip[3]);
//  }
//  beep(3, 50);
//  beep(2, 50);
//
//  udp = WiFiUDP();
//  udp.begin(localPort);
//
//  digitalWrite(ledR, LOW);
//  return true; // connection successful
//}
//
//bool disconnect() {
//  beep(3, 50);
//
//  status = -1;
//  digitalWrite(ledR, HIGH);
//  WiFi.disconnect();
//
//  Serial.println("3,Net Disconnect");
//}
//
//bool setUnityIP(String data) {
//  int dot1 = data.indexOf('.');
//  if (dot1 < 1) return false;
//  int dot2 = data.indexOf('.', dot1 + 1);
//  if (dot2 < 1) return false;
//  int dot3 = data.indexOf('.', dot2 + 1);
//  if (dot3 < 1) return false;
//
//  remoteAddr = IPAddress(
//                 data.substring(0, dot1).toInt(),
//                 data.substring(dot1 + 1, dot2).toInt(),
//                 data.substring(dot2 + 1, dot3).toInt(),
//                 data.substring(dot3 + 1).toInt()
//               );
//
//  return true;
//}
//
//void sendToUnity(String data) {
//
//  if (status == WL_CONNECTED) {
//    WiFiUDP send;
//
//    if (send.beginPacket(remoteAddr, remotePort) == 1) {  // BEGIN TRANSMISSION!
//
//      // Attach the received data line
//      //send.write("just a test\n");
//      //      data = String("testing again");
//      char buffer[data.length() + 1];
//      data.toCharArray(buffer, data.length() + 1);
//      send.write(buffer);
//
//      // End UDP packet
//      send.endPacket();                              // END THE PACKET AND START AGAIN!!!!!!
//    }
//  }
//}
//
//
//
//void sendToArduino(String data) {
//  Serial.println(data);
//}
//
//
//void playMelody(String data) {
//  int choice = data.toInt();
//
//  switch (choice) {
//    case 1: // MARIO
//      playSong(mario_tempo, mario_melody, sizeof(mario_melody) / sizeof(int), 750);
//      delay(200);
//      playSong(underworld_tempo, underworld_melody, sizeof(underworld_melody) / sizeof(int), 750);
//      break;
//
//    case 2: // BOND
//      playSong(bond_tempo, bond_melody, sizeof(bond_melody) / sizeof(int), 750);
//      break;
//
//    case 0:
//    default:
//      Serial.println("5,Melody Selection:");
//      Serial.println("5,   1. Mario");
//      Serial.println("5,   2. James Bond");
//      break;
//  }
//}
//
///*
//void writeIndexHTML() {
//}
//*/
//
//
///*
//* Simple (manual) beep
//*
//* Used for testing right now.
//* For some reason the built-in tone() function is not working
//*/
//void beep(unsigned int delayTime, unsigned long duration) {
//  bool speak = false;
//
//  for (int i = 0; i < duration / delayTime; i++) {
//    speak = !speak;
//    digitalWrite(SPEAKER_PIN, speak);
//    delay(delayTime);
//  }
//  digitalWrite(SPEAKER_PIN, LOW);
//}
//
//// ------------------------------------------    Musical stuff
//void buzz(long frequency, long length) {
//  if (frequency <= 0) {
//    delay(length);
//  }
//  else {
//    long delayValue = 1000000 / frequency / 2; // calculate the delay value between transitions
//    //// 1 second's worth of microseconds, divided by the frequency, then split in half since
//    //// there are two phases to each cycle
//    long numCycles = frequency * length / 1000; // calculate the number of cycles for proper timing
//    //// multiply frequency, which is really cycles per second, by the number of seconds to
//    //// get the total number of cycles to produce
//    for (long i = 0; i < numCycles; i++) { // for the calculated length of time...
//      //digitalWrite(r_led, redstate);
//      digitalWrite(SPEAKER_PIN, HIGH); // write the buzzer pin high to push out the diaphram
//      delayMicroseconds(delayValue); // wait for the calculated delay value
//      digitalWrite(SPEAKER_PIN, LOW); // write the buzzer pin low to pull back the diaphram
//      delayMicroseconds(delayValue); // wait again or the calculated delay value
//    }
//  }
//
//  // finish on low
//  digitalWrite(SPEAKER_PIN, LOW);
//}
//
//// EXAMPLE _playSong(underworld_tempo, underworld_melody);
//void playSong(int _tempo[], int _Kelody[], int size, int slowness) {
//  // LCD PRINT out to dispay should be in here
//  for (int thisNote = 0; thisNote < size; thisNote++) {
//    int noteDuration = slowness / _tempo[thisNote];
//    buzz(_Kelody[thisNote], noteDuration);
//    int pauseBetweenNotes = noteDuration * 1.30;
//    delay(pauseBetweenNotes);
//  }
//}
//
//// ------------------------------------------    WebServer stuff
///*
//bool Web_Server(bool verbose) {
//  digitalWrite(ledR, HIGH);
////  beep(5, 50);
//  if (verbose) {
//    Serial.println("3,Web Server Connecting...");
//  }
//  server.handleClient();
//  return true; // connection successful
//}
//*/