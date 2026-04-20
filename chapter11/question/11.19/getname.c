#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
  FILE *pfp = NULL;
  int mypid = getpid();
  char cmd[BUFSIZ];
  char filename[BUFSIZ];
  snprintf(cmd, sizeof(cmd), "ps -p %d -o comm=", mypid);
  // snprintf(cmd, sizeof(cmd), "ps | awk '{if ($1 == %d){print $4}}'", mypid);
  // // awk版。psの実装によって安定しないので非推奨

  if ((pfp = popen(cmd, "r")) == NULL) {
    perror("popen");
    exit(1);
  }

  int c;

  while ((c = getc(pfp)) != EOF) {
    printf("%c", c);
  }
  pclose(pfp);
  return 0;
}
