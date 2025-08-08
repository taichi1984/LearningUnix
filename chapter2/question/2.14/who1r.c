/*
 *  utmpの全ての内容が表示されるようにし、コマンドライン引数に指定したファイルから開けるようにする。
 */
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <utmp.h>

#define SHOWHOST
void show_info(struct utmp *utbufp);
void show_header();

int main(int argc, char *argv[]) {
  struct utmp current_record;
  int utmpfd;
  int reclen = sizeof(current_record);

  if (argc == 2) {
    if ((utmpfd = open(argv[1], O_RDONLY)) == -1) {
      perror(argv[1]);
      exit(1);
    }
  } else {
    if ((utmpfd = open(UTMP_FILE, O_RDONLY)) == -1) {
      perror(UTMP_FILE);
      exit(1);
    }
  }
  show_header();
  while (read(utmpfd, &current_record, reclen) == reclen)
    show_info(&current_record);
  close(utmpfd);
  return 0;
}

void show_header() {
  printf("%-10s", "ut_type");
  printf(" ");
  printf("%-12s", "ut_pid");
  printf(" ");
  printf("%-16s", "ut_line");
  printf(" ");
  printf("%-16s", "ut_id");
  printf(" ");
  printf("%-16s", "ut_user");
  printf(" ");
  printf("%-12s", "ut_time");
  printf(" ");
  printf("%-12s", "ut_session");
  printf(" ");
  printf("%-12s", "tv_sec");
  printf(" ");
  printf("%-12s", "tv_usec");
  printf(" ");
  printf("%-10s", "ut_host");
  printf("\n");
}

void show_info(struct utmp *utbufp) {
  printf("%-10d", utbufp->ut_type);
  printf(" ");
  printf("%-12d", utbufp->ut_pid);
  printf(" ");
  printf("%-16s", utbufp->ut_line);
  printf(" ");
  printf("%-16s", utbufp->ut_id);
  printf(" ");
  printf("%-16s", utbufp->ut_user);
  printf(" ");
  printf("%-12d", utbufp->ut_time);
  printf(" ");
  printf("%-12d", utbufp->ut_session);
  printf(" ");
  printf("%-12d", utbufp->ut_tv.tv_sec);
  printf(" ");
  printf("%-12d", utbufp->ut_tv.tv_usec);
  printf(" ");

#ifdef SHOWHOST
  printf("(%s)", utbufp->ut_host);
#endif
  printf("\n");
}
