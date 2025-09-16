#include <stdio.h>
#include <unistd.h>

int do_rename(char *from, char *to) {
  if (link(from, to) != -1) {
    if (unlink(from) == -1) {
      perror("unlink");

      if (unlink(to) == -1) {
        perror("unlink");
      }

      return -1;
    }
  } else {
    perror("link");
    return -1;
  }
  return 0;
}

int main(int ac, char *av[]) {
  if (ac == 3) {
    do_rename(av[1], av[2]);
  } else {
    fprintf(stderr, "引数が正しくありません\n");
  }

  return 0;
}
