/* write0.c
 * 目的：他の端末にメッセージを送る。
 * 方法：他の端末を書き込みようにオープンし、標準入力をその端末にコピーする。
 * 学習ポイント: 端末は、通常の入出力をサポートするファイルにすぎないこと
 * 使い方 : write0 ttyname
 */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int ac, char *av[]) {
  int fd;
  char buf[BUFSIZ];

  /*引数をチェックする*/
  if (ac != 2) {
    fprintf(stderr, "usage: write0 ttyname\n");
    exit(1);
  }

  /* デバイスをオープンする。*/

  fd = open(av[1], O_WRONLY);
  if (fd == -1) {
    perror(av[1]);
    exit(1);
  }

  /* 入力にEOFが現れるまでループ */
  while (fgets(buf, BUFSIZ, stdin) != NULL)
    if (write(fd, buf, strlen(buf)) == -1)
      break;
  close(fd);
}
