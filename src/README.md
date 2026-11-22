# Source Directory

The firmware source is divided into three layers:

| Directory | Responsibility |
|---|---|
| `APP/` | Application state, clock logic, menus, alarm logic, and LCD presentation |
| `HAL/` | Hardware-oriented drivers for the LCD, buttons, and buzzer |
| `MCAL/` | Direct ATmega328P register and interrupt drivers |

## Files

| File | Functionality |
|---|---|
| `main.c` | Calls `ClockApp_Init()` and starts the application's infinite loop |
| `APP/clock_app.c` | Implements timekeeping, setting fields, 12/24-hour formatting, alarm matching, button actions, and display updates |
| `HAL/lcd.c` | Sends HD44780 commands and data through six GPIO pins in 4-bit mode |
| `HAL/pushbuttons.c` | Enables input pull-ups and reads SET, UP, DOWN, and ACK button states |
| `HAL/buzzer.c` | Initializes PB0 and starts/stops a 250 ms intermittent buzzer pattern |
| `MCAL/gpio.c` | Accesses ATmega328P GPIO direction, output, input, and pull-up registers |
| `MCAL/timer0.c` | Provides a 1 ms callback interrupt for buzzer timing |
| `MCAL/timer1.c` | Provides a 1-second callback interrupt for clock timing |
| `MCAL/pcint.c` | Supplies reusable PORTD pin-change interrupt setup and callback dispatch |
