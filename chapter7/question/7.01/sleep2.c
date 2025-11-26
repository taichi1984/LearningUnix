/* sleep2.c
 * 目的:sleepの仕組みを示す。
 * 使い方 : sleep1
 * 概要: ハンドラを設定し、alarm() , pause（）を呼び出して再び目を覚ます。
 * CTRL+CでSIGINTで割り込んだ場合は、\nDetect SIGINT\nと表示する。
 */

#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

// #define SHHHH

int main() {
  void wakeup(int);
  void detect_sigint(int);

  printf("about to sleep for 4seconds\n");
  signal(SIGALRM, wakeup);
  signal(SIGINT, detect_sigint);
  alarm(4);
  pause();
  printf("Morning so soon? \n");
}

void wakeup(int signum) {
#ifndef SHHHH
  printf("Alarm received from kernel\n");
#endif
}

void detect_sigint(int signum) {
  write(STDOUT_FILENO, "\nDetect SIGINT \n",
        strlen("\nDetect SIGINT \n")); // エラー処理省略
}
