#include <alloca.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

int main() {
  void f(int);
  int i;

  signal(SIGINT, f);
  for (i = 0; i < 20; i++) {
    printf("hello\n");
    sleep(1);
  }
  return 0;
}

void f(int signum) {

  int c;
  printf("    Interrupted! OK to Quit(y/n)?");
  c = getchar();

  if (c == 'y' || c == 'Y') {
    exit(2);
  }
}
