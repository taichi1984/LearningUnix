/* hello6
 * 目的: メッセージを画面内で跳ね返らせる。
 * 　　　画面中央にきたら移動を早く、端に近くなったら遅くする。
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
  int sleep_time = (RIGHTEDGE - pos) * 200;
  initscr();
  clear();

  while (1) {
    move(ROW, pos);
    addstr(message);
    move(LINES - 1, COLS - 1);
    refresh();
    usleep(sleep_time);
    move(ROW, pos);
    addstr(blank);
    pos += dir;

    if (pos >= RIGHTEDGE)
      dir = -1;
    if (pos <= LEFTEDGE)
      dir = +1;

    if (pos < (RIGHTEDGE + LEFTEDGE) / 2) {
      sleep_time = (100 - pos) * 500;
    } else {
      sleep_time = (pos) * 500;
    }
  }
}
