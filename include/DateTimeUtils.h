// Maxalarm-clock
// https://gitlab.com/6wheels/maxalarm-clock
// Copyright (C) 2018 by Bertrand Ciroux and licensed under
// MIT https://opensource.org/licenses/MIT

#ifndef DATETIMEUTILS_H
#define DATETIMEUTILS_H

#include <Arduino.h>

byte daysInMonth(const unsigned int year, const byte month);
bool isLeap(const unsigned int year);

#endif
