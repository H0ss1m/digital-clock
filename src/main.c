/**
 * @file    main.c
 * @author  Group 6
 * @date    2026-11-22
 * @brief   Main entry point for the Digital Clock project.
 *
 * Initializes and starts the Digital Clock application.
 */

#include "APP/clock_app.h"

/**
 * @brief Program entry point.
 *
 * @return int Should never return.
 */
int main(void)
{
    ClockApp_Init();
    ClockApp_Start();

    while (1)
    {
        /* Main program should not reach here */
    }

    return 0;
}