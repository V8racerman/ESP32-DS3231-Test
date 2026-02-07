#ifndef setgpio_h
#define setgpio_h

// Set up the ESP32 digital GP i/o pins
#define ONBOARD_LED      2
#define BLUE_LED         2
#define GREEN_LED_RIGHT 16
#define GREEN_LED_LEFT   4
#define DOWN_SW         39 //36 //26
#define SELECT_SW       36 //39 //27
#define UP_SW           35 //34 //25
#define EDIT_SW         34 //35 //33
#define BUZZER_PIN       4 //13
#define SQW_PIN         23 //32   

// 7 Segment Module connection pins (Digital Pins)
#define CLK 13
#define DIO 14
#define COLON_ON  0b01000000
#define COLON_OFF 0b00000000

// 6 Pin OLED Display
#define OLED_CLK_D0 17 //19 //18 //32  // D0
#define OLED_MOSI_D1 5 //18 //23 //33  //D1
#define OLED_RESET  18 // 5 // 5
#define OLED_DC     19 //17 //17 //26
#define OLED_CS     16 //23 //16 //-1 Not Used

#endif
