/**
 * @file    timer0.c
 * @author  Group 6
 * @date    2026-11-23
 * @brief   Generic Timer0 driver implementation for ATmega328P.
 *
 * Configures Timer0 in CTC mode and calculates OCR0A dynamically based on
 * F_CPU, the selected prescaler, and the user-supplied time in milliseconds.
 */

#include "MCAL/timer0.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stddef.h>

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

static void (*Timer0_Callback)(void) = NULL;

void Timer0_Init(uint16_t time_ms, void (*callback)(void))
{
    uint32_t prescaler = 64UL;
    uint8_t  cs_bits   = (1U << CS01) | (1U << CS00); /* Default Prescaler = 64 */

    Timer0_Callback = callback;

    if (time_ms == 0U)
    {
        time_ms = 1U;
    }

    /* Select smallest suitable prescaler so OCR0A fits in 8-bit register (<= 255) */
    if (time_ms <= 1U)
    {
        prescaler = 64UL;
        cs_bits   = (1U << CS01) | (1U << CS00);
    }
    else if (time_ms <= 4U)
    {
        prescaler = 256UL;
        cs_bits   = (1U << CS02);
    }
    else
    {
        prescaler = 1024UL;
        cs_bits   = (1U << CS02) | (1U << CS00);
    }

    /* Lecture equation: OCR0A = ((F_CPU * T_req) / Prescaler) - 1 */
    uint32_t ocr_val = (((uint32_t)(F_CPU / 1000UL) * (uint32_t)time_ms) / prescaler) - 1UL;

    TCCR0A = (1U << WGM01);      /* CTC mode */
    TCNT0  = 0U;                 /* Reset counter */
    OCR0A  = (uint8_t)ocr_val;   /* Load calculated compare value */
    TIMSK0 |= (1U << OCIE0A);    /* Enable Compare Match A interrupt */
    TCCR0B = cs_bits;            /* Start timer with selected prescaler */
    sei();
}

void Timer0_Stop(void)
{
    TCCR0B = 0U;                 /* Stop timer clock */
    TIMSK0 &= ~(1U << OCIE0A);   /* Disable Compare Match A interrupt */
}

ISR(TIMER0_COMPA_vect)
{
    if (Timer0_Callback != NULL)
    {
        Timer0_Callback();
    }
}