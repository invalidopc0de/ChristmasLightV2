#include <Arduino.h>

#include <SPI.h>
#include "RF24.h"

// Constants 
const uint8_t node_address[] = { 0x12, 0x34, 0x56, 0x78, 0x9A };
const uint8_t ctrl_address[] = { 0x12, 0x34, 0x56, 0x78, 0x9B };

// Radio instance
RF24 radio(PB4,PB5);

void setup() {
  Serial.begin(115200);
  Serial.println(F("Christmas Light Node V2"));
  
  radio.begin();

  // Set the PA Level low to prevent power supply related issues since this is a
  // getting_started sketch, and the likelihood of close proximity of the devices. RF24_PA_MAX is default.
  radio.setPALevel(RF24_PA_LOW);

  radio.openWritingPipe(node_address);
  radio.openReadingPipe(1,ctrl_address);

  // Start the radio listening for data
  radio.startListening();
}

void loop() {
  // put your main code here, to run repeatedly:
}