/* hello5.c
 * 目的: メッセージを画面内で跳ね返らせる。
 * 概要: 初期化、描画、入力待ち、終了
 */

#include <curses.h>
#include <stdio.h>
#include <unistd.h>

#define LEFTEDGE 10
#define RIGHTEDGE 100
#define ROW 10

int main() {
  char message[] = "Hello";
  char blank[] = "     ";
  int dir = +1;
  int pos = LEFTEDGE;

  initscr();
  clear();

  while (1) {
    move(ROW, pos);
    addstr(message);
    move(LINES - 1, COLS - 1);
    refresh();
    usleep(20000);
    move(ROW, pos);
    addstr(blank);
    pos += dir;

    if (pos >= RIGHTEDGE)
      dir = -1;
    if (pos <= LEFTEDGE)
      dir = +1;
  }
}
