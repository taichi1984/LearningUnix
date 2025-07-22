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

#include<stdio.h>
#include<fcntl.h>
#include<sys/types.h>
#include<utmp.h>

#define NRECS   16
#define NULLUT  ((struct utmp *)NULL)
#define UTSIZE  (sizeof(struct utmp))

static char utmpbuf[NRECS * UTSIZE];  // メモリ領域、NRECS個の構造体を一次保存するバッファ
static int num_recs;  //格納してある個数
static int cur_red;   // 次のレコード番号
static int fd_utmp = -1;  //読み出し先


utmp_open( char *filename)
{
  fd_utmp = open( filename , O_RDONLY);
  cur_rec = num_recs = 0;
  return fd_utmp;
}



