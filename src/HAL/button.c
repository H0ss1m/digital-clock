#include "HAL/button.h"
#include "MCAL/gpio.h"
#include "MCAL/pcint.h"
#include <stddef.h>

static void (*App_ButtonCallback)(uint8_t btn_pin) = NULL;

static void Button_InterruptHandler(uint8_t pressed_mask) {
    if (App_ButtonCallback == NULL) return;

    if (pressed_mask & (1 << BTN_MODE)) App_ButtonCallback(BTN_MODE);
    if (pressed_mask & (1 << BTN_UP))   App_ButtonCallback(BTN_UP);
    if (pressed_mask & (1 << BTN_DOWN)) App_ButtonCallback(BTN_DOWN);
    if (pressed_mask & (1 << BTN_ACK))  App_ButtonCallback(BTN_ACK);
}

void Button_InitAll(void (*on_press_callback)(uint8_t btn_pin)) {
    uint8_t pins[] = {BTN_MODE, BTN_UP, BTN_DOWN, BTN_ACK};
    uint8_t mask = 0;

    for (uint8_t i = 0; i < 4; i++) {
        GPIO_SetPinDirection(GPIO_PORTD, pins[i], GPIO_INPUT);
        GPIO_EnablePullUp(GPIO_PORTD, pins[i]);
        mask |= (1 << pins[i]);
    }

    App_ButtonCallback = on_press_callback;
    PCINT2_SetCallback(Button_InterruptHandler);
    PCINT2_Init(mask);
}