#ifndef main_h
#define main_h
// #include "external_variables.h"

// Define OLED display parameters
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

const int flashInterval = 500;         // Alarm flashing interval

char *mode_str[] = {"Daily", "Weekday", "Weekend", "Once"};
char *dow_str[] = {"--", "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};



const float t_cal  = -3.5;

// States ClockState = ShowClock;
// States PrevState = EditAlarm2;

const char* ntpServer  = "pool.ntp.org";
const long  gmtOffset_sec = 0;

#endif