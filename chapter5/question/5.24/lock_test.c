#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {

  sleep(2);
  int rv = 0;
  if (link("/etc/passwd", "passwd.LCK") == -1) {
    rv = (errno == EEXIST ? 1 : 2);
    perror("link");
  }
  return rv;
}
