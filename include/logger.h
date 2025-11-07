#ifndef __LOGGING_H__
#define __LOGGING_H__

#include "recomputils.h"

typedef enum {
    LOGGER_LEVEL_ALL,
    LOGGER_LEVEL_INFO,
    LOGGER_LEVEL_WARNING,
    LOGGER_LEVEL_ERROR,
    LOGGER_LEVEL_NONE,
} LoggerLevel;

extern LoggerLevel gLoggerLevel;

#define Logger_printLine(...)       \
    {                               \
        recomp_printf(__VA_ARGS__); \
        recomp_printf("\n");        \
    }

#define Logger_printInfo(...)                        \
    {                                                \
        if (gLoggerLevel <= LOGGER_LEVEL_INFO) {     \
            Logger_printLine("[INFO] " __VA_ARGS__); \
        }                                            \
    }

#define Logger_printWarning(...)                        \
    {                                                   \
        if (gLoggerLevel <= LOGGER_LEVEL_WARNING) {     \
            Logger_printLine("[WARNING] " __VA_ARGS__); \
        }                                               \
    }

#define Logger_printError(...)                        \
    {                                                 \
        if (gLoggerLevel <= LOGGER_LEVEL_ERROR) {     \
            Logger_printLine("[ERROR] " __VA_ARGS__); \
        }                                             \
    }

void Logger_setLoggerLevel(LoggerLevel level);

#endif
