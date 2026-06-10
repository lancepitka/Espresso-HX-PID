/**
 * @file timer.h
 * @brief Software app timer
 */

#include <stdbool.h>
#include <stdint.h>

#ifndef APP_UTIL_TIMER_H_
#define APP_UTIL_TIMER_H_

#define APP_TICK_CNT_MAX 0xFDE8 /*Max. value of the app's main tick counter*/
#define APP_SYS_TICK_MS  1U     /*This is the pre-scaler of the 1ms HAL's default tick*/
#define APP_SCHD_2MS     (2 / APP_SYS_TICK_MS)
#define APP_SCHD_5MS     (5 / APP_SYS_TICK_MS)
#define APP_SCHD_10MS    (10 / APP_SYS_TICK_MS)
#define APP_SCHD_20MS    (20 / APP_SYS_TICK_MS)
#define APP_SCHD_25MS    (25 / APP_SYS_TICK_MS)
#define APP_SCHD_50MS    (50 / APP_SYS_TICK_MS)
#define APP_SCHD_60MS    (60 / APP_SYS_TICK_MS)
#define APP_SCHD_100MS   (100 / APP_SYS_TICK_MS)
#define APP_SCHD_150MS   (150 / APP_SYS_TICK_MS)
#define APP_SCHD_250MS   (250 / APP_SYS_TICK_MS)
#define APP_SCHD_500MS   (500 / APP_SYS_TICK_MS)
#define APP_SCHD_1000MS  (1000 / APP_SYS_TICK_MS)
#define APP_SCHD_2000MS  (2000 / APP_SYS_TICK_MS)
#define APP_SCHD_3000MS  (3000 / APP_SYS_TICK_MS)
#define APP_SCHD_5000MS  (5000 / APP_SYS_TICK_MS)
#define APP_SCHD_5S      (5000 / APP_SYS_TICK_MS)
#define APP_SCHD_10S     (10000 / APP_SYS_TICK_MS)
#define APP_SCHD_30S     (30000 / APP_SYS_TICK_MS)
#define APP_SCHD_1Min    (60 * APP_SCHD_1000MS)
#define APP_SCHD_5Mins   (300 * APP_SCHD_1000MS)

typedef struct hal_tick_timer_t
{
    bool bWork;
    uint32_t ntick;
    bool bReloaded;
} hal_tick_timer_t;

typedef struct sys_hal_timer_hdlr_t
{
    void (*start)(struct hal_tick_timer_t* ptTimer);
    int8_t (*wait)(struct hal_tick_timer_t* ptTimer, uint32_t nMSec);
    void (*loopWait)(uint32_t dlyTicks);
    uint8_t (*tm)(struct hal_tick_timer_t* ptTimer, uint32_t nMSec);
} sys_hal_timer_hdlr_t;

struct sys_hal_timer_hdlr_t* _sys_init_timer_hdlr(void);
int8_t _get_sys_timer_handler(struct sys_hal_timer_hdlr_t** hdlr);

#endif /* APP_UTIL_TIMER_H_ */
