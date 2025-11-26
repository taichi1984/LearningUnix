#include <curses.h>
#include <signal.h>
#include <stdatomic.h>
#include <stdio.h>
#include <sys/time.h>

int x = 0;
int y = 0;
int main(void) {
  int ch;
  struct itimerval timer;
  struct sigaction sa;
  timer.it_interval.tv_sec = 3;
  timer.it_interval.tv_usec = 100;
  timer.it_value.tv_sec = 3;
  timer.it_value.tv_usec = 1000;

  void sigint_handler(int);
  sa.sa_handler = sigint_handler;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;
  sigaction(SIGALRM, &sa, NULL);
  setitimer(ITIMER_REAL, &timer, NULL);

  initscr();
  clear();

  while ((ch = getch()) != 'Q') {
    if (ch == '\n') {
      mvaddch(y, x, ch);
      y++;
      x = 0;
    } else if (ch == EOF) {
      y++;
      x = 0;
      mvaddstr(y, x, "interrupted\n");
    } else {
      mvaddch(y, x, ch);
      x++;
    }
  }
  endwin();
}

void sigint_handler(int signum) {
  y++;
  x = 0;
  addstr("interrupting\n");
}
