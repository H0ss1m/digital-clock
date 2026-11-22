/**
 * @file    timer1.c
 * @author  Group 6
 * @date    2026-11-22
 * @brief   Timer1 driver implementation for ATmega328P.
 *
 * Implements Timer1 CTC configuration for 1-second ticks and dispatches
 * the registered callback inside the TIMER1_COMPA ISR.
 */

#include "MCAL/timer1.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stddef.h>

static void (*Timer1_Callback)(void) = NULL;

void Timer1_Init_1Sec(void (*callback)(void))
{
    Timer1_Callback = callback;

    TCCR1A = 0U;
    TCNT1  = 0U;
    OCR1A  = 62499U;                         /* 1s tick at 16 MHz / 256 */
    TIMSK1 |= (1U << OCIE1A);                /* Enable Compare Match A interrupt */
    TCCR1B = (1U << WGM12) | (1U << CS12);   /* CTC mode, Prescaler = 256 */
    sei();
}

ISR(TIMER1_COMPA_vect)
{
    if (Timer1_Callback != NULL)
    {
        Timer1_Callback();
    }
}