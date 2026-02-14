#include <Arduino.h>
#include "Tones.h"
#include "mygpio.h"

const int number_of_melodies = 5;
const int loop_max = 5;

  int nx = 0;
  int melody_index = 0;
  int loop_counter = 0;

 // ESP32 version of the Arduino Beep function - modified to work with alarm
int beep(int note, int duration, int counter) {

      if (note !=0) {
      //Play tone on buzzerPin
      ledcWriteTone(LEDC_CHANNEL_0, (double)note);
    }
   
      //Play different LED depending on value of 'counter'
      if(counter % 2 == 0)
      {
      digitalWrite(LED_RIGHT, HIGH);
      delay(duration);
      digitalWrite(LED_RIGHT, LOW);
      } else {
      digitalWrite(LED_LEFT, HIGH);
      delay(duration);
      digitalWrite(LED_LEFT, LOW);
      }
      //Stop tone on buzzerPin
      ledcWrite(LEDC_CHANNEL_0,0);
     
      //Increment counter
      counter++;
      return (counter);
  }

 bool Play_Alarm(void) {

switch (melody_index) {
  case 0:
    nx = beep(first_section_notes[nx], 25*first_section_durations[nx], nx);
    break;
  case 1:
    nx = beep(second_section_notes[nx], 25*second_section_durations[nx], nx);
    break; 
  case 2:
    nx = beep(interlude_one_notes[nx], 25*interlude_one_durations[nx], nx);
    break; 
  case 3:
    nx = beep(second_section_notes[nx], 25*second_section_durations[nx], nx);
    break; 
  case 4:
    nx = beep(interlude_two_notes[nx], 25*interlude_two_durations[nx], nx);
    break;
  default:
    break;       
  } // end switch melody_index

  if (nx == number_of_notes[melody_index]) {
    nx = 0;
    melody_index++;
    melody_index = melody_index % number_of_melodies;
    if (melody_index == 0) { loop_counter++; }
    if (loop_counter == loop_max) {
      nx = 0;
      melody_index = 0;
      loop_counter = 0;
      Serial.print("Alarm Ended");
      clearAlarms();
      // ClockState = ShowClock; 
      return (false); 
    }     
  } 
  return (true);
  }


 