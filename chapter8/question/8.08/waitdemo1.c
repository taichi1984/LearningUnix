/* waitdemo1.c - 子プロセスが終了するまで親が１時停止する様子を示す */
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

int main(int ac, char *av[]) {
  srand(time(NULL));
  if (ac != 2) {
    fprintf(stderr, "引数の数が違います。");
    exit(1);
  }

  int proc_num = atoi(av[1]);
  int newpid_list[proc_num];
  void child_code(int), parent_code(int);

  printf("before: mypid is %d\n", getpid());

  for (int i = 0; i < proc_num; i++) {
    int delay = rand() % 10 + 1;
    if ((newpid_list[i] = fork()) == -1)
      perror("fork");
    else if (newpid_list[i] == 0)
      child_code(delay);
  }

  for (int i = 0; i < proc_num; i++) {
    if (newpid_list[i] != 0) {
      parent_code(newpid_list[i]);
    }
  }
}

/*
 *  新プロセスは居眠りをして終了する。
 */

void child_code(int delay) {
  printf(" child %d here. will sleep for %d seconds \n", getpid(), delay);
  sleep(delay);
  printf("child done. about to exit\n");
  exit(17);
}

/*
 * 親は子が終了するのを待ってからメッセージを出力する
 */

void parent_code(int childpid) {
  int wait_rv;
  int wstatus;
  wait_rv = waitpid(childpid, &wstatus, 0);
  printf("mypid is %d . done wating for %d. Wait returned: %d\n", getpid(),
         childpid, wait_rv);
  printf("status exit is %d, sig = %d , core = %d\n", WEXITSTATUS(wstatus),
         WTERMSIG(wstatus), WCOREDUMP(wstatus));
}
