#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>

#define LINELEN 512

void do_more(FILE *);
int see_more(FILE *);

int main(int ac, char *av[]) {
  FILE *fp;

  if (ac == 1)
    do_more(stdin);
  else
    while (--ac)
      if ((fp = fopen(*++av, "r")) != NULL) {
        do_more(fp);
        fclose(fp);
      } else
        exit(1);

  return 0;
}

void do_more(FILE *fp) {
  char line[LINELEN];
  struct winsize wbuf;
  int pagelen;

  if (ioctl(0, TIOCGWINSZ, &wbuf) == -1) {
    perror("ioctl:TIOCGWINSZ");
    exit(1);
  }

  int num_of_lines = 0;
  int see_more(FILE *), reply;
  FILE *fp_tty;

  fp_tty = fopen("/dev/tty", "r");
  if (fp_tty == NULL)
    exit(1);

  while (fgets(line, LINELEN, fp)) {
    if (num_of_lines == wbuf.ws_row) {
      reply = see_more(fp_tty);
      if (reply == 0)
        break;
      num_of_lines -= reply;
    }
    if (fputs(line, stdout) == EOF)
      exit(1);
    num_of_lines++;
  }
}

int see_more(FILE *cmd) {
  int c;
  struct winsize wbuf;
  if (ioctl(0, TIOCGWINSZ, &wbuf) == -1) {
    perror("ioctl:TIOCGWINSZ");
    exit(1);
  }
  printf("\033[7m more? \033[m");
  while ((c = getc(cmd)) != EOF) {
    if (c == 'q')
      return 0;
    if (c == ' ')
      return wbuf.ws_row;
    if (c == '\n')
      return 1;
  }
  return 0;
}
