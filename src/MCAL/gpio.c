#include "MCAL/gpio.h"
#include <avr/io.h>

void GPIO_SetPinDirection(uint8_t port, uint8_t pin, uint8_t direction) {
    if (direction == GPIO_OUTPUT) {
        if (port == GPIO_PORTB) DDRB |= (1 << pin);
        else if (port == GPIO_PORTC) DDRC |= (1 << pin);
        else if (port == GPIO_PORTD) DDRD |= (1 << pin);
    } else {
        if (port == GPIO_PORTB) DDRB &= ~(1 << pin);
        else if (port == GPIO_PORTC) DDRC &= ~(1 << pin);
        else if (port == GPIO_PORTD) DDRD &= ~(1 << pin);
    }
}

void GPIO_WritePin(uint8_t port, uint8_t pin, uint8_t value) {
    if (value == GPIO_HIGH) {
        if (port == GPIO_PORTB) PORTB |= (1 << pin);
        else if (port == GPIO_PORTC) PORTC |= (1 << pin);
        else if (port == GPIO_PORTD) PORTD |= (1 << pin);
    } else {
        if (port == GPIO_PORTB) PORTB &= ~(1 << pin);
        else if (port == GPIO_PORTC) PORTC &= ~(1 << pin);
        else if (port == GPIO_PORTD) PORTD &= ~(1 << pin);
    }
}

uint8_t GPIO_ReadPin(uint8_t port, uint8_t pin) {
    if (port == GPIO_PORTB) return (PINB & (1 << pin)) ? 1 : 0;
    if (port == GPIO_PORTC) return (PINC & (1 << pin)) ? 1 : 0;
    if (port == GPIO_PORTD) return (PIND & (1 << pin)) ? 1 : 0;
    return 0;
}

void GPIO_EnablePullUp(uint8_t port, uint8_t pin) {
    GPIO_WritePin(port, pin, GPIO_HIGH);
}