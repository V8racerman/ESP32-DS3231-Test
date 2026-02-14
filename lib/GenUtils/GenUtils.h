#ifndef GenUtils_H
#define GenUtils_H
#include <Arduino.h>
#include "keypad.h"
#include "mygpio.h"

void toggleLED(bool ledON, byte my_led);
byte acd_key_press(void);

#endif