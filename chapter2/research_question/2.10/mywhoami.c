#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
  struct passwd *pw = getpwuid(getuid());
  if (pw) {

    printf("%s\n", pw->pw_name);
  } else {
    perror("getpwuid");
    return 1;
  }
  return 0;
}
