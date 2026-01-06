#include "smsh.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define DFL_PROMPT "> "
int main() {
  char *cmdline, *prompt, **arglist;
  int result, process(char **);
  char *end;
  void setup();

  prompt = DFL_PROMPT;
  setup();

  while ((cmdline = next_cmd(prompt, stdin)) != NULL) {
    if ((arglist = splitline(cmdline)) != NULL) {
      result = process(arglist);
      freelist(arglist);
    }
    free(cmdline);
  }
  return 0;
}

void setup()
/*
 * 目的:シェルの初期化
 * 戻り値：なし、トラブルが起きたらfatal()を呼び出す。
 */
{
  signal(SIGINT, SIG_IGN);
  signal(SIGQUIT, SIG_IGN);
}

void fatal(char *s1, char *s2, int n) {
  fprintf(stderr, "Error: %s,%s\n", s1, s2);
  exit(n);
}
