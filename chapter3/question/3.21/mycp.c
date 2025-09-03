#define _POSIX_C_SOURCE 200809L
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define BUFFERSIZE 4096
#define COPYMODE 0644

void oops(char *, char *);

int main(int ac, char *av[]) {

  int in_fd, out_fd, n_chars;
  char buf[BUFFERSIZE];
  struct stat source;
  struct timespec source_times[2];
  struct timespec target_times[2];

  if (ac != 3) {
    fprintf(stderr, "usage: %s source desination\n", *av);
    exit(1);
  }

  if (stat(av[1], &source) == -1) {
    perror("stat");
    return 1;
  }

  source_times[0] = source.st_atim;
  source_times[1].tv_nsec = UTIME_OMIT;

  target_times[0].tv_nsec = UTIME_OMIT;
  target_times[1] = source.st_mtim;

  if ((in_fd = open(av[1], O_RDONLY)) == -1)
    oops("Cannot open ", av[1]);
  if ((out_fd = creat(av[2], COPYMODE)) == -1)
    oops("Cannot creat ", av[2]);

  while ((n_chars = read(in_fd, buf, BUFFERSIZE)) > 0)
    if (write(out_fd, buf, n_chars) != n_chars)
      oops("Write error to ", av[2]);

  if (n_chars == -1)
    oops("Read error from ", av[1]);

  futimens(in_fd, source_times);
  futimens(out_fd, target_times);

  if (close(in_fd) == -1 || close(out_fd) == -1)
    oops("Error closing files", "");
}

void oops(char *s1, char *s2) {
  fprintf(stderr, "Error : %s ", s1);
  perror(s2);
  exit(1);
}
