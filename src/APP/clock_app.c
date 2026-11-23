/**
 * @file    clock_app.c
 * @author  Group 6
 * @date    2026-11-22
 * @brief   Digital Clock application layer implementation.
 *
 * Implements the clock state machine:
 * - Normal mode: Displays "Keep Going Grp 6" and current time (12h or 24h).
 * - ACK button: Toggles between 12-hour and 24-hour display formats.
 * - UP / DOWN buttons: Silence the ringing alarm; otherwise display alarm time.
 * - SET button: Steps through Set Time (HH -> MM -> SS -> AM/PM) and
 *               Set Alarm (HH -> MM -> SS -> AM/PM) with a blinking underline.
 */

#include "APP/clock_app.h"
#include "MCAL/timer1.h"
#include "HAL/lcd.h"
#include "HAL/pushbuttons.h"
#include "HAL/buzzer.h"
#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>

/* Timing constants based on 20ms main loop period */
#define LOOP_PERIOD_MS       20U
#define CURSOR_BLINK_TICKS   20U  /* 400ms blink interval */
#define ALARM_SHOW_TICKS     100U /* 2000ms alarm preview duration */

/**
 * @brief Operating modes of the clock application.
 */
typedef enum
{
    APP_MODE_NORMAL = 0,
    APP_MODE_SHOW_ALARM,
    APP_MODE_SET_TIME,
    APP_MODE_SET_ALARM
} app_mode_t;

/**
 * @brief Active editable field during Set Time or Set Alarm modes.
 */
typedef enum
{
    FIELD_HOURS = 0,
    FIELD_MINUTES,
    FIELD_SECONDS,
    FIELD_AMPM
} edit_field_t;

/**
 * @brief Time display format (12-hour or 24-hour).
 */
typedef enum
{
    FORMAT_12H = 0,
    FORMAT_24H
} time_format_t;

/* Time and Alarm stored internally in 24-hour format (0..23) */
static volatile uint8_t cur_hour = 12U;
static volatile uint8_t cur_min  = 0U;
static volatile uint8_t cur_sec  = 0U;

static uint8_t alm_hour = 12U;
static uint8_t alm_min  = 1U;
static uint8_t alm_sec  = 0U;

/* Application state variables */
static volatile app_mode_t app_mode      = APP_MODE_NORMAL;
static edit_field_t        edit_field    = FIELD_HOURS;
static time_format_t       time_format   = FORMAT_12H;
static volatile uint8_t    alarm_ringing = 0U;
static volatile uint8_t    display_dirty = 1U;

/* Previous button states for edge detection (debouncing) */
static pb_state_t prev_pb_state[4] = {
    PB_NOT_PRESSED, PB_NOT_PRESSED, PB_NOT_PRESSED, PB_NOT_PRESSED
};

/**
 * @brief Timer1 1-second interrupt callback to increment clock time.
 *
 * Pauses time progression while in APP_MODE_SET_TIME. Triggers the alarm
 * buzzer when current time matches the configured alarm time.
 */
static void ClockApp_Tick1Sec(void)
{
    if (app_mode == APP_MODE_SET_TIME)
    {
        return;
    }

    cur_sec++;
    if (cur_sec >= 60U)
    {
        cur_sec = 0U;
        cur_min++;
        if (cur_min >= 60U)
        {
            cur_min = 0U;
            cur_hour++;
            if (cur_hour >= 24U)
            {
                cur_hour = 0U;
            }
        }
    }

    if ((cur_hour == alm_hour) && (cur_min == alm_min) && (cur_sec == alm_sec))
    {
        if (alarm_ringing == 0U)
        {
            alarm_ringing = 1U;
            Buzzer_StartBeep();
        }
    }

    if (app_mode == APP_MODE_NORMAL)
    {
        display_dirty = 1U;
    }
}

/**
 * @brief Detect a fresh falling-edge press on a pushbutton.
 *
 * @param btn Button identifier to check.
 * @return uint8_t 1 if newly pressed since last poll, 0 otherwise.
 */
static uint8_t ClockApp_IsButtonClicked(pb_type_t btn)
{
    pb_state_t current = PB_GetState(btn);
    uint8_t clicked    = 0U;

    if ((current == PB_PRESSED) && (prev_pb_state[btn] == PB_NOT_PRESSED))
    {
        clicked = 1U;
    }

    prev_pb_state[btn] = current;
    return clicked;
}

/**
 * @brief Convert a 24-hour value into 12-hour display value and AM/PM string.
 *
 * @param h24       Hour in 24-hour format (0..23).
 * @param out_h12   Pointer to store 12-hour value (1..12).
 * @param out_ampm  Pointer to store "am" or "pm" string literal.
 */
static void ClockApp_ConvertTo12H(uint8_t h24, uint8_t *out_h12, const char **out_ampm)
{
    *out_ampm = (h24 >= 12U) ? "pm" : "am";
    *out_h12  = h24 % 12U;
    if (*out_h12 == 0U)
    {
        *out_h12 = 12U;
    }
}

