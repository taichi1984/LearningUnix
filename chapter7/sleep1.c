/* sleep1.c
 * 目的:sleepの仕組みを示す。
 * 使い方 : sleep1
 * 概要: ハンドラを設定し、alarm() , pause（）を呼び出して再び目を覚ます。
 */

#include <signal.h>
#include <stdio.h>
#include <unistd.h>

// #define SHHHH

int main() {
  void wakeup(int);

  printf("about to sleep for 4seconds\n");
  signal(SIGALRM, wakeup);
  alarm(4);
  pause();
  printf("Morning so soon? \n");
}

void wakeup(int signum) {
#ifndef SHHHH
  printf("Alarm received from kernel\n");
#endif
}
