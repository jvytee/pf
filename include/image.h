#ifndef IMAGE_H
#define IMAGE_H

#include <png.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

struct image {
  uint32_t height;
  uint32_t width;
  uint8_t channels;
  uint8_t **pixels;
};

struct command {
  uint32_t x;
  uint32_t y;
  char *color;
};

int read_png(FILE *, struct image *);
void free_image(struct image *);
int generate_commands(struct command **, const struct image *, const uint32_t, const uint32_t);
void shuffle_commands(struct command **, const size_t);
int serialize_commands(char *, struct command *const *, const size_t);
int quantize_command_string(char *, const char *, const size_t);
void free_commands(struct command **, size_t);

#endif
