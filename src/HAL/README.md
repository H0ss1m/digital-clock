# Hardware Abstraction Layer

The HAL provides feature-level operations so application code does not manipulate registers directly.

| File | Functionality |
|---|---|
| `lcd.c` | Initializes an HD44780 16x2 LCD in 4-bit mode and provides command, character, string, cursor, clear, and underline-cursor operations |
| `pushbuttons.c` | Initializes PD2-PD5 as inputs with pull-ups and translates active-low pin levels into button states |
| `buzzer.c` | Initializes PB0 and uses Timer0 to toggle the alarm output every 250 ms |

Corresponding public declarations and pin mappings are located in the matching files under `include/HAL/`.
