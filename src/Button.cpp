#include "Button.h"

Button::Button(uint8_t pin) : _pin(pin)
{
}

void Button::begin()
{
    pinMode(_pin, INPUT_PULLUP);
    _state = digitalRead(_pin);
}

bool Button::isReleased()
{
    bool v = digitalRead(_pin);
    if (v != _state)
    {
        _state = v;
        if (_state)
        {
            return true;
        }
    }
    return false;
}