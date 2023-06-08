#ifndef IMAGE_H
#define IMAGE_H

#include <png.h>
#include <stdint.h>
#include <stdio.h>

struct image {
  uint32_t height;
  uint32_t width;
  uint8_t channels;
  uint8_t **pixels;
};

int read_png(FILE *, struct image *);
void free_image(struct image *);
int commands(char *cmd, const struct image *, const uint32_t, const uint32_t);

#endif
