#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

int timespec_diff_sec(const struct timespec *a, const struct timespec *b) {
  return a->tv_sec - b->tv_sec;
}

int main(int ac, char *av[]) {

  int pid;
  int time;
  struct stat st;
  struct timespec now;

  if (ac != 3) {
    fprintf(stderr, "usage : timeout pid time\n");
    exit(1);
  }

  pid = atoi(av[1]);
  time = atoi(av[2]);

  char path[64];
  char buf[256];

  snprintf(path, sizeof(path), "/proc/%d/fd/0", pid);
  ssize_t len = readlink(path, buf, sizeof(buf) - 1);
  if (len < 0) {
    perror("readlink");
    exit(1);
  }

  buf[len] = '\0';

  sleep(time);
  stat(buf, &st);
  clock_gettime(CLOCK_REALTIME, &now);

  if (timespec_diff_sec(&now, &st.st_atim) > 20) {
    printf("20秒以上操作されていないのでKILLします\n");
    kill(pid, SIGKILL);
  } else {
    printf(
        "20秒以内に操作されているので、KILLせずにプログラムを終了します。\n");
  }

  return 0;
}
