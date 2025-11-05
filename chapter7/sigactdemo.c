/* sigactdemo.c
 * 目的: sigaction()の使い方を示す
 * 特徴: Ctrl+Cの処理中、Ctrl+\をブロックする。
 * Ctrl+Cハンドラを設定しなおさないので、２度目で終了する。
 */

#include <signal.h>
#include <stdio.h>
#include <unistd.h>

#define INPUTLEN 100

int main() {
  struct sigaction newhandler;
  sigset_t blocked;
  void inthandler(int);
  char x[INPUTLEN];

  /* まず、これら２つのメンバをロードする。*/
  newhandler.sa_handler = inthandler;
  newhandler.sa_flags = SA_RESTART;

  /* 次にブロックされるシグナルのリストを作る*/
  sigemptyset(&blocked);
  sigaddset(&blocked, SIGINT);

  newhandler.sa_mask = blocked;

  if (sigaction(SIGQUIT, &newhandler, NULL) == -1)
    perror("sigaction");
  else
    while (1) {
      fgets(x, INPUTLEN, stdin);
      printf("input: %s", x);
    }
}

void inthandler(int s) {
  printf("Calledi with signal %d\n", s);
  sleep(s);
  printf("done handling signal %d\n", s);
}
