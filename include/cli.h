#ifndef CLI_H
#define CLI_H

#include <stdint.h>

#define CLI_FAILURE -1
#define CLI_SUCCESS 0
#define CLI_HELP 1

struct arguments {
  char *image;
  char *host;
  uint16_t port;
  uint32_t x;
  uint32_t y;
  uint8_t log_level;
};

int parse_arguments(const int, char *const *, struct arguments *);

#endif
