#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>

int main(void) {
  struct winsize w;
  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1) {
    perror("ioctl");
    return 1;
  }
  printf("Rows : %d, Cols %d \n", w.ws_row, w.ws_col);
  return 0;
}
