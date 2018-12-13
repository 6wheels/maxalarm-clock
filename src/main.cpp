#include <Arduino.h>
#include <DS3232RTC.h>
#include <Streaming.h>
#include <OneButton.h>

// ----- Local libraries
#include "ClockUtils.h"

// ----- PIN definition
// Push buttons for clock setup
const byte MODE_SW_PIN = 8;
const byte MINUS_SW_PIN = 9;
const byte PLUS_SW_PIN = 10;

// asleep / awake LEDs
const byte awakeLed = 5;
const byte asleepLed = 6;

// alarm interrupt
const byte alarmInterrupt = 2;
// -----

// ----- Global vars
unsigned long displayTimer = 0; // currently used for debugging purpose
byte clockMode = 0;
tmElements_t timeToSet;

// ----- Buttons
OneButton modeBtn(8, true);
OneButton minusBtn(9, true);
OneButton plusBtn(10, true);
// -----

// ----- Prototypes
void printDateTime(time_t t);
void printDateTime(tmElements_t t);
void modeBtnClick();
void minusBtnClick();
void plusBtnClick();
void displayClock();
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

    // LED configuration
    pinMode(awakeLed, OUTPUT);
    pinMode(asleepLed, OUTPUT);

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
    }
    else if (clockMode == 6)
    {
        RTC.write(timeToSet);
        clockMode = 0;
    }

    displayClock();
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
        updateYear(timeToSet, MINUS);
        break;
    case 2:
        updateMonth(timeToSet, MINUS);
        break;
    case 3:
        updateDay(timeToSet, MINUS);
        break;
    case 4:
        updateHour(timeToSet, MINUS);
        break;
    case 5:
        updateMinute(timeToSet, MINUS);
        break;
    }
}

void plusBtnClick()
{
    switch (clockMode)
    {
    case 1:
        updateYear(timeToSet, PLUS);
        break;
    case 2:
        updateMonth(timeToSet, PLUS);
        break;
    case 3:
        updateDay(timeToSet, PLUS);
        break;
    case 4:
        updateHour(timeToSet, PLUS);
        break;
    case 5:
        updateMinute(timeToSet, PLUS);
        break;
    }
}

void displayClock()
{
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