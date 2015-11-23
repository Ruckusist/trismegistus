#include "Wifi_Handler.h"
using namespace NODEMCU;

Node::Node() {
	Init(true);
	SDinit();
	if (!Handle_MDNS()) {
		Serial.println("MDNS responder setup");
	}
}

void Node::Responder() {
	server->handleClient();
}


void Node::SDinit() {
	if (SD.begin(SS)) {
		DBG_OUTPUT_PORT.println("SD Card initialized.");
		hasSD = true;
	}
}

void Node::Init(bool verbose) {
	//  Begin Wifi
	if (WiFi.status() == WL_NO_SHIELD) {
		if (verbose) {
			Serial.println("WiFi shield not present");
		}
		// don't continue:
		while (true);
	}

	// attempt to connect to Wifi network:
	while (status != WL_CONNECTED) {

		Serial.print("Attempting to connect to SSID: ");
		Serial.println(ssid);
		// Connect to WPA/WPA2 network. Change this line if using open or WEP network:
		status = WiFi.begin(ssid, pass);

		// wait 10 seconds for connection:
		if (status != WL_CONNECTED) { Serial.println("Reattempting connect in 10 secs... hold on..."); }
		delay(10000);
	}
	// you're connected now, so print out the status:
	if (verbose) {
		//_node.printwifiStatus();
	}
	printwifiStatus(false);
}


void Node::printwifiStatus(bool verbose) {
	// print the SSID of the network you're attached to:
	Serial.print("SSID: ");
	Serial.println(WiFi.SSID());
	// print your WiFi shield's IP address:
	IPAddress ip = WiFi.localIP();
	Serial.print("IP Address: ");
	Serial.println(ip);

	// print the received signal strength:
	long rssi = WiFi.RSSI();
	Serial.print("signal strength (RSSI):");
	Serial.print(rssi);
	Serial.println(" dBm");
}

uint8_t Node::CheckConnect(bool verbose) {
	if (status == WL_CONNECTED) {
		if (verbose) {
			Serial.println("Looks good");
		}
		return 1;
	}
	return 0;
}

bool Node::Handle_BasicServer() {
	// Hand Connection
	_server = new WiFiServer(SERVER_PORT);
	_server->begin();
	delay(10);
	Serial.println("Server started on port : " + SERVER_PORT);
}

bool Node::Handle_MDNS() {
	// MDNS connection
	server = new ESP8266WebServer(SERVER_PORT);
	if (MDNS.begin(host)) {
		MDNS.addService("http", "tcp", 80);
		DBG_OUTPUT_PORT.println("MDNS responder started");
		DBG_OUTPUT_PORT.print("You can now connect to http://");
		DBG_OUTPUT_PORT.print(host);
		DBG_OUTPUT_PORT.println(".local");
	}
	
	/*
	server->on("/list", HTTP_GET, printDirectory);
	server->on("/edit", HTTP_DELETE, handleDelete);
	server->on("/edit", HTTP_PUT, handleCreate);
	server->on("/edit", HTTP_POST, []() { returnOK(); });
	server->onNotFound(handleNotFound);
	server->onFileUpload(handleFileUpload);
	*/
	server->begin();
	return true;

}

