#ifndef CONTROLFLOW_H
#define CONTROLFLOW_H
#include "stmt.h"

typedef struct if_block {
  stmt_block if_part;
  stmt_block then_part;
  stmt_block else_part;
} if_block;


int exec_if_block(if_block *ifb);
char **parse_if_block(if_block *ifb , char **args);


#endif
