/**
 * @file app.c
 * @brief Application code for Pressure PID Controller
 */

#include "app.h"
#include "app_adc.h"
#include "button.h"
#include "display.h"
#include "i2c.h"
#include "statuses.h"
#include "util/logging.h"
#include "util/timer.h"

static struct sys_hal_timer_hdlr_t* tmrHdlr;
static struct adc_hdlr_t* adcHdlr;
static struct button_hdlr_t* buttonHdlr;
static struct display_hdlr_t* displayHdlr;

void app(void)
{
    /* Set log level */
    Log_SetLevel(LOG_LEVEL_INFO);

    /* Print startup message */
    printf("=============================================\r\n");
    printf("      Espresso Pressure Controller \r\n");
    printf("=============================================\r\n\r\n");

    bool stat = false;

    /* Init Drivers */
    stat = (tmrHdlr = _sys_init_timer_hdlr()) == NULL;
    stat |= (adcHdlr = app_adc_init(tmrHdlr)) == NULL;
    stat |= (buttonHdlr = button_init(tmrHdlr)) == NULL;
    stat |= (displayHdlr = display_init(tmrHdlr, &hi2c2)) == NULL;

    if (stat)
    {
        LOG_ERROR("APP", "Failed to initialize drivers");
        return;
    }

    while (1)
    {
        adcHdlr->lp();
        buttonHdlr->lp();
        displayHdlr->lp();
    }
}