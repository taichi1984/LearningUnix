#ifndef LOOPFLOW_H
#define LOOPFLOW_H

#include "stmt.h"

typedef struct while_block{
  stmt_block while_part;
  stmt_block do_part;
} while_block;

char **parse_while_block(while_block *wb, char **args);
int exec_while_block(while_block *wb);

#endif


