#include "loopflow.h"
#include "smsh.h"
#include "stmt.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define INIT_CAP 4

enum states {
  NEUTRAL,
  WHILE_BLOCK,
  DO_BLOCK,
};

int syn_err(char *, int while_state);

char **parse_while_block(while_block *wb, char **args)
/*
 * 目的 : "if" , "then " , "fi" を処理する --
 * 状態を変更するかエラーを検出する。 戻り値 : OK なら0、構文エラーなら-1 メモ
 * : I would have put returns all over the place , Barry says "no"
 */
{
  char *cmdline;
  char **arglist = args;
  int while_state = NEUTRAL;

  stmt_block_init(&wb->while_part);
  stmt_block_init(&wb->do_part);

  while (arglist != NULL) {
    if ((while_state == NEUTRAL && strcmp(arglist[0], "while") == 0)) {
      if (arglist[1] == NULL) {
        syn_err("missing while condition", while_state);
        break;
      }
      if (wb->while_part.size > 0) {
        syn_err("multiple while condition", while_state);
      }
      while_state = WHILE_BLOCK;

      stmt s = (stmt){.type = STMT_CMD, .cmd = &arglist[1]};
      stmt_push(&wb->while_part, s);

    } else if (strcmp(arglist[0], "do") == 0) {
      if (while_state != WHILE_BLOCK) {
        syn_err("do unexpected", while_state);
        break;
      }

      while_state = DO_BLOCK;

    } else if (strcmp(arglist[0], "done") == 0) {
      if (while_state != DO_BLOCK) {
        syn_err("done unexpected", while_state);
        break;
      } else {
        while_state = NEUTRAL;
        break;
      }
    } else {
      if (while_state == DO_BLOCK) {
        stmt s = (stmt){.type = STMT_CMD, .cmd = arglist};
        stmt_push(&wb->do_part, s);
      }
    }

    cmdline = next_cmd(">", stdin);
    if (!cmdline)
      return NULL;

    arglist = splitline(cmdline);
  }

  return arglist;
}

int exec_while_block(while_block *blk) { return 0; }

int syn_err(char *msg, int while_state)
/* 目的:制御構造内の構文エラーを処理する。
 *  詳細:状態をNEUTRALにリセットする。
 *  戻り値: 対話的モードなら1、スクリプト内では、fatal呼び出しになる。
 */
{
  while_state = NEUTRAL;
  fprintf(stderr, "syntax error : %s\n", msg);
  return -1;
}
