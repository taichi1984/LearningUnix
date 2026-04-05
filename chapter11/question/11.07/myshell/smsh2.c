
#include "smsh.h"
#include <getopt.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define DFL_PROMPT "> "

typedef struct {
  bool flag_c;
} Option;

int main(int ac, char *av[]) {
  char *cmdline, *prompt, **arglist;
  int process(char **);
  void setup();

  prompt = DFL_PROMPT;
  setup();

  Option opt = {0};
  int c;

  while ((c = getopt(ac, av, "c")) != -1) {
    switch (c) {
    case 'c':
      opt.flag_c = true;
      break;
    case '?':
      exit(1);
      break;
    }
  }

  if (opt.flag_c) {
    if (av[optind] == NULL) {
      fprintf(stderr, "-c requires argument\n");
      exit(1);
    }
    if ((cmdline = strdup(av[optind])) == NULL) {
      perror("strdup");
      exit(1);
    }
    if ((arglist = splitline(cmdline)) != NULL) {
      process(arglist);
      freelist(arglist);
    }
    free(cmdline);
  } else {
    while ((cmdline = next_cmd(prompt, stdin)) != NULL) {
      if ((arglist = splitline(cmdline)) != NULL) {
        process(arglist);
        freelist(arglist);
      }
      free(cmdline);
    }
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
