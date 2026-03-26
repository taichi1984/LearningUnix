

/* sortfromfile.c
 * 目的: sortの入力をリダイレクトし、ファイルから読み込めるようにする。
 * 考え方：forkしてから、子の中で入力をリダイレクトし、最後にexecする。
 */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int ad, char *av[]) {
  int pid;
  int fd;

  if (ad != 2) {
    fprintf(stderr, "usage : sortfromfile filename\n");
    exit(1);
  }

  /* 新プロセスを作成する。失敗したら終了*/

  if ((pid = fork()) == -1) {
    perror("fork");
    exit(1);
  }
  if (pid == 0) {
    fd = open(av[1], O_RDONLY);
    if (fd == -1) {
      perror("open");
      exit(1);
    }

    dup2(fd, 0);
    close(fd);
    execlp("sort", "sort", NULL);
    perror("execlp");
    exit(1);
  }
  if (pid != 0) {
    wait(NULL);
  }
}
