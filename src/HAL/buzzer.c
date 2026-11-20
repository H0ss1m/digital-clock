#include "HAL/buzzer.h"
#include "MCAL/gpio.h"

#define BUZ_PORT GPIO_PORTB
#define BUZ_PIN  0 // PB0

void Buzzer_Init(void) {
    GPIO_SetPinDirection(BUZ_PORT, BUZ_PIN, GPIO_OUTPUT);
    Buzzer_Off();
}

void Buzzer_On(void) {
    GPIO_WritePin(BUZ_PORT, BUZ_PIN, GPIO_HIGH);
}

void Buzzer_Off(void) {
    GPIO_WritePin(BUZ_PORT, BUZ_PIN, GPIO_LOW);
}