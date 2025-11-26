#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void sighup_handler(int signum);
int main(void) {
  signal(SIGHUP, sighup_handler);
  pause();
}

void sighup_handler(int signum) {
  FILE *fp = fopen("/home/taichi84/デスクトップ/testfile", "w");

  fprintf(fp, "sighup is happend\n");

  fclose(fp);
}
