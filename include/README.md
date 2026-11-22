# Public Headers

This directory contains declarations shared by the firmware source modules.

| Directory | File guide |
|---|---|
| `APP/` | `clock_app.h` exposes application initialization and the infinite application entry point |
| `HAL/` | `lcd.h`, `pushbuttons.h`, and `buzzer.h` expose peripheral APIs and pin mappings |
| `MCAL/` | `gpio.h`, `timer0.h`, `timer1.h`, and `pcint.h` expose register-level hardware APIs |

Source files include these headers with paths such as `APP/clock_app.h`, `HAL/lcd.h`, or `MCAL/gpio.h`.
