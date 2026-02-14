#ifndef mygpio_h
#define mygpio_h

// Global definition of the GPIO pin functions

// define push button switches
#define DOWN_SW    39 //36 //26
#define SELECT_SW  36 //39 //27
#define UP_SW      35 //34 //25
#define EDIT_SW    34 //35 //33

// define passive buzzer pin
#define BUZZER_PIN  4 //13

// define alarm trigger pin
#define SQW_PIN    23 //32

// define LED control pins
#define LED_LEFT   25
#define LED_RIGHT  27
#define LED_MIDDLE 26


// 6 Pin OLED Display
#define OLED_CLK_D0 17 //19 //18 //32  // D0
#define OLED_MOSI_D1 5 //18 //23 //33  //D1
#define OLED_RESET  18 // 5 // 5
#define OLED_DC     19 //17 //17 //26
#define OLED_CS     16 //23 //16 //-1 Not Used

// Passive buzzer
#define BUZZER_PIN 4

// 7 Segment Module connection pins (Digital Pins)
#define CLK 13
#define DIO 14

#endif
