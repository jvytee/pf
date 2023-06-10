#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bits/pthreadtypes.h"
#include "cli.h"
#include "image.h"
#include "log.h"
#include "network.h"

struct worker_args {
  struct command **cmds;
  size_t len_cmds;
  const struct arguments *args;
  uint8_t num;
};

void *work(void *args) {
  struct worker_args *worker_args = (struct worker_args*) args;
  char source_thread[11];
  snprintf(source_thread, 11, "Worker %d", worker_args->num);
  set_source_thread(source_thread);

  lprintf(LOG_INFO, "Shuffling %d pixelflut commands", worker_args->len_cmds);
  struct command **cmds_shuffled = calloc(worker_args->len_cmds, sizeof(struct command *));
  shuffle_commands(cmds_shuffled, worker_args->cmds, worker_args->len_cmds);

  lprintf(LOG_INFO, "Serializing %d pixelflut commands", worker_args->len_cmds);
  char *cmds_serialized = calloc(worker_args->len_cmds * 20 + 1, sizeof(char));
  size_t len_serialized = serialize_commands(cmds_serialized, cmds_shuffled, worker_args->len_cmds);

  lprintf(LOG_INFO, "Quantizing %d bytes pixelflut command string", len_serialized);
  char *cmds_quantized = calloc(worker_args->args->len_buffer, sizeof(char));
  const size_t len_command_string = quantize_command_string(cmds_quantized, cmds_serialized, worker_args->args->len_buffer);

  lprintf(LOG_INFO, "Sending %d bytes pixelflut command string to %s:%d", len_command_string, worker_args->args->host, worker_args->args->port);
  if (worker_args->args->host != NULL) {
    if (send_forever(worker_args->args->host, worker_args->args->port, cmds_quantized) != EXIT_SUCCESS) {
      lprintf(LOG_ERROR, "Could not send commands to %s:%d", worker_args->args->host, worker_args->args->port);
    }
  } else {
    printf("%s", cmds_quantized);
  }

  free(cmds_shuffled);
  free(cmds_quantized);
  free(cmds_serialized);

  return NULL;
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
    lprintf(LOG_ERROR, "Could not open %s", args.image);
    return EXIT_FAILURE;
  }

  lprintf(LOG_INFO, "Reading PNG %s", args.image);
  struct image img;
  if (read_png(fp, &img) != 0) {
    lprintf(LOG_ERROR, "Could not read PNG %s", args.image);
    fclose(fp);
    return EXIT_FAILURE;
  }

  if (fclose(fp) != 0) {
    lprintf(LOG_ERROR, "Could not close %s\n", args.image);
    return EXIT_FAILURE;
  }

  lprintf(LOG_INFO, "Creating pixelflut commands");
  struct command **cmds = calloc(img.height * img.width, sizeof(struct command *));
  const size_t len_cmds = generate_commands(cmds, &img);

  lprintf(LOG_INFO, "Applying offsets (%d, %d) to %d pixelflut commands", args.x, args.y, len_cmds);
  apply_offsets(cmds, len_cmds, args.x, args.y);

  lprintf(LOG_INFO, "Starting %d worker threads", args.num_workers);
  pthread_t *worker_threads = calloc(args.num_workers, sizeof(pthread_t));
  struct worker_args *worker_args = calloc(args.num_workers, sizeof(struct worker_args));
  for (uint8_t i = 0; i < args.num_workers; i++) {
    worker_args[i].cmds = cmds;
    worker_args[i].len_cmds = len_cmds;
    worker_args[i].args = &args;
    worker_args[i].num = i;
    pthread_create(&worker_threads[i], NULL, work, &worker_args[i]);
  }

  for (size_t i = 0; i < args.num_workers; i++) {
    pthread_join(worker_threads[i], NULL);
  }

  lprintf(LOG_INFO, "Shutting down");
  free(worker_args);
  free(worker_threads);
  free_commands(cmds, len_cmds);
  free_image(&img);

  lprintf(LOG_INFO, "Done.");
  return EXIT_SUCCESS;
}
