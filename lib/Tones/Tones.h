#ifndef tones_h
#define tones_h
#include "Clocks.h"

#define BUZZER_PIN 4

/* Set up the LEDC timer Functions */
// use first channel of 16 channels (started from zero)
#define LEDC_CHANNEL_0      0

// use 2 bit precission for LEDC timer
#define LEDC_TIMER_2_BIT    2

// use 440 Hz as a LEDC tone base frequency
#define LEDC_TONE_FREQ     440

// Note frequencies
const int c = 261;
const int d = 294;
const int e = 329;
const int f = 349;
const int g = 391;
const int gS = 415;
const int a = 440;
const int aS = 455;
const int b = 466;
const int cH = 523;
const int cSH = 554;
const int dH = 587;
const int dSH = 622;
const int eH = 659;
const int fH = 698;
const int fSH = 740;
const int gH = 784;
const int gSH = 830;
const int aH = 880;

const int first_section_notes[] = { a, a, a, f, cH, a, f, cH, a, 0, eH, eH, eH, fH, cH, gS, f, cH, a, 0 };
const int first_section_durations[] = {20, 20, 20, 14, 6, 20, 14, 6, 26, 20, 20, 20, 20, 14, 6, 20, 14, 6, 26, 20 };
const int second_section_notes[] = {aH, a, a, aH, gSH, gH, fSH, fH, fSH, 0, aS, dSH, dH, cSH, cH, b, cH, 0 };
const int second_section_durations[] = {20, 12, 6, 20, 13, 7, 5, 5, 10, 13, 10, 20, 13, 7, 5, 5, 10, 14 };
const int interlude_one_notes[] = {f, gS, f, a, cH, a, cH, eH, 0 };
const int interlude_one_durations[] = {10, 20, 14, 5, 20, 15, 5, 23, 20 }; 
const int interlude_two_notes[] = {f, gS, f, cH, a, f, cH, a, 0 };
const int interlude_two_durations[] = {10, 20, 15, 5, 20, 15, 5, 26, 26 };
const int number_of_notes[] = {20, 18, 9, 18, 9};

int beeps(int note, int duration, int counter);
bool Play_Alarm(void);
void Setup_Tone(void);

#endif
