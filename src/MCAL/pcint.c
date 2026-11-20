#include "MCAL/pcint.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stddef.h>

static void (*PCINT2_Callback)(uint8_t pressed_mask) = NULL;
static volatile uint8_t prev_pind = 0xFF;

void PCINT2_Init(uint8_t pin_mask) {
    PCICR  |= (1 << PCIE2);   // تفعيل مقاطعات تغيير الحالة على PORTD
    PCMSK2 |= pin_mask;       // تحديد الأطراف المطلوبة
    prev_pind = PIND;
    sei();
}

void PCINT2_SetCallback(void (*callback)(uint8_t pressed_mask)) {
    PCINT2_Callback = callback;
}

ISR(PCINT2_vect) {
    uint8_t curr_pind = PIND;
    uint8_t changed   = prev_pind ^ curr_pind;
    uint8_t pressed   = changed & (~curr_pind); // اكتشاف لحظة الضغط (Falling Edge)
    prev_pind = curr_pind;

    if (PCINT2_Callback != NULL && pressed != 0) {
        PCINT2_Callback(pressed);
    }
}