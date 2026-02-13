#ifndef clock_constants_h
#define clock_constants_h
#include <Arduino.h>

const byte SnoozePeriod = 5;            // Snooze value, in minutes
//Alarm types:
const byte Daily   = 0;
const byte Weekday = 1;
const byte Weekend = 2;
const byte Once    = 3;
//Clock Modes:
const byte M24hr   = 2;
//Clocks
const byte clock0  = 0;
const byte alarm1  = 1;
const byte alarm2  = 2;

#endif