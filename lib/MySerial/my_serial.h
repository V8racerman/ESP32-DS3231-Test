#ifndef my_serial_h
#define my_serial_h

#include "SimpleAlarmClock.h"
#include "/home/alan/PlatformIO/Projects/Development/ESP32/ESP32 DS3231 Test/include/external_variables.h"

void my_serial_monitor(DateTime NowT, bool h12Flagx, bool pmFlagx, bool monitor_enable);

#endif