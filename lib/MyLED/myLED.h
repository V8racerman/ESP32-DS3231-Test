#ifndef My_LED_h
#define My_LED_h
#include <Arduino.h>
#include <TM1637Display.h>
#include "mygpio.h"


#define COLON_ON  0b01000000
#define COLON_OFF 0b00000000

void segment7_setup(uint8_t m_hr, uint8_t m_mn);
void segment7_time(uint8_t m_hour, uint8_t m_min, bool colon);
void segment7_brightness(uint8_t m_hour);

#endif
