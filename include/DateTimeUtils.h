// Maxalarm-clock
// https://gitlab.com/6wheels/maxalarm-clock
// Copyright (C) 2018 by Bertrand Ciroux and licensed under
// MIT https://opensource.org/licenses/MIT

#ifndef DATETIMEUTILS_H
#define DATETIMEUTILS_H

#include <Arduino.h>

byte daysInMonth(unsigned int year, byte month);
bool isLeap(unsigned int year);

#endif
