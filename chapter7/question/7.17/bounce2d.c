/* bounce2d 1.0
 * 文字(デフォルトは'Q')を画面の端で弾ませる。
 * 動きは複数のパラメータによって定義される。
 * ユーザー入力 :w ラケットを上に動かす
 *               s ラケットを下に動かす
 *               スペース　サーブを打つ（１回のみ）
 *               Q 終了
 *
 *               キーボード読み出しでブロックされるが、タイマチックはSIGALRMを送り、それはball_moveにキャッチされる。
 *
 * コンパイル方法: gcc bounce2d.c set_ticker.c -lcurses -o bounce2d
 */

#include "bounce.h"
#include <curses.h>
#include <signal.h>

struct ppball the_ball;
struct racket the_racket;
void set_up();
void wrap_up();
void set_ticker(int);
void ball_move(int);
int bounce_or_lose(struct ppball *bp, struct racket *rk);
/** メインループ **/

int main() {
  int c;
  int served = 0;
  set_up();

  while ((c = getchar()) != 'Q') {
    if (c == 'w') {
      if (the_racket.y_pos > TOP_ROW) {
        the_racket.y_pos--;
        mvaddch(the_racket.y_pos + 1, RIGHT_EDGE + 1, BLANK);
        mvaddch(the_racket.y_pos, RIGHT_EDGE + 1, the_racket.symbol);
        if (served == 0) {
          the_ball.y_pos = the_racket.y_pos;
          mvaddch(the_ball.y_pos + 1, the_ball.x_pos, BLANK);
          mvaddch(the_ball.y_pos, the_ball.x_pos, the_ball.symbol);
        }
      }
    } else if (c == 's') {
      if (the_racket.y_pos < BOT_ROW) {
        the_racket.y_pos++;
        mvaddch(the_racket.y_pos - 1, RIGHT_EDGE + 1, BLANK);
        mvaddch(the_racket.y_pos, RIGHT_EDGE + 1, the_racket.symbol);
        if (served == 0) {
          the_ball.y_pos = the_racket.y_pos;
          mvaddch(the_ball.y_pos - 1, the_ball.x_pos, BLANK);
          mvaddch(the_ball.y_pos, the_ball.x_pos, the_ball.symbol);
        }
      }
    } else if (c == ' ' && served == 0) {
      signal(SIGALRM, ball_move);
      set_ticker(1000 / TICKS_PER_SEC);
      served = 1;
    }
    refresh();
  }

  wrap_up();
}
void set_up_wall() {
  /*上側の壁*/
  for (int i = LEFT_EDGE; i < RIGHT_EDGE; i++)
    mvaddch(TOP_ROW - 1, i, UNDER_WALL_SYMBOL);
  /*下側の壁*/
  for (int i = LEFT_EDGE; i < RIGHT_EDGE; i++)
    mvaddch(BOT_ROW + 1, i, UNDER_WALL_SYMBOL);
  /*左側の壁*/
  for (int i = TOP_ROW; i <= BOT_ROW + 1; i++)
    mvaddch(i, LEFT_EDGE - 1, SIDE_WALL_SYMBOL);
  /*右側の壁、ラケットを置くために開ける。*/
  //  for (int i = TOP_ROW; i <= BOT_ROW + 1; i++)
  //   mvaddch(i, RIGHT_EDGE + 1, SIDE_WALL_SYMBOL);
}

void set_up()
/*
 * 構造体、その他のものを初期化する。
 */
{

  the_racket.y_pos = TOP_ROW + 2;
  the_racket.symbol = RACKET_SYMBOL;
  the_ball.y_pos = the_racket.y_pos;
  the_ball.x_pos = X_INIT;
  the_ball.y_ttg = the_ball.y_ttm = Y_TTM;
  the_ball.x_ttg = the_ball.x_ttm = X_TTM;
  the_ball.y_dir = 1;
  the_ball.x_dir = 1;
  the_ball.symbol = DFL_SYMBOL;

  initscr();
  noecho();
  crmode();

  signal(SIGINT, SIG_IGN);
  mvaddch(the_racket.y_pos, RIGHT_EDGE + 1, the_racket.symbol);
  mvaddch(the_ball.y_pos, the_ball.x_pos, the_ball.symbol);
  set_up_wall();

  refresh();
  // signal(SIGALRM, ball_move);
  // set_ticker(1000 / TICKS_PER_SEC);
}

void wrap_up() {
  set_ticker(0);
  endwin();
}
void racket_move(int c) {}

void ball_move(int signum) {
  int y_cur, x_cur, moved;

  signal(SIGALRM, SIG_IGN);

  y_cur = the_ball.y_pos;
  x_cur = the_ball.x_pos;
  moved = 0;

  if (the_ball.y_ttm > 0 && the_ball.y_ttg-- == 1) {
    the_ball.y_pos += the_ball.y_dir;
    the_ball.y_ttg = the_ball.y_ttm;
    moved = 1;
  }

  if (the_ball.x_ttm > 0 && the_ball.x_ttg-- == 1) {
    the_ball.x_pos += the_ball.x_dir;
    the_ball.x_ttg = the_ball.x_ttm;
    moved = 1;
  }

  if (moved) {
    // mvaddch(y_cur, x_cur, BLANK);
    mvaddch(y_cur, x_cur, BLANK);
    mvaddch(the_ball.y_pos, the_ball.x_pos, the_ball.symbol);
    if (bounce_or_lose(&the_ball, &the_racket) == -1) {
      move(BOT_ROW + 5, LEFT_EDGE + 5);
      addstr("LOST_BALL");
    }

    move(LINES - 1, COLS - 1);
    refresh();
  }
  signal(SIGALRM, ball_move);
}

int bounce_or_lose(struct ppball *bp, struct racket *rk) {
  int return_val = 0;
  if (bp->y_pos == TOP_ROW) {
    bp->y_dir = 1;
    return_val = 1;
  } else if (bp->y_pos == BOT_ROW) {
    bp->y_dir = -1;
    return_val = 1;
  }

  if (bp->x_pos == LEFT_EDGE) {
    bp->x_dir = 1;
    return_val = 1;
  } else if (bp->x_pos == RIGHT_EDGE) {
    if (bp->y_pos == rk->y_pos || bp->y_pos == rk->y_pos - 1 ||
        bp->y_pos == rk->y_pos + 1) { // 判定を甘めに設定
      bp->x_dir = -1;
      return_val = 1;
    } else {
      return_val = -1;
    }
  }

  return return_val;
}
