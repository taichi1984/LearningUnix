/* hello2.c
 * 目的: cursesを使うために必要な最小限の呼び出しを示す。
 * 概要: 初期化、描画、入力待ち、終了
 */

#include <curses.h>
#include <stdio.h>
#include <unistd.h>

int main() {
  int i;
  initscr();
  clear();

  for (i = 0; i < LINES; i++) {
    move(i, i + i);
    if (i % 2 == 1)
      standout();
    addstr("Hello world");
    if (i % 2 == 1)
      standend();
  }
  refresh();
  getch();
  endwin();
}
