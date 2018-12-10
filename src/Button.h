#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

class Button
{
  private:
    bool _state;
    uint8_t _pin;

  public:
    Button(uint8_t pin);
    void begin();
    bool isReleased();
};

#endif