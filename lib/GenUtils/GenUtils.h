#ifndef GenUtils_H
#define GenUtils_H
#include <Arduino.h>
#include "/home/alan/PlatformIO/Projects/Development/ESP32/ESP32 DS3231 Test/include/Keypad.h"
#include "/home/alan/PlatformIO/Projects/Development/ESP32/ESP32 DS3231 Test/include/setgpio.h"

void toggleLED(bool ledON, byte my_led);
byte acd_key_press(void);

#endif