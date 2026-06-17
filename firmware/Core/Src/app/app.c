/**
 * @file app.c
 * @brief Application code for Pressure PID Controller
 */

#include "app.h"
#include "statuses.h"
#include "util/logging.h"
#include "util/timer.h"

static struct sys_hal_timer_hdlr_t* tmrHdlr;

void app(void)
{
    /* Set log level */
    Log_SetLevel(LOG_LEVEL_INFO);

    /* Print startup message */
    printf("=============================================\r\n");
    printf("      Espresso Pressure Controller \r\n");
    printf("=============================================\r\n\r\n");

    /* Init Drivers */
    tmrHdlr = _sys_init_timer_hdlr();

    /* Init App Controllers */

    while (1)
    {
    }
}