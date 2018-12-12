#include <Arduino.h>

#include <SPI.h>
#include "RF24.h"

// Constants
constexpr int NODE_ID_PINS[4] = {PB12, PB13, PB14, PB15};
constexpr int AC_CTRL_PIN = PB0;

const uint8_t MASTER_ADDRESS[] = { 0x12, 0x34, 0x56, 0x78, 0x9A };
const uint8_t NODE_ADDRESS[] = { 0x12, 0x34, 0x56, 0x78, 0x9B };

// Definitions
enum PacketType : uint8_t
{
  CALIBRATION = 0x0,
  UPDATE = 0x0
};

struct RfMessage {
  PacketType type;
  uint16_t data;
};

// Radio instance
RF24 Radio(PB4, PB5);
uint8_t NodeId = 0;

uint8_t read_id(const int pins[4])
{
  uint8_t id = 0;
  for (int i = 0; i < 4; i++)
  {
    id |= (digitalRead(NODE_ID_PINS[i]) << i);
  }
  return id;
}

void setup() {
  Serial.begin(115200);
  Serial.println(F("Christmas Light Node V2"));

  // Initialize Pins
  for (int i = 0; i < 4; i++)
  {
    pinMode(NODE_ID_PINS[i], INPUT_PULLDOWN);
  }
  pinMode(AC_CTRL_PIN, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  digitalWrite(AC_CTRL_PIN, 0);
  digitalWrite(LED_BUILTIN, 0);

  // Read Node ID from pins
  NodeId = read_id(NODE_ID_PINS);
  Serial.print("Node: ");
  Serial.println(NodeId);

  // Configure radio
  Radio.begin();

  // Set the PA Level low to prevent power supply related issues since this is a
  // getting_started sketch, and the likelihood of close proximity of the devices. RF24_PA_MAX is default.
  Radio.setPALevel(RF24_PA_LOW);

  Radio.openWritingPipe(MASTER_ADDRESS);
  Radio.openReadingPipe(1, NODE_ADDRESS);

  // Start the radio listening for data
  Radio.startListening();
}

void loop() {
  Serial.println("Ready");
  digitalWrite(LED_BUILTIN, 1);

  RfMessage last_message = {};
  RfMessage calibration_response = {
    CALIBRATION,
    NodeId
  };

  while (true) {
    if (Radio.available())
    {
      // Read message from radio
      Radio.read(&last_message, sizeof(last_message));

      // Handle message
      if (last_message.type == CALIBRATION)
      {
        // Send back calibration response
        if (last_message.data == NodeId)
        {
          Radio.stopListening();
          Radio.write(&calibration_response, sizeof(RfMessage));
          Radio.startListening();
        }
      }
      else if (last_message.type == UPDATE)
      {
        // Update GPIO
        digitalWrite(AC_CTRL_PIN, last_message.data & (1 << NodeId));
      }
    }
  }
}