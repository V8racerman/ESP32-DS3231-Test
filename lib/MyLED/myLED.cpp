#include <Arduino.h>
#include "myLED.h"

TM1637Display segment7(CLK, DIO);

void segment7_setup(uint8_t my_hr, uint8_t my_mn) {

segment7.setBrightness(0x05);
segment7_time(my_hr, my_mn, false);

}

void segment7_time(uint8_t m_hour, uint8_t m_min, bool colon) {

uint8_t segment7_brightness = 0x04;

switch ( m_hour ) {
  case ( 21, 22, 23, 0, 1, 2, 3, 4, 5, 6 ):
    segment7_brightness = 0x03;
    break;
  case (7, 8, 17, 18, 19, 20):
    segment7_brightness = 0x04;
    break;
  case (9, 10, 11, 12, 13, 14, 15, 16):
    segment7_brightness = 0x05;
    break;
  default:
    break;  
  }
  segment7.setBrightness(segment7_brightness);
  segment7.showNumberDecEx(m_min, COLON_OFF, true, 2, 2);
  if (!colon)
  {
    segment7.showNumberDecEx(m_hour, COLON_OFF, false, 2, 0);
  }
  else
  {
    segment7.showNumberDecEx(m_hour, COLON_ON, false, 2, 0);
  }
  digitalWrite(LED_BUILTIN, (uint8_t)colon);
}
