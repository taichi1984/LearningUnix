#include <stdio.h>
#include <stdlib.h>

int main() {

  FILE *fp;
  char buf[BUFSIZ];

  if ((fp = popen("ls aakjckjalj", "r")) == NULL) {
    perror("popen");
    exit(1);
  }

  while (fgets(buf, sizeof(buf), fp) != NULL) {
    printf("%s", buf);
  }

  pclose(fp);
}
