/**
 * @file    pushbuttons.c
 * @author  Group 6
 * @date    2026-11-22
 * @brief   Pushbutton HAL driver implementation.
 *
 * Implements initialization and active-low state reading for the clock buttons.
 */

#include "HAL/pushbuttons.h"

void PB_Init(void)
{
    GPIO_SetPinDirection(SET_PB_GPIO_Port, SET_PB_Pin, GPIO_INPUT);
    GPIO_EnablePullUp(SET_PB_GPIO_Port, SET_PB_Pin);

    GPIO_SetPinDirection(UP_PB_GPIO_Port, UP_PB_Pin, GPIO_INPUT);
    GPIO_EnablePullUp(UP_PB_GPIO_Port, UP_PB_Pin);

    GPIO_SetPinDirection(DWN_PB_GPIO_Port, DWN_PB_Pin, GPIO_INPUT);
    GPIO_EnablePullUp(DWN_PB_GPIO_Port, DWN_PB_Pin);

    GPIO_SetPinDirection(ACK_PB_GPIO_Port, ACK_PB_Pin, GPIO_INPUT);
    GPIO_EnablePullUp(ACK_PB_GPIO_Port, ACK_PB_Pin);
}

pb_state_t PB_GetState(pb_type_t pb)
{
    uint8_t pin_level = GPIO_HIGH;

    switch (pb)
    {
        case SET_PB:
            pin_level = GPIO_ReadPin(SET_PB_GPIO_Port, SET_PB_Pin);
            break;
        case UP_PB:
            pin_level = GPIO_ReadPin(UP_PB_GPIO_Port, UP_PB_Pin);
            break;
        case DWN_PB:
            pin_level = GPIO_ReadPin(DWN_PB_GPIO_Port, DWN_PB_Pin);
            break;
        case ACK_PB:
            pin_level = GPIO_ReadPin(ACK_PB_GPIO_Port, ACK_PB_Pin);
            break;
        default:
            break;
    }

    return (pin_level == GPIO_LOW) ? PB_PRESSED : PB_NOT_PRESSED;
}