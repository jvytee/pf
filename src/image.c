#include <png.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "image.h"

int read_png(FILE *fp, struct image *img) {
  unsigned char sig[8];
  const size_t items_read = fread(sig, 1, 8, fp);
  if (items_read < 8 || !png_check_sig(sig, 8)) {
    return EXIT_FAILURE;
  }

  png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (!png_ptr) {
    return EXIT_FAILURE;
  }

  png_infop info_ptr = png_create_info_struct(png_ptr);
  if (!info_ptr) {
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    return EXIT_FAILURE;
  }

  png_infop info_end_ptr = png_create_info_struct(png_ptr);
  if (!info_end_ptr) {
    png_destroy_read_struct(&png_ptr, &info_ptr, &info_end_ptr);
    return EXIT_FAILURE;
  }

  png_set_sig_bytes(png_ptr, 8);
  png_init_io(png_ptr, fp);
  png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_EXPAND | PNG_TRANSFORM_GRAY_TO_RGB, NULL);

  uint32_t height = png_get_image_height(png_ptr, info_ptr);
  uint32_t width = png_get_image_width(png_ptr, info_ptr);
  uint8_t channels = png_get_channels(png_ptr, info_ptr);

  png_bytepp rows = png_get_rows(png_ptr, info_ptr);
  if (!rows) {
    png_destroy_read_struct(&png_ptr, &info_ptr, &info_end_ptr);
    return EXIT_FAILURE;
  }

  img->height = height;
  img->width = width;
  img->channels = channels;

  img->pixels = calloc(height, sizeof(uint8_t *));
  if (img->pixels == NULL) {
    png_destroy_read_struct(&png_ptr, &info_ptr, &info_end_ptr);
    return EXIT_FAILURE;
  }

  for (uint32_t y = 0; y < height; y++) {
    img->pixels[y] = calloc(width * channels, sizeof(uint8_t));
    if (img->pixels[y] == NULL) {
      png_destroy_read_struct(&png_ptr, &info_ptr, &info_end_ptr);
      return EXIT_FAILURE;
    }

    memcpy(img->pixels[y], rows[y], width * channels * sizeof(uint8_t));
  }

  png_destroy_read_struct(&png_ptr, &info_ptr, &info_end_ptr);
  return EXIT_SUCCESS;
}

void free_image(struct image *img) {
  for (uint32_t y = 0; y < img->height; y++) {
    free(img->pixels[y]);
  }
  free(img->pixels);
}

int commands(char *cmds, const struct image *img, const uint32_t x_offset, const uint32_t y_offset) {
  if (img->height > 9999 || img->width > 9999) {
    return -1;
  }

  char *cmd_ptr = cmds;
  uint8_t *row;
  uint8_t *pixel;
  int bytes_written;
  int bytes_total = 0;

  for (uint32_t y = 0; y < img->height; y++) {
    row = img->pixels[y];

    for (uint32_t x = 0; x < img->width; x++) {
      pixel = &row[x * img->channels];
      if (pixel[3] == 0x00) {
        continue;
      }

      bytes_written = snprintf(cmd_ptr, 21, "PX %u %u %02x%02x%02x\n", x + x_offset, y + y_offset, pixel[0], pixel[1], pixel[2]);
      if (bytes_written < 0) {
        return bytes_written;
      }

      bytes_total += bytes_written;
      cmd_ptr += bytes_written;
    }
  }

  return bytes_total;
}
