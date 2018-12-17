#include "ClockDisplay.h"
#include "Constants.h"

void displayCurrentTime(TM1637Display display, const byte hour, const byte minute, const bool doDisplay)
{
    display.showNumberDecEx(hour * 100 + minute, doDisplay ? DOTS : 0);
}

void displayYearSetup(TM1637Display display, const byte year, const bool doDisplay)
{
    if (doDisplay)
    {
        display.showNumberDec(year + 1970);
    }
    else
    {
        display.clear();
    }
}

void displayMonthSetup(TM1637Display display, const byte month, const bool doDisplay)
{
    if (doDisplay)
    {
        display.showNumberDec(month);
    }
    else
    {
        display.clear();
    }
}

void displayDaySetup(TM1637Display display, const byte day, const bool doDisplay)
{
    if (doDisplay)
    {
        display.showNumberDec(day);
    }
    else
    {
        display.clear();
    }
}

void displayHourSetup(TM1637Display display, const byte hour, const byte minute, const bool doDisplay)
{
    if (doDisplay)
    {
        display.showNumberDecEx(hour * 100 + minute, DOTS);
    }
    else
    {
        byte data[] = {
            0x00,
            0x00,
            display.encodeDigit(minute / 10),
            display.encodeDigit(minute % 10)};
        display.setSegments(data);
    }
}

void displayMinuteSetup(TM1637Display display, const byte hour, const byte minute, const bool doDisplay)
{
    if (doDisplay)
    {
        display.showNumberDecEx(hour * 100 + minute, DOTS);
    }
    else
    {
        byte data[] = {
            display.encodeDigit(hour / 10),
            display.encodeDigit(hour % 10),
            0x00,
            0x00};
        display.setSegments(data);
    }
}
