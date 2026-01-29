#include <Arduino.h>

/*
DS3231_test.pde
Eric Ayars
4/11

Test/demo of read routines for a DS3231 RTC.

Turn on the serial monitor after loading this to check if things are
working as they should.

*/

#include <DS3231.h>
#include <Wire.h>

/*
* External event counter
*/

#include "driver/pcnt.h"

const int INPUT_PIN = 23; 
const int16_t THRESHOLD = 1000; // Trigger interrupt near 16-bit limit
volatile long totalOverflows = 0;
volatile bool tik = false;
volatile bool tok = false;

void IRAM_ATTR pcnt_intr_handler(void *arg) {
  // uint32_t status;
  // pcnt_get_event_status(PCNT_UNIT_0, &status);
  // if (status & PCNT_EVT_H_LIM) {
  // totalOverflows++; // Track total pulses beyond 16-bit limit
  // pcnt_counter_clear(PCNT_UNIT_0);
  tik = true;
  tok = !tok;
}

DS3231 myRTC;
bool century = false;
bool h12Flag;
bool pmFlag;
byte alarmDay, alarmHour, alarmMinute, alarmSecond, alarmBits;
bool alarmDy, alarmH12Flag, alarmPmFlag;

void setClock(void) {
	myRTC.turnOffAlarm(1);
	myRTC.turnOffAlarm(2);
	myRTC.enableOscillator(true, false, 0);
	myRTC.enable32kHz(true);
	// myRTC.setSecond(0);
	// myRTC.setMinute(13);
	// myRTC.setHour(16);
	// myRTC.setMonth(1);
	// myRTC.setYear(26);
}

void setup() {

  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
  // Start the I2C interface
  Wire.begin();
 
  // Start the serial interface
  Serial.begin(9600);
  setClock();

  // Setup interrupt
  pcnt_config_t pcnt_config = {
    .pulse_gpio_num = INPUT_PIN,
    .ctrl_gpio_num = -1,          // No control pin needed
    .lctrl_mode = PCNT_MODE_KEEP,
    .hctrl_mode = PCNT_MODE_KEEP,
    .pos_mode = PCNT_COUNT_DIS,   // Ignore /// Count up on rising edge
    .neg_mode = PCNT_COUNT_INC,   // Count up /// Ignore falling edge
    .counter_h_lim = THRESHOLD,
    .unit = PCNT_UNIT_0,
    .channel = PCNT_CHANNEL_0,
   };

  pcnt_unit_config(&pcnt_config);
  pcnt_event_enable(PCNT_UNIT_0, PCNT_EVT_H_LIM);
  pcnt_isr_register(pcnt_intr_handler, NULL, 0, NULL);
  pcnt_intr_enable(PCNT_UNIT_0);
  
  pcnt_counter_pause(PCNT_UNIT_0);
  pcnt_counter_clear(PCNT_UNIT_0);
  pcnt_counter_resume(PCNT_UNIT_0);
}

void loop() {

	if (tik) {// Display Alarm 2 information
		tik = false;
		digitalWrite(2, !digitalRead(2)); 
  		pcnt_counter_pause(PCNT_UNIT_0);
  		pcnt_counter_clear(PCNT_UNIT_0);
  		pcnt_counter_resume(PCNT_UNIT_0);
	}

	/*
		// send what's going on to the serial monitor.
	
		// Start with the year
		Serial.print("2");
		if (century) {			// Won't need this for 89 years.
			Serial.print("1");
		} else {
			Serial.print("0");
		}
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
		if (h12Flag) {
			if (pmFlag) {
				Serial.print(" PM ");
			} else {
				Serial.print(" AM ");
			}
		} else {
			Serial.print(" 24h ");
		}
 
		// Display the temperature
		Serial.print("T=");
		Serial.print(myRTC.getTemperature(), 2);
  
		// Tell whether the time is (likely to be) valid
		if (myRTC.oscillatorCheck()) {
			Serial.print(" O+");
		} else {
			Serial.print(" O-");
		}
 
		// Indicate whether an alarm went off
		if (myRTC.checkIfAlarm(1)) {
			Serial.print(" A1!");
		}
 
		if (myRTC.checkIfAlarm(2)) {
			Serial.print(" A2!");
		}
 
		// New line on display
		Serial.println();
	
		// Display Alarm 1 information
		Serial.print("Alarm 1: ");
		myRTC.getA1Time(alarmDay, alarmHour, alarmMinute, alarmSecond, alarmBits, alarmDy, alarmH12Flag, alarmPmFlag);
		Serial.print(alarmDay, DEC);
		if (alarmDy) {
			Serial.print(" DoW");
		} else {
			Serial.print(" Date");
		}
		Serial.print(' ');
		Serial.print(alarmHour, DEC);
		Serial.print(' ');
		Serial.print(alarmMinute, DEC);
		Serial.print(' ');
		Serial.print(alarmSecond, DEC);
		Serial.print(' ');
		if (alarmH12Flag) {
			if (alarmPmFlag) {
				Serial.print("pm ");
			} else {
				Serial.print("am ");
			}
		}
		if (myRTC.checkAlarmEnabled(1)) {
			Serial.print("enabled");
		}
		Serial.println();
 
		Serial.print("Alarm 2: ");
		myRTC.getA2Time(alarmDay, alarmHour, alarmMinute, alarmBits, alarmDy, alarmH12Flag, alarmPmFlag);
		Serial.print(alarmDay, DEC);
		if (alarmDy) {
			Serial.print(" DoW");
		} else {
			Serial.print(" Date");
		}
		Serial.print(" ");
		Serial.print(alarmHour, DEC);
		Serial.print(" ");
		Serial.print(alarmMinute, DEC);
		Serial.print(" ");
		if (alarmH12Flag) {
			if (alarmPmFlag) {
				Serial.print("pm");
			} else {
				Serial.print("am");
			}
		}
		if (myRTC.checkAlarmEnabled(2)) {
			Serial.print("enabled");
		}
 
		// display alarm bits
  	Serial.println();
	Serial.print("Alarm bits: ");
	Serial.println(alarmBits, BIN);
	Serial.println();
  	// if (tok) { digitalWrite(2, HIGH); } else { digitalWrite(2, LOW); }
    }
	*/
}