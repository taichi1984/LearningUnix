#define _POSIX_C_SOURCE 200809L
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <utmp.h>

int main(void) {
  struct utmp user;
  struct stat userstat;
  char buf[512];
  int fd = 0;
  int i = 0;

  if ((fd = open(UTMP_FILE, O_RDONLY)) == -1) {
    perror("open");
    return 1;
  }

  while (read(fd, &user, sizeof(struct utmp))) {

    if (user.ut_type == USER_PROCESS) {
      snprintf(buf, sizeof(buf), "/dev/%s", user.ut_line);

      if (stat(buf, &userstat) == -1) {
        perror("stat");
      }
      char time_s[512];
      time_t t = userstat.st_mtim.tv_sec;
      struct tm *lt = localtime(&t);

      strftime(time_s, sizeof(time_s), "%Y-%m-%d %H:%M:%S", lt);

      printf("%-10s  %-5s   ", user.ut_user, user.ut_line);
      printf("%s \n", time_s);
    }
    i++;
  }

  if (close(fd) == -1) {
    perror("close");
    return 1;
  }
}
