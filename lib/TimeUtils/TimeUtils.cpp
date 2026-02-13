#include "TimeUtils.h"
// #include "/home/alan/PlatformIO/Projects/Development/ESP32/Star Wars Clock/include/Clock_Defs.h"

void Time_Copy(DateTime *prev, DateTime *now) {
    
    prev->Hour      = now->Hour; 
    prev->Minute    = now->Minute;
    prev->Second    = now->Second;
    prev->Dow       = now->Dow;
    prev->ClockMode = now->ClockMode;
    prev->Day       = now->Day;
    prev->Month     = now->Month;
    prev->Year      = now->Year; 
}

void Alarm_Copy(AlarmTime *prev, AlarmTime *now) {
    
    prev->Hour      = now->Hour; 
    prev->Minute    = now->Minute;
    prev->Second    = now->Second;
    prev->AlarmMode = now->AlarmMode;
    prev->ClockMode = now->ClockMode;
    prev->Enabled   = now->Enabled;
}

DateTime internet_to_RTC(tm internet, byte clk_mode) {
    DateTime NowTime;
        
    NowTime.Hour      = internet.tm_hour; 
    NowTime.Minute    = internet.tm_min;
    NowTime.Second    = internet.tm_sec;
    NowTime.Day       = internet.tm_mday;
    NowTime.Month     = internet.tm_mon + 1;
    NowTime.Year      = (byte)(internet.tm_year - 100);
    NowTime.Dow      = internet.tm_wday + 1; 
    NowTime.ClockMode = (byte)clk_mode;
    return (NowTime);
}

bool british_summer_time(DateTime NowTime) {
  byte month = NowTime.Month;
  bool bst = false;

switch (month) {
  case 1:
  case 2:
  case 11:
  case 12:
    bst = false;
    break;
  case 4:
  case 5:
  case 6:
  case 7:
  case 8:
  case 9:
    bst = true;
    break;
  case 3:  
    if ((NowTime.Day - NowTime.Dow) >= (31 - 7)) { bst = true; } else { bst = false; }
    break;
  case 10:
    if ((NowTime.Dow != 1) || (NowTime.Hour > 1)) { 
      if ((NowTime.Day - NowTime.Dow) >= (31 - 7)) { bst = false; } else {bst = true; } 
    }
    else {bst = true; }
    break; 
  default:
    break;   
  }
return (bst);  
}