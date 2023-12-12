// mymain.h

#ifndef MYMAIN_H
#define MYMAIN_H

#include <WebServer.h>
#include "DHT.h"

extern WebServer server; // Declare the server object as external

#define DHTPin 4
#define FlashRed 18
#define FlashGreen 19
#define FlashBlue 21
#define DHTTYPE DHT11

float temperature = 0;
unsigned long flashdelay = 0;
unsigned long updatedelay = 2500;
unsigned long datawritedelay = 20000;
unsigned long currentdatadelay = 0;
unsigned long currentdelay = 0;
unsigned long currentMillis = 0;
unsigned long ledtime = 0;
boolean ledstate = false;
String tempdata = "";

DHT dht(DHTPin, DHTTYPE);

void SetupRoutes() {
  server.on("/temperature", HTTP_GET, []() {
    String html = "<h1>Current Temp: " + String(temperature) + "°C</h1>";
    server.send(200, "text/plain", html);
  });
}

void WriteData() {
  boolean FileExist = LittleFS.exists("/mydata.txt");
  if (!FileExist) {
    // initialize file
    File tdatafile = LittleFS.open("/mydata.txt", "w");
    tdatafile.println("OneCircuit RemoteESP32 Temp data:");
    tdatafile.close();
  } else {
    // write file
    File tdatafile = LittleFS.open("/mydata.txt", "a");
    tempdata = (String)temperature;
    tdatafile.println(tempdata);
    tdatafile.close();
  }

}

void MySetup(WebServer &srv) {
  SetupRoutes();
  pinMode(FlashRed, OUTPUT);
  pinMode(FlashGreen, OUTPUT);
  pinMode(FlashBlue, OUTPUT);
  dht.begin();

  // initialise all the timers
  currentMillis = millis();
  currentdelay = currentMillis;
  currentdatadelay = currentMillis;
  ledtime = currentMillis;

}

void flashLed(int LedPin, unsigned long timeflash) {
  if (currentMillis - ledtime >= timeflash) {
    ledstate = !ledstate;
    digitalWrite(LedPin, ledstate);
    ledtime = currentMillis;
  }
}

void MyLoop(WebServer & srv) {
  currentMillis = millis();
  if (currentMillis - currentdelay >= updatedelay) {
    temperature = dht.readTemperature();
    currentdelay = currentMillis;
  }

  if (currentMillis - currentdatadelay >= datawritedelay) {
    WriteData();
    currentdatadelay = currentMillis;
  }

  // Determine the appropriate LED and flash delay based on temperature
  if (temperature < 10) {
    flashdelay = map(temperature, 1, 10, 200, 1000);
    digitalWrite(FlashRed, LOW);
    digitalWrite(FlashGreen, LOW);
    flashLed(FlashBlue, flashdelay);
  } else if (temperature < 20) {
    flashdelay = map(temperature, 10, 20, 200, 1000);
    digitalWrite(FlashRed, LOW);
    digitalWrite(FlashBlue, LOW);
    flashLed(FlashGreen, flashdelay);
  } else {
    flashdelay = map(temperature, 20, 40, 200, 1000);
    digitalWrite(FlashGreen, LOW);
    digitalWrite(FlashBlue, LOW);
    flashLed(FlashRed, flashdelay);
  }

}

#endif
