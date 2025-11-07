/* boucne.h */

/* ゲームのための設定 */

#define BLANK ' '
#define DFL_SYMBOL 'o'
#define TOP_WALL_SYMBOL '-'
#define UNDER_WALL_SYMBOL '_'
#define SIDE_WALL_SYMBOL '|'
#define RACKET_SYMBOL 'R'

#define TOP_ROW 5
#define BOT_ROW 20
#define LEFT_EDGE 10
#define RIGHT_EDGE 70
#define X_INIT RIGHT_EDGE - 1
#define Y_INIT 10
#define TICKS_PER_SEC 50

#define X_TTM 5
#define Y_TTM 8

/** ピンポン玉 **/

struct ppball{
  int y_pos, x_pos,
      y_ttm, x_ttm,
      y_ttg, x_ttg,
      y_dir, x_dir;
      char symbol;
  };

struct racket{
  int y_pos,size;
  char symbol;
};

