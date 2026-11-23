/**
 * @file    timer1.c
 * @author  Group 6
 * @date    2026-11-23
 * @brief   Generic Timer1 driver implementation for ATmega328P.
 *
 * Configures 16-bit Timer1 in CTC mode and calculates OCR1A dynamically based on
 * F_CPU, the selected prescaler, and the user-supplied time in milliseconds.
 */

#include "MCAL/timer1.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stddef.h>

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

static void (*Timer1_Callback)(void) = NULL;

void Timer1_Init(uint16_t time_ms, void (*callback)(void))
{
    uint32_t prescaler = 256UL;
    uint8_t  cs_bits   = (1U << CS12); /* Default Prescaler = 256 */

    Timer1_Callback = callback;

    if (time_ms == 0U)
    {
        time_ms = 1U;
    }

    /* Select prescaler so OCR1A fits in 16-bit register (<= 65535) */
    if (time_ms <= 1048U)
    {
        prescaler = 256UL;
        cs_bits   = (1U << CS12);
    }
    else
    {
        prescaler = 1024UL;
        cs_bits   = (1U << CS12) | (1U << CS10);
    }

    /* Lecture equation: OCR1A = ((F_CPU * T_req) / Prescaler) - 1 */
    uint32_t ocr_val = (((uint32_t)(F_CPU / 1000UL) * (uint32_t)time_ms) / prescaler) - 1UL;

    TCCR1A = 0U;
    TCNT1  = 0U;
    OCR1A  = (uint16_t)ocr_val;              /* Load calculated compare value */
    TIMSK1 |= (1U << OCIE1A);                /* Enable Compare Match A interrupt */
    TCCR1B = (1U << WGM12) | cs_bits;        /* CTC mode + selected prescaler */
    sei();
}

ISR(TIMER1_COMPA_vect)
{
    if (Timer1_Callback != NULL)
    {
        Timer1_Callback();
    }
}