#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

char buf[4000];
int count = 100;

int main(void) {
  int fd;
  if ((fd = open("sampleFile", O_RDWR)) == -1) {
    perror("open");
    return 1;
  }

  read(fd, buf, 10);
  printf("10バイトだけ読む : %s\n", buf);

  printf("ここからスリープ\n");
  sleep(20);
  printf("ここまでスリープ\n");

  while (read(fd, buf, count) > 0) {
    printf("%s", buf);
  }
}
