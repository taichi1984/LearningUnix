/*whoの最初のバージョン*/
#include "utmplib.c"
#include <fcntl.h>
#include <libgen.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <utmp.h>

#define SHOWHOST
void show_info(struct utmp *utbufp);
void showtime(long timeval);

int main(int argc, char *argv[]) {

  struct utmp *utbufp;
  char *username = ttyname(0);
  if (strncmp(username, "/dev/", 5) == 0) {
    username += 5;
  }
  if (utmp_open(UTMP_FILE) == -1) {
    perror(UTMP_FILE);
    exit(1);
  }
  if (argc == 3) {
    while ((utbufp = utmp_next()) != ((struct utmp *)NULL)) {
      if (strncmp(utbufp->ut_line, username, UT_NAMESIZE) == 0) {
        show_info(utbufp);
      }
    }
  } else {
    while ((utbufp = utmp_next()) != ((struct utmp *)NULL))
      show_info(utbufp);
  }
  utmp_close();
  return 0;
}
void show_info(struct utmp *utbufp) {
  if (utbufp->ut_type != USER_PROCESS)
    return;
  printf("%-8.8s", utbufp->ut_name);
  printf(" ");
  printf("%-8.8s", utbufp->ut_line);
  printf(" ");
  showtime(utbufp->ut_time);

#ifdef SHOWHOST
  printf("(%s)", utbufp->ut_host);
#endif
  printf("\n");
}

void showtime(long timeval) {
  time_t t = (time_t)timeval;
  char *cp = ctime(&timeval);
  printf("%12.12s", cp + 4);
}
