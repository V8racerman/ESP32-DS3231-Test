#ifndef Clocks_h
#define Clocks_h
#include <Arduino.h>
#include <WiFi.h>
#include "/home/alan/PlatformIO/Projects/Development/ESP32/ESP32 DS3231 Test/include/setgpio.h"

#include "TimeUtils.h"

const byte RTC_addr    = 0x68;               // I2C address of DS3231 RTC
const byte EEPROM_addr = 0x57;               // I2C address of AT24C32N EEPROM
const bool INTCN       = true;               // allows SQW pin to be monitored

void changeHour(byte func, bool increment);
void changeMinute(byte func, bool increment);
void changeSecond(bool increment);
void changeDay(bool increment);
void changeMonth(bool increment);
void changeYear(bool increment);
void changeAlarmMode(byte alarm, bool increment);
void changeAlarmState(byte alarm);
byte CheckAlarmStatus();
void clearAlarms(void);
void Snooze(byte active);
void Initial_Clock();
void Setup_Clock(void);
void internet_time(void);
void Update_temperature(float *temper);
void preset_Alarms(uint8_t My_hr, uint8_t My_min, uint8_t My_sec, uint8_t selected_alarm);

#endif