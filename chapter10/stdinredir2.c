/* stdinredir2.c
 * 標準入力を使ってリダイレクトするための２種類の方法を示す。
 * #defineを使ってどちらを使うか指定する。
 */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*#define CLOSE_DUP /*オープン、クローズ、複製、クローズ*/
/* #define USE_DUP2 /* オープン、複製2,クローズ*/

int main() {
  int fd;
  int newfd;
  char line[100];

  /*3行読んで出力する*/

  fgets(line, 100, stdin);
  printf("%s", line);
  fgets(line, 100, stdin);
  printf("%s", line);
  fgets(line, 100, stdin);
  printf("%s", line);

  /* 入力をリダイレクトする。*/
  fd = open("data", O_RDONLY);
#ifdef CLOSE_DUP
  close(0);
  newfd = dup(fd);
#else
  newfd = dup2(fd, 0);
#endif
  if (newfd != 0) {
    fprintf(stderr, "Could not duplicate fd to 0\n");
    exit(1);
  }

  close(fd);

  /*3行読んで出力する*/

  fgets(line, 100, stdin);
  printf("%s", line);
  fgets(line, 100, stdin);
  printf("%s", line);
  fgets(line, 100, stdin);
  printf("%s", line);
}
