/* process.c
 * コマンド処理レイヤ
 *
 * process(char **arglist)関数は、メインループから呼び出され、
 * execute()関数の前に位置する。このレイヤは、主として、次の２つの処理を行う。
 * a) 組み込み関数 ( 例えば, exit() , set , = , read など）
 * b) 制御構造 (例えば、 if while ,forなど)
 *
 */

#include "controlflow.h"
#include "loopflow.h"
#include "smsh.h"
#include "stmt.h"
#include <stdio.h>
#include <string.h>

int is_control_command(char *);
int do_control_command(char **);
int ok_to_execute();

int process(char **args)
/*
 * 目的:ユーザーコマンドの処理
 * 戻り値:コマンドの処理結果
 * 詳細: 組み込みコマンドなら適切な関数を呼び出す、そうでなければexecute()する。
 * エラー: サブルーチンで処理されたものが上がってくる。
 */
{
  int rv = 0;

  if (args[0] == NULL)
    return 0;

  if (strcmp(args[0], "while") == 0) {
    while_block wb;
    parse_while_block(&wb, args);
    exec_while_block(&wb);
  }

  if (strcmp(args[0], "if") == 0) {
    if_block ifb;
    parse_if_block(&ifb, args);
    exec_if_block(&ifb);
  }

  else {
    rv = execute(args);
  }
  return rv;
}
