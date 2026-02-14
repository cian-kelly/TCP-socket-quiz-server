#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include "socket_io.h"

int main(int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(stderr, "Usage: %s <IP address of server> <port number>.\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in serverAddress;

  memset(&serverAddress, 0, sizeof(struct sockaddr_in));
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_addr.s_addr = inet_addr(argv[1]);
  serverAddress.sin_port = htons(atoi(argv[2]));

  int cfd = socket(AF_INET, SOCK_STREAM, 0);
  if (cfd == -1) {
    fprintf(stderr, "socket() error.\n");
    exit(EXIT_FAILURE);
  }

  int rc = connect(cfd, (struct sockaddr *)&serverAddress, sizeof(struct sockaddr));
  if (rc == -1) {
    fprintf(stderr, "connect() error, errno %d.\n", errno);
    exit(EXIT_FAILURE);
  }
  char buf[BUFSIZE];

  readFromSocket(buf, cfd);
  printf("%s\n", buf);
  fflush(stdout);

  int checker = 0;
  while (checker == 0) {
    scanf("%s", buf);

    if ((strcmp(buf, "q") == 0) || (strcmp(buf, "Y") == 0)) {
      checker = 1;
    } else {
        printf("Please type q to quit or Y to continue\n");
      }
  }
  writeToSocket(buf, cfd);

  if (strcmp(buf, "q") == 0) {
     if (close(cfd) == -1) {
       fprintf(stderr, "close error.\n");
       exit(EXIT_FAILURE);
     } 
     exit(EXIT_SUCCESS);
  } else {
       int count = 0;
       while (count < 5) {
         readFromSocket(buf, cfd);
         printf("\nQuestion %d:\n%s\n", (count+1), buf);
         fflush(stdout);

         scanf("%s", buf);
         writeToSocket(buf, cfd);

         readFromSocket(buf, cfd);
         printf("\n%s\n", buf);
         count++;
       }
       readFromSocket(buf, cfd);
       printf("\n%s\n", buf);
    }
  
  if (close(cfd) == -1) {
    fprintf(stderr, "close error.\n");
    exit(EXIT_FAILURE);
  }
  exit(EXIT_SUCCESS);
}
