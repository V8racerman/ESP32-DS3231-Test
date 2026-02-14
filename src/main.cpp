#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>

#include "mygpio.h"
#include "SimpleAlarmClock.h"
#include "Clocks.h"
#include "TimeUtils.h"
#include "clock_constants.h"
#include "myOLED.h"
#include "myLED.h"
#include "Tones.h"
#include "keypad.h"
#include "GenUtils.h"

SimpleAlarmClock extern Clock;
SimpleAlarmClock Clock(RTC_addr, EEPROM_addr, INTCN);

float temperature = 20.0;

DateTime NowTime;
AlarmTime myAlarm1, myAlarm2;
byte previous_second = 0;
byte previous_minute = 0;
byte         cpIndex = 0;              // Cursor Position Index - used for edit mode
bool      changeFlag = true;
byte    ActiveAlarms = 0;
bool     globalflash = false;
unsigned long PreviousMillis;

States    ClockState = PowerLoss;
States     PrevState = ShowClock;

 /* ***********************************************************
 *            Operation Mode Functions                        *
 * ********************************************************* */

void Do_PowerLoss(void) {
  ClockState = ShowClock;
  Clock.clearOSFStatus();
}

void Do_ShowClock(byte command) {
  switch (command){
      case EDIT:
          // Edit clock
          ClockState = EditClock;
          break;
      case UP:
          //Edit alarm 1
          ClockState = EditAlarm1;
          break;
      case DOWN:
          //Edit Alarm 2
          ClockState = EditAlarm2;
          break;
      default:
          //do nothing
          break;
   } // end switch button
}

void Do_Alarm(byte command) {
  switch (command){
      case UP: 
      case DOWN:
          //Snooze alarm 
          Snooze(Clock.flaggedAlarms());
          ClockState = ShowClock;
          break;
      case EDIT:
      case SELECT:
          //turn off alarms
          clearAlarms();
          ClockState = ShowClock;
          break;
      default:
          //do nothing
          break;
      }
}

void Do_EditClock(byte command) {
    switch (command){
        case EDIT:
            //Increments cursor position
            //cpIndex += 1 % 6;
            cpIndex += 1;
            cpIndex %= 6;
            break;
        case DOWN:
            // Decrements value
            //                3  4    5
            // First Row dow dd/mm/yyyy
            //             0  1  2 
            // First Row  hh:mm:ss
            switch (cpIndex){
                case 0:
                    //edit Hours
                    changeHour(clock0, false);
                    break;
                case 1:
                    //edit Minute
                    changeMinute(clock0, false);
                    break;
                case 2: 
                    // edit Second    
                    changeSecond(false);
                    break;
                case 3:
                  //edit day
                    changeDay(false);
                    break;
                 case 4:
                    //edit month
                    changeMonth(false);
                    break;
                 case 5:
                    //edit year
                    changeYear(false);
                    break;
                 default:
                    //do nothing
                    break;
               } // end cpIndex switch
            break; // end DOWN case
        case UP:
            // Increments value
            switch (cpIndex){
                case 0:
                    //edit Hours
                    changeHour(clock0, true);
                    break;
                case 1:
                    //edit Minute
                    changeMinute(clock0, true);
                    break;
                case 2:
                    // edit Second
                    changeSecond(true);
                    break;
                case 3:    
                    //edit day
                    changeDay(true);
                    break;
                case 4:
                    //edit month
                    changeMonth(true);
                    break;
                case 5:
                    //edit year
                    changeYear(true);
                    break;
                default:
                    //do nothing
                    break;
                } // end cpIndex switch
            break; // end UP case
        case SELECT:
            ClockState = ShowClock;
            break;
        default:
            //do nothing
            break; 
        }// end button switch
  }

void Do_EditAlarm(byte alarm, byte command) {
  AlarmTime local_alarm;
  local_alarm = Clock.readAlarm(alarm);
    switch (command){
        case EDIT:
            //Increments cursor position
            cpIndex += 1;
            cpIndex %= 4;
            if (!local_alarm.Enabled) {cpIndex = 3; }
            break;
         case DOWN:
             // Decrements value      hh:mm AM Weekday
             //cpIndex is global
             switch (cpIndex){
                case 0:
                    //edit Hours
                    changeHour(alarm, false);
                    break;
                case 1:
                    //edit Minute
                    changeMinute(alarm, false);  
                    break;
                case 2:
                    //AlarmMode
                    changeAlarmMode(alarm, false);
                    break;
                case 3:
                    //Alarm on-off
                    changeAlarmState(alarm);
                    break;
                default:
                   //do nothing
                   break;
                }       
                break;
           case UP:
             // Increments value
             //cpIndex is global
             switch (cpIndex){
                case 0:
                    //edit Hours
                    changeHour(alarm, true);
                    break;
                case 1:
                    //edit Minute
                    changeMinute(alarm, true);
                    break;
                case 2:
                    //AlarmMode
                    changeAlarmMode(alarm, true);
                    break;
                case 3:
                    //Alarm on-off
                    changeAlarmState(alarm);
                    break;
                default:
                   //do nothing
                  break;
                }
            break;
        case SELECT:
            ClockState = ShowClock;
            break;     
        default:
           //do nothing
           break;
       }
}

