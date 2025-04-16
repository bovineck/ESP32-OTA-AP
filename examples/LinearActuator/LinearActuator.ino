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

      OneCircuit Fri 04 Apr 2025 12:14:09 AEDT
      https://www.youtube.com/@onecircuit-as
      https://onecircuit.blogspot.com/
*/

#include <WiFi.h>
#include <ESPmDNS.h>
#include <Update.h>
#include <ArduinoOTA.h>
#include <FS.h>
#include <LittleFS.h>
#include <ArduinoJson.h>
#include <Arduino.h>
#include "mymain.h"
#include <WebServer.h>

WebServer server;

// set identity and login for ESP to be an access point
// as well as a password to unlock binary firmware update
const char* ssid = "LinearActuatorSSID";
const char* password = "LAPassword";
const char* firmwareUpdatePassword = "LA-update";

/*
   Handle routines for serving pages and other files from
   the LittleFS file system
*/

void handleRoot() {
  File uploadPage = LittleFS.open("/main.html", "r");
  server.streamFile(uploadPage, "text/html");
  uploadPage.close();
}

void handleAvatarFile() {
  File avatarpic = LittleFS.open("/avatar.png", "r");
  server.streamFile(avatarpic, "image/png");
  avatarpic.close();
}

void handleCSS() {
  server.sendHeader("Content-Type", "text/css");
  File cssFile = LittleFS.open("/style.css", "r");
  if (cssFile) {
    server.streamFile(cssFile, "text/css");
    cssFile.close();
  }
}

void handleUploadFile() {
  server.sendHeader("Content-Type", "text/html");
  File uploadFile = LittleFS.open("/upload.html", "r");
  if (uploadFile) {
    server.streamFile(uploadFile, "text/html");
    uploadFile.close();
  }
}

void handleLittleFSUpload() {
  HTTPUpload& upload = server.upload();
  String filename = "/" + upload.filename;

  // Check if the filename is valid
  if (filename.indexOf("..") != -1 || filename.charAt(0) != '/') {
    server.send(400, "text/plain", "Invalid filename");

    return;
  }

  File file = LittleFS.open(filename, "w");
  // server.send(200, "text/plain", "File opened");
  if (!file) {
    server.send(500, "text/plain", "Failed to open file for writing");
    server.send(200, "text/plain", "File closed");
    return;
  }
  // server.send(200, "text/plain", "File upload started");

  file.write(upload.buf, upload.currentSize);
  file.close();
  server.client().flush();  // Flush the response
  server.send(200, "text/html", "<br></br><br></br><h3 style='text-align:center;'>File uploaded successfully</h3><meta http-equiv='refresh' content='1;url=/access.html'>");
}

void handleAccessFile() {
  File accessFile = LittleFS.open("/access.html", "r");
  if (accessFile) {
    server.streamFile(accessFile, "text/html");
    accessFile.close();
  } else {
    server.send(404, "text/plain", "File not found");
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
      server.send(200, "text/html", "Update successful. Rebooting...<a href='/'>Return to main page</a>");
      server.client().flush();  // Flush the response
      delay(1000);              // Delay for 1 second
      ESP.restart();
    } else {
      Update.printError(Serial);
    }
  }
}

void handleListFiles() {
  String fileList = "<h2>List of Files:</h2>";

  File root = LittleFS.open("/");
  File file = root.openNextFile();

  while (file) {
    String fileName = file.name();
    // exclude the actual access page, otherwise...corruption!
    if (fileName != "access.html") {
      fileList += "<p>" + fileName + " - <a href='/download?file=/" + fileName + "'>Download</a> - <a href='/delete?file=/" + fileName + "'>Delete</a></p>";
    }
    file = root.openNextFile();
  }

  server.send(200, "text/html", fileList);
}

void handleDownloadFile() {
  String downloadFile = server.arg("file");
  if (LittleFS.exists(downloadFile)) {
    File file = LittleFS.open(downloadFile, "r");
    server.streamFile(file, "application/octet-stream");
    file.close();
    // Reload the access.html page using JavaScript after download
    server.sendContent("<script>window.location.href='/access.html';</script>");
  } else {
    server.send(404, "text/plain", "File not found");
  }
}

void handleDeleteFile() {
  String deleteFile = server.arg("file");
  if (LittleFS.remove(deleteFile)) {
    server.send(200, "text/html", "<p>File deleted</p><meta http-equiv='refresh' content='1;url=/access.html'>");
  } else {
    server.send(404, "text/plain", "File not found");
  }
}

void handleOpenWindow() {
  Serial.println("Received request to open window - setting flag");
  openWindowRequested = true;
  server.send(200, "text/html", "<script>window.location.href = '/window.html';</script>");
}

void handleCloseWindow() {
  Serial.println("Received request to close window - setting flag");
  closeWindowRequested = true;
  server.send(200, "text/html", "<script>window.location.href = '/window.html';</script>");
}


void setup() {

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
  // change to your own IP Access Point
  IPAddress Ip(192, 168, 19, 63);
  IPAddress NMask(255, 255, 255, 0);
  WiFi.softAPConfig(Ip, Ip, NMask);
  // if debugging uncomment

  // Core 0 setup
  xTaskCreatePinnedToCore(
    otaWebServerTask,
    "otaWebServerTask",
    10000,
    NULL,
    1,
    NULL,
    0);

  // Core 1 setup
  xTaskCreatePinnedToCore(
    mainApplicationTask,
    "mainApplicationTask",
    10000,
    NULL,
    1,
    NULL,
    1);
}

void loop() {
  // Nothing here, tasks handle the execution
}

void otaWebServerTask(void* parameter) {
  ArduinoOTA.setHostname("OTA_ESP32");
  ArduinoOTA.begin();

  if (!LittleFS.begin()) {
    LittleFS.format();
  }


  MDNS.begin("otaesp32");

  server.on("/", HTTP_GET, handleRoot);
  server.on(
    "/upload", HTTP_POST, []() {
      server.sendHeader("Connection", "close");
      server.send(200, "text/html", "<h1>Uploading...</h1>");
    },
    handleUpdate);
  server.on("/style.css", HTTP_GET, handleCSS);
  server.on("/upload.html", HTTP_GET, handleUploadFile);
  server.on("/access.html", HTTP_GET, handleAccessFile);
  server.on("/list", HTTP_GET, handleListFiles);
  server.on("/download", HTTP_GET, handleDownloadFile);
  server.on("/uploadLittleFS", HTTP_POST, handleLittleFSUpload);
  server.on("/delete", HTTP_GET, handleDeleteFile);
  server.on("/avatar.png", HTTP_GET, handleAvatarFile);
  server.on("/window.html", HTTP_GET, handleWindowPage);    // Serve the window control page
  server.on("/window/open", HTTP_GET, handleOpenWindow);    // Handle open request
  server.on("/window/close", HTTP_GET, handleCloseWindow);  // Handle close request
  server.begin();

  while (1) {
    ArduinoOTA.handle();
    server.handleClient();
  }
}

void mainApplicationTask(void* parameter) {
  MySetup(server);
  while (1) {
    MyLoop(server);
  }
}
