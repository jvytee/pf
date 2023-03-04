#include <stdio.h>
#include <stdlib.h>

#include "cli.h"
#include "image.h"
#include "network.h"

int main(const int argc, char *const *argv) {
  struct arguments args;
  switch (parse_arguments(argc, argv, &args)) {
  case CLI_SUCCESS:
    break;
  case CLI_HELP:
    return EXIT_SUCCESS;
    break;
  case CLI_FAILURE:
  default:
    printf("Could not parse command line arguments\n");
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
    if (send_forever(args.host, args.port, cmds) != EXIT_SUCCESS) {
      printf("Could not send commands\n");
    }
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
