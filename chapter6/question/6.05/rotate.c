#include <ctype.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>

void tty_mode(int how);
void set_cr_noecho_mode();
void signal_setting();
void keyboard_signal_handler(int);

int main(void) {

  tty_mode(0);
  set_cr_noecho_mode();
  signal_setting();

  int c;
  while ((c = getchar()) != EOF) {
    if (c == 'Q') {
      tty_mode(1);
      break;
    } else if (c == 'z')
      c = 'a';
    else if (islower(c))
      c++;
    putchar(c);
  }
  return 0;
}

void tty_mode(int how) {
  static struct termios original_mode;
  static int original_flags;
  if (how == 0) {
    tcgetattr(0, &original_mode);
    original_flags = fcntl(0, F_GETFL);
  } else {
    tcsetattr(0, TCSANOW, &original_mode);
    fcntl(0, F_SETFL, original_flags);
  }
}

void set_cr_noecho_mode() {
  struct termios ttystate;

  tcgetattr(0, &ttystate);
  ttystate.c_lflag &= ~ICANON;
  ttystate.c_lflag &= ~ECHO;
  ttystate.c_cc[VMIN] = 1;
  tcsetattr(0, TCSANOW, &ttystate);
}

void signal_setting() {
  signal(SIGINT, keyboard_signal_handler);
  signal(SIGQUIT, keyboard_signal_handler);
  signal(SIGTSTP, keyboard_signal_handler);
}

void keyboard_signal_handler(int signum) {
  {
    tty_mode(1);
    exit(2);
  }
}
