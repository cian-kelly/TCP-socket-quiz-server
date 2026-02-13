#ifndef SOCKET_IO_H
#define SOCKET_IO_H

#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

#define BUFSIZE 500

void readFromSocket(char *buf, int cfd);
void writeToSocket(char *buf, int cfd);

void readFromSocket(char *buf, int cfd) {
 size_t totRead = 0;
 char *bufr = buf;
 memset(buf, 0, BUFSIZE);

 while (totRead < BUFSIZE) {
   ssize_t numRead = read(cfd, bufr, BUFSIZE - totRead);
   if (numRead <= 0) {
      if (numRead == -1 && errno == EINTR)
         continue;
      else {
        fprintf(stderr, "Read error. Errno %d.\n", errno);
	break;
      }
   }
   totRead += numRead;
   bufr += numRead;
 }
}

void writeToSocket(char *buf, int cfd) {
 size_t totWritten = 0;
 char *bufw = buf;

 while (totWritten < BUFSIZE) {
   ssize_t numWritten = write(cfd, bufw, BUFSIZE - totWritten);
   if (numWritten <= 0) {
      if (numWritten == -1 && errno == EINTR)
         continue;
      else {
        fprintf(stderr, "Write error. Errno %d.\n", errno);
      	break;
	}
   }
   totWritten += numWritten;
   bufw += numWritten;
 }
}

#endif