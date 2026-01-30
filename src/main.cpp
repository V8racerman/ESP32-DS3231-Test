#include <Arduino.h>

/*
DS3231_test.pde
Eric Ayars
4/11

Test/demo of read routines for a DS3231 RTC.

*/

#include <DS3231.h>
#include <Wire.h>
#include <WiFi.h>

DS3231 myRTC;
bool century = false;
bool h12Flag;
bool pmFlag;
byte alarmDay, alarmHour, alarmMinute, alarmSecond, alarmBits;
bool alarmDy, alarmH12Flag, alarmPmFlag;

void setClock(void)
{
myRTC.turnOffAlarm(1);
myRTC.turnOffAlarm(2);
myRTC.enableOscillator(false, false, 0);
myRTC.enable32kHz(false);
//myRTC.setSecond(0);
//myRTC.setMinute(29);
//myRTC.setHour(15);
//myRTC.setDate(30);
//myRTC.setMonth(1);
//myRTC.setYear(26);
}

void setup() {

pinMode(LED_BUILTIN, OUTPUT);
digitalWrite(LED_BUILTIN, HIGH);
// Start the I2C interface
Wire.begin();

// Start the serial interface
Serial.begin(115200);
setClock();
}

void loop()	{

digitalWrite(2, !digitalRead(2));
// send what's going on to the serial monitor.

// Start with the year
Serial.print("2");
if (century) { Serial.print("1"); } else { Serial.print("0"); }  // Won't need this for 89 years. 
Serial.print(myRTC.getYear(), DEC);
Serial.print(' ');

// then the month
Serial.print(myRTC.getMonth(century), DEC);
Serial.print(" ");

// then the date
Serial.print(myRTC.getDate(), DEC);
Serial.print(" ");

// and the day of the week
Serial.print(myRTC.getDoW(), DEC);
Serial.print(" ");

// Finally the hour, minute, and second
Serial.print(myRTC.getHour(h12Flag, pmFlag), DEC);
Serial.print(" ");
Serial.print(myRTC.getMinute(), DEC);
Serial.print(" ");
Serial.print(myRTC.getSecond(), DEC);

// Add AM/PM indicator
if (h12Flag) { if (pmFlag) { Serial.print(" PM "); } else {	Serial.print(" AM "); }	}
else { Serial.print(" 24h "); }

// Display the temperature
Serial.print("T=");
Serial.print(myRTC.getTemperature(), 2);

// Tell whether the time is (likely to be) valid
if (myRTC.oscillatorCheck()) { Serial.print(" O+");	} else { Serial.print(" O-"); }

// Indicate whether an alarm went off
if (myRTC.checkIfAlarm(1)) { Serial.print(" A1!"); }
if (myRTC.checkIfAlarm(2)) { Serial.print(" A2!"); }

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
Serial.println();
delay(500);
digitalWrite(LED_BUILTIN, LOW);
}