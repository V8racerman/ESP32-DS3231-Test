#include "my_serial.h"


void my_serial_monitor(DateTime NowT, bool h12Flagx, bool pmFlagx, bool monitor_enable) {

if (monitor_enable) { 
  // Start with the year
  Serial.print("2");
  Serial.print("0"); 
  Serial.print(NowT.Year, DEC);
  Serial.print(' ');

  // then the month
  Serial.print(NowT.Month, DEC);
  Serial.print(" ");

  // then the date
  Serial.print(NowT.Day, DEC);
  Serial.print(" ");

  // and the day of the week
  Serial.print(dow_str[NowT.Dow]);
  Serial.print(" ");

  // Finally the hour, minute, and second
  Serial.print(NowT.Hour);
  Serial.print(":");
  Serial.print(NowT.Minute);
  Serial.print(":");
  Serial.print(NowT.Second, DEC);

  // Add AM/PM indicator
  if (h12Flagx) { if (pmFlagx) { Serial.print(" PM "); } else {	Serial.print(" AM "); }	}
  else { Serial.print(" 24h "); }
  Serial.println();

// Display the temperature
// Serial.print("T=");
// Serial.print(myRTC.getTemperature(), 2);

// Tell whether the time is (likely to be) valid
// if (myRTC.oscillatorCheck()) { Serial.print(" O+");	} else { Serial.print(" O-"); }

// Indicate whether an alarm went off
// if (myRTC.checkIfAlarm(1)) { Serial.print(" A1!"); }// 
// if (myRTC.checkIfAlarm(2)) { Serial.print(" A2!"); }
/*
// Display Alarm 1 information
Serial.println();
Serial.print("Alarm 1: ");
myRTC.getA1Time(alarmDay, alarmHour, alarmMinute, alarmSecond, alarmBits, alarmDy, alarmH12Flag, alarmPmFlag);
Serial.print(alarmDay, DEC);
if (alarmDy) { Serial.print(" DoW"); } else { Serial.print(" Date"); }
Serial.print(' ');
Serial.print(alarmHour, DEC);
Serial.print(' ');
Serial.print(alarmMinute, DEC);
Serial.print(' ');
Serial.print(alarmSecond, DEC);
Serial.print(' ');
if (alarmH12Flag) { if (alarmPmFlag) { Serial.print("pm ");	} else { Serial.print("am "); }	}
if (myRTC.checkAlarmEnabled(1))	{ Serial.print("enabled"); }
Serial.println();
Serial.print("Alarm 2: ");
myRTC.getA2Time(alarmDay, alarmHour, alarmMinute, alarmBits, alarmDy, alarmH12Flag, alarmPmFlag);
Serial.print(alarmDay, DEC);
if (alarmDy) { Serial.print(" DoW"); } else { Serial.print(" Date"); }
Serial.print(" ");
Serial.print(alarmHour, DEC);
Serial.print(" ");
Serial.print(alarmMinute, DEC);
Serial.print(" ");
if (alarmH12Flag) {	if (alarmPmFlag) { Serial.print("pm"); } else { Serial.print("am");	} }
if (myRTC.checkAlarmEnabled(2))	{ Serial.print("enabled"); }

// display alarm bits
Serial.println();
Serial.print("Alarm bits: ");
Serial.println(alarmBits, BIN);
*/
  }
}