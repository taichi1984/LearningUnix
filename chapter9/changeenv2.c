#include <stdio.h>
#include <unistd.h>

extern char **environ;

int main() {
  char *table[3];

  table[0] = "TERM=vt100";
  table[1] = "HOME=/on/the/range";
  table[2] = 0;

  environ = table;

  char *argv[2];

  argv[0] = "env";
  argv[1] = NULL;

  if (execve("/usr/bin/env", argv, environ) == -1) {
    printf("exec失敗\n");
  }
}
