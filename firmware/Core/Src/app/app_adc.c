/**
 * @file app_adc.c
 * @brief ADC application handler
 */

#include "app_adc.h"
#include "adc.h"
#include "cmn.h"
#include "util/aema.h"
#include "util/logging.h"

#define ADC_HDLR &hadc1

static void adc_lp(void);
static uint32_t adc_read(void);

struct
{
    struct sys_hal_timer_hdlr_t* tmr;
    struct hal_tick_timer_t mlTmr;
    enum sys_dev_work_state_t ws;
    struct AdaptiveEMA_t adcEma;
    uint32_t voltage_mv;
} adc_dev = {
    .ws         = SYS_DEV_WORK_INIT,
    .voltage_mv = 0,
};

static struct adc_hdlr_t adc_hdlr = {
    .lp   = adc_lp,
    .read = adc_read,
};

adc_hdlr_t* app_adc_init(struct sys_hal_timer_hdlr_t* _tmr)
{
    if (_tmr == NULL)
    {
        return NULL;
    }

    adc_dev.tmr = _tmr;
    AdaptiveEMA_Init(&adc_dev.adcEma, 0.5f, 0.8f, 5.0f);

    adc_dev.tmr->start(&adc_dev.mlTmr);

    return &adc_hdlr;
}

static uint32_t adc_read_raw(void)
{
    if (HAL_ADC_PollForConversion(ADC_HDLR, 10) == HAL_OK)
    {
        return HAL_ADC_GetValue(ADC_HDLR);
    }
    else
    {
        LOG_ERROR("ADC", "ADC conversion failed");
        return 0;
    }
}

static uint32_t adc_read_voltage(void)
{
    uint32_t rawAdc = adc_read_raw();
    if (rawAdc == 0)
    {
        return 0;
    }

    // Assuming a reference voltage of 3.3V and a 16-bit ADC resolution
    const float referenceVoltage = 3.3f;
    const uint32_t adcResolution = 65535; // 2^16 - 1

    float voltage = ((float)rawAdc / (float)adcResolution) * referenceVoltage;

    /* Resistor divider */
    voltage = voltage * (10.0f + 5.1f) / (10.0f);

    return (uint32_t)(voltage * 1000); // Return voltage in millivolts
}

static void adc_process(void)
{
    adc_dev.voltage_mv = (uint32_t)AdaptiveEMA_Update(&adc_dev.adcEma, (float)adc_read_voltage());
    // LOG_INFO("ADC", "ADC Voltage: %lu mV", adc_dev.voltage_mv);
}

static void adc_lp(void)
{
    if (adc_dev.tmr->tm(&adc_dev.mlTmr, APP_SCHD_25MS) == 0)
    {
        switch (adc_dev.ws)
        {
            case SYS_DEV_WORK_INIT:
                HAL_ADC_Start(ADC_HDLR);
                adc_dev.ws = SYS_DEV_WORK_OPS;
                break;

            case SYS_DEV_WORK_OPS:
                adc_process();
                break;

            default:
                adc_dev.ws = SYS_DEV_WORK_INIT;
                break;
        }
    }
}

static uint32_t adc_read(void)
{
    return adc_dev.voltage_mv;
}