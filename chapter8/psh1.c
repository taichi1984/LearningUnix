/* prompting shell version1
 *     コマンドと引数の入力を促す。
 *     execvp呼び出しのために、引数配列を作る。
 *     execvp()を使い、制御を返さない
 */

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAXARGS 20
#define ARGLEN 100

int execute(char *arglist[]);
int main() {
  char *arglist[MAXARGS + 1];
  int numargs;
  char argbuf[ARGLEN];
  char *makestring();

  numargs = 0;
  while (numargs < MAXARGS) {
    printf("Arg[%d] ? ", numargs);
    if (fgets(argbuf, ARGLEN, stdin) && *argbuf != '\n')
      arglist[numargs++] = makestring(argbuf);
    else {
      if (numargs > 0) {
        arglist[numargs] = NULL;
        execute(arglist);
        numargs = 0;
      }
    }
  }
  return 0;
}

int execute(char *arglist[])
/*
 * * execvpを使ってコマンドを起動する。
 * */
{
  execvp(arglist[0], arglist);
  perror("execvp failed");
  exit(1);
}

char *makestring(char *buf) {
  char *cp;

  buf[strlen(buf) - 1] = '\0';
  cp = malloc(strlen(buf) + 1);
  if (cp == NULL) {
    fprintf(stderr, "no memory\n");
    exit(1);
  }
  strcpy(cp, buf);
  return cp;
}
