# ATmega328P Digital Clock

A PlatformIO firmware project that implements a 12-hour/24-hour digital clock and alarm on an ATmega328P. The firmware is organized into application, hardware abstraction, and microcontroller abstraction layers.

The application is built around:

- Timer1 for a 1-second clock tick
- Timer0 for the buzzer toggle interval
- A 16x2 HD44780-compatible LCD
- Four active-low pushbuttons
- A buzzer output

## Features

- 24-hour internal time storage (`00:00:00` through `23:59:59`)
- Selectable 12-hour or 24-hour LCD output
- Initial time of `12:00:00`
- Initial alarm of `12:01:00`
- Second, minute, hour, and midnight rollover
- Editable clock and alarm hours, minutes, and seconds
- 12-hour AM/PM editing
- An intermittent hardware-timed alarm buzzer
- UP/DOWN alarm silence while ringing
- SET-menu navigation with a blinking underline cursor
- A two-second alarm-time preview
- 16x2 LCD output in 4-bit data mode
- Modular APP, HAL, and MCAL source organization

## Display

### Normal mode

```text
Keep Going Grp 6
Time:12:34:56 pm
```

The first row remains unchanged in normal mode. ACK switches the time between 12-hour and 24-hour formats.

### Alarm preview

UP or DOWN in normal mode displays the configured alarm for two seconds:

```text
Alarm Time:
 12:35:00 pm
```

### Setting modes

SET steps through the clock fields and alarm fields:

```text
Set time:
12:34:56 pm

Set alarm:
12:35:00 pm
```

SET advances from hours to minutes to seconds and, in 12-hour mode, AM/PM. A blinking underline identifies the active field.

## Required Hardware

- ATmega328P at 16 MHz
- 16x2 HD44780-compatible LCD
- 4 normally-open push buttons
- Buzzer or buzzer module
- 5 V supply
- Common ground for every component
- Optional LCD contrast potentiometer

An Arduino Uno or an equivalent ATmega328P board can be used, but the firmware pin numbers and board voltage requirements must match the wiring below.

## Pin Mapping

### Connection Diagram

```text
                      +-----------------------------+
                      |                             |
                      |         ATmega328P          |
                      |                             |
                      |                    PC0 (A0) |-------> [ RS ] LCD 16x2
                      |                    PC1 (A1) |-------> [ E  ] LCD 16x2
 [GND] ---[ Mode ]----| PD2                PC2 (A2) |-------> [ D4 ] LCD 16x2
 [GND] ---[  Up  ]----| PD3                PC3 (A3) |-------> [ D5 ] LCD 16x2
 [GND] ---[ Down ]----| PD4                PC4 (A4) |-------> [ D6 ] LCD 16x2
 [GND] ---[  Ack ]----| PD5                PC5 (A5) |-------> [ D7 ] LCD 16x2
                      |                             |
                      |                             |
 [GND] --[- BUZ +]----| PB0                         |
                      |                             |
                      +-----------------------------+

-------------------------------------------------------------------------------
* LCD power and control pins:
  [ VDD ] ---------------------> Vdd (5V)
  [ VSS ] ---------------------> Ground (GND)
  [ VEE ] (Contrast) ----------> Ground (GND)
  [ RW  ] (Read/Write) --------> Ground (GND)
```

The button wiring assumes that each button connects the indicated pin directly to GND. The ATmega328P internal pull-ups keep the pins HIGH when the buttons are released.

### LCD

| LCD pin | ATmega328P pin | Arduino Uno pin | Function |
|---|---:|---:|---|
| RS | PC0 | A0 | Register select |
| EN | PC1 | A1 | Enable |
| D4 | PC2 | A2 | Data bit 4 |
| D5 | PC3 | A3 | Data bit 5 |
| D6 | PC4 | A4 | Data bit 6 |
| D7 | PC5 | A5 | Data bit 7 |
| VSS | GND | GND | Ground |
| VDD | 5 V | 5 V | LCD supply |
| VO | Potentiometer wiper | - | Contrast adjustment |
| RW | GND | GND | Write mode (firmware only writes) |
| A | 5 V through resistor | - | Backlight positive |
| K | GND | GND | Backlight negative |

The LCD interface uses 4-bit mode. LCD pins D0 through D3 are not used and do not need to be connected.

### Buttons

The buttons connect between ground and their PORTD pin. Internal pull-ups are enabled in firmware.

| Button | ATmega328P pin | Arduino Uno pin | Function |
|---|---:|---:|---|
| SET | PD2 | D2 | Enters settings and advances the active clock/alarm field |
| UP | PD3 | D3 | Increases a setting or opens the alarm-time preview |
| DOWN | PD4 | D4 | Decreases a setting or opens the alarm-time preview |
| ACK | PD5 | D5 | Switches the display between 12-hour and 24-hour formats |

