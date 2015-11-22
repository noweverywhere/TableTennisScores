/**
 * Radio Buttons Transmitter
 **/
#include <SPI.h>

/****************** Radio Config ***************************/
/***      Set this radio as radio number 0 or 1         ***/
bool radioNumber = 1;

/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 7 & 8 */
//RF24 radio(7,8);

byte addresses[][6] = {"trans","recvr"};

// Used to control whether this node is sending or receiving
bool role = 1; // 1 = transmitter, 0 = receiver



/****** Buttons Debounce ******/

#define DEBOUNCE 10  // button debouncer, how many ms to debounce, 5+ ms is usually plenty

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

void setup() 
{
  byte i;
  
  // set up serial port
  Serial.begin(115200);
  Serial.print("Button checker with ");
  Serial.print(NUMBUTTONS, DEC);
  Serial.println(" buttons");

  // pin13 LED
  pinMode(13, OUTPUT);
 
  // Make input & enable pull-up resistors on switch pins
  for (byte i=0; i < NUMBUTTONS; i++) {
    pinMode(buttons[i], INPUT);
    digitalWrite(buttons[i], HIGH);
  }
}

void loop() {
  check_switches();      // when we check the switches we'll get the current state
  
  for (byte i = 0; i < NUMBUTTONS; i++) {
    //if (justpressed[i]) {
    //  Serial.print(i, DEC);
    //  Serial.println(" Just pressed"); 
      // remember, check_switches() will CLEAR the 'just pressed' flag
   // }
    if (justreleased[i]) {
      Serial.print(i, DEC);
      Serial.println(" Just released");
      // remember, check_switches() will CLEAR the 'just pressed' flag
    }
    if (pressed[i]) {
      Serial.print(i, DEC);
      Serial.println(" pressed");
      // is the button pressed down at this moment
    }
  }
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
    
    /*     
    Serial.print(index, DEC);
    Serial.print(": cstate=");
    Serial.print(currentstate[index], DEC);
    Serial.print(", pstate=");
    Serial.print(previousstate[index], DEC);
    Serial.print(", press=");
    */
    
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
