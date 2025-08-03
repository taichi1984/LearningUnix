#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  DIR *dir;
  char *target = ".";
  struct dirent *entry;

  if (argc == 2)
    target = argv[1];

  if ((dir = opendir(target)) == NULL) {
    perror("opendir");
    exit(1);
  }

  while ((entry = readdir(dir)) != NULL) {
    printf("%s \n ", entry->d_name);
  }

  closedir(dir);
}
