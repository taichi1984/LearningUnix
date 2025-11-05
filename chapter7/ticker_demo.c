/*
 * ticker_demo.c
 * インターバルタイマを使って、周期的にシグナルを生成させる
 * シグナルはカウントダウンのためにキャッチする
 */

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

int set_ticker(int);

int main() {
  void countdown(int);
  signal(SIGALRM, countdown);

  if (set_ticker(500) == -1)
    perror("set_ticker");
  else
    while (1) {
      pause();
    }
  return 0;
}

void countdown(int signum) {
  static int num = 10;
  printf("%d ..", num--);
  fflush(stdout);
  if (num < 0) {
    printf("DONE!\n");
    exit(0);
  }
}

/* [from set_ticker.c]
 * set_ticker(number_of_milliseconds)
 * SIGALRMが周期的に発行されるようにインターバルタイマを設定する
 * エラーなら-1、成功なら0を返す。
 * 引数はミリ秒単位なので、秒とマイクロ秒の組み合わせに変換する
 * 注意 : set_ticker(0)とすると、シグナルは送られない
 */

int set_ticker(int n_msecs) {
  struct itimerval new_timeset;
  long n_sec, n_usecs;

  n_sec = n_msecs / 1000;
  n_usecs = (n_msecs % 1000) * 1000L;

  new_timeset.it_interval.tv_sec = n_sec;
  new_timeset.it_interval.tv_usec = n_usecs;
  new_timeset.it_value.tv_sec = n_sec;
  new_timeset.it_value.tv_usec = n_usecs;

  return setitimer(ITIMER_REAL, &new_timeset, NULL);
}
