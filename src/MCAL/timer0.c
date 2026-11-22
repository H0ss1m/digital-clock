/**
 * @file    timer0.c
 * @author  Group 6
 * @date    2026-11-22
 * @brief   Timer0 driver implementation for ATmega328P.
 *
 * Implements Timer0 CTC configuration for 1ms ticks and handles the
 * TIMER0_COMPA interrupt vector.
 */

#include "MCAL/timer0.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stddef.h>

static void (*Timer0_Callback)(void) = NULL;

void Timer0_Start_1ms(void (*callback)(void))
{
    Timer0_Callback = callback;

    TCCR0A = (1U << WGM01);                /* CTC mode */
    TCNT0  = 0U;                           /* Reset counter */
    OCR0A  = 249U;                         /* 1ms tick at 16 MHz / 64 */
    TIMSK0 |= (1U << OCIE0A);              /* Enable Compare Match A interrupt */
    TCCR0B = (1U << CS01) | (1U << CS00);  /* Start timer with Prescaler = 64 */
    sei();
}

void Timer0_Stop(void)
{
    TCCR0B = 0U;                           /* Stop timer clock */
    TIMSK0 &= ~(1U << OCIE0A);             /* Disable Compare Match A interrupt */
}

ISR(TIMER0_COMPA_vect)
{
    if (Timer0_Callback != NULL)
    {
        Timer0_Callback();
    }
}