#include <SPI.h>
#include "RF24.h"
#include <MFRC522.h>

/****************** Radio Config ***************************/
/***      Set this radio as radio number 0 or 1         ***/
bool radioNumber = 0;
#define RADIO_SS   8
RF24 radio(7,RADIO_SS);
byte addresses[][6] = {"1Node","2Node"};
/**********************************************************/


/****************** Buttons Config ***************************/
#define DEBOUNCE 100  // button debouncer, how many ms to debounce, 5+ ms is usually plenty
// here is where we define the buttons that we'll use. button "1" is the first, button "6" is the 6th, etc
byte buttons[] = {14, 15, 16, 17, 18, 19}; // the analog 0-5 pins are also known as 14-19
// This handy macro lets us determine how big the array up above is, by checking the size
#define NUMBUTTONS sizeof(buttons)
// we will track if a button is just pressed, just released, or 'currently pressed' 
byte pressed[NUMBUTTONS];
byte justpressed[NUMBUTTONS];
byte justreleased[NUMBUTTONS];
/**********************************************************/

/****************** RFID Config ***************************/
#define RST_PIN    6 
#define RFID_SS    5 // SDA
MFRC522 mfrc522(RFID_SS, RST_PIN);
/**********************************************************/

void setup() {
  Serial.begin(115200);
  Serial.println(F("TABLE"));
  configureRadio();
  configureButtons();
  Serial.println(F("Configuration Complete"));
}

void configureRadio() {
  
  radio.begin();
  radio.powerUp();

  // Set the PA Level low to prevent power supply related issues since this is a
 // getting_started sketch, and the likelihood of close proximity of the devices. RF24_PA_MAX is default.
  radio.setPALevel(RF24_PA_LOW);
  radio.openWritingPipe(addresses[0]);
  radio.openReadingPipe(1,addresses[1]);
}

void configureButtons() {
  for (byte i=0; i < NUMBUTTONS; i++) {
    pinMode(buttons[i], INPUT);
    digitalWrite(buttons[i], HIGH);
  }
}

static long lasttime;
void check_switches()
{
  static byte previousstate[NUMBUTTONS];
  static byte currentstate[NUMBUTTONS];
  
  byte index;
  if ((lasttime + DEBOUNCE) > millis()) {
    // not enough time has passed to debounce
    return; 
  }
  // ok we have waited DEBOUNCE milliseconds, lets reset the timer
  lasttime = millis();
  
  for (index = 0; index < NUMBUTTONS; index++) { // when we start, we clear out the "just" indicators
    justreleased[index] = 0;
     
    currentstate[index] = digitalRead(buttons[index]);   // read the button

    
    if (currentstate[index] == previousstate[index]) {
      if ((pressed[index] == LOW) && (currentstate[index] == LOW)) {
          // just pressed
          justpressed[index] = 1;
      }
      else if ((pressed[index] == HIGH) && (currentstate[index] == HIGH)) {
          // just released
          justreleased[index] = 1;
      }
      pressed[index] = !currentstate[index];  // remember, digital HIGH means NOT pressed
    }
    //Serial.println(pressed[index], DEC);
    previousstate[index] = currentstate[index];   // keep a running tally of the buttons
  }
}


void loop() {
  check_switches();

  for (byte i = 0; i < NUMBUTTONS; i++) {
    if (justpressed[i]) {
      //sendButtonPress(1,i);
    }
    if (justreleased[i]) {
      sendButtonPress(2,i);
    }
    if (pressed[i]) {
      sendButtonPress(3,i);
    }
  }
}

void switchToRadio() {
  Serial.println(F("Select radio as slave"));
  pinMode(RFID_SS, OUTPUT);
  pinMode(RADIO_SS, OUTPUT);
  digitalWrite(RFID_SS, HIGH);
  digitalWrite(RADIO_SS, LOW); 
}

void switchToRFID() {
  Serial.println(F("Select RFID as slave"));
  pinMode(RFID_SS, OUTPUT);
  pinMode(RADIO_SS, OUTPUT);
  digitalWrite(RFID_SS, LOW);
  digitalWrite(RADIO_SS, HIGH);
}

byte message[2];
void sendButtonPress(byte pressType, byte button) {
  Serial.print(F("Button "));
  Serial.print(button, DEC);
  Serial.print(F(" of type: "));
  Serial.println(pressType, DEC);
  switchToRadio(); 
  message[0] = pressType;
  message[1] = button;
  radio.write( &message, sizeof(message));
  switchToRFID();
}

