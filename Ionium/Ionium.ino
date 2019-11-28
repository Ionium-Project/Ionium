//TODO:
//Encryption
#include <LoRa.h> // include LoRa library
#include "src/Ionium.h"

//#define USE_DEBUG //Used for debugging stuff
//#define EXPERIMENTAL //Use for enabling experimental features that have not been tested properly.
#define Bluetooth Serial1


void setup() {
  #ifndef EXPERIMENTAL
#ifdef USE_DEBUG
  Serial.begin(9600);  // initialize serial
#endif
  Bluetooth.begin(9600);
#endif

  while (!Serial);
  while (!Bluetooth);
#ifdef EXPERIMENTAL


#else
  if (!LoRa.begin(866E6)) {    // initialize radio at 866 MHz

#ifdef USE_DEBUG
    Serial.println("IONIUM::ERROR >> 404");
#endif
    Bluetooth.println("IONIUM::ERROR >> 404");


    while (true); // if failed, do nothing
  }
  #endif

#ifdef USE_DEBUG
  Serial.println("Welcome to Ionium (DEBUG)");
#endif
  Bluetooth.println("Welcome to Ionium");
}

void loop() {
  if (Bluetooth.available()) {
    String message = "";

    // read message from bluetooth
    while (Bluetooth.available()) {
      message += Bluetooth.readString();
    }

#ifdef USE_DEBUG
    Serial.println("You: " + message);
#endif
    Bluetooth.println("You: " + message);

    sendMessage(message);

  }

  // parse for a packet, and call onReceive with the result:
  onReceive(LoRa.parsePacket());
}
