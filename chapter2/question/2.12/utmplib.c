/* utmpファイルからの読み出しをバッファリングするための関数*
 *  含まれる関数は次の通り
 *
 *  utmp_open( file_name ) ファイルをオープンする。
 *   ファイルをオープンする。エラーの時には-1を返す。
 *  utmp_next( ) - 次の構造体を指すポインタを返す。
 *    eofのときにはNULLを返す。
 *   utmp_close() - ファイルを閉じる
 *
 *   一度の読み出しでNRECS個の構造体を読み出し、バッファから構造体を１つずつ提供する。*/

#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <utmp.h>

#define NRECS 16
#define NULLUT ((struct utmp *)NULL)
#define UTSIZE (sizeof(struct utmp))

static char
    utmpbuf[NRECS *
            UTSIZE]; // メモリ領域、NRECS個の構造体を一次保存するバッファ
static int num_recs;     //格納してある個数
static int cur_rec;      // 次のレコード番号
static int fd_utmp = -1; //読み出し先

int utmp_reload();

int utmp_open(char *filename) {
  fd_utmp = open(filename, O_RDONLY);
  cur_rec = num_recs = 0;
  return fd_utmp;
}

struct utmp *utmp_next() {
  struct utmp *recp;

  while (
      1) { // ここにループを追加することによって、USER_PROCESSを発見するか、全てのutmpを確保するまで、繰り返す(次のレコードを探し続ける。)。
    if (fd_utmp == -1)
      return NULLUT;
    if (cur_rec == num_recs && utmp_reload() == 0)
      return NULLUT;

    recp = (struct utmp *)&utmpbuf[cur_rec * UTSIZE];
    cur_rec++;

    if (recp->ut_type ==
        USER_PROCESS) // USER_PROCESSであればreturnでループから抜けつつ戻り値を返す。
      return recp;
  }
}

int utmp_reload() {
  int amt_read;

  amt_read = read(fd_utmp, utmpbuf, NRECS * UTSIZE);

  num_recs = amt_read / UTSIZE;
  cur_rec = 0;
  return num_recs;
}

void utmp_close() {
  if (fd_utmp != -1)
    close(fd_utmp);
}
