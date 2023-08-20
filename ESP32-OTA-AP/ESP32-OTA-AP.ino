/*
   OTA via an AP (ESP32 version)
   This little wonder will set up it's own AP
   according to the code below - then you can
   upload and change the code by logging into
   the AP (find it on the WiFi and connect to
   it via the IP coded below)
   To change the code, see the relevant parts
   below, including MyVariables,  setup() and
   dostuff().

   You need the bin file to upload, then copy
   it to the ESP32 via the html page it shows
   when you login. See the blog below for all
   the code and the instructions.

      OneCircuit Wed 02 Aug 2023 18:40:22 AEST
      https://www.youtube.com/@onecircuit-as
      https://onecircuit.blogspot.com/
*/

#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Update.h>
#include <ArduinoOTA.h>
#include "FS.h"
#include <SPIFFS.h>
#include "mymain.h"

const char* ssid = "YourAccessPointSSID";
const char* password = "YourAccessPointPassword";
const char* firmwareUpdatePassword = "your_custom_password_here";

WebServer server(80);

void handleRoot() {
  File uploadPage = SPIFFS.open("/upload.html", "r");
  server.streamFile(uploadPage, "text/html");
}

void handleCSS() {
  server.sendHeader("Content-Type", "text/css");
  File cssFile = SPIFFS.open("/style.css", "r");
  if (cssFile) {
    server.streamFile(cssFile, "text/css");
    cssFile.close();
  }
}

void handleUpdate() {
  String enteredPassword = server.arg("password");
  if (enteredPassword != firmwareUpdatePassword) {
    server.send(401, "text/plain", "Unauthorized");
    return;
  }

  HTTPUpload& upload = server.upload();
  if (upload.status == UPLOAD_FILE_START) {
    if (!Update.begin(UPDATE_SIZE_UNKNOWN)) {
      Update.printError(Serial);
    }
  } else if (upload.status == UPLOAD_FILE_WRITE) {
    if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
      Update.printError(Serial);
    }
  } else if (upload.status == UPLOAD_FILE_END) {
    if (Update.end(true)) {
      server.send(200, "text/plain", "Update successful. Rebooting...");
      ESP.restart();
    } else {
      Update.printError(Serial);
    }
  }
}

void setup() {
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
  // change to your own IP Access Point
  IPAddress Ip(192, 168, 19, 63);
  IPAddress NMask(255, 255, 255, 0);
  WiFi.softAPConfig(Ip, Ip, NMask);

  // Core 0 setup
  xTaskCreatePinnedToCore(
    otaWebServerTask,
    "otaWebServerTask",
    10000,
    NULL,
    1,
    NULL,
    0
  );

  // Core 1 setup
  xTaskCreatePinnedToCore(
    mainApplicationTask,
    "mainApplicationTask",
    10000,
    NULL,
    1,
    NULL,
    1
  );
}

void loop() {
  // Nothing here, tasks handle the execution
}

void otaWebServerTask(void* parameter) {
  ArduinoOTA.setHostname("OTA_ESP32");
  ArduinoOTA.begin();
  
  if (!SPIFFS.begin(true)) {
    return;
  }

  MDNS.begin("otaesp32");

  server.on("/", HTTP_GET, handleRoot);
  server.on("/upload", HTTP_POST, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", "<h1>Uploading...</h1>");
  }, handleUpdate);
  server.on("/style.css", HTTP_GET, handleCSS);

  server.begin();

  while (1) {
    ArduinoOTA.handle();
    server.handleClient();
  }
}

void mainApplicationTask(void* parameter) {
  MySetup();
  while (1) {
    MyLoop();
  }
}
