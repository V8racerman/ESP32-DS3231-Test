#include "myOLED.h"


Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,
   OLED_MOSI_D1, OLED_CLK_D0, OLED_DC, OLED_RESET, OLED_CS);

char *mode_str[] = {"Daily", "Weekday", "Weekend", "Once"};
char *dow_str[] = {"--", "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

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