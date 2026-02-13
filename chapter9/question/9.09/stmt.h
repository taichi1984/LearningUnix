#ifndef STMT_H
#define STMT_H

typedef enum { STMT_CMD, STMT_IF } stmt_type;

struct if_block;

typedef struct {
  stmt_type type;
  union {
    char **cmd;
    struct if_block *ifb;
  };
} stmt;

typedef struct {
  stmt *stmts;
  int cap;
  int size;
} stmt_block;

typedef struct if_block {
  stmt_block if_part;
  stmt_block then_part;
  stmt_block else_part;
} if_block;

#endif
