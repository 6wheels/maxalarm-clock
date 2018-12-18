// Maxalarm-clock
// https://gitlab.com/6wheels/maxalarm-clock
// Copyright (C) 2018 by Bertrand Ciroux and licensed under
// MIT https://opensource.org/licenses/MIT

#include "ClockDisplay.h"

int8_t data[] = {0x00, 0x00, 0x00, 0x00};

void displayCurrentTime(TM1637 display, const byte hour, const byte minute, const bool doDisplay)
{
    data[0] = hour < 10 ? 0x7f : hour / 10;
    data[1] = hour % 10;
    data[2] = minute / 10;
    data[3] = minute % 10;
    display.point(doDisplay);
    display.display(data);
}

void displayYearSetup(TM1637 display, const byte year, const bool doDisplay)
{
    display.point(false);
    if (doDisplay)
    {
        data[0] = (year + 1970) / 1000;
        data[1] = (year + 1970) / 100 % 10;
        data[2] = (year + 1970) / 10 % 10;
        data[3] = (year + 1970) % 10;
        display.display(data);
    }
    else
    {
        display.init();
    }
}

void displayMonthSetup(TM1637 display, const byte month, const bool doDisplay)
{
    display.point(false);
    if (doDisplay)
    {
        data[0] = 0x7f;
        data[1] = 0x7f;
        data[2] = month / 10;
        data[3] = month % 10;
        display.display(data);
    }
    else
    {
        display.init();
    }
}

void displayDaySetup(TM1637 display, const byte day, const bool doDisplay)
{
    display.point(false);
    if (doDisplay)
    {
        data[0] = 0x7f;
        data[1] = 0x7f;
        data[2] = day / 10;
        data[3] = day % 10;
        display.display(data);
    }
    else
    {
        display.init();
    }
}

void displayHourSetup(TM1637 display, const byte hour, const byte minute, const bool doDisplay)
{
    display.point(doDisplay);
    if (doDisplay)
    {
        data[0] = hour < 10 ? 0x7f : hour / 10;
        data[1] = hour % 10;
        data[2] = minute / 10;
        data[3] = minute % 10;
        display.display(data);
    }
    else
    {
        data[0] = 0x7f;
        data[1] = 0x7f;
        data[2] = minute / 10;
        data[3] = minute % 10;
        display.display(data);
    }
}

void displayMinuteSetup(TM1637 display, const byte hour, const byte minute, const bool doDisplay)
{
    display.point(doDisplay);
    if (doDisplay)
    {
        data[0] = hour < 10 ? 0x7f : hour / 10;
        data[1] = hour % 10;
        data[2] = minute / 10;
        data[3] = minute % 10;
        display.display(data);
    }
    else
    {
        data[0] = hour < 10 ? 0x7f : hour / 10;
        data[1] = hour % 10;
        data[2] = 0x7f;
        data[3] = 0x7f;
        display.display(data);
    }
}
