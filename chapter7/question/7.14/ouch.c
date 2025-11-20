#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int num = 0;
int print_flag = 0;

int main(int ac, char *av[]) {
  if (ac != 3) {
    fprintf(stderr, "usage : ouch [message] [times] ");
  }

  void f(int);

  int times = atoi(av[2]);

  signal(SIGINT, f);
  while (1) {
    pause();
    if (num % times == 0) {
      printf("%s\n", av[1]);
    }
  }
}

void f(int signum) { num++; }
