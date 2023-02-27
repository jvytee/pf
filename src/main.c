#include <png.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"

int main(void) {
  FILE *fp = fopen("sea-watch_logo_140.png", "rb");
  if (!fp) {
    printf("Could not open file\n");
    return EXIT_FAILURE;
  }

  struct image img;
  if (read_png(fp, &img) != 0) {
    printf("Could not read PNG\n");
    return EXIT_FAILURE;
  }

  if (fclose(fp) != 0) {
    printf("Could not close file\n");
    return EXIT_FAILURE;
  }

  printf("%d x %d x %d\n", img.height, img.width, img.channels);
  return EXIT_SUCCESS;
}

int read_png(FILE *fp, struct image *img) {
  unsigned char sig[8];
  const size_t items_read = fread(sig, 1, 8, fp);
  if (items_read < 8 || !png_check_sig(sig, 8)) {
    return EXIT_FAILURE;
  }

  png_structp png_ptr =
      png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
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
  png_read_png(png_ptr, info_ptr,
               PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_EXPAND |
                   PNG_TRANSFORM_GRAY_TO_RGB,
               NULL);

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

  img->pixels = calloc(height * width * channels, sizeof(uint8_t));
  memcpy(img->pixels, rows, height * width * channels * sizeof(uint8_t));

  png_destroy_read_struct(&png_ptr, &info_ptr, &info_end_ptr);
  return EXIT_SUCCESS;
}
