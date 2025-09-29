#include <stdio.h>

int main() {
  FILE *fp1;
  FILE *fp2;
  fp1 = fopen("testfile", "a");
  fp2 = fopen("testfile", "a");

  fprintf(fp1, "this is fp1\n");
  fprintf(fp2, "this is fp2\n");

  fclose(fp1);
  fclose(fp2);

  return 0;
}
