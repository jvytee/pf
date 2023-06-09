#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "bits/getopt_core.h"
#include "cli.h"

int parse_arguments(const int argc, char *const *argv, struct arguments *args) {
  args->image = NULL;
  args->host = NULL;
  args->port = 1234;
  args->x = 0;
  args->y = 0;

  int opt;
  while ((opt = getopt(argc, argv, "hH:p:x:y:")) != -1) {
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
    case 'h':
      printf("Usage: pf [-H <host>] [-p <port>] [-x <offset>] [-y <offset>] <image>\n");
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
