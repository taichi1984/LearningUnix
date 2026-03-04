/* execute.c - smshがコマンドを実行するために使うコード*/

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int execute(char *argv[])
/*
 * 目的:引数を渡してプログラムを起動する。
 * 戻り値:waitを介して、返された終了ステータス。エラーの場合は-1。
 * エラー:fork()かwait()がエラーを起こしたときには-1
 */
{
  int pid;
  int child_info = -1;

  if (argv[0] == NULL)
    return 0;

  if ((pid = fork()) == -1)
    perror("fork");
  else if (pid == 0) {
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
    execvp(argv[0], argv);
    perror("cannot execute command");
    exit(1);
  } else {
    if (wait(&child_info) == -1)
      perror("wait");
  }
  return child_info;
}
