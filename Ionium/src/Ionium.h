#ifndef IONIUM_H
#define IONIUM_H
#include <BluetoothSerial.h>

#ifdef ARDUINO_AVR_UNO
#define Bluetooth Serial
#elif ARDUINO_AVR_MEGA2560
#define Bluetooth Serial1
#elif ARDUINO_ARCH_ESP32
extern BluetoothSerial Bluetooth;
#endif

extern uint32_t addressPart1;
extern uint32_t addressPart2;
extern String localAddress;
extern union {
    uint8_t destArray[8];
    uint64_t destination;
} destUnion;
extern union {
    uint8_t recArray[8];
    uint64_t recipient;
} recUnion;
extern union {
    uint8_t localArray[8];
    uint64_t localAddress;
} localUnion;
extern union {
    uint8_t senArray[8];
    uint64_t sender;
} senUnion;
extern int messageCount;
void readMessage();
void sendMessage( String);
void onReceive( int);
#endif
