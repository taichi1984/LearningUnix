
/* waitdemo_sigchld.c -
 * SIGCHLDを取得して、子がexitを呼び出したら、子の終了ステータスを報告するメッセージを出力して終了する。*/

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#define DELAY 5
volatile sig_atomic_t sigchld_counter = 0;
volatile sig_atomic_t sigchld_flag = 0;

int main(int ac, char *av[]) {
  int newpid;
  void child_code(int), parent_code(int, int);
  void sigchld_handler(int);
  int num_of_child = 0;

  num_of_child = atoi(av[1]);

  printf("before : mypid is %d\n", getpid());

  signal(SIGCHLD, sigchld_handler);
  for (int i = 0; i < num_of_child; i++) {
    if ((newpid = fork()) == -1)
      perror("fork");
    else if (newpid == 0)
      child_code(DELAY);
  }

  if (newpid != -1 && newpid != 0) {
    parent_code(newpid, num_of_child);
  }
}

/*
 * 新プロセスは昼寝して終了
 */

void child_code(int delay) {
  printf("child %d here. will sleep for %d seconds\n", getpid(), delay);
  sleep(delay);
  printf("child (%d) done. about to exit\n", getpid());
  exit(17);
}

/*
 * 親は子が終了するのを待ち、メッセージを出力する
 */

void parent_code(int childpid, int num_of_child) {
  int wait_rv;
  int child_status;
  int high_8, low_7, bit_7;

  while (sigchld_counter < num_of_child) {
    printf("waiting...\n");
    sleep(1);
  }
}

void sigchld_handler(int signum) {
  int status;
  pid_t pid;

  while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
    sigchld_counter++;

    char buf[64];
    int len = snprintf(buf, sizeof(buf), "child %d exited\n", pid);
    write(STDOUT_FILENO, buf, len);
  }
}
