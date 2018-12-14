# Maxalarm-clock
## Description
This is a simple alarm-clock for children. A blue led lights when it's time to wake up, a red led indicates to stay in bed.
Why Max? because of my son's name.

## Hardware
- Arduino Nano
- DS3231 RTC Module
- 3 push buttons
- 2 LEDs (a blue and a red one)
- Resistors
  - 220Ω for the red led
  - 440Ω for the blue led
  - 4.7kΩ for the SQW interrupt (currently unused)
- 4*7 segment digits display using i2c bus

## How to use
1. Open the `schematic.fzz` with Fritzing to know how to wire everything.
2. Clone the project, I use VSCode and PlatformIO for convenience.
3. Adjust the pin numbers in the `main.cpp` if you changes the wiring
4. Upload and test!

By default, if the RTC module is powered for the first or when replacing the battery, the date is set to December, 1st, 2018, 00:00:00.
- Use `mode button` (default pin 8) to cycle through these steps:
```
|>  0 (default, display clock)
|       |
|       v
|   1 (set year)
|       |
|       v
|   2 (set month)
|       |
|       v
|   3 (set day)
|       |
|       v
|   4 (set hour)
|       |
|       v
|   5 (set minute)
|       |
--------+
```
- Use `-` (default pin 9) and `+` (default pin 10) buttons to adjust values

Time is written to RTC module when returning to mode 0.

## Librairies
If you don't use PlatformIO, here are the librairies used in this project:
- OneButton, easy to use button handling: https://github.com/mathertel/OneButton
- DS3232RTC, RTC read and write : https://github.com/JChristensen/DS3232RTC
- Time, useful date and time manipulation: https://github.com/PaulStoffregen/Time
- Streaming, to use the << operator for printing: https://platformio.org/lib/show/560/Streaming
