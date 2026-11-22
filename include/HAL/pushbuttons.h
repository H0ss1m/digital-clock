/**
 * @file    pushbuttons.h
 * @author  Group 6
 * @date    2026-11-22
 * @brief   Pushbutton HAL driver header file.
 *
 * Defines hardware pin mappings, button types, and pressed/not-pressed states
 * for the digital clock user interface buttons.
 */

#ifndef PUSHBUTTONS_H
#define PUSHBUTTONS_H

#include "MCAL/gpio.h"

/* Hardware Pin Mappings */
#define SET_PB_Pin        GPIO_PIN_2
#define SET_PB_GPIO_Port  GPIO_PORTD

#define UP_PB_Pin         GPIO_PIN_3
#define UP_PB_GPIO_Port   GPIO_PORTD

#define DWN_PB_Pin        GPIO_PIN_4
#define DWN_PB_GPIO_Port  GPIO_PORTD

#define ACK_PB_Pin        GPIO_PIN_5
#define ACK_PB_GPIO_Port  GPIO_PORTD

/**
 * @brief Logical states of a pushbutton.
 */
typedef enum
{
    PB_PRESSED = 0,
    PB_NOT_PRESSED
} pb_state_t;

/**
 * @brief Identifiers for the available pushbuttons.
 */
typedef enum
{
    SET_PB = 0,
    UP_PB,
    DWN_PB,
    ACK_PB
} pb_type_t;

/**
 * @brief Initialize all pushbuttons as inputs with internal pull-ups enabled.
 */
void PB_Init(void);

/**
 * @brief Read the current state of a specific pushbutton.
 *
 * @param pb Button identifier (SET_PB, UP_PB, DWN_PB, or ACK_PB).
 * @return pb_state_t PB_PRESSED if the button is pressed, otherwise PB_NOT_PRESSED.
 */
pb_state_t PB_GetState(pb_type_t pb);

#endif /* PUSHBUTTONS_H */