#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main(int ac, char *av[]) {
  char buf[BUFSIZ];
  FILE *fp = popen("cat", "w");

  while (fgets(buf, sizeof(buf), stdin)) {
    fprintf(fp, "%s", buf);
    fflush(fp);
  }

  pclose(fp);
}
