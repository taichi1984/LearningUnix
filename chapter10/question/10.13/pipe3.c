/* pipe3.c
 * 目的：1つのプロセスから別のプロセスにデータを流す。
 * 　　　流されたプロセスはさらに別のプロセスにデータを流す。
 * パイプラインの作り方を示す。引数として２つのコマンドを取り、av[1]の出力をav[2]の出力に接続する。
 *
 *  使い方 pipe command1 command2 command3
 *  効果 : command 1 | command2 | command3
 *  制限 : コマンドは引数を取ることができない。
 *  引数の個数がわかっているのでexeclp()を使う。
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#define oops(m, x)                                                             \
  {                                                                            \
    perror(m);                                                                 \
    exit(x);                                                                   \
  }

int main(int ac, char **av) {
  int thepipe1[2];
  int thepipe2[2];
  int newfd;
  int pid1;
  int pid2;
  int pid3;

  if (ac != 4) {
    fprintf(stderr, "usage:pipe cmd1 cmd2\n");
    exit(1);
  }
  if (pipe(thepipe1) == -1)
    oops("Cannot get a pipe", 1);

  if (pipe(thepipe2) == -1)
    oops("Cannot get a pipe", 1);

  /* ----------------------------------------------------------
   * パイプを作ったのでプロセスを２つにする */

  /* -----------------------------------------------------------
   * この時点でプロセスは２個ある。親はパイプから読み出す*/

  /* 子はav[1]を実行し、パイプに書き込む*/
  if ((pid1 = fork()) == 0) {
    if (dup2(thepipe1[1], 1) == -1)
      oops("could not redirect stdout", 4);

    close(thepipe1[0]); /*子はパイプから読み出さない*/
    close(thepipe2[0]);
    close(thepipe2[1]); // 子1はパイプ２を使わない
    close(thepipe1[1]); /*標準出力の複製を作り、パイプをクローズする。*/
    execlp(av[1], av[1], NULL);
    oops(av[1], 5);
  }

  /* 子はav[2]を実行し、パイプに書き込む*/
  if ((pid2 = fork()) == 0) {

    if (dup2(thepipe1[0], 0) == -1)
      oops("could not redirect stdout", 4);

    if (dup2(thepipe2[1], 1) == -1)
      oops("could not redirect stdout", 4);

    close(thepipe1[1]); /*子はパイプ１へ書き出さない*/
    close(thepipe2[0]); /*子はパイプ２から読み出さない*/
    close(thepipe1[0]); /*標準出力の複製を作り、パイプをクローズする。*/
    close(thepipe2[1]); /*標準入力の複製を作り、パイプをクローズする。*/

    execlp(av[2], av[2], NULL);
    oops(av[2], 5);
  }

  /* 子3はav[3]を実行し、パイプに書き込む*/
  if ((pid3 = fork()) == 0) {

    if (dup2(thepipe2[0], 0) == -1)
      oops("could not redirect stdout", 4);

    close(thepipe1[0]);
    close(thepipe1[1]); // 子３はpipe1は使わない。
    close(thepipe2[1]); /*子３はパイプ2へ書き出さない*/
    close(thepipe2[0]); /*標準入力の複製を作り、パイプをクローズする。*/
    execlp(av[3], av[3], NULL);
    oops(av[3], 5);
  }

  close(thepipe1[0]);
  close(thepipe1[1]);
  close(thepipe2[0]);
  close(thepipe2[1]);
  while (wait(NULL) > 0)
    ;

  return 0;
}
