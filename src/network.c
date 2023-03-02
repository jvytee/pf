#include <netdb.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sendfile.h>
#include <sys/types.h>
#include <unistd.h>

int send_forever(const char *host, const uint16_t port, const char *cmds) {
  int retval = EXIT_FAILURE;
  struct addrinfo hints;
  struct addrinfo *res;
  char port_str[5];
  snprintf(port_str, 5, "%d", port);

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_flags = AI_PASSIVE;
  hints.ai_socktype = SOCK_STREAM;
  if (getaddrinfo(host, port_str, &hints, &res) != 0) {
    freeaddrinfo(res);
    return retval;
  }

  int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  if (sockfd == -1) {
    freeaddrinfo(res);
    return retval;
  }

  if (connect(sockfd, res->ai_addr, res->ai_addrlen) == -1) {
    close(sockfd);
    freeaddrinfo(res);
    return retval;
  }

  // FILE *buffer = fmemopen((void *)cmds, strlen(cmds) + 1, "r");
  // int buffd = fileno(buffer);

  if (write(sockfd, cmds, strlen(cmds)) > 0) {
    retval = EXIT_SUCCESS;
  }

  // fclose(buffer);
  close(sockfd);
  freeaddrinfo(res);

  return EXIT_SUCCESS;
}
