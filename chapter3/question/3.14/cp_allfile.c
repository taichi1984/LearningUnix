#include <dirent.h>
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
  struct stat source;
  struct stat target;
  struct dirent *entry;
  DIR *dp;

  if (ac != 3) {
    fprintf(stderr, "usage: %s source desination\n", *av);
    exit(1);
  }

  if (stat(av[1], &source) == -1) {
    oops("Cannot stat ", av[1]);
  }

  if (stat(av[2], &target) == -1) {
    oops("Cannot stat ", av[2]);
  }

  if (S_ISDIR(source.st_mode) && S_ISDIR(target.st_mode)) {
    dp = opendir(av[1]);
    if (dp == NULL)
      oops("Can't open dir", av[1]);

    while ((entry = readdir(dp)) != NULL) {
      struct stat checkstat;

      char src_path[255] = "";
      snprintf(src_path, sizeof(src_path), "%s/%s", av[1], entry->d_name);

      if (stat(src_path, &checkstat) == -1) {
        oops("Cannot stat ", entry->d_name);
      }

      if (S_ISDIR(checkstat.st_mode) || strcmp(entry->d_name, ".") == 0 ||
          strcmp(entry->d_name, "..") == 0)
        continue;

      if ((in_fd = open(src_path, O_RDONLY)) == -1)
        oops("Cannot open ", av[1]);

      char out_path[255] = "";
      snprintf(out_path, sizeof(out_path), "%s/%s", av[2], entry->d_name);

      if ((out_fd = creat(out_path, COPYMODE)) == -1)
        oops("Cannot creat ", av[2]);

      while ((n_chars = read(in_fd, buf, BUFFERSIZE)) > 0)
        if (write(out_fd, buf, n_chars) != n_chars)
          oops("Write error to ", av[2]);

      if (n_chars == -1)
        oops("Read error from ", av[1]);

      if (close(in_fd) == -1 || close(out_fd) == -1)
        oops("Error closing files", "");
    }

    if (closedir(dp) == -1)
      oops("Error closing directory", "");
    return 0;
  }

  if ((in_fd = open(av[1], O_RDONLY)) == -1)
    oops("Cannot open ", av[1]);
  if ((out_fd = creat(av[2], COPYMODE)) == -1)
    oops("Cannot creat ", av[2]);

  while ((n_chars = read(in_fd, buf, BUFFERSIZE)) > 0)
    if (write(out_fd, buf, n_chars) != n_chars)
      oops("Write error to ", av[2]);

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
