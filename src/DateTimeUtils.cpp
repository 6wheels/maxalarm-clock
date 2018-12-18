// Maxalarm-clock
// https://gitlab.com/6wheels/maxalarm-clock
// Copyright (C) 2018 by Bertrand Ciroux and licensed under
// MIT https://opensource.org/licenses/MIT

#include <DateTimeUtils.h>

byte daysInMonth(const unsigned int year, const byte month)
{
    byte nbDaysInMonth = 0;
    if (month == 2)
    {
        // compute number of days for february, according to the current year (leap or not)
        nbDaysInMonth = isLeap(year) ? 29 : 28;
    }
    else
    {
        // other months
        nbDaysInMonth = month <= 7 ? (month % 2 != 0 ? 31 : 30) : (month % 2 == 0 ? 31 : 30);
    }
    return nbDaysInMonth;
}

bool isLeap(const unsigned int year)
{
    return ((1970 + year) > 0) && !((1970 + year) % 4) && (((1970 + year) % 100) || !((1970 + year) % 400));
}
