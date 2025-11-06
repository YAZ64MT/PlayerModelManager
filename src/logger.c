#include "logger.h"
#include "recomputils.h"
#include "libc/stdarg.h"

static LoggerLevel sLogLevel = LOGGER_LEVEL_ALL;

void Logger_print(char *s, ...) {
    if (sLogLevel < LOGGER_LEVEL_NONE) {
        va_list args;
        va_start(args, s);

        recomp_printf(s, args);
    }
}

void Logger_printLine(char *s, ...) {
    va_list args;
    va_start(args, s);

    Logger_print(s, args);
    Logger_print("\n");
}

void Logger_printInfo(char *s, ...) {
    if (sLogLevel <= LOGGER_LEVEL_INFO) {
        va_list args;
        va_start(args, s);

        recomp_printf("[INFO] ");
        Logger_printLine(s, args);
    }
}

void Logger_printWarning(char *s, ...) {
    if (sLogLevel <= LOGGER_LEVEL_WARNING) {
        va_list args;
        va_start(args, s);

        recomp_printf("[WARNING] ");
        Logger_printLine(s, args);
    }
}

void Logger_printError(char *s, ...) {
    if (sLogLevel <= LOGGER_LEVEL_ERROR) {
        va_list args;
        va_start(args, s);

        recomp_printf("[ERROR] ");
        Logger_printLine(s, args);
    }
}

void Logger_setLoggerLevel(LoggerLevel level) {
    if (level < 0 || level >LOGGER_LEVEL_NONE) {
        Logger_printWarning("Called Logger_setLoggerLevel with invalid level %d", level);
        return;
    }

    sLogLevel = level;
}
