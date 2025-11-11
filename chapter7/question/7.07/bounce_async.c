/* bounce_async.c
 * 目的 : ユーザー制御のアニメーション、　fdに対してO_ASYNCを指定する
 * 注意 : set_ticker()はSIGALRMを送り、ハンドラはアニメーションを実現する。
 * キーボードはSIGIOを送り、mainはpause()を呼び出すだけである。
 * コンパイル : gcc bounce_async.c set_ticker.c -l ncurses -o bounce_async
 *
 */

#include <curses.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

/* ゲームの状態 */

#define MESSAGE "hello"
#define BLANK "     "

int row = 10;
int col = 0;
int dir = 1;
int delay = 200;
int done = 0;
void set_ticker(int);

int main(void) {
  void on_alarm(int);
  void on_input(int);
  void enable_kbd_signals();

  initscr();
  crmode();
  noecho();
  clear();

  signal(SIGIO, on_input);
  enable_kbd_signals();
  signal(SIGALRM, on_alarm);
  set_ticker(delay);

  move(row, col);
  addstr(MESSAGE);

  while (!done)
    pause();

  endwin();
}

void on_input(int signum) {
  int c = getch();
  sleep(3);
  if (c == 'Q' || c == EOF)
    done = 1;
  else if (c == ' ')
    dir = -dir;
}

void on_alarm(int signum) {
  signal(SIGALRM, on_alarm);
  mvaddstr(row, col, BLANK);
  col += dir;
  mvaddstr(row, col, MESSAGE);
  refresh();

  if (dir == -1 && col <= 0)
    dir = 1;
  else if (dir == 1 && col + strlen(MESSAGE) >= COLS)
    dir = -1;
}

void enable_kbd_signals() {
  int fd_flags;
  fcntl(0, F_SETOWN, getpid());
  fd_flags = fcntl(0, F_GETFL);
  fcntl(0, F_SETFL, (fd_flags | O_ASYNC));
}
