#include <DateTimeUtils.h>

unsigned int daysInMonth(unsigned int year, unsigned int month)
{
    unsigned int nbDaysInMonth = 0;
    if (month == 2)
    {
        // compute number of days in the current month, according to the current year (leap or not)
        nbDaysInMonth = (year % 4 == 0 && year % 100 != 0) || year % 400 == 0 ? 29 : 28;
    }
    else
    {
        nbDaysInMonth = month <= 7 ? (month % 2 != 0 ? 31 : 30) : (month % 2 == 0 ? 31 : 30);
    }
    return nbDaysInMonth;
}