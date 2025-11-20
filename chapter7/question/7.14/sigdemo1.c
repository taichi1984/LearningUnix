#include <signal.h>
#include <stdio.h>
#include <unistd.h>

int num = 0;

int main() {
  void f(int);

  int i;

  signal(SIGINT, f);
  while (1) {
    pause();
  }
}

void f(int signum) {
  if (num % 10 == 0)
    printf("OUCH!\n");
}
