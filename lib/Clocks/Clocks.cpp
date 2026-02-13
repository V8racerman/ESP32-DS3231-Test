
#include "SimpleAlarmClock.h"
#include "/home/alan/PlatformIO/Projects/Development/ESP32/ESP32 DS3231 Test/include/external_variables.h"
#include "/home/alan/PlatformIO/Projects/Development/ESP32/ESP32 DS3231 Test/include/clock_constants.h"
#include "Clocks.h"
#include "GenUtils.h"
#include "/home/alan/PlatformIO/credentials.h"

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

const char* ntpServer  = "pool.ntp.org";
const long  gmtOffset_sec = 0;

/* instantiate SimpleAlarmClock */

SimpleAlarmClock extern Clock;

void Update_temperature(float *temper) {
    
    *temper = Clock.getTemperatureFloat() - 1.5;
}

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

void internet_time(void) {

  DateTime NowTime;
  struct tm current_time;
  int daylightOffset_sec = 0; // 3600;
  bool bst = false;
  bool old_bst = true;
  int attempts = 12;
  int attempt = 0;
  int connected_to_wifi;
  int connected_to_remote_server = 0;

  // connect to WiFi
  digitalWrite(LED_BUILTIN, HIGH);
  digitalWrite(GREEN_LED_LEFT, LOW);
  digitalWrite(GREEN_LED_RIGHT, LOW);
  Serial.print("Connecting to ");
  Serial.print(ssid);
  WiFi.begin(ssid, password);
  connected_to_wifi = WiFi.status();
  while ((connected_to_wifi != WL_CONNECTED) && (attempt < attempts))
  {
    // Display_Clock();
    Serial.print(".");
    delay(500);
    attempt++;
    connected_to_wifi = WiFi.status();
  }

  if (connected_to_wifi != WL_CONNECTED)
  {
    digitalWrite(GREEN_LED_LEFT, HIGH);
    Serial.println(" ");
  }
  else
  {
    attempt = 0;
    Serial.println(" CONNECTED.  Initialising time reference ...");
    // init and get the time
    NowTime = Clock.read();
    bst = british_summer_time(NowTime);
    if (bst)
    {
      daylightOffset_sec = 3600;
    }
    else
    {
      daylightOffset_sec = 0;
    }
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    connected_to_remote_server = getLocalTime(&current_time);
    while ((!connected_to_remote_server) && (attempt < attempts))
    {
      Serial.println("Unable to connect to remote time server.  Retrying ...");
      // Display_Clock();
      delay(500);
      attempt++;
      configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
      connected_to_remote_server = getLocalTime(&current_time);
    }
    if (!connected_to_remote_server)
    {
      digitalWrite(GREEN_LED_RIGHT, HIGH);
    }
    else
    {
      old_bst = bst;
      NowTime = internet_to_RTC(current_time, M24hr);
      bst = british_summer_time(NowTime);
      if (bst != old_bst)
      {
        attempt = 0;
        if (bst)
        {
          daylightOffset_sec = 3600;
        }
        else
        {
          daylightOffset_sec = 0;
        }
        configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
        connected_to_remote_server = getLocalTime(&current_time);
        while ((!connected_to_remote_server) && (attempt < attempts))
        {
          Serial.println("Unable to connect to remote time server.  Retrying ...");
          // Display_Clock();
          delay(500);
          attempt++;
          configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
          connected_to_remote_server = getLocalTime(&current_time);
        }
      }
    }
    if (!connected_to_remote_server) {
      digitalWrite(GREEN_LED_RIGHT, HIGH);
    }
    else {
      // NowTime = Clock.read();
      NowTime = internet_to_RTC(current_time, M24hr);
      old_bst = bst;
      bst = british_summer_time(NowTime);
      Clock.write(NowTime);
      Serial.print("Time reference initialised. ");
      digitalWrite(GREEN_LED_RIGHT, LOW);
    }
    // disconnect WiFi as it's no longer needed
    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);
    Serial.println("Wi-Fi disconnected and turned off.");
  }
  digitalWrite(BLUE_LED, LOW);
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