/**
 * @brief Adjust the currently selected time or alarm field up or down.
 *
 * @param h     Pointer to hour variable (0..23).
 * @param m     Pointer to minute variable (0..59).
 * @param s     Pointer to second variable (0..59).
 * @param delta +1 to increment, -1 to decrement.
 */
static void ClockApp_AdjustField(volatile uint8_t *h, volatile uint8_t *m,
                                 volatile uint8_t *s, int8_t delta)
{
    if (edit_field == FIELD_HOURS)
    {
        if (time_format == FORMAT_12H)
        {
            uint8_t base = (*h >= 12U) ? 12U : 0U;
            uint8_t h12  = *h % 12U;
            h12 = (delta > 0) ? ((h12 + 1U) % 12U) : ((h12 + 11U) % 12U);
            *h = base + h12;
        }
        else
        {
            *h = (delta > 0) ? ((*h + 1U) % 24U) : ((*h + 23U) % 24U);
        }
    }
    else if (edit_field == FIELD_MINUTES)
    {
        *m = (delta > 0) ? ((*m + 1U) % 60U) : ((*m + 59U) % 60U);
    }
    else if (edit_field == FIELD_SECONDS)
    {
        *s = (delta > 0) ? ((*s + 1U) % 60U) : ((*s + 59U) % 60U);
    }
    else if (edit_field == FIELD_AMPM)
    {
        *h = (*h + 12U) % 24U;
    }
}

/**
 * @brief Get the LCD column index for the active edit field underline cursor.
 *
 * @return uint8_t Column index (0..15) on row 1.
 */
static uint8_t ClockApp_GetEditCursorCol(void)
{
    uint8_t col = 1U;

    switch (edit_field)
    {
        case FIELD_HOURS:   col = 1U; break; /* Under HH */
        case FIELD_MINUTES: col = 4U; break; /* Under MM */
        case FIELD_SECONDS: col = 7U; break; /* Under SS */
        case FIELD_AMPM:    col = 9U; break; /* Under am/pm */
        default:            col = 1U; break;
    }

    return col;
}

/**
 * @brief Render the LCD contents according to the current application mode.
 */
static void ClockApp_UpdateDisplay(void)
{
    char line0[17];
    char line1[17];
    uint8_t h12;
    const char *ampm;

    /* Hide cursor briefly while redrawing to avoid visual artifacts */
    LCD_SetUnderlineCursor(0U);

    if (app_mode == APP_MODE_NORMAL)
    {
        snprintf(line0, sizeof(line0), "Keep Going Grp 6");
        if (time_format == FORMAT_12H)
        {
            ClockApp_ConvertTo12H(cur_hour, &h12, &ampm);
            snprintf(line1, sizeof(line1), "Time:%02u:%02u:%02u %s",
                     h12, cur_min, cur_sec, ampm);
        }
        else
        {
            snprintf(line1, sizeof(line1), "Time: %02u:%02u:%02u  ",
                     cur_hour, cur_min, cur_sec);
        }
    }
    else if (app_mode == APP_MODE_SHOW_ALARM)
    {
        snprintf(line0, sizeof(line0), "Alarm Time:     ");
        if (time_format == FORMAT_12H)
        {
            ClockApp_ConvertTo12H(alm_hour, &h12, &ampm);
            snprintf(line1, sizeof(line1), " %02u:%02u:%02u %s    ",
                     h12, alm_min, alm_sec, ampm);
        }
        else
        {
            snprintf(line1, sizeof(line1), " %02u:%02u:%02u       ",
                     alm_hour, alm_min, alm_sec);
        }
    }
    else if (app_mode == APP_MODE_SET_TIME)
    {
        snprintf(line0, sizeof(line0), "Set time:       ");
        if (time_format == FORMAT_12H)
        {
            ClockApp_ConvertTo12H(cur_hour, &h12, &ampm);
            snprintf(line1, sizeof(line1), "%02u:%02u:%02u %s     ",
                     h12, cur_min, cur_sec, ampm);
        }
        else
        {
            snprintf(line1, sizeof(line1), "%02u:%02u:%02u        ",
                     cur_hour, cur_min, cur_sec);
        }
    }
    else /* APP_MODE_SET_ALARM */
    {
        snprintf(line0, sizeof(line0), "Set alarm:      ");
        if (time_format == FORMAT_12H)
        {
            ClockApp_ConvertTo12H(alm_hour, &h12, &ampm);
            snprintf(line1, sizeof(line1), "%02u:%02u:%02u %s     ",
                     h12, alm_min, alm_sec, ampm);
        }
        else
        {
            snprintf(line1, sizeof(line1), "%02u:%02u:%02u        ",
                     alm_hour, alm_min, alm_sec);
        }
    }

    LCD_SetCursor(0U, 0U);
    LCD_PrintString(line0);
    LCD_SetCursor(1U, 0U);
    LCD_PrintString(line1);
}

void ClockApp_Init(void)
{
    LCD_Init();
    PB_Init();
    Buzzer_Init();

    /* Pass required time (1000 ms) and callback to generic Timer1_Init */
    Timer1_Init(1000U, ClockApp_Tick1Sec);
    display_dirty = 1U;
}

