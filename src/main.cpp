#include <Arduino.h>
#include <DS3232RTC.h>
#include <Streaming.h>
#include <OneButton.h>

// ----- Local libraries
#include "DateTimeUtils.h"

// ----- PIN definition
// Push buttons for clock setup
const int MODE_SW_PIN = 8;
const int MINUS_SW_PIN = 9;
const int PLUS_SW_PIN = 10;

// asleep / awake LEDs
const int awakeLed = 6;
const int asleepLed = 7;
// -----

// ----- Global vars
unsigned long displayTimer = 0; // currently used for debugging purpose
unsigned int clockMode = 0;
tmElements_t timeToSet;

// ----- Buttons
OneButton modeBtn(8, true);
OneButton minusBtn(9, true);
OneButton plusBtn(10, true);
// -----

// ----- Enum
typedef enum
{
    PLUS,
    MINUS
} updateAction;
// ----

// ----- Prototypes
void printDateTime(time_t t);
void printDateTime(tmElements_t t);
void modeBtnClick();
void minusBtnClick();
void plusBtnClick();
void displayClock();
void updateYear(updateAction action);
void updateMonth(updateAction action);
void updateDay(updateAction action);
void updateHour(updateAction action);
void updateMinute(updateAction action);
// -----

void setup()
{
    // setup serial
    Serial.begin(9600);

    // init RTC if needed
    if (RTC.oscStopped(false))
    {
        setTime(0, 0, 0, 1, 12, 2018);
        RTC.set(now());
    }

    // setup button handler
    modeBtn.attachClick(modeBtnClick);
    minusBtn.attachClick(minusBtnClick);
    plusBtn.attachClick(plusBtnClick);
}

void loop()
{
    // read buttons
    modeBtn.tick();
    minusBtn.tick();
    plusBtn.tick();

    // update the timeToSet
    if (clockMode == 0)
    {
        RTC.read(timeToSet);
        displayClock();
    }
    else if (clockMode == 6)
    {
        RTC.write(timeToSet);
        clockMode = 0;
    }

    if (millis() >= displayTimer)
    {
        if (clockMode != 0)
        {
            printDateTime(timeToSet);
        }
        else
        {
            printDateTime(RTC.get());
        }
        displayTimer = millis() + 1000;
    }
}

void modeBtnClick()
{
    clockMode++;
    Serial << "ClockMode: " << clockMode << endl;
}

void minusBtnClick()
{
    switch (clockMode)
    {
    case 1:
        updateYear(MINUS);
        break;
    case 2:
        updateMonth(MINUS);
        break;
    case 3:
        updateDay(MINUS);
        break;
    case 4:
        updateHour(MINUS);
        break;
    case 5:
        updateMinute(MINUS);
        break;
    }
}

void plusBtnClick()
{
    switch (clockMode)
    {
    case 1:
        updateYear(PLUS);
        break;
    case 2:
        updateMonth(PLUS);
        break;
    case 3:
        updateDay(PLUS);
        break;
    case 4:
        updateHour(PLUS);
        break;
    case 5:
        updateMinute(PLUS);
        break;
    }
}

void displayClock()
{
}

void updateYear(updateAction action)
{
    if (action == PLUS)
    {
        timeToSet.Year++;
    }
    else
    {
        timeToSet.Year--;
    }
}
void updateMonth(updateAction action)
{
    if (action == PLUS)
    {
        timeToSet.Month = timeToSet.Month == 12 ? 1 : timeToSet.Month + 1;
    }
    else
    {
        timeToSet.Month = timeToSet.Month == 1 ? 12 : timeToSet.Month - 1;
    }
}
void updateDay(updateAction action)
{
    int nbDaysInMonth = daysInMonth(timeToSet.Year, timeToSet.Month);
    if (action == PLUS)
    {
        timeToSet.Day = timeToSet.Day == nbDaysInMonth ? 1 : timeToSet.Day + 1;
    }
    else
    {
        timeToSet.Day = timeToSet.Day == nbDaysInMonth ? 31 : timeToSet.Day - 1;
    }
}
void updateHour(updateAction action)
{
    if (action == PLUS)
    {
        timeToSet.Hour = timeToSet.Hour == 23 ? 0 : timeToSet.Hour + 1;
    }
    else
    {
        timeToSet.Hour = timeToSet.Hour == 0 ? 23 : timeToSet.Hour - 1;
    }
}
void updateMinute(updateAction action)
{
    if (action == PLUS)
    {
        timeToSet.Minute = timeToSet.Minute == 59 ? 0 : timeToSet.Minute + 1;
    }
    else
    {
        timeToSet.Minute = timeToSet.Minute == 0 ? 59 : timeToSet.Minute - 1;
    }
}

void printDateTime(time_t t)
{
    Serial << ((day(t) < 10) ? "0" : "") << _DEC(day(t));
    Serial << monthShortStr(month(t)) << _DEC(year(t)) << ' ';
    Serial << ((hour(t) < 10) ? "0" : "") << _DEC(hour(t)) << ':';
    Serial << ((minute(t) < 10) ? "0" : "") << _DEC(minute(t)) << ':';
    Serial << ((second(t) < 10) ? "0" : "") << _DEC(second(t)) << endl;
}

void printDateTime(tmElements_t t)
{
    Serial << t.Day << "-" << t.Month << "-" << t.Year + 1970 << " " << t.Hour << ":" << t.Minute << ":" << t.Second << endl;
}