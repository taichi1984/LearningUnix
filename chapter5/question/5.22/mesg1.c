#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int ac, char *av[]) {

  char *ttypath = ttyname(STDIN_FILENO);
  char ans;

  if (ac != 2) {
    fprintf(stderr, "引数の数が違います\n");
    exit(1);
  }

  ans = av[1][0];

  if (ans == 'y') {
    if (chmod(ttypath, 0622) == -1) {
      perror("chmod failed");
      return 1;
    }

  } else {
    if (chmod(ttypath, 0600) == -1) {
      perror("chmod failed");
      return 1;
    }
  }

  return 0;
}
