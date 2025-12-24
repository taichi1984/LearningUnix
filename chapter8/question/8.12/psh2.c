/* prompting shell version1
 *     コマンドと引数の入力を促す。
 *     execvp呼び出しのために、引数配列を作る。
 *     execvp()を使い、制御を返さない
 */

#include <signal.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAXARGS 20
#define ARGLEN 100

void execute(char *arglist[]);
int main() {
  char *arglist[MAXARGS + 1];
  int numargs;
  char argbuf[ARGLEN];
  char *makestring(char *buf);

  signal(SIGINT, SIG_IGN);
  signal(SIGQUIT, SIG_IGN);
  signal(SIGTSTP, SIG_IGN);

  numargs = 0;
  while (numargs < MAXARGS) {
    printf("Arg[%d] ? ", numargs);
    if (fgets(argbuf, ARGLEN, stdin) == NULL) {
      exit(0);
    }

    if (*argbuf != '\n') {
      arglist[numargs++] = makestring(argbuf);

      if (strcmp(arglist[0], "exit") == 0)
        exit(0);
    } else {

      if (numargs > 0) {
        arglist[numargs] = NULL;
        execute(arglist);
        numargs = 0;
      }
    }
  }
  return 0;
}

void execute(char *arglist[])
/*
 * * execvpを使ってコマンドを起動する。
 * */
{
  int pid, exitstatus;

  pid = fork();

  switch (pid) {
  case -1:
    perror("fork failed");
    exit(1);
  case 0:

    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
    signal(SIGTSTP, SIG_DFL);
    execvp(arglist[0], arglist);
    perror("execvp failed");
    exit(1);
  default:
    while (wait(&exitstatus) != pid)
      ;
    printf("child exited with status %d, %d\n", exitstatus >> 8,
           exitstatus & 0377);
  }
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
