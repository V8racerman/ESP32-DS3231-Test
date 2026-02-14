#include "myOLED.h"

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,
   OLED_MOSI_D1, OLED_CLK_D0, OLED_DC, OLED_RESET, OLED_CS);

char *mode_str[] = {"Daily", "Weekday", "Weekend", "Once"};
char *dow_str[] = {"--", "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
const int flashInterval = 500;         // Alarm flashing interval
bool  bflash = true;            // used to track the digit editing flash
bool  oldbflash = false;        // used to detect the bflash transition
unsigned long previousMillis = 0;       // used to track time flash 

void setup_OLED(void) {

// SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  display.cp437(true);         // Use full 256 char 'Code Page 437' font
  display.setTextSize(2);      // 2:1 pixel scale

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
}   


 /* ***********************************************************
 *                    Basic Functions                         *
 * ********************************************************* */

 void DisplayInvert() {
  
  display.invertDisplay(false);
 }

 void flashDisplay(unsigned long *prev_millis, bool *state) {
  
  if ((millis() - *prev_millis) >= flashInterval) {
      *prev_millis = millis();
      *state = !*state;
      // toggleLED(true, BLUE_LED);
      }
  display.invertDisplay(*state);
}

 char* mode2str(byte bMode) {
 // Alarm mode to string or char array
      return(mode_str[bMode]);
  
}

char* dow2Str(byte bDow) {
    // Day of week to string or char array. DOW 1=Sunday, 0 is undefined
   if (bDow > 7) bDow = 0;
    return(dow_str[bDow]);
}

void twoDigitPrint(byte number) {
  if ((number >= 0) && (number < 10)) { display.print("0"); }
  display.print(number);
 }


 /* ***********************************************************
 *            High Level Display Functions                    *
 * ********************************************************* */
 
void Update_Display(DateTime *clk, AlarmTime *A1, AlarmTime *A2, int Temperature) {

     /* Update Display - Only change display if change is detected
     *  
     *  DOW DD/MM/YY
     *  HH:MM:SS
     *  10:00 Weekend
     *  10:00 Weekend 18'
     */
  
        display.clearDisplay();
        display.setTextColor(SSD1306_WHITE); // Draw white text
        //First Row: Dow dd/mm/yy
        display.setTextSize(2);      // 2:1 pixel scale
        display.setCursor(row1_c1, row1);
        display.print(dow2Str(clk->Dow));          // Integer Day of the week 
        display.setCursor(row1_c2, row1);
        twoDigitPrint(clk->Day);
        display.setCursor(row1_c3, row1);
        display.print("/"); 
        display.setCursor(row1_c4, row1);
        twoDigitPrint(clk->Month);
        display.setCursor(row1_c5, row1);
        display.print("/");
        display.setCursor(row1_c6, row1);
        twoDigitPrint(clk->Year);
                      
        //Second Row  dd/mm/yy
        display.setTextSize(3);      // 3:1 pixel scale
        display.setCursor(row2_c1, row2); 
        twoDigitPrint(clk->Hour);  // convert to String with an optional leading zero
        display.setCursor(row2_c2, row2);
        display.print(":");
        display.setCursor(row2_c3, row2);
        twoDigitPrint(clk->Minute);
        display.setCursor(row2_c4, row2);
        display.print(":");
        display.setCursor(row2_c5, row2);
        twoDigitPrint(clk->Second);
        
        // Third Row
        display.setTextSize(1);      // 3:1 pixel scale
        display.setCursor(row3_c1, row3);
        if (A1->Enabled) {
          twoDigitPrint(A1->Hour);
          display.setCursor(row3_c2, row3);
          display.print(":");
          display.setCursor(row3_c3, row3);
          twoDigitPrint(A1->Minute);
          display.setCursor(row3_c4, row3);
          display.print(mode2str(A1->AlarmMode));
        } else {
          display.print("ALARM 1 OFF");
        }
                
        // Fourth Row
        display.setTextSize(1);      // 3:1 pixel scale
        display.setCursor(row4_c1, row4);
        if (A2->Enabled) {
          twoDigitPrint(A2->Hour);
          display.setCursor(row4_c2, row4);
          display.print(":");
          display.setCursor(row4_c3, row4);
          twoDigitPrint(A2->Minute);
          display.setCursor(row4_c4, row4);
          display.print(mode2str(A2->AlarmMode));
          } else {
          display.print("ALARM 2 OFF");
        }

        //Third Row (Column 3)
        display.setTextSize(2);      // 2:1 pixel scale
        display.setCursor(row3_c5, row3);   //Column, Row
        display.print(Temperature);
        display.print(degree);
        display.display();

}



void editAlarm(byte alarm, byte index, DateTime *myTime, AlarmTime *myalarm1, AlarmTime *myalarm2, float temperature){
    /*   Alarm 1 hh:mm mode/enabled
     *   Alarm 2 hh:mm mode/enabled
     */
    //Note valid index values are 0-3
    //(Column, Row)               
    //Alarm 1                     hh             mm             mode           enabled
    byte cursorPositions1[][2]= {{row3_c1,row3},{row3_c3,row3},{row3_c4,row3},{row3_c1,row3}};
    //Alarm 2                     hh             mm             mode           enabled
    byte cursorPositions2[][2]= {{row4_c1,row4},{row4_c3,row4},{row4_c4,row4},{row4_c1,row4}};
    int counter, blank_counter = 0;
    
    unsigned long int time_marker;
    time_marker = millis();
    if ((time_marker - previousMillis) >= flashInterval) {
      previousMillis = time_marker;
      bflash = !bflash;
    }
    if (bflash && !oldbflash) {
        oldbflash = bflash;
        switch (index) {
          case 0:
          case 1:
            blank_counter = 2;
            break;
          case 2:
            blank_counter = 7;
            break;
          case 3:
            blank_counter = 13;  
            break;
          default:
          break;
        }   
        if ((alarm == alarm1) || (alarm == alarm2)) {
          display.setTextColor(SSD1306_BLACK);  // Draw black text
          display.setTextSize(1);               // 1:1 pixel scale 
          if (alarm == alarm1) {display.setCursor(cursorPositions1[index][0], cursorPositions1[index][1]);}
          if (alarm == alarm2) {display.setCursor(cursorPositions2[index][0], cursorPositions2[index][1]);}
          for (counter = 0; counter < blank_counter; counter++) { display.write(block_char);}
          display.setTextColor(SSD1306_WHITE); // Draw white text
          display.display();
        }
    }
    if (!bflash && oldbflash)  {
        Update_Display(myTime, myalarm1, myalarm2, temperature);
        oldbflash = bflash;
    }   
}

void editClock(byte index, DateTime *myTime, AlarmTime *myalarm1, AlarmTime *myalarm2, float temperature){
     /* Display format:
     *  
     *  DOW DD/MM/YY
     *  HH:MM:SS
     *  10:00 Weekend
     *  10:00 Weekend 18'
     */
    //                            hh             mm             ss             dd             mm             yyyy
    byte cursorPositions[][2] = {{row2_c1,row2},{row2_c3,row2},{row2_c5,row2},{row1_c2,row1},{row1_c4,row1},{row1_c6,row1}};
    unsigned long int time_marker;
    time_marker = millis();
    if ((time_marker - previousMillis) >= flashInterval) {
      previousMillis = time_marker;
      bflash = !bflash;
    }
    if (bflash && !oldbflash) {
      oldbflash = bflash;
      display.setTextColor(SSD1306_BLACK); // Draw black text
      if (index > 2) {
      display.setTextSize(2);      // 2:1 pixel scale 
      } else {
      display.setTextSize(3);      // 3:1 pixel scale
      }
      display.setCursor(cursorPositions[index][0],cursorPositions[index][1]);
      display.write(block_char);
      display.write(block_char);
      display.setTextColor(SSD1306_WHITE); // Draw white text
      display.display();
    }  
    if (!bflash && oldbflash)  {
      Update_Display(myTime, myalarm1, myalarm2, temperature);
      oldbflash = bflash;
    }
}
