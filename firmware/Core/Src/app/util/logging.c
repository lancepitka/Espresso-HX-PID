/**
 * @file logging.c
 * @brief Logging utilities
 */

#include "logging.h"
#include <stdarg.h>
#include <stdio.h>

static LogLevel_t s_logLevel = LOG_LEVEL_DEBUG;

static const char* const s_levelStrings[] = {
    [LOG_LEVEL_DEBUG] = "DEBUG",
    [LOG_LEVEL_INFO]  = "INFO",
    [LOG_LEVEL_WARN]  = "WARN",
    [LOG_LEVEL_ERROR] = "ERROR",
};

void Log_SetLevel(LogLevel_t level)
{
    s_logLevel = level;
}

LogLevel_t Log_GetLevel(void)
{
    return s_logLevel;
}

void Log_Write(LogLevel_t level, const char* tag, const char* fmt, ...)
{
    if (level < s_logLevel)
    {
        return;
    }

    printf("[%s] %s: ", s_levelStrings[level], tag);

    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);

    printf("\r\n");
}