/*
 Name:		DataLogger.ino
 Created:	1/8/2019 11:47:58 AM
 Author:	PTL00023-PAN006
*/
#include "ThingSpeak.h"
#include "secrets.h"
#include <ESP8266WiFi.h>

char ssid[] = SECRET_SSID;   // your network SSID (name) 
char pass[] = SECRET_PASS;   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiClient  client;

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

// Initialize our values
int number1 = 0;
int number2 = 0;
int number3 = 0;
int number4 = 0;
float aux = 0;
float Temperatura = 0;
String myStatus = "";

void setup() {
	Serial.begin(115200);  // Initialize serial

	WiFi.mode(WIFI_STA);
	ThingSpeak.begin(client);  // Initialize ThingSpeak
	pinMode(A0, INPUT);
}

void loop() {

	// Connect or reconnect to WiFi
	if (WiFi.status() != WL_CONNECTED) {
		Serial.print("Attempting to connect to SSID: ");
		Serial.println(SECRET_SSID);
		while (WiFi.status() != WL_CONNECTED) {
			WiFi.begin(ssid, pass);  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
			Serial.print(".");
			delay(5000);
		}
		Serial.println("\nConnected.");
	}

	// set the fields with the values
	aux = analogRead(A0);
	Temperatura = (aux * 330) / 1024;
	ThingSpeak.setField(1, Temperatura);
	//ThingSpeak.setField(2, number2);
	//ThingSpeak.setField(3, number3);
	//ThingSpeak.setField(4, number4);

	// set the status
	ThingSpeak.setStatus(myStatus);

	// write to the ThingSpeak channel
	int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
	if (x == 200) {
		Serial.println("Channel update successful.");
		Serial.println(Temperatura);
	}
	else {
		Serial.println("Problem updating channel. HTTP error code " + String(x));
	}
	delay(20000); // Wait 20 seconds to update the channel again
}