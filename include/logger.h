#ifndef __LOGGING_H__
#define __LOGGING_H__

typedef enum {
    LOGGER_LEVEL_ALL,
    LOGGER_LEVEL_ERROR,
    LOGGER_LEVEL_WARNING,
    LOGGER_LEVEL_INFO,
    LOGGER_LEVEL_NONE,
} LoggerLevel;

void Logger_print(char *s, ...);
void Logger_printLine(char *s, ...);
void Logger_printInfo(char *s, ...);
void Logger_printWarning(char *s, ...);
void Logger_printError(char *s, ...);

#endif
