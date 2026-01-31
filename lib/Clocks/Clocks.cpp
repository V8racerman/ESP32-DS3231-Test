
#include "SimpleAlarmClock.h"
#include "//home/alan/PlatformIO/Projects/Development/ESP32/ESP32 DS3231 Test/include/setgpio.h"
#include "/home/alan/PlatformIO/Projects/Development/ESP32/ESP32 DS3231 Test/include/external_variables.h"
#include "/home/alan/PlatformIO/Projects/Development/ESP32/ESP32 DS3231 Test/include/clock_constants.h"
#include "Clocks.h"
#include "GenUtils.h"

      /* I2C address can be found in the datasheet Figure 1. Device
         Address ZS-040 module has pull-up resistors on these pins
         giving them a default value of 1.
         Shorting an individual pad results in different address:
             pads      Binary    Hex    Dec
         | Default  | b1101111 | 0x57 | 87 |
         | short A0 | b1101110 | 0x56 | 86 |
         | short A1 | b1101101 | 0x55 | 85 |
         | short A2 | b1101011 | 0x53 | 83 |
         | All Short| b1101000 | 0x50 | 80 |
         allowing up to eight combinations                      */

/* instantiate SimpleAlarmClock */

SimpleAlarmClock extern Clock;

void Initial_Clock(void) {

 DateTime TempTime;
  
 TempTime.Second = (byte) 0;
 TempTime.Minute = (byte) 0;
 TempTime.Hour = (byte) 16;
 TempTime.Dow = (byte) 2;
 TempTime.Day = (byte) 5;
 TempTime.Month = (byte) 10;
 TempTime.Year = (byte) 20;
 TempTime.ClockMode = (byte)M24hr;
 Clock.write(TempTime);
}

void Setup_Clock(void) {

 DateTime TempTime;
 AlarmTime TempAlarm;
  
 TempTime = Clock.read();

  if (TempTime.ClockMode != M24hr) {
      TempTime.ClockMode = M24hr;
      Clock.write(TempTime);
  }

  TempAlarm = Clock.readAlarm(alarm1);
  if (TempAlarm.ClockMode != M24hr) {  
    TempAlarm.ClockMode = M24hr;
    Clock.setAlarm(TempAlarm, alarm1);
  }
  
  TempAlarm = Clock.readAlarm(alarm2);
  if (TempAlarm.ClockMode != M24hr) {
    TempAlarm.ClockMode = M24hr;
    Clock.setAlarm(TempAlarm, alarm2);
  }
}

/* ***********************************************************
 *            Edit Functions                                  *
 * ********************************************************* */

void changeHour(byte func, bool increment){
    /*  Increments or decrements the hour by one
     *    i = 0 Clock
     *      = 1 Alarm1
     *      = 2 Alarm2
     */
    AlarmTime local_alarm;
    DateTime NowTime;                  //create DateTime struct from Library
    int Hour;
    
    switch (func){
        case clock0:
            //Clock
            NowTime = Clock.read();        // get the latest clock values
            Hour = NowTime.Hour;
            break;
        case alarm1:
            //alarm1
            local_alarm = Clock.readAlarm(alarm1);
            Hour = local_alarm.Hour;
            break;
        case alarm2:
            //alarm2
            local_alarm = Clock.readAlarm(alarm2);
            Hour = local_alarm.Hour;
            break;
        default:
            //Default
            break;
    }
    if (increment == true){
        Hour++;
        Hour %= 24;
    } else {
        Hour--;
        Hour %= 24;
    }
    if (Hour < 0) { Hour = 23;}
    
    switch (func){
        case clock0:
            //Clock
            NowTime.Hour = byte(Hour);
            Clock.write(NowTime);
            break;
        case alarm1:
            //alarm1
            local_alarm.Hour = byte(Hour);
            Clock.setAlarm(local_alarm,alarm1);
            break;
        case alarm2:
            //alarm2
            local_alarm.Hour = Hour;
            Clock.setAlarm(local_alarm,alarm2);
            break;
        default:
            //Default
            break;
    }
}

void changeMinute(byte func, bool increment){
    /*  Increments or decrements the minute by one
     *  func = 0 Clock
     *       = 1 Alarm1
     *       = 2 Alarm2
     */
    AlarmTime alarm;
    DateTime NowTime;            //create DateTime struct from Library
    int Minute;

    switch (func){
        case clock0:
            //Clock
            NowTime = Clock.read();        // get the latest clock values
            Minute = NowTime.Minute;
            break;
        case alarm1:
            //alarm1
            alarm = Clock.readAlarm(alarm1);
            Minute = alarm.Minute;
            break;
        case alarm2:
            //alarm2
            alarm = Clock.readAlarm(alarm2);
            Minute = alarm.Minute;
            break;
        default:
            //Default
            break;
    }
    if (increment == true) {
        Minute++;
        Minute %= 60;
    } else {
        Minute--;
        Minute %= 60;
    }
    if (Minute < 0) { Minute = 59; }
    switch (func){
        case clock0:
            //Clock
            NowTime.Minute = byte(Minute);
            Clock.write(NowTime);
            break;
        case alarm1:
            //alarm1
            alarm.Minute = byte(Minute);
            Clock.setAlarm(alarm,alarm1);
            break;
        case alarm2:
            //alarm2
            alarm.Minute = byte(Minute);
            Clock.setAlarm(alarm,alarm2);
            break;
        default:
            //default
            break;
    }
}

