/** tinybc.c * dcを使って仕事をする簡単な電卓
 *　　　　　　* 双方向パイプの作り方、使い方を示す。
 *            * 入力は、数値、演算子、数値の順番で指定する。
 *            * tinybcは、これを数値、\n、数値、\n,演算子,
 *            \n , p に変換してdcに渡す。
 *            * 返された結果は、標準出力に書き込む。
 *
 *            *プログラムの概要
 *            *a. socketpairで双方向パイプを作る。
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

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#define oops(m, x)                                                             \
  {                                                                            \
    perror(m);                                                                 \
    exit(x);                                                                   \
  }
void be_dc(int apipe[2]);
void be_bc(int apipe[2]);
void fatal(char *mess);

int main() {
  int pid = -1;
  int apipe[2];

  /* socketpairを作る */

  if (socketpair(AF_UNIX, SOCK_STREAM, PF_UNSPEC, apipe) == -1) {
    perror("socketpair");
    exit(1);
  }

  /* ユーザーインターフェースのためのプロセスを作る */

  if ((pid = fork()) == -1)
    oops("cannot fork", 2);

  if (pid == 0)
    be_dc(apipe);
  else {
    be_bc(apipe);
    wait(NULL);
  }
}

void be_dc(int apipe[2])
/*
 * 標準入出力をセットアップしてから、最後にexecl dc
 */
{
  close(apipe[0]);
  /* 標準入力を子用パイプに接続する*/
  if (dup2(apipe[1], 0) == -1)
    oops("dc: cannot redirect stdin", 3);

  /*標準出力を子用パイプに接続する*/
  if (dup2(apipe[1], 1) == -1)
    oops("dc:cannot redirect stdout", 4);

  close(apipe[1]);
  /* -オプションをつけてdcをexeclにする */
  execlp("dc", "dc", "-", NULL);
  oops("Cannot run dc", 5);
}

void be_bc(int apipe[2])
/*
 * 標準入力から読み出したものを逆ポーランド記法に変換し、パイプに送る。
 * 同じパイプから処理結果を読み出し、ユーザーにしめす。
 * fdopen()を呼び出して、ファイルディスクリプタをストリームに変換する。
 */
{
  int num1, num2;
  char operation[BUFSIZ], message[BUFSIZ];
  FILE *fp;

  /* セットアップ */
  close(apipe[1]);

  fp = fdopen(apipe[0], "r+");

  if (fp == NULL)
    fatal("Error convering pipes to streams");

  /* メインループ */
  while (1) {

    printf("tinybc : ");
    if (fgets(message, BUFSIZ, stdin) == NULL)
      break;

    /* parse input */
    if (sscanf(message, "%d%1[-+*/^]%d", &num1, operation, &num2) != 3) {
      printf("syntax error\n");
      continue;
    }

    if (fprintf(fp, "%d\n%d\n%c\np\n", num1, num2, *operation) == EOF)

      fatal("Error writing");

    fflush(fp);

    if (fgets(message, BUFSIZ, fp) == NULL)
      break;

    printf("%d %c %d = %s", num1, *operation, num2, message);
  }
  fclose(fp);
}

void fatal(char *mess) {
  fprintf(stderr, "Error: %s\n", mess);
  exit(1);
}
