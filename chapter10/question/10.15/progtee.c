#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
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
  int thepipe[2];
  ssize_t n;

  if (ac < 2) {
    fprintf(stderr, "usage: prog command\n");
    exit(1);
  }

  if ((pipe(thepipe)) == -1) {
    perror("pipe");
    exit(1);
  }
  if (fork() == 0) {
    dup2(thepipe[0], 0);
    close(thepipe[0]);
    close(thepipe[1]);
    execvp(av[1], &av[1]);
    perror("exec");
    exit(1);
  }

  close(thepipe[0]);
  while ((n = read(STDIN_FILENO, buf, sizeof(buf))) > 0) {
    if (write_all(STDOUT_FILENO, buf, n) == -1)
      exit(1);
    if (write_all(thepipe[1], buf, n) == -1)
      exit(1);
  }
  close(thepipe[1]);
  if (n == -1) {
    perror("read");
    exit(1);
  }
  wait(NULL);
}
