
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int main() {

  int apipe[2];
  if (socketpair(AF_UNIX, SOCK_STREAM, PF_UNSPEC, apipe) == -1) {
    perror("socketpair");
    exit(1);
  }

  int pid = -1;

  if ((pid = fork()) == -1) {
    perror("fork");
    exit(1);
  }

  if (pid == 0) {
    close(apipe[1]); // 親用FDは閉じる
    char buf[BUFSIZ];
    int n = 0;

    while ((n = read(apipe[0], buf, sizeof(buf))) != -1) {
      write(STDOUT_FILENO, buf, n);
    }

  } else {
    close(apipe[0]); // 子用のFDは閉じる
    int c = 0;
    while ((c = getchar()) != -1) {
      write(apipe[1], &c, 1);
    }
  }
}
