#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int lock_passwd() {
  int rv = 0;
  if (link("/etc/passwd", "/etc/passwd.LCK") == -1)
    rv = (errno == EEXIST ? 1 : 2);
  return rv;
}

int unlock_passwd() {
  if (unlink("/etc/passwd.LCK") == -1) {
    perror("unlink");
    exit(1);
  }
  return 0;
}

int main(void) {
  printf("lockしました\n");
  unlock_passwd();
  printf("unlockしました\n");

  return 0;
}
