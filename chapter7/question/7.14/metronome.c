#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/time.h>

int do_sigint = 0;

int main(int ac, char *av[]) {

  int processes[ac - 2];
  int timersec = atoi(av[1]);
  struct itimerval new_timeset;
  void send_sigint(int signum);
  for (int i = 2; i < ac; i++) {
    processes[i - 2] = atoi(av[i]);
  }

  signal(SIGALRM, send_sigint);
  new_timeset.it_interval.tv_sec = timersec;
  new_timeset.it_interval.tv_usec = 0;
  new_timeset.it_value.tv_sec = timersec;
  new_timeset.it_value.tv_usec = 0;

  setitimer(ITIMER_REAL, &new_timeset, NULL);

  while (1) {
    pause();
    if (do_sigint == 1) {
      for (int i = 0; i < ac - 2; i++) {
        kill(processes[i], SIGINT);
      }
      do_sigint = 0;
    }
  }
}

void send_sigint(int signum) { do_sigint = 1; }
