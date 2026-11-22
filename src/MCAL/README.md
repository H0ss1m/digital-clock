# Microcontroller Abstraction Layer

The MCAL contains the direct ATmega328P hardware drivers.

| File | Functionality |
|---|---|
| `gpio.c` | Configures GPIO direction and reads or writes PORTB, PORTC, and PORTD pins |
| `timer0.c` | Starts/stops Timer0 CTC mode for 1 ms callback ticks used by the buzzer |
| `timer1.c` | Initializes Timer1 CTC mode for a 1-second callback used by clock state |
| `pcint.c` | Enables pin-change interrupts for a PORTD pin mask and dispatches falling-edge callback masks |

All register access in this layer uses AVR I/O definitions and interrupt vectors. Higher-level application and HAL code should call these APIs rather than editing registers directly.
