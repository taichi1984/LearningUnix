#define _GNU_SOURCE
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <sys/types.h>
#include <unistd.h>

int main(int ac, char *av[]) {
  if (ac == 2) {
    if (mknod(av[1], S_IFCHR, makedev(13, 63)) == -1) {
      perror("mknod");
      exit(1);
    }

  } else {
    fprintf(stderr, "引数が不正です。\n");
  }
  return 0;
}
