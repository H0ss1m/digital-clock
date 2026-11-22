/**
 * @file    gpio.h
 * @author  Group 6
 * @date    2026-11-22
 * @brief   GPIO driver header file for ATmega328P.
 *
 * Provides port/pin definitions and function prototypes for configuring
 * pin directions, writing digital outputs, reading digital inputs,
 * and enabling internal pull-up resistors.
 */

#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>

/* Port Identifiers */
#define GPIO_PORTB  0U
#define GPIO_PORTC  1U
#define GPIO_PORTD  2U

/* Pin Identifiers */
#define GPIO_PIN_0  0U
#define GPIO_PIN_1  1U
#define GPIO_PIN_2  2U
#define GPIO_PIN_3  3U
#define GPIO_PIN_4  4U
#define GPIO_PIN_5  5U
#define GPIO_PIN_6  6U
#define GPIO_PIN_7  7U

/* Direction Definitions */
#define GPIO_INPUT  0U
#define GPIO_OUTPUT 1U

/* Logic Level Definitions */
#define GPIO_LOW    0U
#define GPIO_HIGH   1U

/**
 * @brief Configure the direction of a specific GPIO pin.
 *
 * Sets the corresponding DDR bit to configure the pin as input or output.
 *
 * @param port      Port identifier (GPIO_PORTB, GPIO_PORTC, or GPIO_PORTD).
 * @param pin       Pin number (GPIO_PIN_0 to GPIO_PIN_7).
 * @param direction Pin direction (GPIO_INPUT or GPIO_OUTPUT).
 */
void GPIO_SetPinDirection(uint8_t port, uint8_t pin, uint8_t direction);

/**
 * @brief Write a logic level to a specific GPIO output pin.
 *
 * Sets or clears the corresponding PORT bit.
 *
 * @param port  Port identifier (GPIO_PORTB, GPIO_PORTC, or GPIO_PORTD).
 * @param pin   Pin number (GPIO_PIN_0 to GPIO_PIN_7).
 * @param value Logic level (GPIO_LOW or GPIO_HIGH).
 */
void GPIO_WritePin(uint8_t port, uint8_t pin, uint8_t value);

/**
 * @brief Read the current logic level of a specific GPIO input pin.
 *
 * Reads the corresponding PIN register bit.
 *
 * @param port Port identifier (GPIO_PORTB, GPIO_PORTC, or GPIO_PORTD).
 * @param pin  Pin number (GPIO_PIN_0 to GPIO_PIN_7).
 * @return uint8_t Logic level read from the pin (GPIO_LOW or GPIO_HIGH).
 */
uint8_t GPIO_ReadPin(uint8_t port, uint8_t pin);

/**
 * @brief Enable the internal pull-up resistor on an input GPIO pin.
 *
 * Writes GPIO_HIGH to the PORT register for the specified input pin.
 *
 * @param port Port identifier (GPIO_PORTB, GPIO_PORTC, or GPIO_PORTD).
 * @param pin  Pin number (GPIO_PIN_0 to GPIO_PIN_7).
 */
void GPIO_EnablePullUp(uint8_t port, uint8_t pin);

#endif /* GPIO_H */