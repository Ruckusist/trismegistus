// Wifi_Handler.h

#ifndef _WIFI_HANDLER_h
#define _WIFI_HANDLER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "ESP8266WiFi.h"           // Replaces Stock wifi with module features
#include "ESP8266WiFiMulti.h"      // Creates the AutoConnect list
#include "ESP8266mDNS.h"           // This is for the DNS 
#include "WiFiClient.h"            // this is the Stock Wifi Client with Refs on the Arduino site
#include "ESP8266WebServer.h"      // THis will run the web server starter
#include "WiFiUdp.h"               // Wifi Transmit fuctionality
#include "SPI.h"                   // Need this
#include "SD.h"                    // SD card basics... maybe there is a better lib outthere

namespace NODEMCU {

	#define DBG_OUTPUT_PORT Serial

	class Node {
	public:
		Node();
		void Init(bool verbose);
		void SDinit();
		void printwifiStatus(bool verbose);
		uint8_t CheckConnect(bool verbose);
		void HandleBasicWifi(bool verbose);
		bool isServer;
		int SERVER_PORT = 80; // Default server port
		void Responder();
		bool Handle_BasicServer();
		bool Handle_MDNS();
		void listNetworks();
		// LIST OF BUILT-IN SD CARD FUNCTIONS
		bool hasSD;
		void returnOK();
		void returnFail(String msg);
		bool loadFromSdCard(String path);
		void handleFileUpload();
		void deleteRecursive(String path);
		void handleDelete();
		void handleCreate();
		void printDirectory();
		void handleNotFound();
		void BackupSD(String _Backup);
	private:
		String _menu = String("|------######|||| AutoMobile Main MENU ||||######------|\n")
							+ "\n"
							+ "|------       0. Show Menu                       ------|\n"
							+ "|------       1. List WiFi Networks              ------|\n"
							+ "|------       2. Set SSID <name>  disconnected   ------|\n"
							+ "\n";


		String _Backup;   // compile the whole data string after parse for SD backup
		int status;    // for use in API
		const char* ssid = "Ruckus";
		const char* pass = "eric2344";
		const char* host = "AutoMobile";
		WiFiServer *_server = NULL;
		ESP8266WebServer *server = NULL;
		File uploadFile;
	};

	class Com {

	public:
		Com();
		void MENU(String output);
		void sendToUnity(String data);

	protected:

	};
}
#endif