void preset_Alarms(uint8_t My_hr, uint8_t My_min, uint8_t My_sec, uint8_t selected_alarm) {

    AlarmTime TempAlarm;

    TempAlarm.Hour = My_hr;
    TempAlarm.Minute = My_min;
    TempAlarm.Second = My_sec;
    TempAlarm.Enabled = true;
    TempAlarm.AlarmMode = 0;
    TempAlarm.ClockMode = 2;
    Clock.setAlarm(TempAlarm, selected_alarm);
}

bool Update_Clock(DateTime *NT, AlarmTime *A1, AlarmTime *A2, float *xtemp) {
  /* ***************************************************** *
   * Display clock - skips update if there are no changes
   *
   * Parameters:
   *   changeFlag - true forces display refresh
   *              - false does nothing
   * ***************************************************** */
    
   float PreviousTemperature;
    DateTime NowTime;            //create DateTime struct from Library
    AlarmTime alarm_one;
    AlarmTime alarm_two;
    bool change_flag = false;
    
    NowTime = Clock.read();
    alarm_one = Clock.readAlarm(alarm1);
    alarm_two = Clock.readAlarm(alarm2);
    //Alarm_Check(alarm_one, alarm_two);
    //Clock_Check(NowTime);

    // check for temperature change every 60 seconds
    if (NowTime.Second == 30) {
        PreviousTemperature = *xtemp;
        *xtemp = Clock.getTemperature() -1.5;
        if (*xtemp != PreviousTemperature) {change_flag = true;}
    }

    // Check for Time change
    change_flag = check_for_change(&NowTime, NT, &alarm_one, A1, &alarm_two, A2);
    //Update Display - Only change display if change is detected
    if (change_flag == true){
        Time_Copy(NT, &NowTime);
        Alarm_Copy(A1, &alarm_one);
        Alarm_Copy(A2, &alarm_two);
        }
  return (change_flag);    
}

bool check_for_change(DateTime *NT, DateTime *PNT, AlarmTime *A1, AlarmTime *PA1, AlarmTime *A2, AlarmTime *PA2) {
  
    if (NT->Second != PNT->Second){ return (true); }
    if (NT->Minute != PNT->Minute){ return (true); }
    if (NT->Hour != PNT->Hour){ return (true); }
    if (NT->Day != PNT->Day) { return (true); }
    if (NT->Month != PNT->Month) { return (true); }
    if (NT->Year != PNT->Year) { return (true); }

    //Check for Alarm1 Change
    if (A1->Hour != PA1->Hour){ return (true); }
    if (A1->Minute != PA1->Minute){ return (true);}
    if (A1->ClockMode != PA1->ClockMode) { return (true); }
    if (A1->AlarmMode != PA1->AlarmMode) { return (true); }
    if (A1->Enabled != PA1->Enabled) { return (true); }

    //Check for Alarm2 Change
    if (A2->Hour != PA2->Hour){ return (true); }
    if (A2->Minute != PA2->Minute){ return (true); }
    if (A2->ClockMode != PA2->ClockMode) { return (true); }
    if (A2->AlarmMode != PA2->AlarmMode) { return (true); }
    if (A2->Enabled != PA2->Enabled) { return (true); }
    
 return (false); 
}

void Alarm_LED(void) {

 switch (Clock.alarmStatus()) {
    case 0:
        digitalWrite(LED_LEFT, HIGH);
        digitalWrite(LED_RIGHT, HIGH);
        break;
    case 1:
        digitalWrite(LED_LEFT, LOW);
        digitalWrite(LED_RIGHT, HIGH);
        break; 
    case 2:
        digitalWrite(LED_LEFT, HIGH);
        digitalWrite(LED_RIGHT, LOW);
        break; 
    case 3:
        digitalWrite(LED_LEFT, LOW);
        digitalWrite(LED_RIGHT, LOW);
        break;
    default:
        break;      

 }

}

