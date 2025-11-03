#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <termios.h>

void set_noecho_nocan();
int tty_mode(int how);
void fill_it(int signum);
static int character = 'A';
int main(void) {
  tty_mode(0);
  set_noecho_nocan();
  signal(SIGWINCH, fill_it);
  int c;
  fill_it(0);
  while (1) {
    c = getchar();
    if (c == 'Q' || c == 'q') {
      printf("\033[2J\033[H");
      break;
    } else {
      character = 'A';
      fill_it(0);
    }
  }

  tty_mode(1);
}

void set_noecho_nocan() {
  struct termios ttystate;
  tcgetattr(0, &ttystate);
  ttystate.c_lflag &= ~ECHO;
  ttystate.c_lflag &= ~ICANON;
  tcsetattr(0, TCSANOW, &ttystate);
}

int tty_mode(int how) {
  static struct termios original_mode;
  if (how == 0) {
    tcgetattr(0, &original_mode);
    return 0;
  } else
    return tcsetattr(0, TCSANOW, &original_mode);
}
void fill_it(int signum) {
  struct winsize wbuf;

  if (ioctl(0, TIOCGWINSZ, &wbuf) == -1) {
    perror("ioctl:TIOCGWINSZ");
    tty_mode(1);
    exit(1);
  }

  printf("\033[2J\033[H");
  for (int i = 0; i < wbuf.ws_row; i++) {
    for (int j = 0; j < wbuf.ws_col; j++) {
      putchar(character);
    }
    putchar('\n');
  }
  character++;
}
