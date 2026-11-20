#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>

#define GPIO_PORTB 0
#define GPIO_PORTC 1
#define GPIO_PORTD 2

#define GPIO_INPUT  0
#define GPIO_OUTPUT 1

#define GPIO_LOW  0
#define GPIO_HIGH 1

void GPIO_SetPinDirection(uint8_t port, uint8_t pin, uint8_t direction);
void GPIO_WritePin(uint8_t port, uint8_t pin, uint8_t value);
uint8_t GPIO_ReadPin(uint8_t port, uint8_t pin);
void GPIO_EnablePullUp(uint8_t port, uint8_t pin);

#endif