#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>

#include "log.h"

static uint8_t LOG_LEVEL = LOG_ERROR;
static __thread char *SOURCE_THREAD = "Main thread";

void set_log_level(const uint8_t level) {
  LOG_LEVEL = level;
}

void set_source_thread(char* thread) {
  SOURCE_THREAD = thread;
}

void lprintf(const uint8_t level, const char *message, ...) {
  if (level >= LOG_LEVEL) {
    va_list args;
    va_start(args, message);
    printf("%s: ", SOURCE_THREAD);
    vprintf(message, args);
    va_end(args);
  }
}
