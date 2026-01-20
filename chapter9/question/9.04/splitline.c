/* splitline.c - smshのためにコマンドを呼んで解析する関数。
 *
 *   char *next_cmd(char *prompt , FILE *fp)
 *   char **splitline(char *str);
 */

#include "smsh.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int need_prompt = 1;
char *next_cmd(char *prompt, FILE *fp)
/*
 * 目的：fpから次のコマンド行を読み出す。
 * 戻り値：コマンド行を保持する動的に確保した文字列。
 * エラー：EOFならNULL(本当はエラーではない)
 *         emalloc()からfatalを呼び出す。
 * メモ  : BUFSIZ単位でメモリを確保する。
 */
{
  char *buf;        /*バッファ*/
  int bufspace = 0; /*サイズ合計*/
  int pos = 0;      /*現在位置*/
  int c;            /*入力文字*/

  if (need_prompt) {
    printf("%s", prompt); /* ユーザーにプロンプトを出力する */
    fflush(stdout);
  }
  while ((c = getc(fp)) != EOF) {
    /* need space ?*/
    if (pos + 1 >= bufspace) {
      if (bufspace == 0)
        buf = emalloc(BUFSIZ);
      else
        buf = erealloc(buf, bufspace + BUFSIZ);
      bufspace += BUFSIZ;
    }
    if (c == '\n') {
      need_prompt = 1;
      break;
    }
    if (c == ';') {
      need_prompt = 0;
      break;
    }

    buf[pos++] = c;
  }
  if (c == EOF && pos == 0)
    return NULL;

  buf[pos] = '\0';
  return buf;
}

/**
** splitline( 行を解析して文字列配列に分割する )
**/
#define is_delim(x) ((x) == ' ' || (x) == '\t')

char **splitline(char *line)
/*
 * 目的：行を空白区切りにトークンに分割する。
 * 戻り値: トークンのコピーを指す末尾がNULLのポインタ配列。
 * 　　　　行にトークンが含まれていない場合はNULL
 * 動作  : 配列を準にチェックし、文字列を探してコピーを作る。
 * メモ  :
 * strtok()を使えば簡単だが、あとでクォートに対応するつもりなので、strtok()は使わない。
 */
{
  char *newstr(char *, int);
  char **args;
  int spots = 0;    /*テーブルの要素数*/
  int bufspace = 0; /*テーブルのバイト数*/
  int argnum = 0;   /*使った要素の数*/
  char *cp = line;  /*文字列のpos*/
  char *start;
  int len;

  if (line == NULL)
    return NULL;

  args = emalloc(BUFSIZ);
  bufspace = BUFSIZ;
  spots = BUFSIZ / sizeof(char *);

  while (*cp != '\0') {
    while (is_delim(*cp))
      cp++;
    if (*cp == '\0')
      break;

    /* 配列が充分なスペースを持つようにする(+1はNULLの分) */
    if (argnum + 1 >= spots) {
      args = erealloc(args, bufspace + BUFSIZ);
      bufspace += BUFSIZ;
      spots += (BUFSIZ / sizeof(char *));
    }

    /* 先頭にマークをつけ、単語の末尾を探す。*/
    start = cp;
    len = 1;
    while (*++cp != '\0' && !(is_delim(*cp)))
      len++;
    args[argnum++] = newstr(start, len);
  }
  args[argnum] = NULL;
  return args;
}

/*
 * 目的：文字列のコンストラクタ
 * 戻り値: 文字列、NLにはならない。
 */
char *newstr(char *s, int l) {
  char *rv = emalloc(l + 1);

  rv[l] = '\0';
  strncpy(rv, s, l);
  return rv;
}

void freelist(char **list)
/*
 *  目的:splitlineが返したリストを開放する。
 *  戻り値：なし
 *  動作：リスト内の全ての文字列を開放して、リストを開放する。
 */
{
  char **cp = list;
  while (*cp)
    free(*cp++);
  free(list);
}

void *emalloc(size_t n) {
  void *rv;
  if ((rv = malloc(n)) == NULL)
    fatal("out of memory", "", 1);
  return rv;
}

void *erealloc(void *p, size_t n) {
  void *rv;
  if ((rv = realloc(p, n)) == NULL)
    fatal("realloc() failed", "", 1);
  return rv;
}
