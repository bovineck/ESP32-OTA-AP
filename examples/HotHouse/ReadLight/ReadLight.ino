#include "PCF8574.h"

int sensorPin = 36;
int LightLevel = 0;
int Relay1 = 0;

unsigned long currentTime = millis();
unsigned long previousTime = 0;

// Time to click in ms
const long timeoutTime = 2000;

bool relaystate = 0;

TwoWire I2Cone = TwoWire(0);

PCF8574 pcf8574_R1(&I2Cone, 0x24, 4, 15);

void setup() {
  pinMode(sensorPin, INPUT);
  Serial.begin(115200);

  for (int i = 0; i <= 5; i++)
  {
    pcf8574_R1.pinMode(i, OUTPUT);
  }

  pcf8574_R1.begin();

  for (int i = 0; i <= 5; i++)
  {
    pcf8574_R1.digitalWrite(i, LOW);
    delay(100);
  }

  for (int i = 0; i <= 5; i++)
  {
    pcf8574_R1.digitalWrite(i, HIGH);
    delay(100);
  }
  currentTime = millis();
  previousTime = currentTime;

}

void loop() {
  currentTime = millis();
  if (currentTime - previousTime > timeoutTime) {
    previousTime = millis();
    LightLevel = analogRead(sensorPin);
    Serial.println(LightLevel);

    if (LightLevel < 100) {
    pcf8574_R1.digitalWrite(Relay1, LOW);
    }
    else {
    pcf8574_R1.digitalWrite(Relay1, HIGH);
    }
    }
}
