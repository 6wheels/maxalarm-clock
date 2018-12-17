#include <Arduino.h>
#include <DS3232RTC.h>
#include <Streaming.h>
#include <OneButton.h>
#include <TM1637Display.h>

// ----- Local libraries
#include "main.h"
#include "ClockUtils.h"
#include "ClockDisplay.h"

// ----- DEBUG MODE
//#define DEBUG

// ----- PIN definition
// Push buttons for clock setup
const byte MODE_SW_PIN = 8;
const byte MINUS_SW_PIN = 9;
const byte PLUS_SW_PIN = 10;

// asleep / awake LEDs
const byte AWAKE_LED_PIN = 5;
const byte ASLEEP_LED_PIN = 6;

// 4*7 segment display
const byte DIO = 11;
const byte CLK = 12;
// -----

// ----- Global vars
TM1637Display display(CLK, DIO);
byte brightness = 2;
byte quarterSecond = 0;
bool doDisplay = false;
unsigned long clockDisplayTimer = 0;

byte clockMode = 0;
bool sleepMode = false;
tmElements_t timeToSet;
tmElements_t currentTime;

// ----- Buttons
OneButton modeBtn(8, true);
OneButton minusBtn(9, true);
OneButton plusBtn(10, true);
// -----

void setup()
{
#ifdef DEBUG
    // setup serial
    Serial.begin(9600);
#endif

    // init RTC if needed
    if (RTC.oscStopped(false))
    {
        setTime(0, 0, 0, 1, 12, 2018);
        RTC.set(now());
    }

    // LED configuration
    pinMode(AWAKE_LED_PIN, OUTPUT);
    pinMode(ASLEEP_LED_PIN, OUTPUT);

    // setup button handler
    modeBtn.attachClick(modeBtnClick);
    minusBtn.attachClick(minusBtnClick);
    plusBtn.attachClick(plusBtnClick);

    // setup display
    display.setBrightness(brightness);
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

    // get current time from rtc
    RTC.read(currentTime);
    manageClock();

    // alarms
    // weekdays 20:00-07:30
    // weekends 20:00-08:00 and 13:00-15:00
    if (currentTime.Wday == 1 || currentTime.Wday == 7)
    {
        sleepMode = !((currentTime.Hour >= 15 && currentTime.Hour < 20) || (currentTime.Hour >= 8 && currentTime.Hour < 13));
    }
    else
    {
        sleepMode = !((currentTime.Hour > 7 || (currentTime.Hour == 7 && currentTime.Minute >= 30)) && currentTime.Hour < 20);
    }

    digitalWrite(AWAKE_LED_PIN, !sleepMode);
    digitalWrite(ASLEEP_LED_PIN, sleepMode);
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
    case 0:
        display.setBrightness(brightness == 0 ? 0 : --brightness);
        break;
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
    case 0:
        display.setBrightness(brightness == 7 ? 7 : ++brightness);
        break;
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

void manageClock()
{
    if (millis() >= clockDisplayTimer)
    {
        quarterSecond++;
        clockDisplayTimer = millis() + 250;
    }
    doDisplay = quarterSecond >= 2;
    switch (clockMode)
    {
    case 0:
        displayCurrentTime(display, currentTime.Hour, currentTime.Minute, doDisplay);
        break;
    case 1:
        displayYearSetup(display, timeToSet.Year, doDisplay);
        break;
    case 2:
        displayMonthSetup(display, timeToSet.Month, doDisplay);
        break;
    case 3:
        displayDaySetup(display, timeToSet.Day, doDisplay);
        break;
    case 4:
        displayHourSetup(display, timeToSet.Hour, timeToSet.Minute, doDisplay);
        break;
    case 5:
        displayMinuteSetup(display, timeToSet.Hour, timeToSet.Minute, doDisplay);
        break;
    }
    // reset quarterSecond
    if (quarterSecond == 4)
    {
        quarterSecond = 0;
    }
#ifdef DEBUG
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
#endif
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
