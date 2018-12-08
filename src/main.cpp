#include <Wire.h>
#include <RtcDS3231.h>
RtcDS3231<TwoWire>
    Rtc(Wire);

#define countof(a) (sizeof(a) / sizeof(a[0]))

// Push buttons for clock setup
const int MODE_SWITCH = 9;
const int INCR_SWITCH = 10;
const int DECR_SWITCH = 11;

// asleep / awake LEDs
const int asleepLed = A2;
const int awakeLed = A3;

#include <TimerOne.h>

//Define 74HC595 Connections with arduino
const int Data = 7;
const int Clock = 8;
const int Latch = 6;

const int SEG0 = 2;
const int SEG1 = 3;
const int SEG2 = 4;
const int SEG3 = 5;

int cc = 0;
char Value[4];

//Refer Table 4.1 7-Segment Decoding
const char SegData[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};

int mode = 0; // running mode

bool sleepMode = true;

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
    // Serial.println(datestring);
}

// void display(unsigned char num)
// {
//     digitalWrite(latch, LOW);
//     shiftOut(data, clock, MSBFIRST, table[num]);
//     digitalWrite(latch, HIGH);
// }

//=============================================================
//             Generates Bargraph
//=============================================================
void DisplayDigit(char d)
{
    int i;

    for (i = 0; i < 8; i++) //Shift bit by bit data in shift register
    {
        if ((d & 0x80) == 0x80)
        {
            digitalWrite(Data, HIGH);
        }
        else
        {
            digitalWrite(Data, LOW);
        }
        d = d << 1;

        //Give Clock pulse
        digitalWrite(Clock, LOW);
        digitalWrite(Clock, HIGH);
    }
    //Latch the data
    digitalWrite(Latch, LOW);
    digitalWrite(Latch, HIGH);
}

//===================================================================
//		SCAN DISPLAY FUNCTION
//===================================================================
void Scanner()
{
    switch (cc) //Depending on which digit is selcted give output
    {
    case 1:
        digitalWrite(SEG3, HIGH);
        DisplayDigit(SegData[Value[0]]);
        digitalWrite(SEG0, LOW);
        break;
    case 2:
        digitalWrite(SEG0, HIGH);
        DisplayDigit(SegData[Value[1]] | 0x80); //0x80 to turn on decimal point
        digitalWrite(SEG1, LOW);
        break;
    case 3:
        digitalWrite(SEG1, HIGH);
        DisplayDigit(SegData[Value[2]]);
        digitalWrite(SEG2, LOW);
        break;
    case 4:
        digitalWrite(SEG2, HIGH);
        DisplayDigit(SegData[Value[3]]);
        digitalWrite(SEG3, LOW);
        break;
    }
}
//===================================================================

//===================================================================
//			TIMER 1 OVERFLOW INTTERRUPT FOR DISPALY
//===================================================================
void timerIsr()
{
    cc++;
    if (cc == 5) //We have only 4 digits
    {
        cc = 1;
    }
    Scanner();
    TCNT0 = 0xCC;
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

    // sleep / awake LED
    pinMode(awakeLed, OUTPUT);
    pinMode(asleepLed, OUTPUT);

    pinMode(Data, OUTPUT);
    pinMode(Clock, OUTPUT);
    pinMode(Latch, OUTPUT);
    pinMode(SEG0, OUTPUT);
    pinMode(SEG1, OUTPUT);
    pinMode(SEG2, OUTPUT);
    pinMode(SEG3, OUTPUT);

    //Initialize Display Scanner
    cc = 0;
    Timer1.initialize(5000);          // set a timer of length 100000 microseconds (or 0.1 sec - or 10Hz => the led will blink 5 times, 5 cycles of on-and-off, per second)
    Timer1.attachInterrupt(timerIsr); // attach the service routine here
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

    digitalWrite(awakeLed, !sleepMode);
    digitalWrite(asleepLed, sleepMode);

    char currentDisplay[4];
    sprintf(currentDisplay, "%02d%02d", Rtc.GetDateTime().Hour(), Rtc.GetDateTime().Minute());

    Value[0] = currentDisplay[0] & 0x0F; //Anding with 0x0F to remove upper nibble
    Value[1] = currentDisplay[1] & 0x0F; //Ex. number 2 in ASCII is 0x32 we want only 2
    Value[2] = currentDisplay[2] & 0x0F;
    Value[3] = currentDisplay[3] & 0x0F;
}
