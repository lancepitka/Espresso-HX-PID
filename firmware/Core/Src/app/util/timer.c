/**
 * @file timer.c
 * @brief Software app timer
 */

#include "timer.h"
#include "main.h"

#define CUR_SYS_TICK HAL_GetTick()

static uint32_t (*getmstick)(void);
static struct sys_hal_timer_hdlr_t timer_hdlr;
static void _timer_start(struct hal_tick_timer_t* ptTimer);
static int8_t _timer_wait(struct hal_tick_timer_t* ptTimer, uint32_t nMSec);
static uint8_t _timer_elapse(struct hal_tick_timer_t* ptTimer, uint32_t nMSec);
static void _timer_loop_wait(uint32_t dlyTicks);
/*
 * Initializations for the driver entry
 */
struct sys_hal_timer_hdlr_t* _sys_init_timer_hdlr(void)
{
    timer_hdlr.start    = _timer_start;
    timer_hdlr.wait     = _timer_wait;
    timer_hdlr.loopWait = _timer_loop_wait;
    timer_hdlr.tm       = _timer_elapse;
    return (&timer_hdlr);
}
int8_t _get_sys_timer_handler(struct sys_hal_timer_hdlr_t** hdlr)
{
    *hdlr = &timer_hdlr;
    return 0;
}
/*
 * Caller for enabling the timer
 */
static void _timer_start(struct hal_tick_timer_t* ptTimer)
{
    ptTimer->bWork     = true;
    ptTimer->ntick     = CUR_SYS_TICK;
    ptTimer->bReloaded = false;
}
static int8_t _timer_wait(struct hal_tick_timer_t* ptTimer, uint32_t nMSec)
{
    uint32_t nTick = ptTimer->ntick + nMSec;
    if (ptTimer->bWork == false)
    {
        return -1;
    }
    if (ptTimer->ntick > (0xFFFFFFFF - nMSec))
    {
        nTick = nMSec - (0xFFFFFFFF - ptTimer->ntick);
        while (CUR_SYS_TICK > ptTimer->ntick)
            ;
        while (CUR_SYS_TICK < nTick)
            ;
    }
    else
    {
        while (CUR_SYS_TICK < nTick)
            ;
    }
    ptTimer->ntick = CUR_SYS_TICK;
    return 0;
}
/*
 * Caller for periodic timer
 * Returns 0 if period expired
 * auto-reload when expired
 */
static uint8_t _timer_elapse(struct hal_tick_timer_t* ptTimer, uint32_t nMSec)
{
    uint32_t nTick = ptTimer->ntick + nMSec;
    uint8_t retval = 1;
    if (ptTimer->bWork == false)
    {
        return (retval);
    }
    /*sample point near re-loading*/
    if (ptTimer->ntick > (0xFFFFFFFF - nMSec))
    {
        nTick = nMSec - (0xFFFFFFFF - ptTimer->ntick);
        if (CUR_SYS_TICK >= ptTimer->ntick)
        {
            retval = 2;
        }
        /*detect reloading event*/
        if (CUR_SYS_TICK < ptTimer->ntick)
        {
            ptTimer->bReloaded = true;
            retval             = 3;
        }
        if (ptTimer->bReloaded == true)
        {
            if (CUR_SYS_TICK >= nTick)
            {
                ptTimer->ntick     = CUR_SYS_TICK;
                ptTimer->bReloaded = false;
                retval             = 0;
            }
        }
    }
    else
    {
        if (CUR_SYS_TICK >= nTick)
        {
            ptTimer->ntick = CUR_SYS_TICK;
            retval         = 0;
        }
    }
    return (retval);
}

/*
 * @brief Delays number of msTick Systicks (typically 1 ms)
 * @param dlyTicks Number of ticks to delay
 */

static void _timer_loop_wait(uint32_t dlyTicks)
{
    uint32_t curTicks;
    curTicks = getmstick();
    while ((getmstick() - curTicks) < dlyTicks)
        ;
}
