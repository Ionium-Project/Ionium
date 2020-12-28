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
extern int messageCount;
void readMessage();
void sendMessage( String, uint64_t);
void onReceive( int);
#endif
