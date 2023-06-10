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
  uint8_t r;
  uint8_t g;
  uint8_t b;
};

int read_png(FILE *, struct image *);
void free_image(struct image *);
int generate_commands(struct command **, const struct image *, int32_t);
void apply_offsets(struct command **, const size_t, const uint32_t, const uint32_t);
void shuffle_commands(struct command **, struct command *const *, const size_t);
int serialize_commands(char *, struct command *const *, const size_t);
int quantize_command_string(char *, const char *, const size_t);
void free_commands(struct command **, size_t);

#endif
