/**
 * @file    lcd.h
 * @author  Group 6
 * @date    2026-11-22
 * @brief   16x2 Character LCD (HD44780) HAL driver header file.
 *
 * Provides 4-bit mode initialization, string printing, cursor positioning,
 * and underline cursor visibility control.
 */

#ifndef LCD_H
#define LCD_H

#include <stdint.h>
#include "MCAL/gpio.h"

/* Hardware Pin Mappings (PORTC) */
#define LCD_PORT   GPIO_PORTC
#define LCD_RS_PIN GPIO_PIN_0
#define LCD_E_PIN  GPIO_PIN_1
#define LCD_D4_PIN GPIO_PIN_2
#define LCD_D5_PIN GPIO_PIN_3
#define LCD_D6_PIN GPIO_PIN_4
#define LCD_D7_PIN GPIO_PIN_5

/**
 * @brief Initialize the HD44780 LCD in 4-bit mode, 2 lines, 5x8 font.
 */
void LCD_Init(void);

/**
 * @brief Send a command byte to the LCD controller.
 *
 * @param cmd Command byte to execute.
 */
void LCD_SendCommand(uint8_t cmd);

/**
 * @brief Print a single ASCII character at the current cursor position.
 *
 * @param data ASCII character to display.
 */
void LCD_PrintChar(char data);

/**
 * @brief Print a null-terminated string starting at the current cursor position.
 *
 * @param str Pointer to the null-terminated string.
 */
void LCD_PrintString(const char *str);

/**
 * @brief Move the LCD cursor to the specified row and column.
 *
 * @param row Row index (0 for top row, 1 for bottom row).
 * @param col Column index (0 to 15).
 */
void LCD_SetCursor(uint8_t row, uint8_t col);

/**
 * @brief Clear the LCD display and return the cursor to home position.
 */
void LCD_Clear(void);

/**
 * @brief Enable or disable the hardware underline cursor on the LCD.
 *
 * @param visible Non-zero to show the underline cursor, 0 to hide it.
 */
void LCD_SetUnderlineCursor(uint8_t visible);

#endif /* LCD_H */