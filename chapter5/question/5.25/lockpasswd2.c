#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <utime.h>

void update_locktime() { utime("/etc/passwd.LCK", NULL); }

void start_timer(int seconds) {
  struct itimerval timer;

  signal(SIGALRM, update_locktime);

  timer.it_interval.tv_sec = seconds;
  timer.it_interval.tv_usec = 0;

  timer.it_value.tv_sec = seconds;
  timer.it_value.tv_usec = 0;

  setitimer(ITIMER_REAL, &timer, NULL);
}

int unlock_passwd() {
  if (unlink("/etc/passwd.LCK") == -1) {
    perror("unlink");
    exit(1);
  }
  return 0;
}

int lock_passwd(int seconds) {
  int rv = 0;

  while (1) {
    if (link("/etc/passwd", "/etc/passwd.LCK") == 0)
      return 0;

    if (errno != EEXIST) {
      return 2;
    }

    struct stat st;
    if (stat("/etc/passwd.LCK", &st) == -1) {
      perror("stat");
      return 1;
    }

    time_t now = time(NULL);
    
    if (now - st.st_mtime > seconds) {
      unlink("/etc/passwd.LCK");
    }
    continue;

    return 1;
  }
}

int main(void) {
  char word[100];
  lock_passwd(5);
  start_timer(5);
  scanf("%s", word);
  printf("%s\n", word);
  unlock_passwd();
  printf("unlockしました\n");

  return 0;
}
