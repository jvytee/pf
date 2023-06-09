#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>

#include "log.h"

uint8_t LOG_LEVEL = LOG_ERROR;

void lprintf(const uint8_t level, const char *message, ...) {
  if (level >= LOG_LEVEL) {
    va_list args;
    va_start(args, message);
    vprintf(message, args);
    va_end(args);
  }
}
