
#include <ctype.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#define ASK "Do you want another transaction?"
#define TRIES 3     // 試行回数の上限
#define SLEEPTIME 2 // １回の施行にかける時間
#define BEEP putchar('\a')

int get_response(char *, int maxtries);
void set_cr_noecho_mode();
void tty_mode(int how);
void set_nodelay_mode();
int get_ok_char();
void ignore_signal();

int main() {
  int response;

  tty_mode(0);
  set_cr_noecho_mode();
  set_nodelay_mode();
  ignore_signal();
  response = get_response(ASK, TRIES);
  tty_mode(1);
  return response;
}
void ignore_signal() {
  signal(SIGINT, SIG_IGN);
  signal(SIGTSTP, SIG_IGN);
  signal(SIGQUIT, SIG_IGN);
}

int get_response(char *question, int maxtries) {
  int input;
  printf("%s (y/n)?", question);
  fflush(stdout);
  while (1) {
    sleep(SLEEPTIME);
    input = tolower(get_ok_char());
    if (input == 'y')
      return 0;
    if (input == 'n')
      return 1;
    if (maxtries-- == 0)
      return 2;
    BEEP;
  }
}

int get_ok_char() {
  int c;
  while ((c = getchar()) != EOF && strchr("yYnN", c) == NULL)
    ;
  return c;
}

void set_cr_noecho_mode() {
  struct termios ttystate;

  tcgetattr(0, &ttystate);
  ttystate.c_lflag &= ~ICANON;
  ttystate.c_lflag &= ~ECHO;
  ttystate.c_cc[VMIN] = 1;
  tcsetattr(0, TCSANOW, &ttystate);
}

void set_nodelay_mode() {
  int termflags;
  termflags = fcntl(0, F_GETFL);
  termflags |= O_NDELAY;
  fcntl(0, F_SETFL, termflags);
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
