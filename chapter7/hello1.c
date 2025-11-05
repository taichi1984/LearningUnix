/* hello1.c
 * 目的: cursesを使うために必要な最小限の呼び出しを示す。
 * 概要: 初期化、描画、入力待ち、終了
 */

#include <curses.h>
#include <stdio.h>

int main() {

  initscr();

  clear();
  move(10, 20);
  addstr("Hello world");
  move(LINES - 1, 0);

  refresh();
  getch();

  endwin();
}
