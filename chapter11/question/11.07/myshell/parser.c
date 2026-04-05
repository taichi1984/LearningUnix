#include "controlflow.h"
#include "loopflow.h"
#include "smsh.h"
#include "stmt.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

stmt *parse_statement(char **args) {
  if (args == NULL || args[0] == NULL)
    return NULL;

  if (strcmp(args[0], "if") == 0) {
    if_block *ifb = malloc(sizeof(if_block));
    char **rest = parse_if_block(ifb, args);

    stmt *s = malloc(sizeof(stmt));
    s->type = STMT_IF;
    s->ifb = ifb;

    return s;
  }
  if (strcmp(args[0], "while") == 0) {
    while_block *wb = malloc(sizeof(while_block));
    char **rest = parse_while_block(wb, args);

    stmt *s = malloc(sizeof(stmt));
    s->type = STMT_WHILE;
    s->wb = wb;

    return s;
  }

  stmt *s = malloc(sizeof(stmt));
  s->type = STMT_CMD;
  s->cmd = copy_cmd(args);

  return s;
}
