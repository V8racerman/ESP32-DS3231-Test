#include "include/setgpio.h"
#include "myOLED.h"

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,
   OLED_MOSI_D1, OLED_CLK_D0, OLED_DC, OLED_RESET, OLED_CS);

   