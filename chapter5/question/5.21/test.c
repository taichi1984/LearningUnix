#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main(void) {

  int fd = open("/dev/zero", O_RDONLY);
  int buf[10];

  read(fd, buf, sizeof(buf));

  printf("%d \n", buf[1]);

  close(fd);

  return 0;
}
