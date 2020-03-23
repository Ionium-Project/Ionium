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

extern byte localAddress; // = 0xBB;     // address of this device
extern byte destination; // = 0xFF;      // destination to send to
extern int messageCount;
void readMessage();
void sendMessage( String);
void onReceive( int);
#endif
