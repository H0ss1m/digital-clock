#include "MCAL/timer1.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stddef.h>

static void (*Timer1_Callback)(void) = NULL;

void Timer1_Init_1Sec(void) {
    TCCR1A = 0;
    TCCR1B = (1 << WGM12) | (1 << CS12); // وضع CTC مع Prescaler 256
    OCR1A  = 62499;                      // 1 ثانية عند تردد 16MHz
    TIMSK1 = (1 << OCIE1A);              // تفعيل مقاطعة المقارنة
    sei();                               // تفعيل المقاطعات العامة
}

void Timer1_SetCallback(void (*callback)(void)) {
    Timer1_Callback = callback;
}

ISR(TIMER1_COMPA_vect) {
    if (Timer1_Callback != NULL) {
        Timer1_Callback();
    }
}