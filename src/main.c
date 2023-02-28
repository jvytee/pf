#include <stdio.h>
#include <stdlib.h>

#include "image.h"

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

  char *cmds = calloc(img.height * img.width * 20 + 1, sizeof(char));
  commands(cmds, &img);
  printf("%s", cmds);
  free(cmds);

  return EXIT_SUCCESS;
}
