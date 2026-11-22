/**
 * @file    buzzer.h
 * @author  Group 6
 * @date    2026-11-22
 * @brief   Buzzer HAL driver header file.
 *
 * Provides hardware abstraction for controlling an intermittent ringing buzzer
 * using Timer0. Logic polarity (NPN vs. PNP) can be changed solely by swapping
 * the order of BUZZER_OFF and BUZZER_ON in buzzer_state_t.
 */

#ifndef BUZZER_H
#define BUZZER_H

#include "MCAL/gpio.h"

/* Hardware Pin Mapping */
#define BUZZER_Pin        GPIO_PIN_0
#define BUZZER_GPIO_Port  GPIO_PORTB

/* Ringing toggle interval in milliseconds */
#define BUZZER_TONE_TIME  250U /* ms */

/**
 * @brief Buzzer logic states.
 *
 * Swap BUZZER_OFF and BUZZER_ON if switching the hardware driver
 * transistor between NPN (active-high) and PNP (active-low).
 */
typedef enum
{
    BUZZER_OFF = 0,
    BUZZER_ON  = 1
} buzzer_state_t;

/**
 * @brief Initialize the buzzer GPIO pin and ensure the buzzer is turned off.
 */
void Buzzer_Init(void);

/**
 * @brief Start intermittent buzzer ringing using hardware Timer0.
 *
 * Continuously toggles the buzzer between BUZZER_ON and BUZZER_OFF
 * every BUZZER_TONE_TIME milliseconds.
 */
void Buzzer_StartBeep(void);

/**
 * @brief Stop buzzer ringing and turn the buzzer off.
 *
 * Stops Timer0 and drives the buzzer pin to BUZZER_OFF.
 */
void Buzzer_StopBeep(void);

#endif /* BUZZER_H */