void Node::HandleBasicWifi(bool verbose) {
	WiFiClient client = _server->available();
	//	bool isServer;
	if (client) {
		Serial.println("new client");
		// an http request ends with a blank line
		boolean currentLineIsBlank = true;
		while (client.connected()) {
			isServer = true;
			if (client.available()) {
				char c = client.read();
				//Serial.write(c);
				// if you've gotten to the end of the line (received a newline
				// character) and the line is blank, the http request has ended,
				// so you can send a reply
				if (c == '\n' && currentLineIsBlank) {
					// send a standard http response header
					client.println("HTTP/1.1 200 OK");
					client.println("Content-Type: text/html");
					client.println("Connection: close");  // the connection will be closed after completion of the response
					client.println("Refresh: 5");  // refresh the page automatically every 5 sec
					client.println();
					client.println("<!DOCTYPE HTML>");
					client.println("<html>");
					// Code to be Printed to web page at Refresh rate
					// print the SSID of the network you're attached to:
//					client.print("SSID: ");
//					client.println(WiFi.SSID());
					// print your WiFi shield's IP address:
//					IPAddress ip = WiFi.localIP();
//					client.print("IP Address: ");
//					client.println(ip);

					// print the received signal strength:
					long rssi = WiFi.RSSI();
					client.print("signal strength (RSSI):");
					client.print(rssi);
					client.println(" dBm");
					if (verbose) {
						//						printwifiStatus(false);
					}
					// output the value of each analog input pin

					/*
					for (int analogChannel = 0; analogChannel < 6; analogChannel++) {
						int sensorReading = analogRead(analogChannel);
						client.print("analog input ");
						client.print(analogChannel);
						client.print(" is ");
						client.print(sensorReading);
						client.println("<br />");
					}
					*/

					client.println("</html>");
					break;
				}
				if (c == '\n') {
					// you're starting a new line
					currentLineIsBlank = true;
				}
				else if (c != '\r') {
					// you've gotten a character on the current line
					currentLineIsBlank = false;
				}
			}
		}

		// give the web browser time to receive the data
		delay(1);

		// close the connection:
		client.stop();
		Serial.println("client disonnected");
	}
}

void Node::returnOK() {
	server->sendHeader("Connection", "close");
	server->sendHeader("Access-Control-Allow-Origin", "*");
	server->send(200, "text/plain", "");
}

void Node::returnFail(String msg) {
	server->sendHeader("Connection", "close");
	server->sendHeader("Access-Control-Allow-Origin", "*");
	server->send(500, "text/plain", msg + "\r\n");
}

bool Node::loadFromSdCard(String path) {
	String dataType = "text/plain";
	if (path.endsWith("/")) path += "index.htm";

	if (path.endsWith(".src")) path = path.substring(0, path.lastIndexOf("."));
	else if (path.endsWith(".htm")) dataType = "text/html";
	else if (path.endsWith(".css")) dataType = "text/css";
	else if (path.endsWith(".js")) dataType = "application/javascript";
	else if (path.endsWith(".png")) dataType = "image/png";
	else if (path.endsWith(".gif")) dataType = "image/gif";
	else if (path.endsWith(".jpg")) dataType = "image/jpeg";
	else if (path.endsWith(".ico")) dataType = "image/x-icon";
	else if (path.endsWith(".xml")) dataType = "text/xml";
	else if (path.endsWith(".pdf")) dataType = "application/pdf";
	else if (path.endsWith(".zip")) dataType = "application/zip";

	File dataFile = SD.open(path.c_str());
	if (dataFile.isDirectory()) {
		path += "/index.htm";
		dataType = "text/html";
		dataFile = SD.open(path.c_str());
	}

	if (!dataFile)
		return false;

	if (server->hasArg("download")) dataType = "application/octet-stream";

	if (server->streamFile(dataFile, dataType) != dataFile.size()) {
		DBG_OUTPUT_PORT.println("Sent less data than expected!");
	}

	dataFile.close();
	return true;
}

void Node::handleFileUpload() {
	if (server->uri() != "/edit") return;
	HTTPUpload& upload = server->upload();
	if (upload.status == UPLOAD_FILE_START) {
		if (SD.exists((char *)upload.filename.c_str())) SD.remove((char *)upload.filename.c_str());
		uploadFile = SD.open(upload.filename.c_str(), FILE_WRITE);
		DBG_OUTPUT_PORT.print("Upload: START, filename: "); DBG_OUTPUT_PORT.println(upload.filename);
	}
	else if (upload.status == UPLOAD_FILE_WRITE) {
		if (uploadFile) uploadFile.write(upload.buf, upload.currentSize);
		DBG_OUTPUT_PORT.print("Upload: WRITE, Bytes: "); DBG_OUTPUT_PORT.println(upload.currentSize);
	}
	else if (upload.status == UPLOAD_FILE_END) {
		if (uploadFile) uploadFile.close();
		DBG_OUTPUT_PORT.print("Upload: END, Size: "); DBG_OUTPUT_PORT.println(upload.totalSize);
	}
}

