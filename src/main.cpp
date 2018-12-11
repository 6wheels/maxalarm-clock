#include <Arduino.h>
#include <DS3232RTC.h>
#include <Streaming.h>
#include <OneButton.h>

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

// ----- Prototypes
void printDateTime(time_t t);
void printDateTime(tmElements_t t);
void modeBtnClick();
void minusBtnClick();
void plusBtnClick();
void displayClock();
void incrementYear();
void decrementYear();
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
    RTC.read(timeToSet);

    // manage clock mode
    switch (clockMode)
    {
    case 0:
        displayClock();
        break;
    case 1:
        // setupYear();
        break;
    case 2:
        // setupMonth();
        break;
    case 3:
        // setupDay();
        break;
    case 4:
        // setupHour();
        break;
    case 5:
        // setupMinute();
        break;
    default:
        RTC.write(timeToSet);
        clockMode = 0;
        break;
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
        decrementYear();
        break;
    }
}

void plusBtnClick()
{
    switch (clockMode)
    {
    case 1:
        incrementYear();
        break;
    }
}

void displayClock()
{
}

void decrementYear()
{
    timeToSet.Year--;
}
void incrementYear()
{
    timeToSet.Year++;
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