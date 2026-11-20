#ifndef BUTTON_H
#define BUTTON_H

#include <stdint.h>

// أطراف الأزرار مطابقة لتوصيلتك على SimulIDE (PD2 -> PD5)
#define BTN_MODE 2 // PD2
#define BTN_UP   3 // PD3
#define BTN_DOWN 4 // PD4
#define BTN_ACK  5 // PD5

void Button_InitAll(void (*on_press_callback)(uint8_t btn_pin));

#endif