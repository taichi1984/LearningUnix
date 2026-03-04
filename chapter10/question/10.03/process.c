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
#include "parser.h"
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
  stmt *s = parse_statement(args);
  int rv = exec_stmt(s);
  free_stmt(s);
  return rv;
}
