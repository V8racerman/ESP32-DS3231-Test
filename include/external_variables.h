extern int       nx;
extern int       melody_index;
extern int       loop_counter;

#ifndef external_variables_h
#define external_variables_h

enum States {
    PowerLoss,
    ShowClock,
    Alarm,
    EditClock,
    EditAlarm1,
    EditAlarm2
  };

  #endif

extern States ClockState;
extern States PrevState;