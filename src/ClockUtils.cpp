#include "ClockUtils.h"
#include "DateTimeUtils.h"

void updateYear(tmElements_t &timeToSet, updateAction action)
{
    if (action == PLUS)
    {
        timeToSet.Year++;
    }
    else
    {
        timeToSet.Year--;
    }
}
void updateMonth(tmElements_t &timeToSet, updateAction action)
{
    if (action == PLUS)
    {
        timeToSet.Month = timeToSet.Month == 12 ? 1 : timeToSet.Month + 1;
    }
    else
    {
        timeToSet.Month = timeToSet.Month == 1 ? 12 : timeToSet.Month - 1;
    }
}
void updateDay(tmElements_t &timeToSet, updateAction action)
{
    int nbDaysInMonth = daysInMonth(timeToSet.Year, timeToSet.Month);
    if (action == PLUS)
    {
        timeToSet.Day = timeToSet.Day == nbDaysInMonth ? 1 : timeToSet.Day + 1;
    }
    else
    {
        timeToSet.Day = timeToSet.Day == 1 ? nbDaysInMonth : timeToSet.Day - 1;
    }
}
void updateHour(tmElements_t &timeToSet, updateAction action)
{
    if (action == PLUS)
    {
        timeToSet.Hour = timeToSet.Hour == 23 ? 0 : timeToSet.Hour + 1;
    }
    else
    {
        timeToSet.Hour = timeToSet.Hour == 0 ? 23 : timeToSet.Hour - 1;
    }
}
void updateMinute(tmElements_t &timeToSet, updateAction action)
{
    if (action == PLUS)
    {
        timeToSet.Minute = timeToSet.Minute == 59 ? 0 : timeToSet.Minute + 1;
    }
    else
    {
        timeToSet.Minute = timeToSet.Minute == 0 ? 59 : timeToSet.Minute - 1;
    }
}
