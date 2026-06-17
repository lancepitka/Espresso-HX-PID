/**
 * @file app_adc.c
 * @brief ADC application handler
 */

#include "app_adc.h"
#include "cmn.h"

static void adc_lp(void);

struct
{
    struct sys_hal_timer_hdlr_t* tmr;
    enum sys_dev_work_state_t ws;
} adc_dev_t = {
    .ws = SYS_DEV_WORK_INIT,
};

static struct adc_hdlr_t adc_hdlr = {
    .lp = adc_lp,
};

adc_hdlr_t* app_adc_init(struct sys_hal_timer_hdlr_t* _tmr)
{
    if (_tmr == NULL)
    {
        return NULL;
    }

    adc_dev_t.tmr = _tmr;

    return &adc_hdlr;
}

static void adc_lp(void)
{
    /* ADC loop code here */
}