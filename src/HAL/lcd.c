#define F_CPU 16000000UL
#include "HAL/lcd.h"
#include "MCAL/gpio.h"
#include <util/delay.h>

// أطراف الشاشة مطابقة لتوصيلتك على SimulIDE (PC0 -> PC5)
#define LCD_PORT GPIO_PORTC
#define RS_PIN   0 // PC0
#define EN_PIN   1 // PC1
#define D4_PIN   2 // PC2
#define D5_PIN   3 // PC3
#define D6_PIN   4 // PC4
#define D7_PIN   5 // PC5

static void LCD_SendNibble(uint8_t nibble) {
    GPIO_WritePin(LCD_PORT, D4_PIN, (nibble & 0x01) ? GPIO_HIGH : GPIO_LOW);
    GPIO_WritePin(LCD_PORT, D5_PIN, (nibble & 0x02) ? GPIO_HIGH : GPIO_LOW);
    GPIO_WritePin(LCD_PORT, D6_PIN, (nibble & 0x04) ? GPIO_HIGH : GPIO_LOW);
    GPIO_WritePin(LCD_PORT, D7_PIN, (nibble & 0x08) ? GPIO_HIGH : GPIO_LOW);
}

static void LCD_PulseEnable(void) {
    GPIO_WritePin(LCD_PORT, EN_PIN, GPIO_HIGH);
    _delay_us(20);
    GPIO_WritePin(LCD_PORT, EN_PIN, GPIO_LOW);
    _delay_us(100);
}

void LCD_Command(uint8_t cmd) {
    GPIO_WritePin(LCD_PORT, RS_PIN, GPIO_LOW);
    LCD_SendNibble(cmd >> 4);
    LCD_PulseEnable();
    LCD_SendNibble(cmd & 0x0F);
    LCD_PulseEnable();
    if (cmd == 0x01 || cmd == 0x02) _delay_ms(2);
}

void LCD_Char(uint8_t data) {
    GPIO_WritePin(LCD_PORT, RS_PIN, GPIO_HIGH);
    LCD_SendNibble(data >> 4);
    LCD_PulseEnable();
    LCD_SendNibble(data & 0x0F);
    LCD_PulseEnable();
}

void LCD_Init(void) {
    for (uint8_t pin = 0; pin <= 5; pin++) {
        GPIO_SetPinDirection(LCD_PORT, pin, GPIO_OUTPUT);
    }
    _delay_ms(50);

    GPIO_WritePin(LCD_PORT, RS_PIN, GPIO_LOW);
    LCD_SendNibble(0x03); LCD_PulseEnable(); _delay_ms(5);
    LCD_SendNibble(0x03); LCD_PulseEnable(); _delay_us(150);
    LCD_SendNibble(0x03); LCD_PulseEnable();
    LCD_SendNibble(0x02); LCD_PulseEnable();

    LCD_Command(0x28); // 4-bit mode, 2 lines
    LCD_Command(0x0C); // Display ON, Cursor OFF
    LCD_Command(0x06); // Auto Increment
    LCD_Clear();
}

void LCD_String(const char *str) {
    while (*str) {
        LCD_Char(*str++);
    }
}

void LCD_SetCursor(uint8_t row, uint8_t col) {
    uint8_t pos = (row == 0) ? (0x80 + col) : (0xC0 + col);
    LCD_Command(pos);
}

void LCD_Clear(void) {
    LCD_Command(0x01);
}