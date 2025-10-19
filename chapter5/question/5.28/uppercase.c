#include <alloca.h>
#include <stdio.h>
#include <stdlib.h>
#include <termio.h>
#include <unistd.h>

int main(void) {
  struct termios info;
  if (tcgetattr(0, &info) == -1) {
    perror("tcgetattr");
    exit(1);
  }

  if (info.c_oflag & OLCUC) {
    info.c_oflag &= ~OLCUC;
  } else {
    info.c_oflag |= OLCUC;
  }

  if (tcsetattr(0, TCSANOW, &info) == -1) {
    perror("tcgetattr");
    exit(1);
  }

  if (info.c_oflag & OLCUC) {
    printf("OLCUC is On\n");
  } else {
    printf("OLCUC is Off\n");
  }
}
