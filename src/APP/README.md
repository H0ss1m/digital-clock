# Application Layer

## `clock_app.c`

This file contains the main digital-clock application.

### Responsibilities

- Maintains hour, minute, and second values in 24-hour format
- Rolls time over at 60 seconds, 60 minutes, and 24 hours
- Maintains alarm hour, minute, and second values
- Converts values to 12-hour display format with AM/PM
- Manages normal, alarm-preview, set-time, and set-alarm states
- Polls buttons and detects fresh presses
- Provides a blinking underline cursor for editable fields
- Starts the buzzer when clock and alarm values match
- Stops the buzzer when UP or DOWN is pressed while ringing
- Updates the LCD after state or time changes

### Public API

The implementation exposes `ClockApp_Init()` and `ClockApp_Start()` through `include/APP/clock_app.h`.
