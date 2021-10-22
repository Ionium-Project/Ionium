#include <LoRa.h>
#include <Arduino.h>
#include <BluetoothSerial.h>
#include <EEPROM.h>
#include "Ionium.h"

uint64_t recTemp = 0;
uint64_t senTemp = 0;
union {
    uint8_t destArray[8];
    uint64_t destination;
} destUnion;
union {
    uint8_t recArray[8];
    uint64_t recipient;
} recUnion;
union {
    uint8_t localArray[8];
    uint64_t localAddress;
} localUnion;
union {
    uint8_t senArray[8];
    uint64_t sender;
} senUnion;

String parseChipID(uint64_t source) {
    uint32_t idPart1 = 0;
    uint32_t idPart2 = 0;
    for(int i=0; i<17; i=i+8) {
        idPart2 |= ((source >> (40 - i)) & 0xff) << i;
        idPart1 |= ((source >> (16 - i)) & 0xFF) << i; //Some bitshifting to get the 64 bit number into 2 32 bit. Arduino and thus ESP do not yet have 64bit variables :(
    }
    return String(idPart1, HEX) + String(idPart2, HEX); //Combine 2 32 bit numbers into string.
}

String localAddress = parseChipID(ESP.getEfuseMac());

void readMessage() {
    // read packet header bytes:
    recTemp = 0;
    senTemp = 0;
    for (int i = 0; i<6; i++) {
        recUnion.recArray[i] = LoRa.read();
    }
    for (int i = 0; i<6; ++i) {
        recTemp |= (uint64_t) recUnion.recArray[i] << (i * 8);
    }
    String recipient = parseChipID(recTemp);          // recipient address

    for (int i = 0; i<6; i++) {
        senUnion.senArray[i] = LoRa.read();
    }
    for (int i = 0; i<6; ++i) {
        senTemp |= (uint64_t) senUnion.senArray[i] << (i * 8);
    }
    String sender = parseChipID(senTemp);            // sender address
    byte incomingLength = LoRa.read();    // incoming msg length

    String incoming = "";

    if (recipient.compareTo(localAddress) == 0) {
        while (LoRa.available()) {
            incoming += (char)LoRa.read();
        }

        if (incomingLength != incoming.length()) {   // check length for error

#ifdef USE_DEBUG
            Serial.println("Error: message length does not match length");
#endif
            Bluetooth.println("Error: Message length does not match");
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
        Bluetooth.println("Meant for: 0x" + recipient);
        Bluetooth.println("Local address: 0x" + localAddress);
        Bluetooth.println("Received from: 0x" + sender);
        Bluetooth.println("Message: " + incoming);
        Bluetooth.println("RSSI: " + String(LoRa.packetRssi()));
        Bluetooth.println();
    }

    else {
#ifdef USE_DEBUG
        Serial.println("Message not meant for this device. Ignoring");
#endif
        Bluetooth.println("Meant for: 0x" + recipient);
        Bluetooth.println("Local address: 0x" + localAddress);
        Bluetooth.println("Received from: 0x" + sender);
        return;
    }
}


void sendMessage(String outgoing, uint64_t destination) {
    localUnion.localAddress = ESP.getEfuseMac();
    destUnion.destination = destination;
    LoRa.beginPacket();                   // start packet
    for (int i = 5; i>-1; --i) {
        LoRa.write(destUnion.destArray[i]); // add destination address
    }
    for (int i = 0; i<6; ++i) {
        LoRa.write(localUnion.localArray[i]); // Add local address
    }
    LoRa.write(outgoing.length());        // add payload length
    LoRa.print(outgoing);                 // add payload
    LoRa.endPacket(true);                     // finish packet and send it
}

void onReceive(int packetSize) {
    if (packetSize == 0) return;          // if there's no packet, return

    readMessage();
}