void changeSecond(bool increment){
    /*  Increments or decrements the second by one   */
    
    DateTime NowTime;            //create DateTime struct
    int Second;

    NowTime = Clock.read();        // get the latest clock values
    Second = NowTime.Second;

    if (increment == true) {
        Second++;
        Second %= 60;
    } else {
        Second--;
        Second %= 60;
    }
    if (Second < 0) { Second = 59; }
    NowTime.Second = byte(Second);
    Clock.write(NowTime);
}

void changeDay(bool increment){
    DateTime NowTime;
    NowTime = Clock.read();
    int Day = NowTime.Day;
    byte Month = NowTime.Month;
    byte Year = NowTime.Year + 2000;
    byte DaysMax = 31;
    switch (Month){
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12:
            DaysMax = 31;
            break;
        case 2:
            DaysMax = 28;
            if (((Year % 4 == 0) && (Year % 100 != 0)) || ( Year % 400 == 0)){
                //those are the conditions to have a leap year
                DaysMax = 29;
            }
            break;
        case 4:
        case 6:
        case 9:
        case 11:
            DaysMax = 30;
            break;
        default:
            break;
    }
    //Serial.print("DaysMax = ");Serial.println(DaysMax);
    if (increment == true) { Day++; } else { Day--; }
    if (Day < 1) { Day = DaysMax; }
    if (Day > DaysMax){Day = 1;}
    //Serial.print("changeDay saved = "); Serial.println(Day);
    NowTime.Day = byte(Day);
    Clock.write(NowTime);
}

void changeMonth(bool increment){
    DateTime NowTime;
    NowTime = Clock.read();
    int Month = NowTime.Month;
    if (increment == true) {
        Month++;
    } else {
        Month--;
    }
    if (Month > 12) { Month = 1; }
    if (Month < 1) { Month = 12; }
    NowTime.Month = byte(Month);
    Clock.write(NowTime);
}

void changeYear(bool increment){
    DateTime NowTime;
    NowTime = Clock.read();
    int Year = NowTime.Year;
    if (increment == true) {
        Year++;
    } else {
        Year--;
    }
    if (Year < 18) { Year = 99; }
    if (Year > 99){ Year = 18; }
    NowTime.Year = byte(Year);
    Clock.write(NowTime);
}
void changeAlarmMode(byte alarm, bool increment){
    /*  Change AlarmMode to 0=Daily, 1=Weekday, 2=Weekend, 3=Once
     *    alarm = 1 Alarm1
     *          = 2 Alarm2
     */
     AlarmTime local_alarm;
     int AlarmMode;
     
    if ((alarm==alarm1)||(alarm==2)){
        local_alarm = Clock.readAlarm(alarm);
        AlarmMode = local_alarm.AlarmMode;

        if (increment == true) {
            AlarmMode--;
            AlarmMode %= 4;
        } else {
            AlarmMode++;
            AlarmMode %= 4;
        }
     
        if (AlarmMode < 0) { AlarmMode = 3; }
        local_alarm.AlarmMode = byte(AlarmMode);
        Clock.setAlarm(local_alarm,alarm);
    }
}

void changeAlarmState(byte alarm) {
    /*  Change Alarm State - enabled or disabled
     *    i = 1 Alarm1
     *      = 2 Alarm2
     */
     AlarmTime local_alarm;
     
    if ((alarm==alarm1)||(alarm==alarm2)){
        local_alarm = Clock.readAlarm(alarm);
        local_alarm.Enabled = !local_alarm.Enabled;
        Clock.setAlarm(local_alarm,alarm);
    }
}

 /* ***********************************************************
 *            Additional Alarm Functions                      *
 * ********************************************************* */

byte CheckAlarmStatus(){
    /* Returns:
     0 - No alarms
     1 - Alarm 1 enabled
     2 - Alarm 2 enabled
     3 - Both alarms enabled
    */
    bool AlarmStatus = digitalRead(SQW_PIN);
    byte flaggedAlarms = Clock.flaggedAlarms();

    //INTSQW is Active-Low Interrupt or Square-Wave Output
    if (AlarmStatus == LOW){
        //Alarm detected
        ClockState = Alarm;
    }
    return flaggedAlarms;
}

void clearAlarms(void){
    //Clear alarm flags
    Clock.clearAlarms();
    toggleLED(false, BLUE_LED);
   }

void Snooze(byte active){
    /* Begin the Clock.snoozealarm method to delay the alarm
    *  for SnoozePeriod.  It also clears alarm flags.
    */
    // active will have which alarm is active
    switch (active){
        case 0:
            //No flagged alarms
            break;
        case alarm1:
            //alarm 1
            Clock.snoozeAlarm(alarm1, SnoozePeriod);
            break;
        case alarm2:
            //alarm 2
            Clock.snoozeAlarm(alarm2, SnoozePeriod);
            break;
        case 3:
            //both alarms
            Clock.snoozeAlarm(alarm1, SnoozePeriod);
            Clock.snoozeAlarm(alarm2, SnoozePeriod);
            break;
        default:
        //do nothing
        break;
    }
    toggleLED(false, BLUE_LED);                  // Confirm LED turned off
 }



