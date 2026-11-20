#ifndef PCINT_H
#define PCINT_H

#include <stdint.h>

void PCINT2_Init(uint8_t pin_mask);
void PCINT2_SetCallback(void (*callback)(uint8_t pressed_mask));

#endif