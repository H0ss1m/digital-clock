# Hardware Abstraction Headers

| File | Functionality |
|---|---|
| `lcd.h` | Defines PORTC pin mappings and the public HD44780 4-bit LCD API |
| `pushbuttons.h` | Defines button identifiers, PORTD pin mappings, button states, and button APIs |
| `buzzer.h` | Defines the PB0 pin mapping, logic states, 250 ms tone interval, and buzzer APIs |

Buttons are active-low and use internal pull-ups. The LCD uses PORTC pins PC0-PC5 in 4-bit mode.