For each button:

```text
5 V MCU rail
    |
internal pull-up (enabled by firmware)
    |
button pin (PD2-PD5)
    |
normally-open button
    |
GND
```

Do not add an external pull-up in addition to the internal pull-up. An external resistor to ground is not required.

### Buzzer

| Buzzer connection | ATmega328P pin | Arduino Uno pin |
|---|---:|---|
| Signal / positive | PB0 | D8 |
| Ground / negative | GND | GND |

`PB0` is driven HIGH while the alarm rings and LOW otherwise. If the buzzer is a low-current piezo element, verify its voltage and current requirements. If it requires more current than the MCU pin can safely provide, use a transistor or a suitable active buzzer module.

## Operation

1. Apply power. The clock starts at `12:00:00`, the alarm starts at `12:01:00`, and the display uses 12-hour format.
2. Read the time and alarm on the LCD.
3. Press **ACK** to toggle 12-hour/24-hour display format.
4. Press **UP** or **DOWN** in normal mode to preview the alarm time.
5. Press **SET** to enter clock setting. SET advances through hours, minutes, seconds, and AM/PM.
6. In a setting screen, press **UP** or **DOWN** to change the active field.
7. Press **SET** repeatedly until the normal clock screen appears.
8. When the time reaches the configured alarm, the buzzer toggles intermittently.
9. Press **UP** or **DOWN** while ringing to silence the alarm.

### Alarm Timing

The alarm activates when the current hour, minute, and second match the configured alarm. The check runs on each Timer1 1-second callback.

Pressing UP or DOWN stops an active ring and consumes that button press. The clock can ring again when the alarm state is matched by a later timer tick.

### State and Reset Behavior

Clock time, alarm time, display format, and setting modes are stored in RAM. They are **not** saved to EEPROM or nonvolatile flash storage. Rebooting or disconnecting power resets:

- Clock time to `12:00:00`
- Alarm time to `12:01:00`
- Current mode to normal mode
- Alarm state to off

## Development Setup

### Prerequisites

- Visual Studio Code
- PlatformIO IDE extension
- A supported programmer or USB-to-serial board, when uploading to hardware
- PlatformIO's Atmel AVR toolchain

The `.vscode/extensions.json` file recommends the PlatformIO IDE extension.

### Build the Firmware

Open the project folder in VS Code and run:

```sh
pio run
```

Or use the PlatformIO **Build** button. The firmware is compiled for a 16 MHz ATmega328P.

Build output is placed under:

```text
.pio/build/ATmega328P/
```

The generated firmware files include `firmware.hex` and `firmware.elf`.

### Upload the Firmware

Connect the programmer or board and run:

```sh
pio run --target upload
```

No custom upload protocol or upload port is declared in `platformio.ini`. PlatformIO will use its environment defaults and may require the correct serial port or programmer to be selected before uploading.

### PlatformIO Configuration

The project configuration is:

```ini
[env:ATmega328P]
platform = atmelavr
board = ATmega328P
board_build.mcu = atmega328p
board_build.f_cpu = 16000000L
```

This configuration selects the AVR GCC toolchain and configures `F_CPU` for 16 MHz timing.

## Project Structure

