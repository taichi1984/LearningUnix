#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
/*
 *  testbdp.c - 双方向パイプをテストする。
 *
 */

int main() {
  int p[2];

  if (pipe(p) == -1)
    exit(1);
  if (write(p[0], "hello", 5) == -1)
    perror("write into pipe[0] failed");
  else
    printf("write into pipe[0] worked\n");
}
