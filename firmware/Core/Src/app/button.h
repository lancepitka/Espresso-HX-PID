/**
 * @file button.h
 * @brief Button handling for Espresso Pressure Controller
 */

#ifndef BUTTON_H_
#define BUTTON_H_

#include "statuses.h"
#include "util/timer.h"

typedef struct button_t
{
    bool (*short_press)(void);
    bool (*long_press)(void);
} button_t;

typedef struct button_hdlr_t
{
    void (*lp)(void);
    button_t* lhButton;
    button_t* rhButton;
} button_hdlr_t;

button_hdlr_t* button_init(struct sys_hal_timer_hdlr_t* _tmr);

#endif /* BUTTON_H_ */