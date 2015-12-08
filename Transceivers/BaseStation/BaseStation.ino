#include <SPI.h>
#include "RF24.h"

/****************** User Config ***************************/
/***      Set this radio as radio number 0 or 1         ***/
bool radioNumber = 1;

/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 7 & 8 */
RF24 radio(9,10);
/**********************************************************/

byte addresses[][6] = {"1Node","2Node"};

// Used to control whether this node is sending or receiving
bool role = 0;

void setup() {
  Serial.begin(115200);
  Serial.println(F("BASE STATION"));  
  radio.begin();
  radio.powerUp();

  // Set the PA Level low to prevent power supply related issues since this is a
 // getting_started sketch, and the likelihood of close proximity of the devices. RF24_PA_MAX is default.
  radio.setPALevel(RF24_PA_LOW);
  
  // Open a writing and reading pipe on each radio, with opposite addresses
  radio.openWritingPipe(addresses[1]);
  radio.openReadingPipe(1,addresses[0]);
  
  // Start the radio listening for data
  radio.startListening();
}

byte message[2];
void loop() {
  
/****************** Listen ***************************/
    
    if( radio.available()){
                                                                    // Variable for the received timestamp
      while (radio.available()) {                                   // While there is data ready
        radio.read( &message, sizeof(message) );             // Get the payload
      }
     
      radio.stopListening();                                        // First, stop listening so we can talk   
      Serial.print(F("Button "));
      Serial.print(message[1], DEC);

      switch (message[0]) {
        case 1:
          Serial.println(F(" just pressed"));
          break;
        case 2:
          Serial.println(F(" just released"));
          break;
        case 3:
          Serial.println(F(" pressed"));
          break;
        default: 
         Serial.println(F(" something"));
        break;
      }
      radio.startListening();
   }
} // Loop

