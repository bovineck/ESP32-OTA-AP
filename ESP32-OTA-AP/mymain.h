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
*/

// Variables to change

unsigned long ledtime = 0;
unsigned long currentMillis = 0;
unsigned long led_delay = 1202;
boolean ledstate = false;

#define led_pin 21

// MySetup stuff
void MySetup(void) {
  pinMode(led_pin, OUTPUT);
  digitalWrite(led_pin, LOW);
  currentMillis = millis();
  ledtime = currentMillis;
}

// loop() visits here for actual code
void MyLoop(void) {

  // fake multi-tasking routines with blinking!
  currentMillis = millis();
  if (currentMillis - ledtime >= led_delay) {
    if (ledstate) {
      digitalWrite(led_pin, LOW);
      ledstate = false;
    }
    else {
      digitalWrite(led_pin, HIGH);
      ledstate = true;
    }
    ledtime = currentMillis;
  }
}
