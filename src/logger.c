#include "logger.h"

LoggerLevel gLoggerLevel = LOGGER_LEVEL_WARNING;

void Logger_setLoggerLevel(LoggerLevel level) {
    if (level < 0 || level > LOGGER_LEVEL_NONE) {
        Logger_printWarning("Called Logger_setLoggerLevel with invalid level %d", level);
        return;
    }

    gLoggerLevel = level;
}
