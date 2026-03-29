#include <stdio.h>

int main() {
  FILE *fp;

  char buf[255];

  fp = popen("ls", "r");

  while (fgets(buf, 255, fp) != NULL)
    printf("%s", buf);
  pclose(fp);
}
