#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <WiFi.h>
#include <TM1637Display.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "/home/alan/PlatformIO/credentials.h"
#include "setgpio.h"
#include "SimpleAlarmClock.h"
#include "Clocks.h"
#include "TimeUtils.h"
#include "main.h"
#include "clock_constants.h"
#include "my_serial.h"

bool pmFlag, h12Flag, century = false;
byte alarmDay, alarmHour, alarmMinute, alarmSecond, alarmBits;
bool alarmDy, alarmH12Flag, alarmPmFlag;

int daylightOffset_sec = 0; // 3600;
bool bst = false;
bool old_bst = true;

SimpleAlarmClock extern Clock;
SimpleAlarmClock Clock(RTC_addr, EEPROM_addr, INTCN);

TM1637Display segment7(CLK, DIO);

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,
   OLED_MOSI_D1, OLED_CLK_D0, OLED_DC, OLED_RESET, OLED_CS);

DateTime NowTime;
byte previous_second = 0;

void segment7_time(uint8_t m_hour, uint8_t m_min, bool colon) {

  if (colon) { my_serial_time(NowTime); }

  segment7.showNumberDecEx(m_min, COLON_OFF, true, 2, 2);
  if (!colon)
  {
    segment7.showNumberDecEx(m_hour, COLON_OFF, false, 2, 0);
  }
  else
  {
    segment7.showNumberDecEx(m_hour, COLON_ON, false, 2, 0);
  }
  digitalWrite(LED_BUILTIN, (uint8_t)colon);
}

void internet_time(void) {

  DateTime NowTime;
  struct tm current_time;
  int attempts = 12;
  int attempt = 0;
  int connected_to_wifi;
  int connected_to_remote_server = 0;

  // connect to WiFi
  digitalWrite(LED_BUILTIN, HIGH);
  digitalWrite(GREEN_LED_LEFT, LOW);
  digitalWrite(GREEN_LED_RIGHT, LOW);
  Serial.print("Connecting to ");
  Serial.print(ssid);
  WiFi.begin(ssid, password);
  connected_to_wifi = WiFi.status();
  while ((connected_to_wifi != WL_CONNECTED) && (attempt < attempts))
  {
    // Display_Clock();
    Serial.print(".");
    delay(500);
    attempt++;
    connected_to_wifi = WiFi.status();
  }

  if (connected_to_wifi != WL_CONNECTED)
  {
    digitalWrite(GREEN_LED_LEFT, HIGH);
    Serial.println(" ");
  }
  else
  {
    attempt = 0;
    Serial.println(" CONNECTED.  Initialising time reference ...");
    // init and get the time
    NowTime = Clock.read();
    bst = british_summer_time(NowTime);
    if (bst)
    {
      daylightOffset_sec = 3600;
    }
    else
    {
      daylightOffset_sec = 0;
    }
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    connected_to_remote_server = getLocalTime(&current_time);
    while ((!connected_to_remote_server) && (attempt < attempts))
    {
      Serial.println("Unable to connect to remote time server.  Retrying ...");
      // Display_Clock();
      delay(500);
      attempt++;
      configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
      connected_to_remote_server = getLocalTime(&current_time);
    }
    if (!connected_to_remote_server)
    {
      digitalWrite(GREEN_LED_RIGHT, HIGH);
    }
    else
    {
      old_bst = bst;
      NowTime = internet_to_RTC(current_time, M24hr);
      bst = british_summer_time(NowTime);
      if (bst != old_bst)
      {
        attempt = 0;
        if (bst)
        {
          daylightOffset_sec = 3600;
        }
        else
        {
          daylightOffset_sec = 0;
        }
        configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
        connected_to_remote_server = getLocalTime(&current_time);
        while ((!connected_to_remote_server) && (attempt < attempts))
        {
          Serial.println("Unable to connect to remote time server.  Retrying ...");
          // Display_Clock();
          delay(500);
          attempt++;
          configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
          connected_to_remote_server = getLocalTime(&current_time);
        }
      }
    }
    if (!connected_to_remote_server) {
      digitalWrite(GREEN_LED_RIGHT, HIGH);
    }
    else {
      // NowTime = Clock.read();
      NowTime = internet_to_RTC(current_time, M24hr);
      old_bst = bst;
      bst = british_summer_time(NowTime);
      Clock.write(NowTime);
      Serial.print("Time reference initialised. ");
      digitalWrite(GREEN_LED_RIGHT, LOW);
    }
    // disconnect WiFi as it's no longer needed
    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);
    Serial.println("Wi-Fi disconnected and turned off.");
  }
  digitalWrite(BLUE_LED, LOW);
}

void setup() {

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(DOWN_SW, INPUT);
  pinMode(UP_SW, INPUT);
  pinMode(EDIT_SW, INPUT);
  pinMode(SELECT_SW, INPUT);

  Wire.begin();
  Serial.begin(115200);
  segment7.setBrightness(0x07);
  NowTime = Clock.read();
  segment7_time(NowTime.Hour, NowTime.Minute, false);
  
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
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();
  display.display();

  
  internet_time();
  previous_second = NowTime.Second;
}

void loop()
{

  NowTime = Clock.read();
  if (NowTime.Second != previous_second) {
    previous_second = NowTime.Second;
    // my_serial_monitor(NowTime, h12Flag, pmFlag, true);
    // my_serial_time(NowTime);
    segment7_time(NowTime.Hour, NowTime.Minute, true);
    delay(500);
    segment7_time(NowTime.Hour, NowTime.Minute, false);
    if (NowTime.Hour == 18 && NowTime.Minute == 55  && NowTime.Second == 0) { internet_time(); }
  }
}