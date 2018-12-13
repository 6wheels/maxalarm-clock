#ifndef CLOCKUTILS_H
#define CLOCKUTILS_H

#include <TimeLib.h>

// ----- Enum
typedef enum
{
    PLUS,
    MINUS
} updateAction;
// ----

void updateYear(tmElements_t &timeToSet, updateAction action);
void updateMonth(tmElements_t &timeToSet, updateAction action);
void updateDay(tmElements_t &timeToSet, updateAction action);
void updateHour(tmElements_t &timeToSet, updateAction action);
void updateMinute(tmElements_t &timeToSet, updateAction action);

#endif
