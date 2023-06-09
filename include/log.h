#ifndef LOG_H
#define LOG_H

#include <stdint.h>

#define LOG_CRITICAL 50
#define LOG_ERROR 40
#define LOG_WARNING 30
#define LOG_INFO 20
#define LOG_DEBUG 10
#define LOG_NOTSET 0

extern uint8_t LOG_LEVEL;

void lprintf(const uint8_t, const char*, ...);

#endif
