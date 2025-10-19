#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int unset_sync(int fd) {
  int s;
  int result;
  s = fcntl(fd, F_GETFL);
  if (s == -1) {
    perror("fcntl GETFL");
    return -1;
  }

  s &= ~O_SYNC;
  result = fcntl(fd, F_SETFL, s);

  if (result == -1) {
    perror("unset SYNC");
    return -1;
  }

  return 0;
}

int main(void) {
  int fd;

  fd = open("test", O_RDWR);
  if (fd == -1) {
    perror("open");
    exit(1);
  }

  unset_sync(fd);


  close(fd);
}
