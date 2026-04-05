#include "stmt.h"
#include "controlflow.h"
#include "loopflow.h"
#include "smsh.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INIT_CAP 4

int exec_while_block(while_block *wb);
int exec_if_block(if_block *ifb);
void free_if_block(if_block *ifb);
void free_while_block(while_block *wb);
void free_cmd(char **);

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

int exec_stmt(stmt *s) {
  if (s->type == STMT_CMD)
    return execute(s->cmd);

  if (s->type == STMT_IF)
    return exec_if_block(s->ifb);

  if (s->type == STMT_WHILE)
    return exec_while_block(s->wb);

  return 0;
}

int exec_block(stmt_block *blk) {

  int status = 0;
  for (int i = 0; i < blk->size; i++) {
    stmt *s = &blk->stmts[i];
    if (s->type == STMT_CMD) {
      status = execute(s->cmd);
    } else if (s->type == STMT_WHILE) {
      status = exec_while_block(s->wb);
    } else if (s->type == STMT_IF) {
      status = exec_if_block(s->ifb);
    }
  }
  return status;
}

void free_stmt_block(stmt_block *blk) {
  if (!blk)
    return;

  for (int i = 0; i < blk->size; i++) {
    stmt *s = &blk->stmts[i];

    switch (s->type) {
    case STMT_CMD:
      free_cmd(s->cmd);

      break;

    case STMT_IF:
      free_if_block(s->ifb);
      break;

    case STMT_WHILE:
      free_while_block(s->wb);
      break;
    }
  }

  free(blk->stmts);

  blk->stmts = NULL;
  blk->size = 0;
  blk->cap = 0;
}

void free_while_block(while_block *wb) {
  free_stmt_block(&wb->while_part);
  free_stmt_block(&wb->do_part);
  free(wb);
}

void free_if_block(if_block *ifb) {
  free_stmt_block(&ifb->if_part);
  free_stmt_block(&ifb->then_part);
  free_stmt_block(&ifb->else_part);
  free(ifb);
}

void free_stmt(stmt *s) {
  if (!s)
    return;

  if (s->type == STMT_IF) {
    free_if_block(s->ifb);
  } else if (s->type == STMT_WHILE) {
    free_while_block(s->wb);
  }
  free(s);
}

char **copy_cmd(char **src) {
  if (!src)
    return NULL;

  int count = 0;
  while (src[count] != NULL)
    count++;

  char **dst = malloc(sizeof(char *) * (count + 1));

  for (int i = 0; i < count; i++)
    dst[i] = strdup(src[i]);

  dst[count] = NULL;
  return dst;
}

void free_cmd(char **cmd) {
  if (!cmd)
    return;

  for (int i = 0; cmd[i] != NULL; i++)
    free(cmd[i]);
  free(cmd);
}
