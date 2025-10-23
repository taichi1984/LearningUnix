#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
  int fd;
  fd = open("/dev/loop23", O_RDWR | O_APPEND);
  if (fd == -1) {
    perror("open");
    exit(1);
  }

  write(fd, "testdata\n", sizeof("testdata"));

  close(fd);
}
