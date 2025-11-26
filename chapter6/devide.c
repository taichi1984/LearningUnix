#include <stdio.h>
#include <stdlib.h>

int main(int ac, char *av[]) {

  if (ac != 3) {
    fprintf(stderr, "コマンドライン引数は２つ取る必要があります。");
    exit(1);
  }

  int answer = (int)atof(av[1]) / (int)atof(av[2]);

  printf("answer : %d", answer);
  return 0;
}
