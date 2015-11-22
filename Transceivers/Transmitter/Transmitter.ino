
/*
* Getting Started example sketch for nRF24L01+ radios
* This is a very basic example of how to send data from one node to another
* Updated: Dec 2014 by TMRh20
*/

#include <SPI.h>
//#include "RF24.h"
//#include <MFRC522.h>

/****************** User Config ***************************/
/***      Set this radio as radio number 0 or 1         ***/
bool radioNumber = 1;

/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 7 & 8 */
//RF24 radio(7,8);

byte addresses[][6] = {"trans","recvr"};

// Used to control whether this node is sending or receiving
bool role = 1; // 1 = transmitter, 0 = receiver


/**********  Buttons debounce config **********************/
#define DEBOUNCE 5  // button debouncer, how many ms to debounce, 5+ ms is usually plenty
// here is where we define the buttons that we'll use. button "1" is the first, button "6" is the 6th, etc
byte buttons[] = {14, 15, 16, 17, 18, 19}; // the analog 0-5 pins are also known as 14-19
// This handy macro lets us determine how big the array up above is, by checking the size
#define NUMBUTTONS sizeof(buttons)
// we will track if a button is just pressed, just released, or 'currently pressed' 
byte pressed[NUMBUTTONS];
byte justpressed[NUMBUTTONS];
byte justreleased[NUMBUTTONS];

/******* RFID reader config **********/
//#define RST_PIN		6 
//#define SS_PIN		5 // SDA
//MFRC522 mfrc522(SS_PIN, RST_PIN);// Create MFRC522 instance

/****** Radio message format *****/
//byte messageBuffer[] = {0x00,0x00,0x00,0x00,0x00};
//byte buttonHeader = 0xAA;
//byte cardHeader = 0xBB;


void sendMessage() {
  /****************** Ping Out Role ***************************/ 
  
  //radio.stopListening();                                    // First, stop listening so we can talk.
  
  /*
  //Serial.println(F("Now sending"));

   if (!radio.write( &messageBuffer, sizeof(messageBuffer) )){
     Serial.println(F("failed"));
   }
      
  radio.startListening();                                    // Now, continue listening  
  */
}


void check_switches()
{
  static byte previousstate[NUMBUTTONS];
  static byte currentstate[NUMBUTTONS];
  static long lasttime;
  byte index;

  //if (millis()  // we wrapped around, lets just try again
  //   lasttime = millis();
  //}
  
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
/*
void checkRFIDReader() {
  if (! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  //Select a new card
  if (! mfrc522.PICC_ReadCardSerial()) {
    return;
  }
  
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    messageBuffer[i+1] = mfrc522.uid.uidByte[i];
  }
  
  messageBuffer[0] = cardHeader;
  sendMessage();
}*/

void setup() {
//  Serial.begin(115200);
  Serial.println(F("RF24 Button Transmitter"));
//  Serial.println(F("*** PRESS 'T' to begin transmitting to the other node"));
  
 /* radio.begin();

  // Set the PA Level low to prevent power supply related issues since this is a
 // getting_started sketch, and the likelihood of close proximity of the devices. RF24_PA_MAX is default.
  radio.setPALevel(RF24_PA_LOW);
  
  // Open a writing and reading pipe on each radio, with opposite addresses
  radio.openWritingPipe(addresses[1]);
  radio.openReadingPipe(1,addresses[0]);
  
  // Start the radio listening for data
  radio.startListening();
  
    // Make input & enable pull-up resistors on switch pins
  for (byte i=0; i < NUMBUTTONS; i++) {
    pinMode(buttons[i], INPUT);
    digitalWrite(buttons[i], HIGH);
  }
  //SPI.begin();		// Init SPI bus
  //mfrc522.PCD_Init();	// Init MFRC522 card
  */
  Serial.println(F("Setup complete"));
}

void loop() {
  
  /*check_switches();      // when we check the switches we'll get the current state
  
  for (byte i = 0; i < NUMBUTTONS; i++) {
    if (pressed[i]) {
      //Serial.print(i, DEC);
      //Serial.println(" pressed");
      // is the button pressed down at this moment
      messageBuffer[0] = buttonHeader;
      messageBuffer[1] = i;
      sendMessage();
    }
  }
  
  //checkRFIDReader();
  */
}

