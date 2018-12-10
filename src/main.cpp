#include <Wire.h>
#include <RtcDS3231.h>
RtcDS3231<TwoWire> Rtc(Wire);

// Local libs
#include "Button.h"
#include "DateTimeUtils.h"

#define countof(a) (sizeof(a) / sizeof(a[0]))

// ----- PIN definition
// Push buttons for clock setup
const int MODE_SW_PIN = 9;
const int PLUS_SW_PIN = 10;
const int MINUS_SW_PIN = 11;

// asleep / awake LEDs
const int asleepLed = A2;
const int awakeLed = A3;
// -----

// ----- Global vars
int mode = 0;           // running mode = 0, set year = 1, set month = 2, etc.
bool sleepMode = false; // tells which led to turn on
RtcDateTime setupDateTime;
unsigned long displayTimer = 0; // currently used for debugging purpose

// ----- Buttons
Button modeButton(MODE_SW_PIN);
Button plusButton(PLUS_SW_PIN);
Button minusButton(MINUS_SW_PIN);
// -----

void printToSerial(const RtcDateTime &dt)
{
    if (millis() >= displayTimer)
    {
        Serial.print("Mode: ");
        Serial.print(mode);
        Serial.print(" ");
        Serial.print("SleepMode: ");
        Serial.print(sleepMode);
        Serial.print(" ");
        Serial.print("WD: ");
        char datestring[22];
        snprintf_P(datestring,
                   countof(datestring),
                   PSTR("%1u %02u/%02u/%04u %02u:%02u:%02u"),
                   dt.DayOfWeek(),
                   dt.Day(),
                   dt.Month(),
                   dt.Year(),
                   dt.Hour(),
                   dt.Minute(),
                   dt.Second());
        Serial.println(datestring);
        displayTimer = millis() + 1000;
    }
}

void setup()
{
    Serial.begin(9600);
    Serial.print("compiled: ");
    Serial.print(__DATE__);
    Serial.println(__TIME__);

    Rtc.Begin();
    modeButton.begin();
    plusButton.begin();
    minusButton.begin();

    RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
    printToSerial(compiled);

    if (!Rtc.IsDateTimeValid())
    {
        Serial.println("RTC lost confidence in the DateTime!");
        Rtc.SetDateTime(compiled);
    }

    if (!Rtc.GetIsRunning())
    {
        Serial.println("RTC was not actively running, starting now");
        Rtc.SetIsRunning(true);
    }

    setupDateTime = Rtc.GetDateTime();
    if (setupDateTime < compiled)
    {
        Serial.println("RTC is older than compile time!  (Updating DateTime)");
        Rtc.SetDateTime(compiled);
    }
    else if (setupDateTime > compiled)
    {
        Serial.println("RTC is newer than compile time. (this is expected)");
    }
    else if (setupDateTime == compiled)
    {
        Serial.println("RTC is the same as compile time! (not expected but all is fine)");
    }

    // never assume the Rtc was last configured by you, so
    // just clear them to your needed state
    Rtc.Enable32kHzPin(false);
    Rtc.SetSquareWavePin(DS3231SquareWavePin_ModeNone);

    // sleep / awake LED
    pinMode(awakeLed, OUTPUT);
    pinMode(asleepLed, OUTPUT);
}

void showCurrentTime()
{
    // will be changed to displaying to the LED segments
}

void setYear()
{
    int value = setupDateTime.Year();
    value = plusButton.isReleased() ? value + 1 : minusButton.isReleased() ? value - 1 : value;
    setupDateTime = RtcDateTime(value,
                                setupDateTime.Month(),
                                setupDateTime.Day(),
                                setupDateTime.Hour(),
                                setupDateTime.Minute(),
                                setupDateTime.Second());
}

void setMonth()
{
    int value = setupDateTime.Month();
    value = plusButton.isReleased() ? (value == 12 ? 1 : value + 1) : (minusButton.isReleased() ? value == 1 ? 12 : value - 1 : value);
    setupDateTime = RtcDateTime(setupDateTime.Year(),
                                value,
                                setupDateTime.Day(),
                                setupDateTime.Hour(),
                                setupDateTime.Minute(),
                                setupDateTime.Second());
}

void setDay()
{
    int value = setupDateTime.Day();
    int nbDaysInMonth = daysInMonth(setupDateTime.Year(), setupDateTime.Month());
    value = plusButton.isReleased() ? (value == nbDaysInMonth ? 1 : value + 1) : (minusButton.isReleased() ? (value == 1 ? nbDaysInMonth : value - 1) : value);
    setupDateTime = RtcDateTime(setupDateTime.Year(),
                                setupDateTime.Month(),
                                value,
                                setupDateTime.Hour(),
                                setupDateTime.Minute(),
                                setupDateTime.Second());
}

void setHour()
{
    int value = setupDateTime.Hour();
    value = plusButton.isReleased() ? (value == 23 ? 0 : value + 1) : (minusButton.isReleased() ? (value == 0 ? 23 : value - 1) : value);
    setupDateTime = RtcDateTime(setupDateTime.Year(),
                                setupDateTime.Month(),
                                setupDateTime.Day(),
                                value,
                                setupDateTime.Minute(),
                                setupDateTime.Second());
}
void setMinute()
{
    int value = setupDateTime.Minute();
    value = plusButton.isReleased() ? (value == 59 ? 0 : value + 1) : (minusButton.isReleased() ? (value == 0 ? 59 : value - 1) : value);
    setupDateTime = RtcDateTime(setupDateTime.Year(),
                                setupDateTime.Month(),
                                setupDateTime.Day(),
                                setupDateTime.Hour(),
                                value,
                                setupDateTime.Second());
}

void setDatetime()
{
    Rtc.SetDateTime(setupDateTime);
}

void loop()
{
    RtcDateTime now = Rtc.GetDateTime();
    // check if mode button is pressed
    if (modeButton.isReleased())
    {
        mode += 1;
    }

    // which mode to apply?
    switch (mode)
    {
    case 0:
        showCurrentTime();
        break;
    case 1:
        setYear();
        break;
    case 2:
        setMonth();
        break;
    case 3:
        setDay();
        break;
    case 4:
        setHour();
        break;
    case 5:
        setMinute();
        break;
    default:
        setDatetime();
        mode = 0;
        break;
    }

    if (mode != 0)
    {
        printToSerial(setupDateTime);
    }
    else
    {
        printToSerial(now);
    }

    // alarms
    // weekdays 20:00-07:30
    // weekends 20:00-08:00 and 13:00-15:00
    if (now.DayOfWeek() == 0 || now.DayOfWeek() == 6)
    {
        sleepMode = !((now.Hour() >= 15 && now.Hour() < 20) || (now.Hour() >= 8 && now.Hour() < 13));
    }
    else
    {
        sleepMode = !((now.Hour() > 7 || (now.Hour() == 7 && now.Minute() >= 30)) && now.Hour() < 20);
    }

    digitalWrite(awakeLed, !sleepMode);
    digitalWrite(asleepLed, sleepMode);
}
