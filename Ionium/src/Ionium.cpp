#include <LoRa.h>
#include "Ionium.h"
byte localAddress = 0xBB;
byte destination = 0xFF;
int messageCount = 0;
void readMessage() {
  // read packet header bytes:
  int recipient = LoRa.read();          // recipient address
  byte sender = LoRa.read();            // sender address
  byte incomingLength = LoRa.read();    // incoming msg length

  String incoming = "";

  // if the recipient isn't this device or broadcast,
  //Needs much better implementation but it will do for now.
  if (recipient != localAddress && recipient != destination) {

#ifdef USE_DEBUG
    Serial.println("This message is not for me.");
#endif

    return;                             // skip rest of function
  }

  while (LoRa.available()) {
    incoming += (char)LoRa.read();
  }

  if (incomingLength != incoming.length()) {   // check length for error

#ifdef USE_DEBUG
    Serial.println("Error: message length does not match length");
#endif

    return;                             // skip rest of function
  }

  // if message is for this device, or broadcast, print details:
#ifdef USE_DEBUG
  Serial.println("Received from: 0x" + String(sender, HEX));
  Serial.println("Sent to: 0x" + String(recipient, HEX));
  Serial.println("Message length: " + String(incomingLength));
  Serial.println("Message: " + incoming);
  Serial.println();
#endif
  Bluetooth.println("Message: " + incoming);
  Bluetooth.println();
}

void sendMessage(String outgoing) {
  LoRa.beginPacket();                   // start packet
  LoRa.write(destination);              // add destination address
  LoRa.write(localAddress);             // add sender address
  LoRa.write(outgoing.length());        // add payload length
  LoRa.print(outgoing);                 // add payload
  LoRa.endPacket();                     // finish packet and send it
  messageCount++;
}

void onReceive(int packetSize) {
  if (packetSize == 0) return;          // if there's no packet, return

  readMessage();
}
