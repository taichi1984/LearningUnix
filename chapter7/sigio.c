#define _GNU_SOURCE
#include <fcntl.h>
#include <signal.h>
#include <termios.h>
#include <unistd.h>

void get_key(int signum) {
  char c;
  int n = read(STDIN_FILENO, &c, 1);
  write(STDOUT_FILENO, &c, 1);
}

void tty_setup() {
  struct termios t;
  signal(SIGIO, get_key);
  tcgetattr(STDIN_FILENO, &t);
  t.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &t);

  fcntl(STDIN_FILENO, F_SETOWN, getpid());
  int flags = fcntl(STDIN_FILENO, F_GETFL);
  fcntl(STDIN_FILENO, F_SETFL, flags | O_ASYNC);
}

int main(void) {
  tty_setup();

  while (1) {
    pause();
  }
}
