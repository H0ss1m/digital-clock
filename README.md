# ATmega328P Digital Clock

A 24-hour digital clock and alarm written in C for the ATmega328P microcontroller. The project uses PlatformIO and is organized as a layered embedded application:

- Application logic for the clock, alarm, menus, and button actions
- Hardware Abstraction Layer (HAL) drivers for buttons, LCD, and buzzer
- Microcontroller Abstraction Layer (MCAL) drivers for GPIO, Timer1, and Pin Change Interrupt 2

The clock displays the current time and configured alarm on a 16x2 character LCD. Four push buttons control the mode and setting menus, and a buzzer signals a matching alarm time.

## Features

- 24-hour time keeping (`00:00:00` to `23:59:59`)
- Initial time: `12:00:00`
- Initial alarm: `12:01`
- Time-based rollover across seconds, minutes, hours, and midnight
- Adjustable clock hours and minutes
- Adjustable alarm hours and minutes
- Alarm that activates when hours and minutes match
- ACK button to silence a ringing alarm
- ACK button in normal mode to reset the seconds to zero
- Interrupt-based, non-blocking button detection
- 16x2 LCD output in 4-bit data mode
- Modular source structure based on APP, HAL, and MCAL layers

## Display

### Normal mode

```text
Time: 12:34:56
Alrm: 12:35
```

The second row changes while the alarm is active:

```text
ALARM RINGING!
```

### Setting modes

Use the MODE button to move through these screens:

```text
Set Clock Hours
Set Clock Mins
Set Alarm Hours
Set Alarm Mins
```

An arrow (`->`) is shown before the currently selected value.

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
[GND] ---[ Mode ]----| PD2     PC2 (A2) |-------> [ D4 ] LCD 16x2
[GND] ---[  Up  ]----| PD3     PC3 (A3) |-------> [ D5 ] LCD 16x2
[GND] ---[ Down ]----| PD4     PC4 (A4) |-------> [ D6 ] LCD 16x2
[GND] ---[  Ack ]----| PD5     PC5 (A5) |-------> [ D7 ] LCD 16x2
|                             |
|                             |
[GND] --[- BUZ +]----| PB0     |
|                             |
+-----------------------------+

-------------------------------------------------------------------------------
LCD power and control pins:
[ VDD ] ---------------------> Vdd (5 V)
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

| Button | ATmega328P pin | Arduino Uno pin | MODE transition |
|---|---:|---:|---|
| MODE | PD2 | D2 | Moves Normal -> Clock Hours -> Clock Minutes -> Alarm Hours -> Alarm Minutes -> Normal |
| UP | PD3 | D3 | Increases the value in a setting mode |
| DOWN | PD4 | D4 | Decreases the value in a setting mode |
| ACK | PD5 | D5 | Silences the alarm or resets seconds in Normal mode |

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

1. Apply power. The clock starts at `12:00:00` and the alarm starts at `12:01`.
2. Read the time and alarm on the LCD.
3. Press **MODE** repeatedly to enter and exit each setting screen.
4. In a setting screen, press **UP** or **DOWN** to change the displayed value.
5. Press **MODE** until the normal clock screen appears.
6. In normal mode, press **ACK** to reset seconds to zero.
7. When the time reaches the configured alarm time, the buzzer turns on and `ALARM RINGING!` appears.
8. Press **ACK** while ringing to turn off the buzzer and leave normal operation.

### Alarm Timing

The alarm activates only when all of these conditions are true:

- The clock is in normal mode
- Hours match
- Minutes match
- Seconds are zero

After the alarm is acknowledged, the current firmware allows it to remain silent until the next matching hour and minute occurrence. A test at a time of `12:01:00` will ring for the next matching minute boundary unless seconds or mode state change.

### State and Reset Behavior

Clock time, alarm time, and setting modes are stored in RAM. They are **not** saved to EEPROM or nonvolatile flash storage. Rebooting or disconnecting power resets:

- Clock time to `12:00:00`
- Alarm time to `12:01`
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
|   |-- APP/
|   |   |-- clock_app.c
|   |-- HAL/
|   |   |-- button.c
|   |   |-- buzzer.c
|   |   |-- lcd.c
|   |-- MCAL/
|       |-- gpio.c
|       |-- pcint.c
|       |-- timer1.c
|-- include/
|   |-- APP/
|   |-- HAL/
|   |-- MCAL/
|-- lib/
|-- test/
|-- .pio/
```

### Source Responsibilities

| Module | Responsibility |
|---|---|
| `main.c` | Initializes the clock application and enters the main update loop |
| `clock_app.c` | Keeps time, manages modes and alarm state, handles button actions, and updates the LCD |
| `button.c` | Configures button pins and forwards falling-edge button events to the application |
| `lcd.c` | Controls the HD44780-compatible LCD in 4-bit mode |
| `buzzer.c` | Turns the buzzer output on or off |
| `gpio.c` | Provides direction, output, input, and pull-up GPIO operations |
| `timer1.c` | Configures ATmega328P Timer1 for a one-second callback |
| `pcint.c` | Configures Pin Change Interrupt 2 for PD2-PD5 |

## Firmware Design

### Main Loop

`main()` calls `ClockApp_Init()` once and then calls `ClockApp_Update()` repeatedly.

The update loop:

1. Clears the LCD when a mode transition occurs
2. Checks for the alarm match
3. Controls the buzzer
4. Writes the appropriate normal or setting text to the LCD
5. Waits 50 ms

### Timer1

Timer1 runs in CTC mode with a 256 prescaler and compare value `62499`. With a 16 MHz clock, this produces a one-second interrupt:

```text
16,000,000 / 256 / (62499 + 1) = approximately 1 Hz
```

Each interrupt increments the clock's seconds. The callback updates minute, hour, and midnight rollover values.

### Button Interrupts

Buttons on PORTD (PD2-PD5) use the PCINT2 pin-change interrupt. On each interrupt, the firmware compares the previous and current pin values and detects the falling edge for an assumed active-low button.

The callback masks the pressed pins and invokes the application button handler immediately.

## Customization

### Change Pin Assignments

Update the pin definitions in the relevant HAL files:

- `include/HAL/button.h` for button pins
- `src/HAL/lcd.c` for LCD pins
- `src/HAL/buzzer.c` for the buzzer pin

Changing pins may require the corresponding MCAL/interrupt configuration to be updated.

### Change the Default Time

The initial clock state is declared in `src/APP/clock_app.c`:

```c
static volatile uint8_t sec = 0, min = 0, hr = 12;
```

Change `hr`, `min`, or `sec` to the desired power-on value.

### Change the Default Alarm

The initial alarm state is declared in `src/APP/clock_app.c`:

```c
static volatile uint8_t alarm_hr = 12, alarm_min = 1;
```

Change `alarm_hr` and `alarm_min` to the desired power-on alarm.

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
7. Confirm the buzzer and `ALARM RINGING!` screen activate at seconds zero.
8. Press ACK and confirm the buzzer turns off.
9. Press ACK in normal mode and confirm seconds reset to zero.
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
- Confirm MODE, UP, DOWN, and ACK are on PD2-PD5
- Confirm all button pins are unpowered and floating internally only when the button is released
- Check for solder joints or loose jumper wires

### Buzzer Stays On

- Press ACK
- Check PB0 wiring
- Verify `Buzzer_On()` and `Buzzer_Off()` behavior
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
