/**
 * @file logging.h
 * @brief Logging utilities
 *
 * @example
 * Set minimum log level at startup (default: LOG_LEVEL_DEBUG):
 * Log_SetLevel(LOG_LEVEL_INFO);
 *
 * Log messages with a module tag and printf-style format:
 * LOG_DEBUG("ADC",  "Raw value: %d", rawAdc);       // filtered out (below INFO)
 * LOG_INFO ("MAIN", "System started");
 * LOG_WARN ("PID",  "Setpoint clamped to %.2f bar", maxBar);
 * LOG_ERROR("I2C",  "Timeout on addr 0x%02X", addr);
 *
 * Output format:  [LEVEL] TAG: message\r\n
 * Example output: [WARN] PID: Setpoint clamped to 1.00 bar
 */

#ifndef LOGGING_H_
#define LOGGING_H_

#include <stdio.h>

typedef enum
{
    LOG_LEVEL_DEBUG = 0,
    LOG_LEVEL_INFO  = 1,
    LOG_LEVEL_WARN  = 2,
    LOG_LEVEL_ERROR = 3,
    LOG_LEVEL_NONE  = 4,
} LogLevel_t;

void Log_SetLevel(LogLevel_t level);
LogLevel_t Log_GetLevel(void);
void Log_Write(LogLevel_t level, const char* tag, const char* fmt, ...);

#define LOG_DEBUG(tag, fmt, ...) Log_Write(LOG_LEVEL_DEBUG, tag, fmt, ##__VA_ARGS__)
#define LOG_INFO(tag, fmt, ...)  Log_Write(LOG_LEVEL_INFO, tag, fmt, ##__VA_ARGS__)
#define LOG_WARN(tag, fmt, ...)  Log_Write(LOG_LEVEL_WARN, tag, fmt, ##__VA_ARGS__)
#define LOG_ERROR(tag, fmt, ...) Log_Write(LOG_LEVEL_ERROR, tag, fmt, ##__VA_ARGS__)

#endif /* LOGGING_H_ */