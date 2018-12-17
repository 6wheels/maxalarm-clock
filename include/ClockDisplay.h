#ifndef CLOCKDISPLAY_H
#define CLOCKDISPLAY_H

#include <Arduino.h>
#include <TM1637Display.h>

void displayCurrentTime(TM1637Display display, const byte hour, const byte minute, const bool doDisplay);
void displayYearSetup(TM1637Display display, const byte year, const bool doDisplay);
void displayMonthSetup(TM1637Display display, const byte month, const bool doDisplay);
void displayDaySetup(TM1637Display display, const byte day, const bool doDisplay);
void displayHourSetup(TM1637Display display, const byte hour, const byte minute, const bool doDisplay);
void displayMinuteSetup(TM1637Display display, const byte hour, const byte minute, const bool doDisplay);

#endif
