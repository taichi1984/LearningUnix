#include <stdatomic.h>
#define _GNU_SOURCE
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

#define TOTAL_PLAYS 10
int remain_plays = TOTAL_PLAYS;
int plays = 0;
int pushed = 0;
void tty_set(int order);
void push_number(int signum);
double average_time(double *seconds_array);
double fastest_time(double *seconds_array);
double slowest_time(double *seconds_array);
int main(void) {
  struct termios ts;
  tty_set(0);
  tcgetattr(STDIN_FILENO, &ts);
  ts.c_lflag &= ~(ICANON | ECHO);
  ts.c_cc[VMIN] = 1;
  ts.c_cc[VTIME] = 0;
  tcsetattr(STDIN_FILENO, TCSANOW, &ts);

  int wait = 0;
  int answer = 0;
  struct timeval before, after, laptime;
  char input;
  srand(time(NULL));

  fcntl(STDIN_FILENO, F_SETOWN, getpid());
  fcntl(STDIN_FILENO, F_SETFL, O_ASYNC | O_NONBLOCK);

  signal(SIGIO, push_number);
  double seconds_array[TOTAL_PLAYS];

  while (1) {
    if (pushed == 0) {
      wait = rand() % 6;
      answer = rand() % 9;

      printf("push number!\n");

      sleep(wait);
      printf("%d\n", answer);
      if (gettimeofday(&before, NULL) == -1) {
        perror("gettimeofday");
        tty_set(1);
        exit(1);
      };

      pause();
    }

    if (pushed == 1) {
      read(STDIN_FILENO, &input, 1);
      printf("%c \n", input);
      if ((input - '0') == answer) {
        if (gettimeofday(&after, NULL) == -1) {
          perror("gettimeofday");
          tty_set(1);
          exit(1);
        };

        laptime.tv_sec = after.tv_sec - before.tv_sec;
        laptime.tv_usec = after.tv_usec - before.tv_usec;
        if (laptime.tv_usec < 0) {
          laptime.tv_usec += 1000000;
          laptime.tv_sec -= 1;
        }
        double seconds = laptime.tv_sec + laptime.tv_usec / 1000000.0;

        seconds_array[plays] = seconds;
        printf("%f\n", seconds);
        pushed = 0;
        remain_plays--;
        plays++;
      } else {
        printf("wrong number\n");
        pushed = 0;
      }
    }

    if (remain_plays == 0) {

      for (int i = 0; i < TOTAL_PLAYS; i++) {
        printf("%d : %f seconds\n", i + 1, seconds_array[i]);
      }

      printf("Finish!\n");
      printf("Avarage time: %f\n", average_time(seconds_array));
      printf("Fastest time: %f\n", fastest_time(seconds_array));
      printf("Slowest time: %f\n", slowest_time(seconds_array));
      break;
    }
  }
  tty_set(1);
}

void push_number(int signum) { pushed = 1; }

double average_time(double *seconds_array) {
  double total_seconds = 0;

  for (int i = 0; i < TOTAL_PLAYS; i++) {
    total_seconds += seconds_array[i];
  }

  return total_seconds / TOTAL_PLAYS;
}

double fastest_time(double *seconds_array) {
  double fastest_time = seconds_array[0];

  for (int i = 1; i < TOTAL_PLAYS; i++) {
    if (fastest_time > seconds_array[i]) {
      fastest_time = seconds_array[i];
    }
  }

  return fastest_time;
}

double slowest_time(double *seconds_array) {
  double slowest_time = seconds_array[0];

  for (int i = 1; i < TOTAL_PLAYS; i++) {
    if (slowest_time < seconds_array[i]) {
      slowest_time = seconds_array[i];
    }
  }

  return slowest_time;
}

void tty_set(int order) {
  static struct termios def;

  if (order == 0) {
    tcgetattr(STDIN_FILENO, &def);
  } else if (order == 1) {
    tcsetattr(STDIN_FILENO, TCSANOW, &def);
  }
}
