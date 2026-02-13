#ifndef HL_CLOCK
#define HL_CLOCK
#include <Arduino.h>
#include <time.h>
#include "SimpleAlarmClock.h"

// #include </home/alan/PlatformIO/Projects/Development/ESP32/Star Wars Clock/include/setgpio.h>

void Time_Copy(DateTime *prev, DateTime *now);
void Alarm_Copy(AlarmTime *prev, AlarmTime *now);  
DateTime internet_to_RTC(tm internet, byte clk_mode);
bool british_summer_time(DateTime NowTime);

#endif
