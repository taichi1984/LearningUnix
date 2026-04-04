#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct {
  bool flag_a;
  bool flag_b;
  bool flag_c;
} Options;

int main(int ac, char *av[]) {
  int c;
  Options opt = {0};
  while ((c = getopt(ac, av, "abc")) != -1) {
    switch (c) {
    case 'a':
      opt.flag_a = true;
      break;
    case 'b':
      opt.flag_b = true;
      break;
    case 'c':
      opt.flag_c = true;
      break;
    case '?':
      exit(1);
    }
  }

  if (opt.flag_a) {
    printf("flag_a is true\n");
  }
  if (opt.flag_b) {
    printf("flag_b is true\n");
  }
  if (opt.flag_c) {
    printf("flag_c is true\n");
  }


  return 0;
}
