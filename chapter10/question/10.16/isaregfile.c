#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

int isaregfile(int fd) {
  struct stat data;

  if (fstat(fd, &data) == -1) {
    return -1;
  }
  return S_ISREG(data.st_mode);
}

int main(int ac, char *av[]) {
  int fd;

  if (ac != 2) {
    fprintf(stderr, "usage : isaregfile filename");
    exit(1);
  }

  fd = open(av[1], O_CREAT | O_TRUNC | O_RDWR, 0644);
  if (fd < 0) {
    perror("open");
    exit(1);
  }

  printf("開いたファイル、%sをチェックします。\n", av[1]);

  int rv = isaregfile(fd);

  if (rv == 1) {
    printf("通常のファイルに接続されています。\n");
  } else if (rv == 0) {
    printf("通常のファイル以外のファイルに接続されています。\n");
  } else if (rv == -1) {
    perror("fstat");
    exit(1);
  }

  close(fd);

  printf("標準出力をチェックします。\n");

  rv = isaregfile(STDOUT_FILENO);

  if (rv == 1) {
    printf("通常のファイルに接続されています。\n");
  } else if (rv == 0) {
    printf("通常のファイル以外のファイルに接続されています。\n");
  } else if (rv == -1) {
    perror("fstat");
    exit(1);
  }
}
