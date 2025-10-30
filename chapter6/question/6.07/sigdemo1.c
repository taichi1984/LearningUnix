#include <alloca.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

static int called = 0;
static int max_called = 0;
void set_noecho() {
  struct termios ttystate;
  tcgetattr(0, &ttystate);
  ttystate.c_lflag &= ~ECHO;
  tcsetattr(0, TCSANOW, &ttystate);
}

int main(int ac, char *av[]) {
  void f(int);
  int i;
  set_noecho();
  if (ac != 2) {
    fprintf(stderr, "引数の数が正しくありません\n");
    exit(2);
  } else if (atoi(av[1]) < 1) {
    fprintf(stderr, "入力される値は1以上の整数である必要があります。\n");
    exit(2);
  }

  max_called = atoi(av[1]);

  signal(SIGINT, f);
  for (i = 0; i < 20; i++) {
    printf("hello\n");
    sleep(1);
  }
  return 0;
}

void f(int signum) {
  called++;

  if (max_called == called) {
    exit(2);
  }
  printf("OUCH");
  for (int i = 0; i < called; i++) {
    putchar('!');
  }
  putchar('\n');
  signal(SIGINT, f);
}
