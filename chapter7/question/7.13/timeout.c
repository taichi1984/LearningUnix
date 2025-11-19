#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include <unistd.h>

int main(int ac, char *av[]) {

  int pid;
  int time;

  if (ac != 3) {
    fprintf(stderr, "usage : timeout pid time\n");
    exit(1);
  }

  pid = atoi(av[1]);
  time = atoi(av[2]);

  sleep(time);

  kill(pid, SIGKILL);

  return 0;
}
