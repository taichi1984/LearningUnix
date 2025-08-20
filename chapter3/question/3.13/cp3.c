#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define BUFFERSIZE 4096
#define COPYMODE 0644

void oops(char *, char *);

int main(int ac, char *av[]) {

  int in_fd, out_fd, n_chars;
  char buf[BUFFERSIZE];
  struct stat target;
  char out_path[255] = "";

  if (ac != 3) {
    fprintf(stderr, "usage: %s source desination\n", *av);
    exit(1);
  }

  if ((in_fd = open(av[1], O_RDONLY)) == -1)
    oops("Cannot open ", av[1]);

  if (stat(av[2], &target) != -1) {
    if (S_ISDIR(target.st_mode)) {
      strcat(out_path, av[2]);
      strcat(out_path, "/");
      strcat(out_path, av[1]);
    } else {
      strcat(out_path, av[2]);
    }
  } else {
    strcat(out_path, av[2]);
  }

  if ((out_fd = creat(out_path, COPYMODE)) == -1)
    oops("Cannot creat ", out_path);

  while ((n_chars = read(in_fd, buf, BUFFERSIZE)) > 0)
    if (write(out_fd, buf, n_chars) != n_chars)
      oops("Write error to ", out_path);

  if (n_chars == -1)
    oops("Read error from ", av[1]);

  if (close(in_fd) == -1 || close(out_fd) == -1)
    oops("Error closing files", "");
}

void oops(char *s1, char *s2) {
  fprintf(stderr, "Error : %s ", s1);
  perror(s2);
  exit(1);
}
