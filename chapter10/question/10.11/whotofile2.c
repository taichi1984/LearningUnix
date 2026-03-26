/* whotofile.c
 * 目的: 他のプログラムの出力をリダイレクトするにはどうすればよいかを示す。
 * 考え方：forkしてから、子の中で出力をリダイレクトし、最後にexecする。
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
    fprintf(stderr, "usage : whotofile2 filename\n");
    exit(1);
  }

  printf("About to run who into a file\n");

  /* 新プロセスを作成する。失敗したら終了*/

  if ((pid = fork()) == -1) {
    perror("fork");
    exit(1);
  }
  if (pid == 0) {
    close(1);
    fd = open(av[1], O_WRONLY | O_APPEND | O_CREAT, 0644);
    execlp("who", "who", NULL);
    perror("execlp");
    exit(1);
  }
  if (pid != 0) {
    wait(NULL);
    printf("Done running who. results in %s\n", av[1]);
  }
}
