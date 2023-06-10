#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cli.h"
#include "image.h"
#include "log.h"
#include "network.h"

void work(struct command **cmds, const size_t len_cmds, const struct arguments *args) {
  lprintf(LOG_INFO, "Shuffling %d pixelflut commands\n", len_cmds);
  shuffle_commands(cmds, len_cmds);

  lprintf(LOG_INFO, "Serializing %d pixelflut commands\n", len_cmds);
  char *cmds_serialized = calloc(len_cmds * 20 + 1, sizeof(char));
  size_t len_serialized = serialize_commands(cmds_serialized, cmds, len_cmds);

  lprintf(LOG_INFO, "Quantizing %d bytes pixelflut command string\n", len_serialized);
  char *cmds_quantized = calloc(args->len_buffer, sizeof(char));
  const size_t len_command_string = quantize_command_string(cmds_quantized, cmds_serialized, args->len_buffer);

  lprintf(LOG_INFO, "Sending %d bytes pixelflut command string to %s:%d\n", len_command_string, args->host, args->port);
  if (args->host != NULL) {
    if (send_forever(args->host, args->port, cmds_quantized) != EXIT_SUCCESS) {
      lprintf(LOG_ERROR, "Could not send commands to %s:%d\n", args->host, args->port);
    }
  } else {
    printf("%s", cmds_quantized);
  }

  free(cmds_quantized);
  free(cmds_serialized);
}

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
  struct command **cmds = calloc(img.height * img.width, sizeof(struct command *));
  const size_t len_cmds = generate_commands(cmds, &img);

  lprintf(LOG_INFO, "Applying offsets (%d, %d) to %d pixelflut commands\n", args.x, args.y, len_cmds);
  apply_offsets(cmds, len_cmds, args.x, args.y);

  work(cmds, len_cmds, &args);

  free_commands(cmds, len_cmds);
  free_image(&img);

  lprintf(LOG_INFO, "Done.\n");
  return EXIT_SUCCESS;
}
