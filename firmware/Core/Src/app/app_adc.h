/**
 * @file    app_adc.h
 * @brief   ADC application handler
 */

#ifndef APP_ADC_H_
#define APP_ADC_H_

#include "statuses.h"
#include "util/timer.h"

typedef struct adc_hdlr_t
{
    void (*lp)(void);
    uint32_t (*read)(void); /* Read ADC value in millivolts */
} adc_hdlr_t;

adc_hdlr_t* app_adc_init(struct sys_hal_timer_hdlr_t* _tmr);

#endif /* APP_ADC_H_ */