#include <Arduino.h>
#include <DS3232RTC.h>
#include <Streaming.h>
#include <OneButton.h>
#include <TM1637.h>
#include <TimerOne.h>

// ----- Local libraries
#include "main.h"
#include "ClockUtils.h"

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
TM1637 tm1637(CLK, DIO);
int8_t timeDisp[] = {0x00, 0x00, 0x00, 0x00};
#define ON 1
#define OFF 0
unsigned char clockpoint = 1;
unsigned char update;
unsigned char halfsecond = 0;

unsigned long displayTimer = 0; // currently used for debugging purpose
byte clockMode = 0;
bool sleepMode = false;
tmElements_t timeToSet;
tmElements_t currentTime;

// ----- Buttons
OneButton modeBtn(8, true);
OneButton minusBtn(9, true);
OneButton plusBtn(10, true);
// -----

void timingISR();
void timeUpdate(void);

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
    pinMode(AWAKE_LED_PIN, OUTPUT);
    pinMode(ASLEEP_LED_PIN, OUTPUT);

    // setup button handler
    modeBtn.attachClick(modeBtnClick);
    minusBtn.attachClick(minusBtnClick);
    plusBtn.attachClick(plusBtnClick);

    // setup display
    tm1637.set();
    tm1637.init();
    Timer1.initialize(500000); // 500ms
    Timer1.attachInterrupt(timingISR);
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
    displayClock();

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
    // fixme update must be used for the time setup as well
    if (update == ON)
    {
        timeUpdate();
        tm1637.display(timeDisp);
    }
    switch (clockMode)
    {
        // todo
    case 1:
        int year = tmYearToCalendar(currentTime.Year);
        timeDisp[3] = year % 10;
        year = year / 10;
        timeDisp[2] = year % 10;
        year = year / 10;
        timeDisp[1] = year % 10;
        year = year / 10;
        timeDisp[0] = year % 10;
        tm1637.display(timeDisp);
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

void timingISR()
{
    halfsecond++;
    update = ON;
    if (halfsecond == 2)
    {
        currentTime.Second++;
        if (currentTime.Second == 60)
        {
            currentTime.Minute++;
            if (currentTime.Minute == 60)
            {
                currentTime.Hour++;
                if (currentTime.Hour == 24)
                    currentTime.Hour = 0;
                currentTime.Minute = 0;
            }
            currentTime.Second = 0;
        }
        halfsecond = 0;
    }
    clockpoint = (~clockpoint) & 0x01;
}

void timeUpdate(void)
{
    if (clockpoint)
        tm1637.point(POINT_ON);
    else
        tm1637.point(POINT_OFF);
    timeDisp[0] = currentTime.Hour / 10;
    timeDisp[1] = currentTime.Hour % 10;
    timeDisp[2] = currentTime.Minute / 10;
    timeDisp[3] = currentTime.Minute % 10;
    update = OFF;
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
