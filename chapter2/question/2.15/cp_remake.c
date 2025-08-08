#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFERSIZE 4096
#define COPYMODE 0644

void oops(char *, char *);

int main(int ac, char *av[]) {

  int in_fd, out_fd, n_chars;
  int interactive = 0; //-iオプションが指定されたか?
  char buf[BUFFERSIZE];
  char *src, *dst;
  char choice;

  /*-iオプションが設定されているかどうかの判定*/
  if (ac == 4 && strcmp(av[1], "-i") == 0) {
    interactive = 1;
    src = av[2];
    dst = av[3];
  } else if (ac == 3) {
    src = av[1];
    dst = av[2];
  } else {
    fprintf(stderr, "usage: %s source destination\n", *av);
    exit(1);
  }
  /* -i オプションが付与されているかつdstファイルが存在する場合の処理 */
  if (interactive && access(dst, F_OK) == 0) {
    printf("cp: overwrite \'%s\'?", av[3]);
    scanf("%c", &choice);
    if (choice != 'y' && choice != 'Y') {
      printf("Not overwritten.\n");
      exit(0);
    }
  }

  if ((in_fd = open(src, O_RDONLY)) == -1)
    oops("Cannot open ", src);
  if ((out_fd = creat(dst, COPYMODE)) == -1)
    oops("Cannot creat ", dst);

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
