#ifndef CLI_H
#define CLI_H

#include <stdint.h>

struct arguments {
  char *image;
  char *host;
  uint16_t port;
};

int parse_arguments(const int, char *const *, struct arguments *);

#endif
