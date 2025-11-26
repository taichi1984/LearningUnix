/* forkdemo2.c - 子プロセスがfork()からリターンしたところからスタートし、
 * fork()を含めて好きなコードを実行できることを示す。
 * 出力の行数を予測せよ。
 */
#include <stdio.h>
#include <unistd.h>

int main(void) {
  printf("my pid is %d \n", getpid());
  fork();
  fork();
  fork();
  printf("my pid is %d\n", getpid());
}
