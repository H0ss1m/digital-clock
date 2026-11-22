/**
 * @file    clock_app.h
 * @author  Group 6
 * @date    2026-11-22
 * @brief   Digital Clock application layer header file.
 *
 * Exposes initialization and main execution loop entry points for the
 * Digital Clock with 12h/24h display modes, alarm setting, and intermittent buzzer.
 */

#ifndef CLOCK_APP_H
#define CLOCK_APP_H

/**
 * @brief Initialize all hardware drivers and application state variables.
 *
 * Initializes LCD, pushbuttons, buzzer, and Timer1 (1-second tick).
 */
void ClockApp_Init(void);

/**
 * @brief Start the main infinite application loop.
 *
 * Handles pushbutton inputs, LCD updates, blinking edit cursor,
 * alarm preview, and alarm silencing. This function does not return.
 */
void ClockApp_Start(void);

#endif /* CLOCK_APP_H */