/**
 * @file display.h
 * Display abstraction layer for the project.
 */

#ifndef DISPLAY_H
#define DISPLAY_H

#include "i2c.h"
#include "statuses.h"
#include "util/timer.h"

typedef struct display_hdlr_t
{
    void (*lp)(void);
    void (*set_number)(int16_t num); /* Set number to display, supports negative values */
} display_hdlr_t;

display_hdlr_t* display_init(struct sys_hal_timer_hdlr_t* _tmr, I2C_HandleTypeDef* _i2c);

#endif /* DISPLAY_H */