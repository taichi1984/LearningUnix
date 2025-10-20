#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
  int fd = open("/dev/tty", O_WRONLY);

  char beep = '\a';

  write(fd, "\a", 1);
  close(fd);
  return 0;
}
