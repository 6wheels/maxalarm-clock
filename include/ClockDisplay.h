// Maxalarm-clock
// https://gitlab.com/6wheels/maxalarm-clock
// Copyright (C) 2018 by Bertrand Ciroux and licensed under
// MIT https://opensource.org/licenses/MIT

#ifndef CLOCKDISPLAY_H
#define CLOCKDISPLAY_H

#include <Arduino.h>
// #include <TM1637.h>
#include <SevenSegmentTM1637.h>

void displayCurrentTime(SevenSegmentTM1637 display, const byte hour, const byte minute, const bool doDisplay);
void displayYearSetup(SevenSegmentTM1637 display, const byte year, const bool doDisplay);
void displayMonthSetup(SevenSegmentTM1637 display, const byte month, const bool doDisplay);
void displayDaySetup(SevenSegmentTM1637 display, const byte day, const bool doDisplay);
void displayHourSetup(SevenSegmentTM1637 display, const byte hour, const byte minute, const bool doDisplay);
void displayMinuteSetup(SevenSegmentTM1637 display, const byte hour, const byte minute, const bool doDisplay);

#endif
