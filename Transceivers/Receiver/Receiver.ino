
/*
* Getting Started example sketch for nRF24L01+ radios
* This is a very basic example of how to send data from one node to another
* Updated: Dec 2014 by TMRh20
*/

#include <SPI.h>
#include "RF24.h"

/****************** User Config ***************************/
/***      Set this radio as radio number 0 or 1         ***/
bool radioNumber = 0;

/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 7 & 8 */
RF24 radio(9,10);
/**********************************************************/

byte addresses[][6] = {"trans","recvr"};

// Used to control whether this node is sending or receiving
bool role = 0;

/****** Radio message format *****/
byte messageBuffer[5];
byte buttonHeader = 0xAA;
byte cardHeader = 0xBB;

void setup() {
  Serial.begin(115200);
  Serial.println(F("Buttons NRF24 Receiver"));
  delay(100);
  Serial.begin(115200);
  Serial.println(F("RF24/examples/GettingStarted"));
  Serial.println(F("*** PRESS 'T' to begin transmitting to the other node"));
  
  radio.begin();

  // Set the PA Level low to prevent power supply related issues since this is a
 // getting_started sketch, and the likelihood of close proximity of the devices. RF24_PA_MAX is default.
  radio.setPALevel(RF24_PA_LOW);
  
  // Open a writing and reading pipe on each radio, with opposite addresses
  radio.openWritingPipe(addresses[0]);
  radio.openReadingPipe(1,addresses[1]);
  
  // Start the radio listening for data
  radio.startListening();
  Serial.println(F("Setup complete"));
}

void loop() {

/****************** Pong Back Role ***************************/
    
    
    if( radio.available()){
      while (radio.available()) {                                   // While there is data ready
        radio.read( &messageBuffer, sizeof(messageBuffer) );             // Get the payload
        printMessage();
      }      
    }
} // Loop

void printMessage() {
  for (byte i = 0; i < sizeof(messageBuffer); i++) {
    Serial.print(messageBuffer[i], HEX);
  }
  Serial.println();
}
