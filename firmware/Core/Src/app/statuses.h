/**
 * @file    statuses.h
 * @brief   Common status codes and types for the application.
 */

 #ifndef INC_TLBRD_APP_STATUSES_H_
 #define INC_TLBRD_APP_STATUSES_H_

#include <stdbool.h>
#include <stdint.h>

typedef enum appStat_t
{
    APP_OK           = 1,
    APP_NONE         = 0,
    APP_ERROR        = -1,
    APP_BUSY         = -3,
} appStat_t;

 #endif /* INC_TLBRD_APP_STATUSES_H_ */