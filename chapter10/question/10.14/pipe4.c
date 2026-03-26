/* pipe4.c
 * 目的：1つのプロセスから別のプロセスにデータを流す。
 * 　　　流されたプロセスはさらに別のプロセスにデータを流す。
 * パイプラインの作り方を示す。引数として２つのコマンドを取り、av[1]の出力をav[2]の出力に接続する。
 *
 *  使い方 pipe4 command1 command2 command3
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

void close_pipes(int ac, int thepipe[][2]) {
  for (int i = 0; i < ac; i++) {
    close(thepipe[i][0]);
    close(thepipe[i][1]);
  }
}

int main(int ac, char **av) {
  int n = ac - 1;
  int thepipe[n - 1][2];

  if (ac < 3) {
    fprintf(stderr, "usage : pipe4 command1 , command2 , command3 ...\n");
    fprintf(stderr, "引数には２つ以上のコマンドを指定する必要があります。\n");
    exit(1);
  }

  for (int i = 0; i < n - 1; i++) {
    if ((pipe(thepipe[i])) == -1)
      oops("Cannot get a pipe", 1);
  }

  for (int i = 0; i < ac - 1; i++) {
    if (fork() == 0) {

      if (i != n - 1) {
        if (dup2(thepipe[i][1], 1) == -1)
          oops("could not redirect stdout", 4);
      }
      if (i != 0) {
        if (dup2(thepipe[i - 1][0], 0) == -1)
          oops("could not redirect stdin", 4);
      }

      close_pipes(n - 1, thepipe);

      execlp(av[i + 1], av[i + 1], NULL);
      oops(av[i + 1], 5);
    }
  }

  close_pipes(n - 1, thepipe);
  while (wait(NULL) > 0)
    ;

  return 0;
}
