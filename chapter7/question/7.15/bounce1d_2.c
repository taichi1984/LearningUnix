#include <curses.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>

/* mainとハンドラが使うグローバル設定 */

#define MESSAGE "hello"
#define BLANK "     "
#define LEFTEDGE 10
#define RIGHTEDGE 100
#define ROW 10

int dir = +1;
int main() {

  int pos = LEFTEDGE;
  int fd = STDIN_FILENO;
  void change_dir(int signum);

  fcntl(fd, F_SETOWN, getpid());
  fcntl(fd, F_SETFL, O_ASYNC | O_NONBLOCK);

  initscr();
  crmode();
  noecho();
  clear();

  signal(SIGIO, change_dir);

  while (1) {
    move(ROW, pos);
    addstr(MESSAGE);
    refresh();
    usleep(20000);
    move(ROW, pos);
    addstr(BLANK);

    pos += dir;

    if (pos >= RIGHTEDGE)
      dir = -1;
    if (pos <= LEFTEDGE)
      dir = +1;
  }

  endwin();
  return 0;
}

void change_dir(int signum) {
  char key;
  read(STDIN_FILENO, &key, 1);
  if (key == ' ') {
    dir = -dir;
  }
}
