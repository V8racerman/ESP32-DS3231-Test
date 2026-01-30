#ifndef Clocks_h
#define Clocks_h
#include <Arduino.h>

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

#endif