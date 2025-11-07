#include <sys/time.h>
#include <stddef.h>

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
