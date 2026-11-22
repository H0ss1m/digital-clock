/**
 * @file    gpio.c
 * @author  Group 6
 * @date    2026-11-22
 * @brief   GPIO driver implementation for ATmega328P.
 *
 * Implements low-level register manipulation for DDRx, PORTx, and PINx
 * registers on ATmega328P.
 */

#include "MCAL/gpio.h"
#include <avr/io.h>

void GPIO_SetPinDirection(uint8_t port, uint8_t pin, uint8_t direction)
{
    if (direction == GPIO_OUTPUT)
    {
        if (port == GPIO_PORTB)      { DDRB |= (1U << pin); }
        else if (port == GPIO_PORTC) { DDRC |= (1U << pin); }
        else if (port == GPIO_PORTD) { DDRD |= (1U << pin); }
    }
    else
    {
        if (port == GPIO_PORTB)      { DDRB &= ~(1U << pin); }
        else if (port == GPIO_PORTC) { DDRC &= ~(1U << pin); }
        else if (port == GPIO_PORTD) { DDRD &= ~(1U << pin); }
    }
}

void GPIO_WritePin(uint8_t port, uint8_t pin, uint8_t value)
{
    if (value == GPIO_HIGH)
    {
        if (port == GPIO_PORTB)      { PORTB |= (1U << pin); }
        else if (port == GPIO_PORTC) { PORTC |= (1U << pin); }
        else if (port == GPIO_PORTD) { PORTD |= (1U << pin); }
    }
    else
    {
        if (port == GPIO_PORTB)      { PORTB &= ~(1U << pin); }
        else if (port == GPIO_PORTC) { PORTC &= ~(1U << pin); }
        else if (port == GPIO_PORTD) { PORTD &= ~(1U << pin); }
    }
}

uint8_t GPIO_ReadPin(uint8_t port, uint8_t pin)
{
    uint8_t val = GPIO_LOW;

    if (port == GPIO_PORTB)      { val = (PINB & (1U << pin)) ? GPIO_HIGH : GPIO_LOW; }
    else if (port == GPIO_PORTC) { val = (PINC & (1U << pin)) ? GPIO_HIGH : GPIO_LOW; }
    else if (port == GPIO_PORTD) { val = (PIND & (1U << pin)) ? GPIO_HIGH : GPIO_LOW; }

    return val;
}

void GPIO_EnablePullUp(uint8_t port, uint8_t pin)
{
    GPIO_WritePin(port, pin, GPIO_HIGH);
}