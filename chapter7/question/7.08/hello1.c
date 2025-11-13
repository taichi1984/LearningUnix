/* hello1.c
 * 目的: cursesを使うために必要な最小限の呼び出しを示す。
 *       インターバルタイマを使って、表示を点滅させる。
 * 概要: 初期化、描画、入力待ち、終了
 */

#include <curses.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

int visible = 0;
int set_ticker(int);
void check_visible();

int main(int ac, char *av[]) {

  char *message;
  size_t len;

  if (ac == 1) {
    message = "Hello world";
  } else if (ac == 2) {
    message = av[1];
  } else {
    fprintf(stderr, "引数が正しくありません\n");
    exit(2);
  }

  len = strlen(message);
  char blank[len + 1];
  memset(blank, ' ', len);
  blank[len] = '\0';

  initscr();
  curs_set(0);
  clear();
  signal(SIGALRM, check_visible);

  set_ticker(100);
  while (1) {
    pause();
    if (visible == 0) {
      move(10, 20);
      addstr(message);
      set_ticker(2000); // 点滅の速さを調整。一定速度で点滅させたい場合は
                        // if(visible == 0)のset_ticker(300)と一緒に削除する。
    } else {
      move(10, 20);
      addstr(blank);
      set_ticker(300); // 点滅の速さを調整。一定速度で点滅させたい場合は
                       // if(visible == 0)のset_ticker(2000)と一緒に削除する。
    }

    refresh();
  }

  endwin();
}

int set_ticker(int n_msecs) {
  struct itimerval new_timeset;
  long n_sec, n_usecs;

  n_sec = n_msecs / 1000;
  n_usecs = (n_msecs % 1000) * 1000L;

  new_timeset.it_interval.tv_sec = n_sec;
  new_timeset.it_interval.tv_usec = n_usecs;

  new_timeset.it_value.tv_sec = n_sec;
  new_timeset.it_value.tv_usec = n_usecs;

  return setitimer(ITIMER_REAL, &new_timeset, NULL);
}

void check_visible() {
  if (visible == 0) {
    visible = 1;
  } else {
    visible = 0;
  }
}
