#ifndef STMT_H
#define STMT_H

typedef enum { STMT_CMD, STMT_IF, STMT_WHILE } stmt_type;

struct if_block;
struct while_block;

typedef struct {
  stmt_type type;
  union {
    char **cmd;
    struct while_block *wb;
    struct if_block *ifb;
  };
} stmt;

typedef struct {
  stmt *stmts;
  int cap;
  int size;
} stmt_block;


void stmt_block_init(stmt_block *blk);
void free_stmt(stmt *s);
int stmt_push(stmt_block *blk, stmt s);
int exec_block(stmt_block *blk); 
int exec_stmt(stmt *s);
char **copy_cmd(char **src);

#endif
