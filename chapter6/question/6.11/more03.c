#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <termios.h>

#define LINELEN 512

void do_more(FILE *);
int see_more(FILE *);
void set_noecho_nocan();
int tty_mode(int how);
void signal_handler(int signum);

int main(int ac, char *av[]) {
  tty_mode(0);
  set_noecho_nocan();
  signal(SIGINT, signal_handler);
  signal(SIGQUIT, signal_handler);
  signal(SIGTSTP, signal_handler);

  FILE *fp;

  if (ac == 1)
    do_more(stdin);
  else
    while (--ac)
      if ((fp = fopen(*++av, "r")) != NULL) {
        do_more(fp);
        fclose(fp);
      } else {
        tty_mode(1);
        exit(1);
      }
  tty_mode(1);

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

void set_noecho_nocan() {
  struct termios ttystate;
  tcgetattr(0, &ttystate);
  ttystate.c_lflag &= ~ECHO;
  ttystate.c_lflag &= ~ICANON;
  tcsetattr(0, TCSANOW, &ttystate);
}

int tty_mode(int how) {
  static struct termios original_mode;
  if (how == 0) {
    tcgetattr(0, &original_mode);
    return 0;
  } else
    return tcsetattr(0, TCSANOW, &original_mode);
}

void signal_handler(int signum) {
  printf("\n割り込みを確認\n");
  tty_mode(1);
  exit(2);
}
