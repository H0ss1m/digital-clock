/**
 * @file    buzzer.c
 * @author  Group 6
 * @date    2026-11-22
 * @brief   Buzzer HAL driver implementation.
 *
 * Controls the buzzer output pin and utilizes Timer0 interrupts to produce
 * an intermittent ringing pattern instead of a continuous tone.
 */

#include "HAL/buzzer.h"
#include "MCAL/timer0.h"
#include <stdint.h>

static volatile buzzer_state_t current_state = BUZZER_OFF;
static volatile uint16_t tone_ms_counter     = 0U;

/**
 * @brief Internal Timer0 1ms callback to toggle buzzer state periodically.
 */
static void Buzzer_TimerTick(void)
{
    tone_ms_counter++;
    if (tone_ms_counter >= BUZZER_TONE_TIME)
    {
        tone_ms_counter = 0U;
        current_state = (current_state == BUZZER_ON) ? BUZZER_OFF : BUZZER_ON;
        GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, (uint8_t)current_state);
    }
}

void Buzzer_Init(void)
{
    GPIO_SetPinDirection(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_OUTPUT);
    current_state = BUZZER_OFF;
    GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, (uint8_t)BUZZER_OFF);
}

void Buzzer_StartBeep(void)
{
    tone_ms_counter = 0U;
    current_state   = BUZZER_ON;
    GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, (uint8_t)BUZZER_ON);
    Timer0_Init(1U, Buzzer_TimerTick); /* Pass 1ms base tick to generic Timer0_Init */
}

void Buzzer_StopBeep(void)
{
    Timer0_Stop();
    tone_ms_counter = 0U;
    current_state   = BUZZER_OFF;
    GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, (uint8_t)BUZZER_OFF);
}