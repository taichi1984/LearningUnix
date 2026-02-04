#include "stmt.h"
#include "smsh.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define INIT_CAP 4

int exec_while_block(while_block *wb);
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
    } else if (s->type == STMT_WHILE) {
      exec_while_block(s->wb);
    } else if (s->type == STMT_IF) {
      exec_if_block(s->ifb);
    }
  }
  return 0;
}
