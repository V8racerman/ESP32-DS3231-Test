#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "setgpio.h"
#include "SimpleAlarmClock.h"
#include "Clocks.h"
#include "TimeUtils.h"
#include "clock_constants.h"
#include "my_serial.h"
#include "myOLED.h"
#include "myLED.h"

const int flashInterval = 500;         // Alarm flashing interval

bool pmFlag, h12Flag, century = false;
byte alarmDay, alarmHour, alarmMinute, alarmSecond, alarmBits;
bool alarmDy, alarmH12Flag, alarmPmFlag;

SimpleAlarmClock extern Clock;
SimpleAlarmClock Clock(RTC_addr, EEPROM_addr, INTCN);

DateTime NowTime;
AlarmTime myAlarm1, myAlarm2;
byte previous_second = 0;

float temperature = 20.0;

void setup() {

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(DOWN_SW, INPUT);
  pinMode(UP_SW, INPUT);
  pinMode(EDIT_SW, INPUT);
  pinMode(SELECT_SW, INPUT);

  Wire.begin();
  Serial.begin(115200);
  NowTime = Clock.read();
  segment7_setup(NowTime.Hour, NowTime.Minute);
  setup_OLED();
  internet_time();
  previous_second = NowTime.Second;
  preset_Alarms(10, 0, 0, alarm1);
  preset_Alarms(17, 50, 0, alarm2);
  Update_temperature(&temperature);
  myAlarm1 = Clock.readAlarm(alarm1);
  myAlarm2 = Clock.readAlarm(alarm2);
}

void loop()
{

  NowTime = Clock.read();
  if (NowTime.Second != previous_second) {
    previous_second = NowTime.Second;
    my_serial_time(NowTime);
    Update_Display(&NowTime, &myAlarm1, &myAlarm2, temperature);
    // my_serial_monitor(NowTime, h12Flag, pmFlag, true);
    // my_serial_time(NowTime);
    segment7_time(NowTime.Hour, NowTime.Minute, true);
    delay(500);
    segment7_time(NowTime.Hour, NowTime.Minute, false);
    if (NowTime.Hour == 18 && NowTime.Minute == 55  && NowTime.Second == 0) { internet_time(); }
    if (NowTime.Second == 0) { Update_temperature(&temperature); }
  }
}