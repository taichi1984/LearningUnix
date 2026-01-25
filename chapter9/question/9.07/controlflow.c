/* controlflow.c
 *
 * " if " 処理は、if_stateとif_resultの２個の状態変数を使って行う。
 */

#include "smsh.h"
#include <stdio.h>
#include <string.h>

enum states {
  NEUTRAL,
  IF_SUCCEEDED,
  IF_FAILED,
  SKIPPING_THEN,
  DOING_THEN,
  SKIPPING_ELSE,
  DOING_ELSE
};

static int if_state = NEUTRAL;
static int last_stat = 0;

int syn_err(char *);

int ok_to_execute()
/*
 * 目的 : シェルがコマンドを実行すべきかどうかを判断する。
 * 戻り値: イエスなら１，ノーなら0
 * 詳細 : THEN_BLOCK内にいて、if_resultがSUCCESSならイエス
 *        THEN_BLOCK内にいて、if_resultがFAILならノー
 *        WANT_THENなら構文エラー(shは異なる)
 */
{
  int rv = 1; /*デフォルトは真 */

  if (if_state == IF_SUCCEEDED || if_state == IF_FAILED) {
    syn_err("then expected");
    rv = 0;
  } else if (if_state == DOING_THEN)
    rv = 1;
  else if (if_state == SKIPPING_THEN)
    rv = 0;
  else if (if_state == SKIPPING_ELSE)
    rv = 0;
  else if (if_state == DOING_ELSE)
    rv = 1;

  return rv;
}

int is_control_command(char *s)
/*
 * コマンドがシェル制御コマンドかどうかを報告するbool関数
 * 戻り値: 0 か 1
 */
{
  return (strcmp(s, "if") == 0 || strcmp(s, "then") == 0 ||
          strcmp(s, "fi") == 0) ||
         strcmp(s, "else") == 0;
}

int do_control_command(char **args)
/*
 * 目的 : "if" , "then " , "fi" を処理する -- 状態を変更するかエラーを検出する。
 * 戻り値 : OK なら0、構文エラーなら-1
 * メモ : I would have put returns all over the place , Barry says "no"
 */
{
  char *cmd = args[0];
  int rv = -1;

  if (strcmp(cmd, "if") == 0) {
    if (if_state != NEUTRAL)
      rv = syn_err("if unexpected");
    else {
      last_stat = process(args + 1);
      if_state = (last_stat == 0 ? IF_SUCCEEDED : IF_FAILED);
      rv = 0;
    }
  } else if (strcmp(cmd, "then") == 0) {
    if (if_state != IF_FAILED && if_state != IF_SUCCEEDED)
      rv = syn_err("then unexpected");
    else {
      if (if_state == IF_SUCCEEDED)
        if_state = DOING_THEN;
      if (if_state == IF_FAILED)
        if_state = SKIPPING_THEN;
      rv = 0;
    }
  } else if (strcmp(cmd, "else") == 0) {
    if (if_state != DOING_THEN && if_state != SKIPPING_THEN) {
      rv = syn_err("else unexpected");
    } else {
      if (if_state == DOING_THEN)
        if_state = SKIPPING_ELSE;
      if (if_state == SKIPPING_THEN)
        if_state = DOING_ELSE;
      rv = 0;
    }
  } else if (strcmp(cmd, "fi") == 0) {
    if (if_state == IF_SUCCEEDED || if_state == IF_FAILED ||
        if_state == NEUTRAL) {
      rv = syn_err("fi unexpected");
    } else {
      if_state = NEUTRAL;
      rv = 0;
    }
  } else
    fatal("internal error processing:", cmd, 2);
  return rv;
}

int syn_err(char *msg)
/* 目的:制御構造内の構文エラーを処理する。
 *  詳細:状態をNEUTRALにリセットする。
 *  戻り値: 対話的モードなら1、スクリプト内では、fatal呼び出しになる。
 */
{
  if_state = NEUTRAL;
  fprintf(stderr, "syntax error : %s\n", msg);
  return -1;
}
