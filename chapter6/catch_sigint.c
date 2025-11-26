#define _POSIX_C_SOURCE 200809L // ← これを最初に置く
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void ignore_sigint(int signum) { printf("SIGINTを無視します\n"); }

int main(void) {

  int i;
  signal(SIGINT, SIG_IGN);
  for (int i = 0; i < 10; i++) {
    printf("%d \n", i);
    sleep(1);
  }

  return 0;
}
