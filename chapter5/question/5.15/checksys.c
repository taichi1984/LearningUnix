#include <errno.h>
#include <fcntl.h>
#include <linux/input.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(void) {

  char *pathlist[3] = {
      "/dev/input/mouse0", // mouse0
      "/dev/dri/card1",    // ディスプレイ
      "/dev/input/event6", // キーボード
  };

  for (int i = 0; i < 3; i++) {
    printf("%s \n", pathlist[i]);
    fflush(stdout);
    int fd = open(pathlist[i], O_RDWR | O_NONBLOCK);
    if (fd == -1) {
      perror("open");
      return 1;
    }

    char buf[1000] = "test";

    if (read(fd, buf, sizeof(struct input_event)) == -1)
      printf("read: %s\n", strerror(errno));
    else
      printf("read: success\n");

    if (write(fd, buf, sizeof(buf)) == -1)
      printf("write: %s\n", strerror(errno));
    else
      printf("write: success\n");

    if (lseek(fd, 0, SEEK_SET) == -1)
      printf("lseek: %s\n", strerror(errno));
    else
      printf("lseek: success\n");

    close(fd);
  }

  return 0;
}
