/* 文字列比較だけではなく、完全に同じファイルであることを確認できる完全版*/

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

#define BUFFERSIZE 4096
#define COPYMODE 0644

void oops(char *, char *);

int main(int ac, char *av[]) {

  struct stat stat1, stat2;
  int in_fd, out_fd, n_chars;
  char buf[BUFFERSIZE];

  if (ac != 3) {
    fprintf(stderr, "usage: %s source desination\n", *av);
    exit(1);
  }
  /* 完全なファイル比較はこうなる。 */
  if (stat(av[1], &stat1) == -1)
    oops("Cannot stat ", av[1]);

  if (stat(av[2], &stat2) == -1)
    oops("Cannot stat ", av[2]);

  if (stat1.st_ino == stat2.st_ino && stat1.st_dev == stat2.st_dev) {
    fprintf(stderr, "cp1 : \'%s\' and \'%s\' are the same file\n", av[1],
            av[2]);
    exit(1);
  }

  /* ファイル名だけで比較した場合の比較
  if (strcmp(av[1], av[2]) == 0) {
    fprintf(stderr, "cp1 : \'%s\' and \'%s\' are the same file\n", av[1],
            av[2]);
    exit(1);
  }
  */

  if ((in_fd = open(av[1], O_RDONLY)) == -1)
    oops("Cannot open ", av[1]);
  if ((out_fd = creat(av[2], COPYMODE)) == -1)
    oops("Cannot creat ", av[2]);

  while ((n_chars = read(in_fd, buf, BUFFERSIZE)) > 0)
    if (write(out_fd, buf, n_chars) != n_chars)
      oops("Write error to ", av[2]);

  if (n_chars == -1)
    oops("Read error from ", av[1]);

  if (close(in_fd) == -1 || close(out_fd) == -1)
    oops("Error closing files", "");
}

void oops(char *s1, char *s2) {
  fprintf(stderr, "Error : %s ", s1);
  perror(s2);
  exit(1);
}
