#include <Wire.h>
#include <RtcDS3231.h>
RtcDS3231<TwoWire>
    Rtc(Wire);

#define countof(a) (sizeof(a) / sizeof(a[0]))

// Door sensors / switches
const int MODE_SWITCH = 2;
const int INCR_SWITCH = 3;
const int DECR_SWITCH = 4;

int mode = 0; // running mode

RtcDateTime currentDateTime;

void printDateTime(const RtcDateTime &dt)
{
    char datestring[20];
    snprintf_P(datestring,
               countof(datestring),
               PSTR("%02u/%02u/%04u\t%02u:%02u:%02u"),
               dt.Day(),
               dt.Month(),
               dt.Year(),
               dt.Hour(),
               dt.Minute(),
               dt.Second());
    Serial.println(datestring);
}

void setup()
{
    Serial.begin(9600);
    Serial.print("compiled: ");
    Serial.print(__DATE__);
    Serial.println(__TIME__);

    Rtc.Begin();

    RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
    printDateTime(compiled);

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

    currentDateTime = Rtc.GetDateTime();
    if (currentDateTime < compiled)
    {
        Serial.println("RTC is older than compile time!  (Updating DateTime)");
        Rtc.SetDateTime(compiled);
    }
    else if (currentDateTime > compiled)
    {
        Serial.println("RTC is newer than compile time. (this is expected)");
    }
    else if (currentDateTime == compiled)
    {
        Serial.println("RTC is the same as compile time! (not expected but all is fine)");
    }

    // never assume the Rtc was last configured by you, so
    // just clear them to your needed state
    Rtc.Enable32kHzPin(false);
    Rtc.SetSquareWavePin(DS3231SquareWavePin_ModeNone);

    // buttons config
    pinMode(MODE_SWITCH, INPUT_PULLUP);
    pinMode(INCR_SWITCH, INPUT_PULLUP);
    pinMode(DECR_SWITCH, INPUT_PULLUP);
}

void showCurrentTime()
{
    // will bw changed to displaying to the LED segments
}

void setYear()
{
    int value = currentDateTime.Year();
    if (!digitalRead(INCR_SWITCH))
    {
        value++;
    }
    if (!digitalRead(DECR_SWITCH))
    {
        value--;
    }
    currentDateTime = RtcDateTime(value,
                                  currentDateTime.Month(),
                                  currentDateTime.Day(),
                                  currentDateTime.Hour(),
                                  currentDateTime.Minute(),
                                  currentDateTime.Second());
}

void setMonth()
{
    int value = currentDateTime.Month();
    if (!digitalRead(INCR_SWITCH))
    {
        if (currentDateTime.Month() == 12)
        {
            value = 1;
        }
        else
        {
            value++;
        }
    }
    if (!digitalRead(DECR_SWITCH))
    {
        if (currentDateTime.Month() == 1)
        {
            value = 12;
        }
        else
        {
            value--;
        }
    }
    currentDateTime = RtcDateTime(currentDateTime.Year(),
                                  value,
                                  currentDateTime.Day(),
                                  currentDateTime.Hour(),
                                  currentDateTime.Minute(),
                                  currentDateTime.Second());
}

void setDay()
{
    int value = currentDateTime.Day();
    int nbDaysInMonth = 0;
    if (currentDateTime.Month() == 2)
    {
        // compute number of days in the current month, according to the current year (leap or not)
        int year = currentDateTime.Year();
        nbDaysInMonth = (year % 4 == 0 && year % 100 != 0) || year % 400 == 0 ? 29 : 28;
    }
    else
    {
        int month = currentDateTime.Month();
        nbDaysInMonth = month <= 7 ? (month % 2 != 0 ? 31 : 30) : (month % 2 == 0 ? 31 : 30);
    }
    if (!digitalRead(INCR_SWITCH))
    {
        if (currentDateTime.Day() == nbDaysInMonth)
        {
            value = 1;
        }
        else
        {
            value++;
        }
    }
    if (!digitalRead(DECR_SWITCH))
    {
        if (currentDateTime.Day() == 1)
        {
            value = nbDaysInMonth;
        }
        else
        {
            value--;
        }
    }
    currentDateTime = RtcDateTime(currentDateTime.Year(),
                                  currentDateTime.Month(),
                                  value,
                                  currentDateTime.Hour(),
                                  currentDateTime.Minute(),
                                  currentDateTime.Second());
}

void setHour()
{
    int value = currentDateTime.Hour();
    if (!digitalRead(INCR_SWITCH))
    {
        if (currentDateTime.Hour() == 23)
        {
            value = 1;
        }
        else
        {
            value++;
        }
    }
    if (!digitalRead(DECR_SWITCH))
    {
        if (currentDateTime.Hour() == 1)
        {
            value = 23;
        }
        else
        {
            value--;
        }
    }
    currentDateTime = RtcDateTime(currentDateTime.Year(),
                                  currentDateTime.Month(),
                                  currentDateTime.Day(),
                                  value,
                                  currentDateTime.Minute(),
                                  currentDateTime.Second());
}
void setMinute()
{
    int value = currentDateTime.Minute();
    if (!digitalRead(INCR_SWITCH))
    {
        if (currentDateTime.Minute() == 59)
        {
            value = 0;
        }
        else
        {
            value++;
        }
    }
    if (!digitalRead(DECR_SWITCH))
    {
        if (currentDateTime.Minute() == 0)
        {
            value = 59;
        }
        else
        {
            value--;
        }
    }
    currentDateTime = RtcDateTime(currentDateTime.Year(),
                                  currentDateTime.Month(),
                                  currentDateTime.Day(),
                                  currentDateTime.Hour(),
                                  value,
                                  currentDateTime.Second());
}

void setDatetime()
{
    Rtc.SetDateTime(currentDateTime);
}

void loop()
{
    Serial.print("Mode: ");
    Serial.println(mode);
    // check if mode button is pressed
    if (!digitalRead(MODE_SWITCH))
    {
        mode += 1;
    }

    // which mode to apply?
    if (mode == 0)
    {
        showCurrentTime();
    }
    if (mode == 1)
    {
        setYear();
    }
    if (mode == 2)
    {
        setMonth();
    }
    if (mode == 3)
    {
        setDay();
    }
    if (mode == 4)
    {
        setHour();
    }
    if (mode == 5)
    {
        setMinute();
    }
    if (mode == 6)
    {
        setDatetime();
        mode = 0;
    }
    if (mode != 0)
    {
        printDateTime(currentDateTime);
    }
    else
    {
        printDateTime(Rtc.GetDateTime());
    }
    delay(250);
}
