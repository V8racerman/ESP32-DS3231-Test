#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>

// #include <Adafruit_GFX.h>
// #include <Adafruit_SSD1306.h>
#include "setgpio.h"
#include "SimpleAlarmClock.h"
#include "Clocks.h"
#include "TimeUtils.h"
#include "clock_constants.h"
#include "myOLED.h"
#include "myLED.h"
#include "Tones.h"

SimpleAlarmClock extern Clock;
SimpleAlarmClock Clock(RTC_addr, EEPROM_addr, INTCN);

float temperature = 20.0;

DateTime NowTime;
AlarmTime myAlarm1, myAlarm2;
byte previous_second = 0;
byte previous_minute = 0;
// byte          cpIndex = 0;              // Cursor Position Index - used for edit mode
bool          changeFlag = true;

States ClockState = PowerLoss;
States PrevState = ShowClock;

void setup() {

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(DOWN_SW, INPUT);
  pinMode(UP_SW, INPUT);
  pinMode(EDIT_SW, INPUT);
  pinMode(SELECT_SW, INPUT);
  pinMode(SQW_PIN, INPUT_PULLUP);
  
  Wire.begin();
  Serial.begin(115200);
  Setup_Tone();
  clearAlarms();
  preset_Alarms(10,  0, 0, alarm1);
  preset_Alarms(17, 50, 0, alarm2);
  Update_temperature(&temperature);
  NowTime = Clock.read();
  myAlarm1 = Clock.readAlarm(alarm1);
  myAlarm2 = Clock.readAlarm(alarm2);
  segment7_setup(NowTime.Hour, NowTime.Minute);
  setup_OLED();
  Update_Display(&NowTime, &myAlarm1, &myAlarm2, temperature);
  internet_time();
  Alarm_LED();
 }

void loop() {

  if (!digitalRead(SQW_PIN)) {
    
    Play_Alarm();
    NowTime = Clock.read();
    if (NowTime.Second != previous_second) {
      previous_second = NowTime.Second;
      Update_Display(&NowTime, &myAlarm1, &myAlarm2, temperature);
      if (NowTime.Minute != previous_minute) {
        previous_minute = NowTime.Minute;
        segment7_time(NowTime.Hour, NowTime.Minute, true);
      }
    }
    } else { 
      if (Update_Clock(&NowTime, &myAlarm1, &myAlarm2, &temperature)) {
        Update_Display(&NowTime, &myAlarm1, &myAlarm2, temperature);
        previous_minute = NowTime.Minute;
        previous_second = NowTime.Second;
        segment7_time(NowTime.Hour, NowTime.Minute, true);
        delay(500);
        segment7_time(NowTime.Hour, NowTime.Minute, false);
        if (NowTime.Hour == 2 && NowTime.Minute == 5  && NowTime.Second == 30) { internet_time(); }
    }
  }
}