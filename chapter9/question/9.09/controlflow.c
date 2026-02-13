/* controlflow.c
 *
 * " if " 処理は、if_stateとif_resultの２個の状態変数を使って行う。
 */

#include "smsh.h"
#include "stmt.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define INIT_CAP 4

enum states {
  NEUTRAL,
  IF_BLOCK,
  THEN_BLOCK,
  ELSE_BLOCK,
};

int syn_err(char *, int if_state);
int exec_if_block(if_block *ifb);

void stmt_block_init(stmt_block *blk) {
  blk->cap = INIT_CAP;
  blk->size = 0;
  blk->stmts = malloc(sizeof(stmt) * blk->cap);
}

int stmt_push(stmt_block *blk, stmt s) {
  if (blk->size >= blk->cap) {
    blk->cap *= 2;
    blk->stmts = realloc(blk->stmts, sizeof(stmt) * blk->cap);
  }
  blk->stmts[blk->size++] = s;
  return 0;
}

int exec_block(stmt_block *blk) {
  for (int i = 0; i < blk->size; i++) {
    stmt *s = &blk->stmts[i];
    if (s->type == STMT_CMD) {
      execute(s->cmd);
    } else if (s->type == STMT_IF) {
      exec_if_block(s->ifb);
    }
  }
  return 0;
}

int exec_if_block(if_block *ifb) {
  int status;
  if (execute(ifb->if_part.stmts[0].cmd) == 0) {
    status = exec_block(&ifb->then_part);
  } else {
    status = exec_block(&ifb->else_part);
  }
  return status;
}

char **parse_if_block(if_block *ifb, char **args)
/*
 * 目的 : "if" , "then " , "fi" を処理する --
 * 状態を変更するかエラーを検出する。 戻り値 : OK なら0、構文エラーなら-1 メモ
 * : I would have put returns all over the place , Barry says "no"
 */
{
  char *cmdline;
  char **arglist = args;
  int if_state = NEUTRAL;

  stmt_block_init(&ifb->if_part);
  stmt_block_init(&ifb->then_part);
  stmt_block_init(&ifb->else_part);

  while (arglist != NULL) {
    if ((if_state == THEN_BLOCK || if_state == ELSE_BLOCK) &&
        strcmp(arglist[0], "if") == 0) {
      if (if_state == THEN_BLOCK) {

        if_block *nested = malloc(sizeof(if_block));
        arglist = parse_if_block(nested, arglist);

        stmt s = (stmt){.type = STMT_IF, .ifb = nested};
        stmt_push(&ifb->then_part, s);

        continue;
      }
      if (if_state == ELSE_BLOCK) {
        if_block *nested = malloc(sizeof(if_block));
        arglist = parse_if_block(nested, arglist);

        stmt s = (stmt){.type = STMT_IF, .ifb = nested};
        stmt_push(&ifb->else_part, s);

        continue;
      }
    } else if (if_state == NEUTRAL && strcmp(arglist[0], "if") == 0) {
      if_state = IF_BLOCK;
      if (arglist[1] == NULL) {
        syn_err("missing if condition", if_state);
        break;
      }
      stmt s = (stmt){.type = STMT_CMD, .cmd = &arglist[1]};
      stmt_push(&ifb->if_part, s);
    } else if (strcmp(arglist[0], "then") == 0) {
      if (if_state != IF_BLOCK) {
        syn_err("then unexpected", if_state);
        break;
      }
      if (ifb->if_part.size == 0) {
        syn_err("missing if condition", if_state);
        break;
      }
      if_state = THEN_BLOCK;
    } else if (strcmp(arglist[0], "else") == 0) {
      if (if_state != THEN_BLOCK) {
        syn_err("else unexpected", if_state);
        break;
      } else {
        if (if_state == THEN_BLOCK)
          if_state = ELSE_BLOCK;
      }
    } else if (strcmp(arglist[0], "fi") == 0) {
      if (if_state != THEN_BLOCK && if_state != ELSE_BLOCK) {
        syn_err("fi unexpected", if_state);
        return NULL;
      }

      cmdline = next_cmd(">", stdin);
      if (!cmdline)
        return NULL;

      arglist = splitline(cmdline);
      return arglist;

    } else {
      if (if_state == IF_BLOCK) {
        if (ifb->if_part.size > 0) { // 条件は１コマンドのみ許可
          syn_err("multiple if conditions", if_state);
          break;
        }
      }
      if (if_state == THEN_BLOCK) {
        stmt s = (stmt){.type = STMT_CMD, .cmd = arglist};
        stmt_push(&ifb->then_part, s);
      }
      if (if_state == ELSE_BLOCK) {
        stmt s = (stmt){.type = STMT_CMD, .cmd = arglist};
        stmt_push(&ifb->else_part, s);
      }
    }
    cmdline = next_cmd(">", stdin);
    if (!cmdline) {
      arglist = NULL;
      break;
    }

    arglist = splitline(cmdline);
    if (!arglist) {
      break;
    }
  }

  return arglist;
}

int syn_err(char *msg, int if_state)
/* 目的:制御構造内の構文エラーを処理する。
 *  詳細:状態をNEUTRALにリセットする。
 *  戻り値: 対話的モードなら1、スクリプト内では、fatal呼び出しになる。
 */
{
  if_state = NEUTRAL;
  fprintf(stderr, "syntax error : %s\n", msg);
  return -1;
}
