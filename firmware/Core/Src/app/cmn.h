/**
 * @file cmn.h
 * @brief Common definitions and utilities for the application.
 */

#ifndef CMN_H_
#define CMN_H_

/* Working states for many of the state machines */
typedef enum sys_dev_work_state_t
{
    SYS_DEV_WORK_INIT = 0,
    SYS_DEV_WORK_POST,
    SYS_DEV_WORK_OPS,
    SYS_DEV_DIAG_REPORT,
    SYS_DEV_DIAG_REPORT_ACK,
    SYS_DEV_DIAG_REPORTED,
    SYS_DEV_DIAG_WAIT,
    SYS_DEV_WORK_LEN
} sys_dev_work_state_t;

#endif /* CMN_H_ */