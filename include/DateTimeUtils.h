#ifndef DATETIMEUTILS_H
#define DATETIMEUTILS_H

#include <Arduino.h>

byte daysInMonth(unsigned int year, byte month);

bool isLeap(unsigned int year);

#endif
