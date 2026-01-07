/* builtin.c
 * 組み込みコマンドのためのswitch文と関数が含まれている。
 */

#include "smsh.h"
#include "varlib.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

int assign(char *);
int okname(char *);

int builtin_command(char **args, int *resultp)
/*
 * 目的:組み込みコマンドを実行する。
 * 戻り値:args[0]が組込コマンドなら１、そうでなければ0を返す。
 * 詳細:args[0]をすべての組み込みコマンドと比較する。
 * 組み込みコマンドなら適切な関数を呼び出す
 */
{
  int rv = 0;

  if (strcmp(args[0], "set") == 0) {
    VLlist();
    *resultp = 0;
    rv = 1;
  } else if (strchr(args[0], '=') != NULL) { /*代入コマンド*/
    *resultp = assign(args[0]);
    if (*resultp != -1)
      rv = 1;
  } else if (strcmp(args[0], "export") == 0) {
    if (args[1] != NULL && okname(args[1]))
      *resultp = VLexport(args[1]);
    else
      *resultp = 1;
    rv = 1;
  }
  return rv;
}
int assign(char *str)
/*
 * 目的:name=valを実行する。名前が適正でなければ代入せずにエラーを返す。
 * 戻り値：無効な左辺値の場合は-1、そうでなければVLstoreの結果。
 * 警告：文字列を書き換えるが、通常のものに復元する。
 */
{
  char *cp;
  int rv;

  cp = strchr(str, '=');
  *cp = '\0';
  rv = (okname(str) ? VLstore(str, cp + 1) : -1);
  *cp = '=';
  return rv;
}

int okname(char *str)
/*
 * 目的:文字列が変数名として認められるかどうか判断する。
 * 戻り値:ノーなら０、イエスなら１
 */
{
  char *cp;

  for (cp = str; *cp; cp++) {
    if ((isdigit(*cp) && cp == str) || !(isalnum(*cp) || *cp == '_'))
      return 0;
  }
  return (cp != str);
}
