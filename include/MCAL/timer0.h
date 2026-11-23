/**
 * @file    timer0.h
 * @author  Group 6
 * @date    2026-11-23
 * @brief   Generic Timer0 driver header file for ATmega328P.
 *
 * Provides periodic hardware timer interrupts using Timer0 in CTC mode.
 * The compare match register (OCR0A) is calculated dynamically from the
 * requested period in milliseconds.
 */

#ifndef TIMER0_H
#define TIMER0_H

#include <stdint.h>

/**
 * @brief Initialize and start Timer0 in CTC mode for a given period in ms.
 *
 * Automatically selects an appropriate prescaler and computes OCR0A using:
 * OCR0A = ((F_CPU / 1000) * time_ms / Prescaler) - 1.
 *
 * @param time_ms  Desired interrupt period in milliseconds (1 to 16 ms at 16 MHz).
 * @param callback Pointer to the function invoked on each compare match interrupt.
 */
void Timer0_Init(uint16_t time_ms, void (*callback)(void));

/**
 * @brief Stop Timer0 and disable its compare match interrupt.
 */
void Timer0_Stop(void);

#endif /* TIMER0_H */