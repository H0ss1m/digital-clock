/**
 * @file    timer1.h
 * @author  Group 6
 * @date    2026-11-22
 * @brief   Timer1 driver header file for ATmega328P.
 *
 * Provides a precise 1-second hardware timer interrupt using Timer1 in CTC mode.
 */

#ifndef TIMER1_H
#define TIMER1_H

/**
 * @brief Initialize Timer1 to generate a 1-second periodic interrupt.
 *
 * Configures Timer1 in CTC mode with a prescaler of 256 and OCR1A = 62499
 * (for 16 MHz F_CPU), registers the callback function, and enables interrupts.
 *
 * @param callback Pointer to the function invoked every 1 second inside the ISR.
 */
void Timer1_Init_1Sec(void (*callback)(void));

#endif /* TIMER1_H */