#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "bits/getopt_core.h"
#include "cli.h"

int parse_arguments(const int argc, char *const *argv,
                     struct arguments *args) {
  args->image = NULL;
  args->host = NULL;
  args->port = 0;

  int opt;
  while ((opt = getopt(argc, argv, "hH:p:")) != -1) {
    switch (opt) {
    case 'H':
      args->host = optarg;
      break;
    case 'p':
      args->port = atoi(optarg);
      break;
    case 'h':
      printf("Usage: pf [-H <host>] [-p <port>] <image>\n");
      return -1;
    default:
      return EXIT_FAILURE;
    }
  }

  if (optind < (argc)) {
    args->image = argv[optind];
    return EXIT_SUCCESS;
  }

  return EXIT_FAILURE;
}
