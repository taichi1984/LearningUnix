/** tinybc.c * dcを使って仕事をする簡単な電卓
 *　　　　　　* 双方向パイプの作り方、使い方を示す。
 *            * 入力は、数値、演算子、数値の順番で指定する。
 *            * tinybcは、これを数値、\n、数値、\n,演算子,
 *            \n , p に変換してdcに渡す。
 *            * 返された結果は、標準出力に書き込む。
 *
 *            *プログラムの概要
 *            *a. 2本のパイプを作る
 *            *b. fork呼び出し（新しいプロセスを得る）
 *            *c.
 *dcになる予定の新プロセスの中で、標準入出力をパイプに接続し、execl dcする。 *d.
 *tinybcプロセスの中では、特に配管の必要はない。
 *
 *            通常の入出力を介して、ユーザーとやり取りし、
 *            パイプを介して、プログラム感の情報のやり取りをする。
 *            最後にパイプをクローズすると、dcは終了する。
 *            *注意:複数行のレスポンスを処理しない。
 *
 */

#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#define oops(m, x)                                                             \
  {                                                                            \
    perror(m);                                                                 \
    exit(x);                                                                   \
  }
typedef struct {
  bool flag_c;
} Option;

void be_dc(int in[2], int out[2]);
void be_bc(int todc[2], int fromdc[2]);
void be_bc_stdout();
void fatal(char *mess);

int main(int ac, char *av[]) {
  Option opt = {0};
  int c;
  int pid, todc[2], fromdc[2];

  pid = getpid();
  while ((c = getopt(ac, av, "c")) != -1) {
    switch (c) {
    case 'c':
      opt.flag_c = true;
      break;
    case '?':
      exit(1);
    }
  }

  if (opt.flag_c) {
    be_bc_stdout();
  } else {
    /* 2本のパイプを作る */
    if (pipe(todc) == -1 || pipe(fromdc) == -1)
      oops("pipe failed", 1);

    /* ユーザーインターフェースのためのプロセスを作る */

    if ((pid = fork()) == -1)
      oops("cannot fork", 2);
    if (pid == 0)
      be_dc(todc, fromdc);
    else {
      be_bc(todc, fromdc);
      wait(NULL);
    }
  }
}

void be_dc(int in[2], int out[2])
/*
 * 標準入出力をセットアップしてから、最後にexecl dc
 */
{
  /* 標準入力を入力用パイプに接続する*/
  if (dup2(in[0], 0) == -1)
    oops("dc: cannot redirect stdin", 3);
  close(in[0]);
  close(in[1]);

  /*標準出力を出力用パイプに接続する*/
  if (dup2(out[1], 1) == -1)
    oops("dc:cannot redirect stdout", 4);
  close(out[1]);
  close(out[0]);

  /* -オプションをつけてdcをexeclにする */
  execlp("dc", "dc", "-", NULL);
  oops("Cannot run dc", 5);
}

void be_bc(int todc[2], int fromdc[2])
/*
 * 標準入力から世j見出したものを逆ポーランド記法に変換し、パイプに送る。
 * 別のパイプから処理結果を読み出し、ユーザーにしめす。
 * fdopen()を呼び出して、ファイルディスクリプタをストリームに変換する。
 */
{
  int num1, num2;
  char operation[2], message[BUFSIZ];
  FILE *fpout, *fpin;

  /* セットアップ */
  close(todc[0]);
  close(fromdc[1]);

  fpout = fdopen(todc[1], "w");
  fpin = fdopen(fromdc[0], "r");

  if (fpout == NULL || fpin == NULL)
    fatal("Error convering pipes to streams");

  /* メインループ */
  while (1) {
    if (isatty(0))
      printf("tinybc : ");

    if (fgets(message, BUFSIZ, stdin) == NULL)
      break;

    /* parse input */
    if (sscanf(message, "%d %1[-+*/^] %d", &num1, operation, &num2) != 3) {
      printf("syntax error\n");
      continue;
    }

    if (fprintf(fpout, "%d\n%d\n%c\np\n", num1, num2, *operation) == EOF)
      fatal("Error writing");

    fflush(fpout);

    if (fgets(message, BUFSIZ, fpin) == NULL)
      break;
    printf("%d %c %d = %s", num1, *operation, num2, message);
  }
  fclose(fpout);
  fclose(fpin);
}

void be_bc_stdout()
/*
 * 標準入力から読み出したものを逆ポーランド記法に変換し、標準出力に送る。
 */
{
  int num1, num2;
  char operation[2], message[BUFSIZ];

  /* メインループ */
  while (1) {
    if (fgets(message, BUFSIZ, stdin) == NULL)
      break;

    /* parse input */
    if (sscanf(message, "%d %1[-+*/^] %d", &num1, operation, &num2) != 3) {
      printf("syntax error\n");
      continue;
    }

    printf("%d\n%d\n%c\np\n", num1, num2, *operation);
    fflush(stdout);
  }
}

void fatal(char *mess) {
  fprintf(stderr, "Error: %s\n", mess);
  exit(1);
}
