#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "bits/getopt_core.h"
#include "cli.h"
#include "log.h"

int parse_arguments(const int argc, char *const *argv, struct arguments *args) {
  args->image = NULL;
  args->host = NULL;
  args->port = 1234;
  args->x = 0;
  args->y = 0;
  args->len_buffer = 1 * 1024 * 1024;
  args->num_workers = 1;
  args->log_level = LOG_ERROR;

  int opt;
  while ((opt = getopt(argc, argv, "hvH:p:x:y:b:w:")) != -1) {
    switch (opt) {
    case 'H':
      args->host = optarg;
      break;
    case 'p':
      args->port = atoi(optarg);
      break;
    case 'x':
      args->x = atoi(optarg);
      break;
    case 'y':
      args->y = atoi(optarg);
      break;
    case 'b':
      args->len_buffer = atoi(optarg);
      break;
    case 'w':
      args->num_workers = atoi(optarg);
      break;
    case 'v':
      args->log_level = LOG_INFO;
      break;
    case 'h':
      printf("Usage: pf [options] <image>\n\n"
             "Options:\n"
             "-H <host>           Target host name\n"
             "-b <buffer size>    Size of command buffer in byte (default: 1 MiB)\n"
             "-h                  Show this help and exit\n"
             "-p <port>           Target port number (default: 1234)\n"
             "-v                  Verbose\n"
             "-w <worker count>   Number of worker threads (default: 1)\n"
             "-x <offset>         X offset for image coordinates (default: 0)\n"
             "-y <offset>         Y offset for image coordinates (default: 0)\n");
      return CLI_HELP;
    default:
      return CLI_FAILURE;
    }
  }

  if (optind < (argc)) {
    args->image = argv[optind];
    return CLI_SUCCESS;
  }

  return CLI_FAILURE;
}
