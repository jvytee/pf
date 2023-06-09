#include <stdio.h>
#include <stdlib.h>

#include "cli.h"
#include "image.h"
#include "log.h"
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
    lprintf(LOG_ERROR, "Could not parse command line arguments\n");
    return EXIT_FAILURE;
  }

  set_log_level(args.log_level);

  FILE *fp = fopen(args.image, "rb");
  if (!fp) {
    lprintf(LOG_ERROR, "Could not open %s\n", args.image);
    return EXIT_FAILURE;
  }

  lprintf(LOG_INFO, "Reading PNG %s\n", args.image);
  struct image img;
  if (read_png(fp, &img) != 0) {
    lprintf(LOG_ERROR, "Could not read PNG %s\n", args.image);
    fclose(fp);
    return EXIT_FAILURE;
  }

  if (fclose(fp) != 0) {
    lprintf(LOG_ERROR, "Could not close %s\n", args.image);
    return EXIT_FAILURE;
  }

  lprintf(LOG_INFO, "Creating pixelflut commands\n");
  char *cmds = calloc(img.height * img.width * 20 + 1, sizeof(char));
  commands(cmds, &img, args.x, args.y);

  lprintf(LOG_INFO, "Sending pixelflut commands to %s:%d\n", args.host, args.port);
  if (args.host != NULL) {
    if (send_forever(args.host, args.port, cmds) != EXIT_SUCCESS) {
      lprintf(LOG_ERROR, "Could not send commands to %s:%d\n", args.host, args.port);
    }
  } else {
    printf("%s", cmds);
  }

  free(cmds);
  free_image(&img);

  lprintf(LOG_INFO, "Done.\n");
  return EXIT_SUCCESS;
}
