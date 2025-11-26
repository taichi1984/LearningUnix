#include <alloca.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

void set_isig_off();

int main() {
  void f(int);
  int i;
  set_isig_off();

  signal(SIGINT, f);
  for (i = 0; i < 20; i++) {
    printf("hello\n");
    sleep(1);
  }
  return 0;
}

void set_isig_off() {
  struct termios ttystate;

  tcgetattr(0, &ttystate);
  ttystate.c_lflag &= ~ISIG;
  tcsetattr(0, TCSANOW, &ttystate);
}

void f(int signum) {

  int c;
  printf("    Interrupted! OK to Quit(y/n)?");
  c = getchar();

  if (c == 'y' || c == 'Y') {
    exit(2);
  }
}
