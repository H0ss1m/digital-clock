/**
 * @file    lcd.c
 * @author  Group 6
 * @date    2026-11-22
 * @brief   16x2 Character LCD (HD44780) HAL driver implementation.
 *
 * Implements 4-bit communication protocol with HD44780-compatible LCDs.
 */

#include "HAL/lcd.h"
#include <util/delay.h>

/**
 * @brief Generate an enable pulse on the LCD E pin.
 */
static void LCD_PulseEnable(void)
{
    GPIO_WritePin(LCD_PORT, LCD_E_PIN, GPIO_HIGH);
    _delay_us(2);
    GPIO_WritePin(LCD_PORT, LCD_E_PIN, GPIO_LOW);
    _delay_us(50);
}

/**
 * @brief Send a 4-bit nibble to the LCD data lines D4..D7.
 *
 * @param nibble Lower 4 bits to transmit.
 */
static void LCD_SendNibble(uint8_t nibble)
{
    GPIO_WritePin(LCD_PORT, LCD_D4_PIN, (nibble & 0x01U) ? GPIO_HIGH : GPIO_LOW);
    GPIO_WritePin(LCD_PORT, LCD_D5_PIN, (nibble & 0x02U) ? GPIO_HIGH : GPIO_LOW);
    GPIO_WritePin(LCD_PORT, LCD_D6_PIN, (nibble & 0x04U) ? GPIO_HIGH : GPIO_LOW);
    GPIO_WritePin(LCD_PORT, LCD_D7_PIN, (nibble & 0x08U) ? GPIO_HIGH : GPIO_LOW);
    LCD_PulseEnable();
}

void LCD_SendCommand(uint8_t cmd)
{
    GPIO_WritePin(LCD_PORT, LCD_RS_PIN, GPIO_LOW);
    LCD_SendNibble(cmd >> 4);
    LCD_SendNibble(cmd & 0x0FU);
    if ((cmd == 0x01U) || (cmd == 0x02U))
    {
        _delay_ms(2);
    }
}

void LCD_PrintChar(char data)
{
    GPIO_WritePin(LCD_PORT, LCD_RS_PIN, GPIO_HIGH);
    LCD_SendNibble((uint8_t)data >> 4);
    LCD_SendNibble((uint8_t)data & 0x0FU);
}

void LCD_Init(void)
{
    GPIO_SetPinDirection(LCD_PORT, LCD_RS_PIN, GPIO_OUTPUT);
    GPIO_SetPinDirection(LCD_PORT, LCD_E_PIN,  GPIO_OUTPUT);
    GPIO_SetPinDirection(LCD_PORT, LCD_D4_PIN, GPIO_OUTPUT);
    GPIO_SetPinDirection(LCD_PORT, LCD_D5_PIN, GPIO_OUTPUT);
    GPIO_SetPinDirection(LCD_PORT, LCD_D6_PIN, GPIO_OUTPUT);
    GPIO_SetPinDirection(LCD_PORT, LCD_D7_PIN, GPIO_OUTPUT);

    _delay_ms(20);
    GPIO_WritePin(LCD_PORT, LCD_RS_PIN, GPIO_LOW);

    /* HD44780 4-bit initialization sequence */
    LCD_SendNibble(0x03U);
    _delay_ms(5);
    LCD_SendNibble(0x03U);
    _delay_us(150);
    LCD_SendNibble(0x03U);
    LCD_SendNibble(0x02U);

    LCD_SendCommand(0x28U); /* 4-bit, 2 lines, 5x8 font */
    LCD_SendCommand(0x0CU); /* Display ON, Cursor OFF */
    LCD_SendCommand(0x06U); /* Auto-increment cursor */
    LCD_Clear();
}

void LCD_PrintString(const char *str)
{
    while (*str != '\0')
    {
        LCD_PrintChar(*str++);
    }
}

void LCD_SetCursor(uint8_t row, uint8_t col)
{
    uint8_t addr = (row == 0U) ? (0x80U + col) : (0xC0U + col);
    LCD_SendCommand(addr);
}

void LCD_Clear(void)
{
    LCD_SendCommand(0x01U);
}

void LCD_SetUnderlineCursor(uint8_t visible)
{
    if (visible != 0U)
    {
        LCD_SendCommand(0x0EU); /* Display ON, Underline Cursor ON */
    }
    else
    {
        LCD_SendCommand(0x0CU); /* Display ON, Cursor OFF */
    }
}