//TODO:
//Encryption
#include <LoRa.h> // include LoRa library
#include <BluetoothSerial.h>
#include "src/Ionium.h"

/* For the brave souls who get this far: You are the chosen ones,
 the valiant knights of programming who toil away, without rest,
 fixing our most awful code. To you, true saviors, kings of men,
 I say this: never gonna give you up, never gonna let you down,
 never gonna run around and desert you. Never gonna make you cry,
 never gonna say goodbye. Never gonna tell a lie and hurt you. */

//#define USE_DEBUG //Used for debugging stuff
//#define EXPERIMENTAL //Use for enabling experimental features that have not been tested properly.
#ifdef ARDUINO_AVR_UNO
#define Bluetooth Serial
#elif ARDUINO_AVR_MEGA2560
#define Bluetooth Serial1
#elif ARDUINO_ARCH_ESP32
BluetoothSerial Bluetooth;
#endif

void setup() {
#ifndef EXPERIMENTAL
#ifdef USE_DEBUG
  Serial.begin(115200);  // initialize serial
#endif
  Bluetooth.begin("IONIUMX"); //Please set to 0 for first device and 1 for second device and so on.
#endif

  //while (!Serial);
//  while (!Bluetooth);
#ifdef EXPERIMENTAL


#else

LoRa.setPins(5, 14, 2);
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
