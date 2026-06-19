/**
 * @file button.c
 * @brief Button handling for Espresso Pressure Controller
 */

#include "button.h"
#include "cmn.h"
#include "main.h"
#include "util/logging.h"

#define BTN_PRESS_DEBOUNCE_MS APP_SCHD_50MS
#define BTN_LONG_PRESS_MS     APP_SCHD_1000MS

typedef enum btn_t
{
    BTN_LH = 0,
    BTN_RH,
    BTN_LEN
} btn_t;

typedef enum btn_press_state_t
{
    BTN_PRESS_NONE = 0,
    BTN_PRESS_DEBOUNCE,
    BTN_PRESS_SHORT,
    BTN_PRESS_LONG
} btn_press_state_t;

void button_lp(void);
bool button_rh_short_press(void);
bool button_rh_long_press(void);
bool button_lh_short_press(void);
bool button_lh_long_press(void);
void button_process(void);

static struct button_t lhButton = {
    .short_press = button_lh_short_press,
    .long_press  = button_lh_long_press,
};

static struct button_t rhButton = {
    .short_press = button_rh_short_press,
    .long_press  = button_rh_long_press,
};

typedef struct btn_gpio_cfg_t
{
    GPIO_TypeDef* port;
    uint16_t pin;
} btn_gpio_cfg_t;

static struct button_hdlr_t button_hdlr = {
    .lp       = button_lp,
    .lhButton = &lhButton,
    .rhButton = &rhButton,
};

struct
{
    struct sys_hal_timer_hdlr_t* tmr;
    struct hal_tick_timer_t mlTmr;
    struct hal_tick_timer_t btnPressTmr[BTN_LEN];
    enum sys_dev_work_state_t ws;
    enum btn_press_state_t btnState[BTN_LEN];
    struct btn_gpio_cfg_t btn_gpio_cfg[BTN_LEN];
    bool shortPressFlag[BTN_LEN];
    bool longPressFlag[BTN_LEN];
} button_dev = {
    .ws = SYS_DEV_WORK_INIT,
    .btn_gpio_cfg =
        {
            [BTN_LH] = {.port = nSW_UP_GPIO_Port, .pin = nSW_UP_Pin},
            [BTN_RH] = {.port = nSW_DOWN_GPIO_Port, .pin = nSW_DOWN_Pin},
        },
};

button_hdlr_t* button_init(struct sys_hal_timer_hdlr_t* _tmr)
{
    if (_tmr == NULL)
    {
        return NULL;
    }

    button_dev.tmr = _tmr;
    button_dev.tmr->start(&button_dev.mlTmr);

    return &button_hdlr;
}

void button_lp(void)
{
    if (button_dev.tmr->tm(&button_dev.mlTmr, APP_SCHD_10MS) == 0)
    {
        switch (button_dev.ws)
        {
            case SYS_DEV_WORK_INIT:
                button_dev.ws = SYS_DEV_WORK_OPS;
                break;

            case SYS_DEV_WORK_OPS:
                button_process();
                break;

            default:
                button_dev.ws = SYS_DEV_WORK_INIT;
                break;
        }
    }
}

/* Scan both buttons. Debounce press, and detect short and long presses */
void button_process(void)
{
    for (btn_t btn = BTN_LH; btn < BTN_LEN; btn++)
    {
        bool pressed =
            (HAL_GPIO_ReadPin(button_dev.btn_gpio_cfg[btn].port, button_dev.btn_gpio_cfg[btn].pin) == GPIO_PIN_RESET);

        switch (button_dev.btnState[btn])
        {
            case BTN_PRESS_NONE:
                if (pressed)
                {
                    button_dev.tmr->start(&button_dev.btnPressTmr[btn]);
                    button_dev.btnState[btn] = BTN_PRESS_DEBOUNCE;
                }
                break;

            case BTN_PRESS_DEBOUNCE:
                if (!pressed)
                {
                    button_dev.btnState[btn] = BTN_PRESS_NONE;
                }
                else if (button_dev.tmr->tm(&button_dev.btnPressTmr[btn], BTN_PRESS_DEBOUNCE_MS) == 0)
                {
                    button_dev.btnState[btn] = BTN_PRESS_SHORT;
                }
                break;

            case BTN_PRESS_SHORT:
                if (!pressed)
                {
                    button_dev.shortPressFlag[btn] = true; /* Set on release */
                    button_dev.btnState[btn]       = BTN_PRESS_NONE;
                    LOG_INFO("BUTTON", "%s Short Press Detected", btn == BTN_LH ? "LH" : "RH");
                }
                else if (button_dev.tmr->tm(&button_dev.btnPressTmr[btn], BTN_LONG_PRESS_MS) == 0)
                {
                    button_dev.btnState[btn]      = BTN_PRESS_LONG;
                    button_dev.longPressFlag[btn] = true; /* Set before button releases */
                    LOG_INFO("BUTTON", "%s Long Press Detected", btn == BTN_LH ? "LH" : "RH");
                }
                break;

            case BTN_PRESS_LONG:
                if (!pressed)
                {
                    button_dev.btnState[btn] = BTN_PRESS_NONE; /* Reset */
                }
                break;

            default:
                button_dev.btnState[btn] = BTN_PRESS_NONE;
                break;
        }
    }
}

bool button_lh_short_press(void)
{
    bool val                          = button_dev.shortPressFlag[BTN_LH];
    button_dev.shortPressFlag[BTN_LH] = false; /* reset */
    return val;
}

bool button_lh_long_press(void)
{
    bool val                         = button_dev.longPressFlag[BTN_LH];
    button_dev.longPressFlag[BTN_LH] = false; /* reset */
    return val;
}

bool button_rh_short_press(void)
{
    bool val                          = button_dev.shortPressFlag[BTN_RH];
    button_dev.shortPressFlag[BTN_RH] = false; /* reset */
    return val;
}

bool button_rh_long_press(void)
{
    bool val                         = button_dev.longPressFlag[BTN_RH];
    button_dev.longPressFlag[BTN_RH] = false; /* reset */
    return val;
}