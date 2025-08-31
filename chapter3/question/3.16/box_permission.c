#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>

#define MY_S_IFDIR 0040000

void print_binary(unsigned int x) {
  x &= 0xFFFF;
  for (int i = 15; i >= 0; i--) {
    printf("[%c]", ((x & (1u << i)) ? '1' : '0'));
  }
}

int main(void) {
  for (int i = 0; i < 16; i++)
    printf("[_]");

  printf("\n");

  mode_t mode;

  mode = MY_S_IFDIR | S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IXGRP |
         S_IROTH | S_IXOTH;

  // printf("%o\n", mode);

  print_binary(mode);

  printf("\n");

  return 0;
}
