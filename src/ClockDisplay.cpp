// Maxalarm-clock
// https://gitlab.com/6wheels/maxalarm-clock
// Copyright (C) 2018 by Bertrand Ciroux and licensed under
// MIT https://opensource.org/licenses/MIT

#include "ClockDisplay.h"

int8_t data[] = {0x00, 0x00, 0x00, 0x00};

void displayCurrentTime(SevenSegmentTM1637 display, const byte hour, const byte minute, const bool doDisplay)
{
    display.setColonOn(doDisplay);
    display.print(hour * 100 + minute);
}

void displayYearSetup(SevenSegmentTM1637 display, const byte year, const bool doDisplay)
{
    display.setColonOn(false);
    if (doDisplay)
    {
        display.print(year + 1970);
    }
    else
    {
        display.clear();
    }
}

void displayMonthSetup(SevenSegmentTM1637 display, const byte month, const bool doDisplay)
{
    display.setColonOn(false);
    if (doDisplay)
    {
        display.home();
        display.print(String(" " + month));
    }
    else
    {
        display.clear();
    }
}

void displayDaySetup(SevenSegmentTM1637 display, const byte day, const bool doDisplay)
{
    display.setColonOn(false);
    if (doDisplay)
    {
        display.print(String("  " + day));
    }
    else
    {
        display.clear();
    }
}

void displayHourSetup(SevenSegmentTM1637 display, const byte hour, const byte minute, const bool doDisplay)
{
    display.setColonOn(doDisplay);
    if (doDisplay)
    {
        display.print(hour * 100 + minute);
    }
    else
    {
        display.clear();
        display.print(String("  " + minute / 10 + minute % 10));
    }
}

void displayMinuteSetup(SevenSegmentTM1637 display, const byte hour, const byte minute, const bool doDisplay)
{
    display.setColonOn(doDisplay);
    if (doDisplay)
    {
        display.print(hour * 100 + minute);
    }
    else
    {
        display.clear();
        display.print(hour);
    }
}
