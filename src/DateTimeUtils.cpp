#include <DateTimeUtils.h>

#define LEAP_YEAR(Y) (((1970 + Y) > 0) && !((1970 + Y) % 4) && (((1970 + Y) % 100) || !((1970 + Y) % 400)))

unsigned int daysInMonth(unsigned int year, unsigned int month)
{
    unsigned int nbDaysInMonth = 0;
    if (month == 2)
    {
        // compute number of days in the current month, according to the current year (leap or not)
        nbDaysInMonth = LEAP_YEAR(year) ? 29 : 28;
    }
    else
    {
        nbDaysInMonth = month <= 7 ? (month % 2 != 0 ? 31 : 30) : (month % 2 == 0 ? 31 : 30);
    }
    return nbDaysInMonth;
}