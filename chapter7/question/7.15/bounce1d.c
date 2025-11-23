#include <curses.h>
#include <signal.h>
#include <string.h>

/* mainとハンドラが使うグローバル設定 */

#define MESSAGE "hello"
#define BLANK "     "
int set_ticker(int n_msecs);

int row; // 現在の行
int col; // 現在の列
int dir; // 現在の方向

int main() {
  int delay;          /* 大きければ大きいほど遅い */
  int ndelay;         /*新しい遅延*/
  int c;              /*ユーザー入力*/
  void move_msg(int); /*タイマハンドラ*/

  initscr();
  crmode();
  noecho();
  clear();

  row = 10;
  col = 0;
  dir = 1;
  delay = 200;

  move(row, col);
  addstr(MESSAGE);
  signal(SIGALRM, move_msg);
  set_ticker(delay);

  while (1) {
    ndelay = 0;
    c = getch();
    if (c == 'Q')
      break;
    if (c == ' ')
      dir = -dir;
    if (c == 'f' && delay > 2)
      ndelay = delay / 2;
    if (c == 's')
      ndelay = delay * 2;
    if (ndelay > 0)
      set_ticker(delay = ndelay);
  }
  endwin();
  return 0;
}

void move_msg(int signum) {
  signal(SIGALRM, move_msg);
  move(row, col);
  addstr(BLANK);
  col += dir;
  move(row, col);
  addstr(MESSAGE);
  refresh();

  /*
   *  境界条件を処理する
   */

  if (dir == -1 && col <= 0)
    dir = 1;
  else if (dir == 1 && col + strlen(MESSAGE) >= COLS)
    dir = -1;
}
