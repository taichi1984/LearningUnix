#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
ssize_t write_all(int fd, const char *buf, ssize_t n) {
  ssize_t total = 0;

  while (total < n) {
    ssize_t w = write(fd, buf + total, n - total);

    if (w == -1) {
      perror("write");
      return -1;
    }

    total += w;
  }
  return total;
}

int main(int ac, char *av[]) {
  char buf[BUFSIZ];
  int fd;
  if (ac != 2) {
    fprintf(stderr, "usage : progtee filename\n");
    exit(1);
  }

  if ((fd = open(av[1], (O_WRONLY | O_CREAT | O_TRUNC), 0644)) == -1) {
    perror("open");
    exit(1);
  }

  ssize_t n;
  while ((n = read(STDIN_FILENO, buf, sizeof(buf))) > 0) {

    if (write_all(fd, buf, n) == -1)
      exit(1);
    if (write_all(STDOUT_FILENO, buf, n) == -1)
      exit(1);
  }
  if (n == -1) {
    perror("read");
    exit(1);
  }

  close(fd);
}
