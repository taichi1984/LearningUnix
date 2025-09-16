#define _POSIX_C_SOURCE 200809L

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int do_mv(char *sourcePath, char *targetPath) {
  int result;
  struct stat source_stat;
  struct stat target_stat;

  if (stat(sourcePath, &source_stat) != 0) {
    perror("stat");
    exit(1);
  }

  if (stat(targetPath, &target_stat) != 0) {
  }

  if (S_ISDIR(source_stat.st_mode)) {
    fprintf(stderr, "移動元にディレクトリを指定できません\n");
  } else {
    if (S_ISDIR(target_stat.st_mode)) {
      char targetFile[PATH_MAX] = "";
      strncat(targetFile, targetPath,
              sizeof(targetFile) - strlen(targetFile) - 1);
      strncat(targetFile, "/", sizeof(targetFile) - strlen(targetFile) - 1);
      strncat(targetFile, sourcePath,
              sizeof(targetFile) - strlen(targetFile) - 1);

      printf("target_file : %s \n", targetFile);
      if (rename(sourcePath, targetFile) != 0) {
        perror("rename");
        exit(1);
      }
    } else {
      if (rename(sourcePath, targetPath) != 0) {
        perror("rename");
        exit(1);
      }
    }
  }
  return result;
}

int main(int ac, char *av[]) {

  if (ac == 3) {
    do_mv(av[1], av[2]);

  } else {
    fprintf(stderr, "引数は２つ指定してください\n");
  }
  return 0;
}
