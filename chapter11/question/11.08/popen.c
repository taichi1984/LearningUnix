/* popen.c - UNIXライブラリ関数のpopen()の実装の１つ
 * FILE *popen(char *command , char *mode)
 * command は通常のシェルコマンド
 * modeは"r"か"w"
 * コマンドに接続されたストリームかNULを返す。
 * execlで"sh" "-c" command を実行する。
 * 課題：子プロセスのシグナル処理をどうするか。
 */

#include <alloca.h>
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#define READ 0
#define WRITE 1
#define MAX_TABLE 255
typedef struct {
  FILE *fp;
  pid_t pid;
} Process;

static Process proc_table[MAX_TABLE];
int regist_process(Process p);
int remove_process(FILE *fp);

FILE *popen(const char *command, const char *mode) {
  int pfp[2], pid;
  int parent_end, child_end;

  if (*mode == 'r') {
    parent_end = READ;
    child_end = WRITE;
  } else if (*mode == 'w') {
    parent_end = WRITE;
    child_end = READ;
  } else
    return NULL;

  if (pipe(pfp) == -1)
    return NULL;
  if ((pid = fork()) == -1) {
    close(pfp[0]);
    close(pfp[1]);
    return NULL;
  }

  if (pid == 0) {
    /* -------------------子のコード ------------------*/
    /* stdin またはstdoutをリダイレクトし、cmdをexec */

    if (close(pfp[parent_end]) == -1)
      _exit(1);

    if (dup2(pfp[child_end], child_end) == -1)
      _exit(1);

    if (close(pfp[child_end]) == -1)
      _exit(1);

    execl("/bin/sh", "sh", "-c", command, NULL);
    _exit(1);
  } else {

    /* ------------------- 親のコード -------------*/
    /* ------------------- 片方をクローズしてもう片方をfdopenする。*/

    Process p;
    if (close(pfp[child_end]) == -1) {
      close(pfp[parent_end]);
      return NULL;
    }

    p.fp = fdopen(pfp[parent_end], mode);
    if (p.fp == NULL) {
      close(pfp[parent_end]);
      return NULL;
    }

    p.pid = pid;

    if (regist_process(p) == -1) {
      fclose(p.fp);
      waitpid(pid, NULL, 0); // 子プロセス回収
      return NULL;
    };

    return p.fp;
  }
}

int pclose(FILE *fp) {
  pid_t pid;
  int wstatus;

  if ((pid = remove_process(fp)) == -1) {
    return -1;
  }
  if (fclose(fp) == -1) {
    return -1;
  }

  while ((wait4(pid, &wstatus, 0, NULL)) == -1) {
    if (errno != EINTR)
      return -1;
  }

  return wstatus;
}

int regist_process(Process p) {
  for (int i = 0; i < MAX_TABLE; i++) {
    if (proc_table[i].fp == NULL) {
      proc_table[i].fp = p.fp;
      proc_table[i].pid = p.pid;
      return 0;
    }
  }
  return -1;
}

pid_t remove_process(FILE *fp) {
  pid_t pid = -1;
  for (int i = 0; i < MAX_TABLE; i++) {
    if (proc_table[i].fp == fp) {
      pid = proc_table[i].pid;
      proc_table[i].fp = NULL;
      return pid;
    }
  }

  return -1;
}
