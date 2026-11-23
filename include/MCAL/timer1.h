/**
 * @file    timer1.h
 * @author  Group 6
 * @date    2026-11-23
 * @brief   Generic Timer1 driver header file for ATmega328P.
 *
 * Provides periodic hardware timer interrupts using 16-bit Timer1 in CTC mode.
 * The compare match register (OCR1A) is calculated dynamically from the
 * requested period in milliseconds.
 */

#ifndef TIMER1_H
#define TIMER1_H

#include <stdint.h>

/**
 * @brief Initialize and start Timer1 in CTC mode for a given period in ms.
 *
 * Automatically selects an appropriate prescaler and computes OCR1A using:
 * OCR1A = ((F_CPU / 1000) * time_ms / Prescaler) - 1.
 *
 * @param time_ms  Desired interrupt period in milliseconds (up to 4194 ms at 16 MHz).
 * @param callback Pointer to the function invoked on each compare match interrupt.
 */
void Timer1_Init(uint16_t time_ms, void (*callback)(void));

#endif /* TIMER1_H */