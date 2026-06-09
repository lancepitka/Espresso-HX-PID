/**
 * @file app.c
 * @brief Application code for Pressure PID Controller
 */

#include "app.h"
#include "util/logging.h"

void app(void)
{
    /* Set log level */
    Log_SetLevel(LOG_LEVEL_INFO);

    /* Print startup message */
    printf("=============================================\r\n");
    printf("      Espresso Pressure Controller \r\n");
    printf("=============================================\r\n\r\n");

    /* Application code goes here */
    while (1)
    {
        // Main loop
    }
}