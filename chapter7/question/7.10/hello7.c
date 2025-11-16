/* hello7
 * 目的: メッセージを画面内で跳ね返らせる。
 * 　　　左から右に落下するようなイメージにし、右端に到達したら文字をバラバラにする。
 * 概要: 初期化、描画、入力待ち、終了
 */

#include <curses.h>
#include <stdio.h>
#include <unistd.h>

#define LEFTEDGE 10
#define RIGHTEDGE 100
#define ROW 20

int main() {
  char message[] = "Hello";
  char blank[] = "     ";
  int dir = +1;
  int pos = LEFTEDGE;
  int sleep_time = (RIGHTEDGE - pos) * 200;
  int break_flag = 0;
  int after_break_time = 0;
  char message_even[6] = "";
  char message_odd[6] = "";
  initscr();
  clear();

  while (1) {
    if (break_flag == 0) {
      move(ROW, pos);
      addstr(message);
      move(LINES - 1, COLS - 1);
      refresh();
      usleep(sleep_time);
      move(ROW, pos);
      addstr(blank);
      pos += dir;

      sleep_time = (100 - pos) * 500;
      if (pos >= RIGHTEDGE) {
        break_flag = 1;

        for (int i = 0; i < 6 - 1; i++) {
          if (i % 2 == 0) {
            message_even[i] = message[i];
            message_odd[i] = ' ';
          } else {
            message_odd[i] = message[i];
            message_even[i] = ' ';
          }

          message_even[5] = '\0';
          message_odd[5] = '\0';

          move(ROW, pos);
        }
      }
    } else {
      mvaddstr(ROW - after_break_time, pos, message_even);
      mvaddstr(ROW + after_break_time, pos, message_odd);
      after_break_time++;
      refresh();
      mvaddstr(ROW - after_break_time + 1, pos, blank);
      mvaddstr(ROW + after_break_time - 1, pos, blank);

      usleep(60000);
    }
  }
}
