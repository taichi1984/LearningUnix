
/* pipedemo.c
 * 目的:パイプの作り方と使い方を示す。
 * 効果:パイプを作成し、書き込み側に出力し、読み出し側から読み出す。
 *     少々回りくどいが、パイプがどのようなものかはわかる。
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main() {
  int len, i, apipe[2]; /*2つのファイルディスクリプタ*/
  char buf[BUFSIZ];     /*読み出し側のためのバッファ*/

  /*パイプを作成する。*/

  if (pipe(apipe) == -1) {
    perror("could not make pipe");
    exit(1);
  }
  /*標準入力を読み出し、パイプに書き込み、パイプから読み出して出力する。*/

  while (fgets(buf, BUFSIZ, stdin)) {
    len = strlen(buf);
    if (write(apipe[1], buf, len) != len) {
      perror("writing to pipe");
      break; /*pipe*/
    }
    for (i = 0; i < len; i++)
      buf[i] = 'X';
    len = read(apipe[0], buf, BUFSIZ);
    if (len == -1) {
      perror("reading from pipe");
      break;
    }
    if (write(1, buf, len) != len) {
      perror("writing to stdout");
      break;
    }
  }
}
