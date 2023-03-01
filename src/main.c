#include <stdio.h>
#include <stdlib.h>

#include "cli.h"
#include "image.h"

int main(const int argc, char *const *argv) {
  struct arguments args;
  if (parse_arguments(argc, argv, &args) != EXIT_SUCCESS) {
    return EXIT_FAILURE;
  }

  FILE *fp = fopen(args.image, "rb");
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

  char *cmds = calloc(img.height * img.width * 20 + 1, sizeof(char));
  commands(cmds, &img);

  if (args.host != NULL) {
    printf("Not implemented\n");
  } else {
    printf("%s", cmds);
  }

  free(cmds);
  for (uint32_t y = 0; y < img.height; y++) {
    free(img.pixels[y]);
  }
  free(img.pixels);

  return EXIT_SUCCESS;
}
