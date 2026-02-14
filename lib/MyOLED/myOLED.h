#ifndef my_OLED_h
#define my_OLED_h

#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include "mygpio.h"
#include "external_variables.h"
#include "clock_constants.h"
#include "SimpleAlarmClock.h"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Display Character Placement Map
const char degree     = 0xF8;   // extended character 248 - degree symbol
const char block_char = 0xDB;   // extendeded character - block character

#define CHAR_WIDTH     5
#define CHAR_HEIGHT    8

const byte row1    = 0;
const byte row2    = 2*(CHAR_HEIGHT + 2) + 1;
const byte row3    = (2+3)*(CHAR_HEIGHT +2) - 1;
const byte row4    = row3 + CHAR_HEIGHT;
const byte row1_c1 = 0;
const byte row1_c2 = 4*2*CHAR_WIDTH;
const byte row1_c3 = 6*2*CHAR_WIDTH;
const byte row1_c4 = 7*2*CHAR_WIDTH;
const byte row1_c5 = 9*2*CHAR_WIDTH;
const byte row1_c6 = 10*2*CHAR_WIDTH;
const byte row2_c1 = 0;
const byte row2_c2 = 2*3*CHAR_WIDTH;
const byte row2_c3 = 3*3*CHAR_WIDTH;
const byte row2_c4 = 5*3*CHAR_WIDTH;
const byte row2_c5 = 6*3*CHAR_WIDTH;
const byte row3_c1 = 0;
const byte row3_c2 = 2*CHAR_WIDTH;
const byte row3_c3 = 3*CHAR_WIDTH;
const byte row3_c4 = 7*CHAR_WIDTH;
const byte row3_c5 = 17*CHAR_WIDTH;
const byte row4_c1 = 0;
const byte row4_c2 = 2*CHAR_WIDTH;
const byte row4_c3 = 3*CHAR_WIDTH;
const byte row4_c4 = 7*CHAR_WIDTH;



char* mode2str(byte bMode);
char* dow2Str(byte bDow);
void twoDigitPrint(byte number);
void Update_Display(DateTime *clk, AlarmTime *A1, AlarmTime *A2, int Temperature);
void setup_OLED(void);
void editAlarm(byte alarm, byte index, DateTime *myTime, AlarmTime *myalarm1, AlarmTime *myalarm2, float temperature);
void editClock(byte index, DateTime *myTime, AlarmTime *myalarm1, AlarmTime *myalarm2, float temperature);
void flashDisplay(unsigned long *prev_millis, bool *state);
void DisplayInvert(void);

#endif