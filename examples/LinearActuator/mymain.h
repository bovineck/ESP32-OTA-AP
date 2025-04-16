// mymain.h

#ifndef MYMAIN_H
#define MYMAIN_H

#include <WebServer.h>
#include <L298N.h>

extern WebServer server;
extern L298N motor;

// Pin definitions
const unsigned int IN1 = 12;
const unsigned int IN2 = 14;
const unsigned int EN = 13;

String windowState = "closed";

L298N motor(EN, IN1, IN2);

extern volatile bool openWindowRequested = false; // Define and optionally initialize
extern volatile bool closeWindowRequested = false; // Define and optionally initialize

void openwindow(int speed, int timer) {
  motor.setSpeed(speed);
  motor.forward();
  delay(timer);
  motor.stop();
  windowState = "open";
}

void closewindow(int speed, int timer) {
  motor.setSpeed(speed);
  motor.backward();
  delay(timer);
  motor.stop();
  windowState = "closed";
}

void handleWindowPage() {
  File windowPage = LittleFS.open("/window.html", "r");
  server.streamFile(windowPage, "text/html");
  windowPage.close();
}

void handleOpenWindow(); // Declaration only, implementation in BlankOTA.ino
void handleCloseWindow(); // Declaration only, implementation in BlankOTA.ino

void MySetup(WebServer &srv) {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(EN, OUTPUT);
}

void MyLoop(WebServer &srv) {
  if (openWindowRequested) {
    openwindow(80, 10000);
    openWindowRequested = false;
  } else if (closeWindowRequested) {
    closewindow(120, 8000);
    closeWindowRequested = false;
  }
  delay(10);
}

#endif
