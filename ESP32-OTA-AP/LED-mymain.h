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
unsigned long relaytime = 0;
unsigned long currentMillis = 0;
unsigned long led_delay = 550;
unsigned long relay_delay = 750;
boolean ledstate = false;
boolean relaystate = false;

#define led_pin 5
#define relay_pin 4

// MySetup stuff
void MySetup(void) {
  pinMode(led_pin, OUTPUT);
  digitalWrite(led_pin, LOW);
  pinMode(relay_pin, OUTPUT);
  digitalWrite(relay_pin, LOW);
  currentMillis = millis();
  ledtime = currentMillis;
  relaytime = currentMillis;
}

// loop() visits here for actual code
void MyLoop() {

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
  if (currentMillis - relaytime >= relay_delay) {
    if (relaystate) {
      digitalWrite(relay_pin, LOW);
      relaystate = false;
    }
    else {
      digitalWrite(relay_pin, HIGH);
      relaystate = true;
    }
    relaytime = currentMillis;
  }
}