void ButtonPress() {
 byte cmd;

  cmd = acd_key_press();
    
    if (cmd != NOTPRESSED) {
    //PowerLoss,ShowClock, Alarm, EditClock, EditAlarm1, EditAlarm2
    switch (ClockState) {
        case PowerLoss:
            Do_PowerLoss();
            break;
        case ShowClock:
            Do_ShowClock(cmd);
            break; 
        case Alarm:
            Do_Alarm(cmd);
            break;
        case EditClock:
            Do_EditClock(cmd);         
            break;
        case EditAlarm1:
            Do_EditAlarm(alarm1, cmd);
            break;
        case EditAlarm2:
            Do_EditAlarm(alarm2, cmd);  
            break;
        default:
            break;
        }
    }
}

void Display_Clock() {
  
  if (Update_Clock(&NowTime, &myAlarm1, &myAlarm2, &temperature)) {
    Update_Display(&NowTime, &myAlarm1, &myAlarm2, temperature);
    Alarm_LED();
    segment7_brightness(NowTime.Hour);
    previous_minute = NowTime.Minute;
    previous_second = NowTime.Second;
    segment7_time(NowTime.Hour, NowTime.Minute, true);
     delay(500);
     segment7_time(NowTime.Hour, NowTime.Minute, false);
     if (NowTime.Hour == 2 && NowTime.Minute == 5  && NowTime.Second == 30) { internet_time(); }
  }
}


void setup() {

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  pinMode(LED_LEFT, OUTPUT);
  digitalWrite(LED_LEFT, HIGH);
  pinMode(LED_RIGHT, OUTPUT);
  digitalWrite(LED_RIGHT, HIGH);
  pinMode(LED_MIDDLE, OUTPUT);
  digitalWrite(LED_MIDDLE, HIGH);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, HIGH);
  pinMode(SQW_PIN, INPUT_PULLUP);
  pinMode(DOWN_SW, INPUT);
  pinMode(UP_SW, INPUT);
  pinMode(EDIT_SW, INPUT);
  pinMode(SELECT_SW, INPUT);
 
  ledcSetup(LEDC_CHANNEL_0, LEDC_TONE_FREQ, LEDC_TIMER_2_BIT);
  ledcAttachPin(BUZZER_PIN, LEDC_CHANNEL_0);

  
  Wire.begin();
  Serial.begin(115200);
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
  PreviousMillis=millis();
 }

void loop() {

ButtonPress();

switch (ClockState) {
  case PowerLoss:
    if (ClockState != PrevState) { PrevState = ClockState; }
    Do_PowerLoss();
    Display_Clock();
    flashDisplay(&PreviousMillis, &globalflash);
    break;
  case ShowClock:
    if (ClockState != PrevState) {
      DisplayInvert();
      PrevState = ClockState; }
    Display_Clock();
    if (!digitalRead(SQW_PIN)) {
      ActiveAlarms = CheckAlarmStatus();  //Returns which alarms are activated
      ClockState = Alarm;
    }
    break;
  case Alarm:
    //Alarm mode
    // Serial.println("Do Alarm");
    if (ClockState != PrevState) { PrevState = ClockState; }
    Play_Alarm();
    // while (Play_Alarm()) { delay(1000); }
    NowTime = Clock.read();
    if (NowTime.Second != previous_second) {
      previous_second = NowTime.Second;
      Update_Display(&NowTime, &myAlarm1, &myAlarm2, temperature);
      flashDisplay(&PreviousMillis, &globalflash);
      if (NowTime.Minute != previous_minute) {
        previous_minute = NowTime.Minute;
        segment7_time(NowTime.Hour, NowTime.Minute, true);
      } 
      if (digitalRead(SQW_PIN)) { ClockState = ShowClock; }        
      break;
    case EditClock:
      //Edit ClockMode
      if (ClockState != PrevState) { 
        cpIndex = 0; 
        DisplayInvert();
        PrevState = ClockState;
        }
      editClock(cpIndex, &NowTime, &myAlarm1, &myAlarm2, temperature);
      Display_Clock();
      break;
    case EditAlarm1:
      //Edit Alarm1
      //Serial.println("Do Edit Alarm One");
      if (ClockState != PrevState) { 
        cpIndex = 0; 
        DisplayInvert();
        PrevState = ClockState;
        }
      editAlarm(alarm1, cpIndex, &NowTime, &myAlarm1, &myAlarm2, temperature);
      Display_Clock();
      break;
    case EditAlarm2:
      //Edit Alarm2
      //Serial.println("Do Edit Alarm Two");
      if (ClockState != PrevState) { 
        cpIndex = 0; 
        DisplayInvert();
        PrevState = ClockState;
        }
      editAlarm(alarm2, cpIndex, &NowTime, &myAlarm1, &myAlarm2, temperature);
      Display_Clock();
      break;
    default:
      break;
    }
  }  
}