```text
digital clock/
|-- platformio.ini
|-- src/
|   |-- main.c
|   |-- APP/clock_app.c
|   |-- HAL/buzzer.c
|   |-- HAL/lcd.c
|   |-- HAL/pushbuttons.c
|   `-- MCAL/gpio.c
|       MCAL/pcint.c
|       MCAL/timer0.c
|       MCAL/timer1.c
|-- include/
|   |-- APP/clock_app.h
|   |-- HAL/buzzer.h
|   |-- HAL/lcd.h
|   |-- HAL/pushbuttons.h
|   `-- MCAL/gpio.h
|       MCAL/pcint.h
|       MCAL/timer0.h
|       MCAL/timer1.h
|-- lib/
|-- test/
`-- .pio/
```

### Source Responsibilities

| Module | Responsibility |
|---|---|
| `main.c` | Initializes the clock application and enters the infinite application loop |
| `clock_app.c` | Keeps time, manages modes and alarm state, handles button actions, and updates the LCD |
| `pushbuttons.c` | Configures PD2-PD5 and reads their active-low states |
| `lcd.c` | Controls the HD44780-compatible LCD in 4-bit mode |
| `buzzer.c` | Produces an intermittent buzzer signal with a 250 ms toggle interval |
| `gpio.c` | Provides GPIO direction, output, input, and pull-up operations |
| `timer0.c` | Generates 1 ms interrupts for buzzer timing |
| `timer1.c` | Generates 1-second interrupts for clock updates |
| `pcint.c` | Provides a pin-change-interrupt callback helper for PORTD |

## Firmware Design

### Main Loop

`main()` calls `ClockApp_Init()` once and then calls `ClockApp_Start()`. `ClockApp_Start()` runs forever and performs:

1. Falling-edge button polling
2. Alarm suppression and format/mode handling
3. Clock and alarm field adjustment
4. Alarm-preview timeout handling
5. LCD rendering and underline-cursor blinking
6. A 20 ms loop delay

### Timer1

Timer1 runs in CTC mode with a 256 prescaler and compare value `62499`. With a 16 MHz clock, this produces a one-second interrupt:

```text
16,000,000 / 256 / (62499 + 1) = approximately 1 Hz
```

Each interrupt increments the clock's seconds. The callback updates minute, hour, and midnight rollover values.

### Button Handling

The application polls the active-low buttons every 20 ms and detects a rising edge of the logical button state (`PB_NOT_PRESSED` to `PB_PRESSED`). Internal pull-ups keep released pins HIGH.

The MCAL `pcint.c` module provides pin-change-interrupt support, but the current application does not register or use it. It reads the four button GPIO pins directly through the HAL.

## Customization

### Change Pin Assignments

Update the pin definitions in the relevant HAL headers:

- `include/HAL/pushbuttons.h` for button pins
- `include/HAL/lcd.h` for LCD pins
- `include/HAL/buzzer.h` for the buzzer pin

Changing pins may require the corresponding MCAL/interrupt configuration to be updated.

### Change the Default Time

The initial clock state is declared in `src/APP/clock_app.c`:

```c
cur_hour = 12U;
cur_min = 0U;
cur_sec = 0U;
```

### Change the Default Alarm

The initial alarm state is declared in `src/APP/clock_app.c`:

```c
alm_hour = 12U;
alm_min = 1U;
alm_sec = 0U;
```

### Change the Clock Frequency

The firmware and Timer1 calculations assume a 16 MHz clock:

```ini
board_build.f_cpu = 16000000L
```

If the MCU frequency changes, update `F_CPU` and verify or recalculate Timer1's compare value.

## Testing and Verification

The repository contains the standard empty PlatformIO `test/README` placeholder; no automated test suite is currently implemented.

Recommended manual test sequence:

1. Build the project successfully.
2. Upload the firmware.
3. Confirm both LCD rows show normally.
4. Wait across a 60-second boundary and confirm minute rollover.
5. Set the clock to `23:59`, wait for midnight, and confirm it becomes `00:00`.
6. Set an alarm to the next minute.
7. Confirm the buzzer toggles at the alarm time.
8. Press UP or DOWN and confirm the buzzer stops.
9. Press ACK and confirm the display switches from 12-hour to 24-hour format and back.
10. Test all UP/DOWN wraparound behavior at `23/00` and `59/00`.

## Troubleshooting

### Blank or Garbled LCD

- Verify that LCD and MCU grounds are common
- Verify RS, EN, D4-D7 wiring against the pin table
- Ground LCD RW
- Adjust the LCD contrast potentiometer
- Confirm the LCD supply voltage
- Check 16 MHz build configuration

### Buttons Do Nothing

- Verify each button connects its MCU pin to ground when pressed
- Confirm SET, UP, DOWN, and ACK are on PD2-PD5
- Confirm all button pins are unpowered and floating internally only when the button is released
- Check for solder joints or loose jumper wires

### Buzzer Stays On

- Press UP or DOWN
- Check PB0 wiring
- Verify `Buzzer_StartBeep()` and `Buzzer_StopBeep()` behavior
- Confirm that the alarm time has not matched again due to the minute/hour rollover

### Clock Runs at the Wrong Speed

- Confirm the board or MCU is running at 16 MHz
- Confirm `board_build.f_cpu = 16000000L`
- Check the Timer1 prescaler and compare configuration
- Check the external crystal or board clock configuration

### Upload or Build Fails

- Confirm the PlatformIO IDE extension is installed
- Close serial-monitor tools using the board port before upload
- Select the correct port/programmer
- Run `pio run -t clean` and then `pio run`
- Confirm the `ATmega328P` MCU and target board are available to PlatformIO

## Safety Notes

- Turn off power before changing wiring
- Do not exceed the ATmega328P pin current rating
- Use a driver circuit or buzzer module for loads that need more current
- Verify polarity for active buzzers and LCD backlight components
- Keep wiring short and use a common ground to reduce noise
- This project is intended for learning, simulation, and low-voltage hobby use
