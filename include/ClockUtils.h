// Maxalarm-clock
// https://gitlab.com/6wheels/maxalarm-clock
// Copyright (C) 2018 by Bertrand Ciroux and licensed under
// MIT https://opensource.org/licenses/MIT

#ifndef CLOCKUTILS_H
#define CLOCKUTILS_H

#include <TimeLib.h>

typedef enum
{
    PLUS,
    MINUS
} updateAction_t;

void updateYear(tmElements_t &timeToSet, updateAction_t action);
void updateMonth(tmElements_t &timeToSet, updateAction_t action);
void updateDay(tmElements_t &timeToSet, updateAction_t action);
void updateHour(tmElements_t &timeToSet, updateAction_t action);
void updateMinute(tmElements_t &timeToSet, updateAction_t action);

#endif