void ClockApp_Start(void)
{
    uint16_t blink_ticks      = 0U;
    uint16_t alarm_show_ticks = 0U;
    uint8_t  cursor_visible   = 0U;

    while (1)
    {
        uint8_t set_clicked = ClockApp_IsButtonClicked(SET_PB);
        uint8_t up_clicked  = ClockApp_IsButtonClicked(UP_PB);
        uint8_t dwn_clicked = ClockApp_IsButtonClicked(DWN_PB);
        uint8_t ack_clicked = ClockApp_IsButtonClicked(ACK_PB);

        /* Priority 1: Stop ringing buzzer on UP or DOWN click */
        if ((alarm_ringing != 0U) && (up_clicked || dwn_clicked))
        {
            alarm_ringing = 0U;
            Buzzer_StopBeep();
            up_clicked  = 0U;
            dwn_clicked = 0U;
        }

        /* Handle ACK button: toggle 12h/24h format */
        if (ack_clicked)
        {
            time_format = (time_format == FORMAT_12H) ? FORMAT_24H : FORMAT_12H;
            if ((time_format == FORMAT_24H) && (edit_field == FIELD_AMPM))
            {
                edit_field = FIELD_SECONDS;
            }
            display_dirty = 1U;
        }

        /* Handle SET button: step through Set Time -> Set Alarm -> Normal */
        if (set_clicked)
        {
            if ((app_mode == APP_MODE_NORMAL) || (app_mode == APP_MODE_SHOW_ALARM))
            {
                app_mode   = APP_MODE_SET_TIME;
                edit_field = FIELD_HOURS;
            }
            else if (app_mode == APP_MODE_SET_TIME)
            {
                edit_field_t last_field = (time_format == FORMAT_12H) ? FIELD_AMPM : FIELD_SECONDS;
                if (edit_field < last_field)
                {
                    edit_field = (edit_field_t)(edit_field + 1);
                }
                else
                {
                    app_mode   = APP_MODE_SET_ALARM;
                    edit_field = FIELD_HOURS;
                }
            }
            else if (app_mode == APP_MODE_SET_ALARM)
            {
                edit_field_t last_field = (time_format == FORMAT_12H) ? FIELD_AMPM : FIELD_SECONDS;
                if (edit_field < last_field)
                {
                    edit_field = (edit_field_t)(edit_field + 1);
                }
                else
                {
                    app_mode = APP_MODE_NORMAL;
                    LCD_SetUnderlineCursor(0U);
                }
            }
            blink_ticks    = 0U;
            cursor_visible = 1U;
            display_dirty  = 1U;
        }

        /* Handle UP / DOWN buttons depending on current mode */
        if (up_clicked || dwn_clicked)
        {
            int8_t delta = up_clicked ? 1 : -1;

            if (app_mode == APP_MODE_NORMAL)
            {
                app_mode         = APP_MODE_SHOW_ALARM;
                alarm_show_ticks = 0U;
                display_dirty    = 1U;
            }
            else if (app_mode == APP_MODE_SHOW_ALARM)
            {
                alarm_show_ticks = 0U; /* Extend preview duration */
            }
            else if (app_mode == APP_MODE_SET_TIME)
            {
                ClockApp_AdjustField(&cur_hour, &cur_min, &cur_sec, delta);
                display_dirty = 1U;
            }
            else if (app_mode == APP_MODE_SET_ALARM)
            {
                ClockApp_AdjustField(&alm_hour, &alm_min, &alm_sec, delta);
                display_dirty = 1U;
            }
        }

        /* Auto-return from Alarm preview back to Normal display */
        if (app_mode == APP_MODE_SHOW_ALARM)
        {
            alarm_show_ticks++;
            if (alarm_show_ticks >= ALARM_SHOW_TICKS)
            {
                app_mode      = APP_MODE_NORMAL;
                display_dirty = 1U;
            }
        }

        /* Refresh LCD content when changed */
        if (display_dirty != 0U)
        {
            display_dirty = 0U;
            ClockApp_UpdateDisplay();

            if ((app_mode == APP_MODE_SET_TIME) || (app_mode == APP_MODE_SET_ALARM))
            {
                LCD_SetCursor(1U, ClockApp_GetEditCursorCol());
                LCD_SetUnderlineCursor(cursor_visible);
            }
        }

        /* Blink underline cursor during Set Time / Set Alarm modes */
        if ((app_mode == APP_MODE_SET_TIME) || (app_mode == APP_MODE_SET_ALARM))
        {
            blink_ticks++;
            if (blink_ticks >= CURSOR_BLINK_TICKS)
            {
                blink_ticks    = 0U;
                cursor_visible = !cursor_visible;
                LCD_SetCursor(1U, ClockApp_GetEditCursorCol());
                LCD_SetUnderlineCursor(cursor_visible);
            }
        }

        _delay_ms(LOOP_PERIOD_MS);
    }
}