/**
 * @file    timer0.h
 * @author  Group 6
 * @date    2026-11-22
 * @brief   Timer0 driver header file for ATmega328P.
 *
 * Provides 1ms periodic interrupt functionality using Timer0 in CTC mode,
 * dedicated to non-blocking peripheral timing such as buzzer ringing.
 */

#ifndef TIMER0_H
#define TIMER0_H

/**
 * @brief Start Timer0 in CTC mode to generate a 1ms periodic interrupt.
 *
 * Configures Timer0 with a prescaler of 64 and OCR0A = 249 (for 16 MHz F_CPU),
 * registers the provided callback, and enables global interrupts.
 *
 * @param callback Pointer to the function invoked every 1ms inside the ISR.
 */
void Timer0_Start_1ms(void (*callback)(void));

/**
 * @brief Stop Timer0 and disable its compare match interrupt.
 *
 * Clears the clock select bits and disables OCIE0A.
 */
void Timer0_Stop(void);

#endif /* TIMER0_H */