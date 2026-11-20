#define F_CPU 16000000UL
#include "APP/clock_app.h"
#include "MCAL/timer1.h"
#include "HAL/lcd.h"
#include "HAL/button.h"
#include "HAL/buzzer.h"
#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>

static volatile uint8_t sec = 0, min = 0, hr = 12;
static volatile uint8_t alarm_hr = 12, alarm_min = 1;
static volatile uint8_t mode = 0; // 0: Normal, 1: Set Hr, 2: Set Min, 3: Alarm Hr, 4: Alarm Min
static volatile uint8_t alarm_triggered = 0;
static volatile uint8_t clear_lcd_flag = 0;

// مقاطعة التايمر كل ثانية
static void ClockApp_Tick1Sec(void) {
    sec++;
    if (sec >= 60) {
        sec = 0;
        min++;
        if (min >= 60) {
            min = 0;
            hr++;
            if (hr >= 24) hr = 0;
        }
    }
}

// استجابة فورية عند الضغط على أي زر
static void ClockApp_OnButtonPress(uint8_t btn_pin) {
    if (btn_pin == BTN_MODE) {
        mode++;
        if (mode > 4) mode = 0;
        clear_lcd_flag = 1;
    } 
    else if (btn_pin == BTN_UP) {
        if (mode == 1)      { hr++; if (hr >= 24) hr = 0; }
        else if (mode == 2) { min++; if (min >= 60) min = 0; }
        else if (mode == 3) { alarm_hr++; if (alarm_hr >= 24) alarm_hr = 0; }
        else if (mode == 4) { alarm_min++; if (alarm_min >= 60) alarm_min = 0; }
    } 
    else if (btn_pin == BTN_DOWN) {
        if (mode == 1)      { hr = (hr == 0) ? 23 : hr - 1; }
        else if (mode == 2) { min = (min == 0) ? 59 : min - 1; }
        else if (mode == 3) { alarm_hr = (alarm_hr == 0) ? 23 : alarm_hr - 1; }
        else if (mode == 4) { alarm_min = (alarm_min == 0) ? 59 : alarm_min - 1; }
    } 
    else if (btn_pin == BTN_ACK) {
        if (alarm_triggered) {
            alarm_triggered = 0;
            Buzzer_Off();
        } else if (mode == 0) {
            sec = 0;
        }
    }
}

void ClockApp_Init(void) {
    LCD_Init();
    Buzzer_Init();
    Button_InitAll(ClockApp_OnButtonPress);
    
    Timer1_SetCallback(ClockApp_Tick1Sec);
    Timer1_Init_1Sec();
}

void ClockApp_Update(void) {
    char buffer[17];

    if (clear_lcd_flag) {
        clear_lcd_flag = 0;
        LCD_Clear();
    }

    // التحقق من تطابق وقت المنبه
    if (hr == alarm_hr && min == alarm_min && sec == 0 && mode == 0) {
        alarm_triggered = 1;
    }

    if (alarm_triggered) {
        Buzzer_On();
    }

    // تحديث شاشة العرض
    LCD_SetCursor(0, 0);
    if (mode == 0) {
        sprintf(buffer, "Time: %02d:%02d:%02d", hr, min, sec);
        LCD_String(buffer);
        
        LCD_SetCursor(1, 0);
        if (alarm_triggered) {
            LCD_String("ALARM RINGING!  ");
        } else {
            sprintf(buffer, "Alrm: %02d:%02d     ", alarm_hr, alarm_min);
            LCD_String(buffer);
        }
    } else if (mode == 1) {
        LCD_String("Set Clock Hours ");
        LCD_SetCursor(1, 0);
        sprintf(buffer, "-> %02d           ", hr);
        LCD_String(buffer);
    } else if (mode == 2) {
        LCD_String("Set Clock Mins  ");
        LCD_SetCursor(1, 0);
        sprintf(buffer, "-> %02d           ", min);
        LCD_String(buffer);
    } else if (mode == 3) {
        LCD_String("Set Alarm Hours ");
        LCD_SetCursor(1, 0);
        sprintf(buffer, "-> %02d           ", alarm_hr);
        LCD_String(buffer);
    } else if (mode == 4) {
        LCD_String("Set Alarm Mins  ");
        LCD_SetCursor(1, 0);
        sprintf(buffer, "-> %02d           ", alarm_min);
        LCD_String(buffer);
    }

    _delay_ms(50);
}