void Node::deleteRecursive(String path) {
	File file = SD.open((char *)path.c_str());
	if (!file.isDirectory()) {
		file.close();
		SD.remove((char *)path.c_str());
		return;
	}

	file.rewindDirectory();
	while (true) {
		File entry = file.openNextFile();
		if (!entry) break;
		String entryPath = path + "/" + entry.name();
		if (entry.isDirectory()) {
			entry.close();
			deleteRecursive(entryPath);
		}
		else {
			entry.close();
			SD.remove((char *)entryPath.c_str());
		}
		yield();
	}

	SD.rmdir((char *)path.c_str());
	file.close();
}

void Node::handleDelete() {
	if (server->args() == 0) return returnFail("BAD ARGS");
	String path = server->arg(0);
	if (path == "/" || !SD.exists((char *)path.c_str())) {
		returnFail("BAD PATH");
		return;
	}
	deleteRecursive(path);
	returnOK();
}

void Node::handleCreate() {
	if (server->args() == 0) return returnFail("BAD ARGS");
	String path = server->arg(0);
	if (path == "/" || SD.exists((char *)path.c_str())) {
		returnFail("BAD PATH");
		return;
	}

	if (path.indexOf('.') > 0) {
		File file = SD.open((char *)path.c_str(), FILE_WRITE);
		if (file) {
			file.write((const char *)0);
			file.close();
		}
	}
	else {
		SD.mkdir((char *)path.c_str());
	}
	returnOK();
}

void Node::printDirectory() {
	if (!server->hasArg("dir")) return returnFail("BAD ARGS");
	String path = server->arg("dir");
	if (path != "/" && !SD.exists((char *)path.c_str())) return returnFail("BAD PATH");
	File dir = SD.open((char *)path.c_str());
	path = String();
	if (!dir.isDirectory()) {
		dir.close();
		return returnFail("NOT DIR");
	}
	dir.rewindDirectory();
	server->setContentLength(CONTENT_LENGTH_UNKNOWN);
	server->send(200, "text/json", "");
	WiFiClient client = server->client();

	server->sendContent("[");
	for (int cnt = 0; true; ++cnt) {
		File entry = dir.openNextFile();
		if (!entry) {
			break;
		}
		String output;
		if (cnt > 0) {
			output = ",";
		}
		output += "{\"type\":\"";
		output += (entry.isDirectory()) ? "dir" : "file";
		output += "\",\"name\":\"";
		output += entry.name();
		output += "\"";
		output += "}";
		server->sendContent(output);
		entry.close();
	}
	server->sendContent("]");
	dir.close();
}

void Node::handleNotFound() {
	if (hasSD && loadFromSdCard(server->uri())) return;
	String message = "SDCARD Not Detected\n\n";
	message += "URI: ";
	message += server->uri();
	message += "\nMethod: ";
	message += (server->method() == HTTP_GET) ? "GET" : "POST";
	message += "\nArguments: ";
	message += server->args();
	message += "\n";
	for (uint8_t i = 0; i<server->args(); i++) {
		message += " NAME:" + server->argName(i) + "\n VALUE:" + server->arg(i) + "\n";
	}
	server->send(404, "text/plain", message);
	DBG_OUTPUT_PORT.print(message);
}

void Node::listNetworks() {
  int count = WiFi.scanNetworks();
  String list;

  if (count < 0) {
    list = "--NO CONNECTION--";
  }
  else if (count > 0) {
    list = "";
    for (int i = 0; i < count; i++) {
      list += String(WiFi.SSID(i)) + "\n";
    }
  }
  else {
    list = "--No Networks Found--";
  }
  
  //Com.sendToUnity(list);
}

void Node::BackupSD(String _Backup) {
		// open the file. note that only one file can be open at a time,
	// so you have to close this one before opening another.
	File dataFile = SD.open("datalog.txt", FILE_WRITE);

	// if the file is available, write to it:
	if (dataFile) {
		dataFile.println(_Backup);
		dataFile.close();
		// print to the serial port too:
		Serial.println(_Backup);
	}
	// if the file isn't open, pop up an error:
	else {
		Serial.println("error opening datalog.txt");
	}
}