#define _GNU_SOURCE
#include <netdb.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/sendfile.h>
#include <sys/types.h>
#include <unistd.h>

static volatile int keep_sending = 1;

void handle_signal(int sig) {
  switch (sig) {
  default:
    keep_sending = 0;
  }
}

int send_forever(const char *host, const uint16_t port, const char *cmds) {
  signal(SIGINT, handle_signal);

  struct addrinfo hints;
  struct addrinfo *res;
  char port_str[5];
  snprintf(port_str, 5, "%d", port);

  int memfd = memfd_create("cmds.txt", 0);
  if (memfd == -1) {
    return EXIT_FAILURE;
  }

  if (write(memfd, cmds, strlen(cmds) + 1) == -1) {
    close(memfd);
    return EXIT_FAILURE;
  }

  if (lseek(memfd, 0, SEEK_SET) != 0) {
    close(memfd);
    return EXIT_FAILURE;
  }

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_flags = AI_PASSIVE;
  hints.ai_socktype = SOCK_STREAM;
  if (getaddrinfo(host, port_str, &hints, &res) != 0) {
    freeaddrinfo(res);
    close(memfd);
    return EXIT_FAILURE;
  }

  int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  if (sockfd == -1) {
    freeaddrinfo(res);
    close(memfd);
    return EXIT_FAILURE;
  }

  if (connect(sockfd, res->ai_addr, res->ai_addrlen) == -1) {
    close(sockfd);
    freeaddrinfo(res);
    close(memfd);
    return EXIT_FAILURE;
  }

  while (keep_sending && sendfile(sockfd, memfd, 0, strlen(cmds)) > 0) {
    lseek(memfd, 0, SEEK_SET);
  }

  close(sockfd);
  freeaddrinfo(res);
  close(memfd);

  return EXIT_SUCCESS;
}
