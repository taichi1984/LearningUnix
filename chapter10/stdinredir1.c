/* stdinredir1.c
 * 目的:ファイルデスクリプタ0を端末からファイルとの接続に置き換えて、
 * 標準入力をリダイレクトする方法を示す。
 * 操作内容：標準入力の３行分のデータを読み出してからfd 0をクローズし、
 * ディスクファイルをオープンしてから、さらに標準入力を３行読み出す。
 */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
  int fd;
  char line[100];

  /* 3行読んで出力する */

  fgets(line, 100, stdin);
  printf("%s", line);
  fgets(line, 100, stdin);
  printf("%s", line);
  fgets(line, 100, stdin);
  printf("%s", line);

  /* 入力をリダイレクトする */

  close(0);
  fd = open("/etc/passwd", O_RDONLY);
  if (fd != 0) {
    fprintf(stderr, "Could not open data as fd 0\n");
    exit(1);
  }

  /* 3行を読んで出力する。 */

  fgets(line, 100, stdin);
  printf("%s", line);
  fgets(line, 100, stdin);
  printf("%s", line);
  fgets(line, 100, stdin);
  printf("%s", line);
}
