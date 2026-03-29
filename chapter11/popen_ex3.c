/* popen_ex3.c
 * popenを使って標準入力から読み出すプロセスに書き込みを行う。
 * このプログラムは二人のユーザーに電子メールを書く
 * fprintfを使って送信データを整形するのがいかに簡単かに注意してほしい。
 */

#include <stdio.h>

int main() {
  FILE *fp;
  fp = popen("mail admin backup", "w");
  fprintf(fp, "Error with backup!!\n");
  pclose(fp);
}
