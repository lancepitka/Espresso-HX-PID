/**
 * @file display.c
 * Display abstraction layer implementation for the project.
 */

#include "display.h"
#include "cmn.h"
#include "i2c.h"
#include "sh1106/SH1106.h"
#include "util/logging.h"

struct
{
    struct sys_hal_timer_hdlr_t* tmr;
    struct hal_tick_timer_t mlTmr;
    struct hal_tick_timer_t stpTmr;
    enum sys_dev_work_state_t ws;
    I2C_HandleTypeDef* i2cHdlr;

    int16_t number_to_display;
    int16_t new_number_to_display;
} display_dev = {
    .ws                    = SYS_DEV_WORK_INIT,
    .number_to_display     = -1,
    .new_number_to_display = 0,
};

static void display_lp(void);
static void display_set_number(int16_t num);
static void display_process(void);

static display_hdlr_t display_hdlr = {
    .lp         = display_lp,
    .set_number = display_set_number,
};

display_hdlr_t* display_init(struct sys_hal_timer_hdlr_t* _tmr, I2C_HandleTypeDef* _i2c)
{
    if (_tmr == NULL || _i2c == NULL)
    {
        return NULL;
    }

    display_dev.tmr     = _tmr;
    display_dev.i2cHdlr = _i2c;

    if (SH1106_Init(display_dev.i2cHdlr) == 0)
    {
        LOG_ERROR("DISPLAY", "Failed to initialize SH1106 display");
        return NULL;
    }

    display_dev.tmr->start(&display_dev.mlTmr);

    return &display_hdlr;
}

static void display_set_number(int16_t num)
{
    display_dev.new_number_to_display = num;
}

static void display_lp(void)
{
    if (display_dev.tmr->tm(&display_dev.mlTmr, 100))
    {
        switch (display_dev.ws)
        {
            case SYS_DEV_WORK_INIT:
                if (SH1106_Init(display_dev.i2cHdlr))
                {
                    display_dev.ws = SYS_DEV_WORK_POST;

                    SH1106_Clear();
                    SH1106_GotoXY(12, 0); // 132x64, so leave 2 from both sides, cursor at 10,0
                    SH1106_Puts("HELLO", &Font_11x18, 1);
                    SH1106_GotoXY(12, 30); // 132x64, so leave 2 from both sides, cursor at 10,30
                    SH1106_Puts("MYLA", &Font_11x18, 1);
                    SH1106_UpdateScreen();

                    display_dev.tmr->start(&display_dev.stpTmr);
                    LOG_INFO("DISPLAY", "SH1106 display initialized successfully");
                }
                break;
            case SYS_DEV_WORK_POST:
                if (display_dev.tmr->tm(&display_dev.stpTmr, 2000))
                {
                    display_dev.ws = SYS_DEV_WORK_OPS;
                    LOG_INFO("DISPLAY", "Entering display operations state");
                }
                break;
            case SYS_DEV_WORK_OPS:
                display_process();
                break;

            default:
                break;
        }
    }
}

static void display_process(void)
{
    if (display_dev.new_number_to_display != display_dev.number_to_display)
    {
        char buf[16];
        snprintf(buf, sizeof(buf), "%d", display_dev.new_number_to_display);
        display_dev.number_to_display = display_dev.new_number_to_display;
        // Update the display with the new number
        SH1106_Clear();
        SH1106_GotoXY(12, 0); // 132x64, so leave 2 from both sides, cursor at 10,0
        SH1106_Puts(buf, &Font_11x18, 1);
        SH1106_UpdateScreen();
    }
}