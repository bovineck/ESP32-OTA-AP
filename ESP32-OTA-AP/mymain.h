/*
   This is the file that is now the actual
   code to upload to the ESP32. It has the
   same structures as a normal sketch file
   which is to say:

    +variables
    +setup
    +loop

   So you keep the OTA file intact, making
   code changes as you wish to this file.

   The example below is for soil moisture
   readings and watering via IoT.
   
*/

#ifndef MYMAIN_H
#define MYMAIN_H

#include <Arduino.h> // Arduino functions

const int AirValue = 620;   // Replace with actual AirValue
const int WaterValue = 310;  // Replace with actual WaterValue
int soilMoisture = 0;

void MySetup(void) {
  pinMode(A0, INPUT); // Assuming the sensor is connected to A0
}

void MyLoop(void) {
  // Read soil moisture and update soilMoisture variable
  soilMoisture = analogRead(A0); // Replace with the appropriate pin if needed
  soilMoisture = map(soilMoisture, AirValue, WaterValue, 0, 100);
  
  // Your custom loop logic here
}

#endif // MYMAIN_H
